<?php

/**
 * @file
 * Display functions.
 */

/**
 * Renders an embedded entity.
 *
 * @param $entity_type
 *   The type of entity, i.e. 'node', 'user'.
 * @param $entity
 *   The entity to be rendered.
 * @param array $context
 *   (optional) Array of context values, corresponding to the attributes on
 *   the embed HTML tag.
 *
 * @return string
 *   The HTML of the entity rendered with the Entity Embed Display plugin.
 */
function entity_embed_render_entity($entity_type, $entity, array $context = array()) {
  // Support the deprecated view-mode data attribute.
  if (isset($context['data-view-mode']) && !isset($context['data-entity-embed-display']) && !isset($context['data-entity-embed-settings'])) {
    $context['data-entity-embed-display'] = 'entityreference:entityreference_entity_view';
    $context['data-entity-embed-settings'] = array('view_mode' => $context['data-view-mode']);
  }

  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  // Merge in default attributes.
  $context += array(
    'data-entity-id' => $id,
    'data-entity-type' => $entity_type,
    'data-entity-embed-display' => 'entityreference:entityreference_entity_view',
    'data-entity-embed-settings' => array(),
  );

  // The default Entity Embed Display plugin has been deprecated by the
  // rendered entity field formatter.
  if ($context['data-entity-embed-display'] === 'default') {
    $context['data-entity-embed-display'] = 'entityreference:entityreference_entity_view';
  }

  // Allow modules to alter the entity prior to embed rendering.
  drupal_alter(array("{$context['data-entity-type']}_embed_context", 'entity_embed_context'), $context, $entity);

  // Build and render the Entity Embed Display plugin, allowing modules to
  // alter the result before rendering.
  $build = render_entity_embed_display_plugin(
    $entity_type,
    $entity,
    $context['data-entity-embed-display'],
    $context['data-entity-embed-settings'],
    $context
  );
  // @todo Should this hook get invoked if $build is an empty array?
  drupal_alter(array("{$context['data-entity-type']}_embed", 'entity_embed'), $build, $entity, $context);
  $entity_output = drupal_render($build);

  return $entity_output;
}

/**
 * Renders an entity using an Entity Embed Display plugin.
 *
 * @param $entity_type
 *   The type of entity, i.e. 'node', 'user'.
 * @param $entity
 *   The entity to be rendered.
 * @param string $plugin_id
 *   The Entity Embed Display plugin ID.
 * @param array $plugin_configuration
 *   (optional) Array of plugin configuration values.
 * @param array $context
 *   (optional) Array of additional context values, usually the embed HTML
 *   tag's attributes.
 *
 * @return array
 *   A render array for the Entity Embed Display plugin.
 */
function render_entity_embed_display_plugin($entity_type, $entity, $plugin_id, array $plugin_configuration = array(), array $context = array()) {
  // Check if the display plugin is accessible.
  if (!entity_access('view', $entity_type, $entity)) {
    return array();
  }

  // Split the formatter into separate parts.
  $formatter_parts = explode(':', $plugin_id);
  $formatter_module = $formatter_parts[0];
  $formatter_type = $formatter_parts[1];

  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  $field_formatter_info = field_info_formatter_types($formatter_type);

  // With the File Entity module enabled, file entities require some special
  // handling. Unlike standard field formatters which specify a list of
  // compatible entity types and will work with any entity as long as it is the
  // correct type, file formatters may only support files with specific MIME
  // types.
  if ($entity_type == 'file' && module_exists('file_entity')) {
    if (isset($field_formatter_info['file formatter']['mime types'])) {
      if (!file_entity_match_mimetypes($field_formatter_info['file formatter']['mime types'], $entity->filemime)) {
        $file_type = file_type_load($entity->type);

        // Inform the user that the file they are trying to embed cannot be
        // displayed using the selected formatter.
        return array(
          '#markup' => '<p>' . t('The %type file %filename is unable to be displayed as %formatter. Please select a different formatter.', array('%type' => $file_type->label, '%filename' => $entity->filename, '%formatter' => $field_formatter_info['label'])) . '</p>',
        );
      }
    }
  }

  if (isset($field_formatter_info['module'])) {
    // Set $display['type'] and $display['settings'] to what
    // hook_field_formatter_*() expects.
    $display['type'] = $formatter_type;
    $display['settings'] = $plugin_configuration += field_info_formatter_settings($formatter_type); // Use default settings if no settings are available.

    // Set $items to what the field formatter expects. See hook_field_load(),
    // and note that, here, $entity is already a fully loaded entity.
    $items = entity_embed_field_get_items($entity_type, $id, $field_formatter_info['module']);

    // Invoke hook_field_formatter_prepare_view() and
    // hook_field_formatter_view(). Note that we are reusing field formatter
    // functions, but we are not displaying a Field API field, so we set
    // $field and $instance accordingly, and do not invoke
    // hook_field_prepare_view(). This assumes that the formatter functions do
    // not rely on $field or $instance. A module that implements formatter
    // functions that rely on $field or $instance (and therefore, can only be
    // used for real fields) can add support for being used on the pseudo-field
    // by adding in any missing information using the entity_embed_field_info
    // hooks.
    $field = entity_embed_field_info_field($entity_type, $field_formatter_info['module']);
    $instance = entity_embed_field_info_instance($formatter_type, $plugin_configuration);

    if (($function = ($field_formatter_info['module'] . '_field_formatter_prepare_view')) && function_exists($function)) {
      // hook_field_formatter_prepare_view() alters $items by reference.
      $grouped_items = array($id => &$items);
      $function($entity_type, array($id => $entity), $field, array($id => $instance), $context['data-langcode'], $grouped_items, array($id => $display));
    }
    if (($function = ($field_formatter_info['module'] . '_field_formatter_view')) && function_exists($function)) {
      $element = $function($entity_type, $entity, $field, $instance, $context['data-langcode'], $items, $display);
      // We passed the entity as $items[0], so return the corresponding element.
      if (isset($element[0])) {
        return $element[0];
      }
    }
  }
}

/**
 * Get the possible field types that an entity might be displayed inside of.
 *
 * @param string $entity_type
 *   The type of entity that might be displayed.
 *
 * @return
 *   An array of field types that might display the passed entity type.
 */
function entity_embed_get_entity_field_types($entity_type) {
  $field_types = array();

  // All entity types, by definition, can be displayed as entityreference.
  $field_types[] = 'entityreference';

  // Support for nodes.
  if ('node' == $entity_type) {
    $field_types[] = 'node_reference';
  }

  // Support for users.
  if ('user' == $entity_type) {
    $field_types[] = 'user_reference';
  }

  // Support for taxonomy.
  if ('taxonomy_term' == $entity_type) {
    $field_types[] = 'taxonomy_term_reference';
  }

  // Support for files.
  if ('file' == $entity_type) {
    $field_types[] = 'image';
    $field_types[] = 'file';
  }

  // Support for any other entity_types.
  $additional_field_types = module_invoke_all('entity_embed_field_types', $entity_type);
  $field_types = array_merge($field_types, $additional_field_types);

  return $field_types;
}

/**
 * Helper function to get all the field formatters that we can use to display
 * entity types.
 */
function entity_embed_get_entity_field_formatters($entity_type) {
  $formatters = field_info_formatter_types();
  $field_types = entity_embed_get_entity_field_types($entity_type);

  $entity_formatters = array();

  foreach ($formatters as $formatter => $info) {
    foreach ($info['field types'] as $field_type) {
      if (in_array($field_type, $field_types)) {
        $entity_formatters[$formatter] = $info;
        break;
      }
    }
  }

  return $entity_formatters;
}

/**
 * @file
 * Helper functions for Entity Embed involving field formatters.
 *
 * All of these functions are by definition hacky, because we're trying to use
 * a couple of hook in ways they were not intended to be used: displaying
 * arbitrary entities. The hooks are:
 * - hook_field_formatter_settings_form()
 * - hook_field_formatter_view()
 */

/**
 * Our own field-less and entity-less version of field_get_items().
 *
 * @param string $entity_type
 *   The entity type of the entity we are trying to embed.
 *
 * @param int $entity_id
 *   The entity id of the entity we are trying to embed.
 *
 * @param string $module
 *   The module that implements the field formatter we are going to use.
 *
 * In our version of field_get_items() we don't care at all about the field
 * or the entity that it is attached to. All we want is the output of the
 * formatter. So the point of this is return an $items array in the format
 * expected by the passed module's particular implementation of
 * hook_field_formatter_view().
 *
 * Ideally this would be a simple hook letting other modules do the work, but
 * we will hard-code support for the major players here.
 */
function entity_embed_field_get_items($entity_type, $entity_id, $module) {
  // Whatever module is involved, we're almost definitely going to need the
  // entity loaded and an access check.
  $entity = entity_load_single($entity_type, $entity_id);
  $access = entity_access('view', $entity_type, $entity);

  $item = array(
    'access' => $access,
  );

  // Support the entityreference module.
  if ('entityreference' == $module) {
    $item += array(
      'entity' => $entity,
      'target_id' => $entity_id,
    );
  }

  // Support the taxonomy module (core).
  elseif ('taxonomy' == $module) {
    $item += array(
      'tid' => $entity_id,
      'name' => entity_label($entity_type, $entity_id),
      'taxonomy_term' => $entity,
    );
  }

  // Support the file  and image modules (core).
  elseif ('file' == $module || 'image' == $module) {
    // The file and image modules just expect an array version of the object.
    $item += (array) $entity;
  }

  // Support the user_reference (references) module.
  elseif ('user_reference' == $module) {
    $item += array(
      'uid' => $entity_id,
      'user' => $entity,
    );
  }

  // Support the node_reference (references) module.
  elseif ('node_reference' == $module) {
    $item += array(
      'nid' => $entity_id,
      'node' => $entity,
    );
  }

  // Support the file_entity module.
  elseif ('file_entity' == $module) {
    // In general the file_entity module expects items to be
    // file objects, but in array form, similar to core image/file.
    $item += (array) $entity;
  }

  // If we didn't find anything, give other modules a chance.
  else {
    $contrib_items = module_invoke_all('entity_embed_field_get_items', $entity_type, $entity_id, $module);
    // Make sure we only get 1 item.
    if (!empty($contrib_items)) {
      $item += array_pop($contrib_items);
    }
  }

  return array($item);
}

/**
 * Our own field-less entity-less version of field_get_display().
 *
 * @param $formatter_type
 *   A formatter type name.
 *
 * @param int $settings
 *   Optional array of settings to pass in. If omitted, the formatter's default
 *   settings will be returned.
 *
 * @return
 *   An array containing 'type' and 'settings'.
 *
 * In our version of field_get_display() we don't care at all about the field
 * or the entity/bundle that it is attached to. All we want is a display
 * array that will help us hack our way through hook_field_formatter_view() and
 * hook_field_formatter_settings_form(). This mainly means making sure the
 * 'settings' and 'type' elements are there and populated appropriately.
 *
 * Luckily this can be the same regardless of formatter/module, so we don't
 * need to hard-code module support or invoke any hooks.
 */
function entity_embed_field_get_display($formatter_type, $settings = array()) {
  // If settings is empty, we need to get defaults.
  if (empty($settings)) {
    $formatter_info = field_info_formatter_types($formatter_type);
    if (!empty($formatter_info['settings'])) {
      $settings = $formatter_info['settings'];
    }
  }

  return array(
    'type' => $formatter_type,
    'settings' => $settings,
  );
}

/**
 * Our own field-less version of field_info_field().
 *
 * @param string $entity_type
 *   The entity type of the entity we are trying to embed.
 *
 * @param string $module
 *   The module that implements the field formatter we are going to use.
 *
 * In our version of field_info_field() we don't care at all about the field
 * itself. All we want is the field info array in a format that lets us hack
 * into hook_field_formatter_view() and hook_field_formatter_settings_form().
 * The problem is that each module may expect something different in this array.
 *
 * Ideally this would be a simple hook letting other modules do the work, but
 * we will hard-code support for the major players here.
 */
function entity_embed_field_info_field($entity_type, $module) {
  $field = array();

  // Support the entityreference module.
  if ('entityreference' == $module) {
    $field['settings'] = field_info_field_settings('entityreference');
    $field['settings']['target_type'] = $entity_type;
  }

  // Support the user_reference (references) module.
  elseif ('user_reference' == $module) {
    $field['settings'] = field_info_field_settings('user_reference');
    $field['field_name'] = 'foo';
  }

  // Support node_reference (references) module.
  elseif ('node_reference' == $module) {
    $field['settings'] = field_info_field_settings('node_reference');
    $field['field_name'] = 'foo';
  }

  // Support taxonomy (core) module.
  elseif ('taxonomy' == $module) {
    $field['settings'] = field_info_field_settings('taxonomy_term_reference');
  }

  // Support file (core) module.
  elseif ('file' == $module) {
    $field['settings'] = field_info_field_settings('file');
  }

  // Support file (core) module.
  elseif ('image' == $module) {
    $field['settings'] = field_info_field_settings('image');
  }

  // If we didn't find anything, give other modules a chance.
  else {
    $field = module_invoke_all('entity_embed_field_info_field', $entity_type, $module);
  }

  return $field;
}

/**
 * Our own field-less entity-less version of field_info_instance().
 *
 * @param $formatter_type
 *   A formatter type name.
 *
 * @param int $settings
 *   Optional array of settings to pass in. If omitted, the formatter's default
 *   settings will be returned.
 *
 * In our version of field_info_instance() we don't care at all about the field
 * itself. All we want is the field instance array in a format that lets us hack
 * into hook_field_formatter_view() and hook_field_formatter_settings_form().
 * The problem is that each module may expect something different in this array.
 *
 * This is mainly a wrapper around entity_embed_field_get_display().
 */
function entity_embed_field_info_instance($formatter_type, $settings = array()) {
  return array(
    'display' => array(
      'default' => entity_embed_field_get_display($formatter_type, $settings),
    ),
  );
}

/**
 * Our own field-less entity-less version of field_info_formatter_settings().
 *
 * @param $entity_type
 *   The type of entity, i.e. 'node', 'user'.
 * @param $entity
 *   The entity to be rendered.
 * @param $formatter_type
 *   A formatter type name.
 * @param int $settings
 *   Optional array of settings to pass in. If omitted, the formatter's default
 *   settings will be returned.
 * @param $form
 *   A form array().
 * @param $form_state
 *   A form state array.
 *
 * In our version of field_info_formatter_settings() we don't care at all about
 * the field itself. All we want is the field info array in a format that lets
 * us hack hook_field_formatter_settings_form().
 */
function entity_embed_field_info_formatter_settings($entity_type, $entity, $formatter_type, $settings = array(), $form, $form_state) {
  $formatter_settings = array();

  $field_formatter_info = field_info_formatter_types($formatter_type);

  // Invoke hook_field_formatter_settings_form(). We are reusing field
  // formatter functions, but we are not working with a Field API field,
  // so set $field accordingly. Unfortunately, the API is for $settings to
  // be transferred via the $instance parameter, so we must mock it.
  if (isset($field_formatter_info['module']) && ($function = ($field_formatter_info['module'] . '_field_formatter_settings_form')) && function_exists($function)) {
    list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

    $field = NULL;
    $view_mode = 'default';
    $mock_instance = array(
      'display' => array(
        $view_mode => array(
          'type' => $formatter_type,
          'settings' => $settings + field_info_formatter_settings($formatter_type),
        ),
      ),
      'entity_type' => $entity_type,
      'bundle' => $bundle,
    );

    $formatter_settings += $function($field, $mock_instance, $view_mode, $form, $form_state);
  }

  return $formatter_settings;
}
