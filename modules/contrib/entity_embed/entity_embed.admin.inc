<?php

/**
 * @file
 * Admin page callbacks for the entity_embed module.
 */

/**
 * Form constructor for the entity embed dialog form.
 *
 * Allows the user to select and embed an entity.
 *
 * @see entity_embed_dialog_form_validate()
 * @ingroup forms
 */
function entity_embed_dialog_form($form, &$form_state, $filter_format, $embed_button) {
  $values = isset($form_state['values']) ? $form_state['values'] : array();
  $input = $_POST;

  // Set embed button element in form state, so that it can be used later in
  // validateForm() function.
  $form['#embed_button'] = $embed_button;

  // Initialize entity element with form attributes, if present.
  $entity_element = empty($values['attributes']) ? array() : $values['attributes'];

  // The default values are set directly from \Drupal::request()->request,
  // provided by the editor plugin opening the dialog.
  if (empty($form_state['entity_element'])) {
    $form_state['entity_element'] = isset($input['editor_object']) ? $input['editor_object'] : array();
  }

  $entity_element += $form_state['entity_element'];
  $entity_element += array(
    'data-entity-type' => $embed_button->entity_type,
    'data-entity-uuid' => '',
    'data-entity-id' => '',
    'data-entity-embed-display' => 'entityreference:entityreference_entity_view',
    'data-entity-embed-settings' => array(),
    'data-align' => '',
  );

  if (empty($form_state['step'])) {
    // If an entity has been selected, then always skip to the embed options.
    if (!empty($entity_element['data-entity-type']) && (!empty($entity_element['data-entity-uuid']) || !empty($entity_element['data-entity-id']))) {
      $form_state['step'] = 'embed';
    }
    else {
      $form_state['step'] = 'select';
    }
  }

  $form['#tree'] = TRUE;
  $form['#attached']['library'][] = array('editor', 'drupal.editor.dialog');
  $form['#prefix'] = '<div id="entity-embed-dialog-form">';
  $form['#suffix'] = '</div>';

  switch ($form_state['step']) {
    case 'select':
      $form['#title'] = t('Embed entity');

      $form['attributes']['data-entity-type'] = array(
        '#type' => 'value',
        '#value' => $entity_element['data-entity-type'],
      );

      $label = t('Label');
      // Attempt to display a better label if we can by getting it from
      // the label field definition.
      $entity_info = entity_get_info($entity_element['data-entity-type']);

      if (!empty($entity_info['label'])) {
        $label = check_plain($entity_info['label']);
      }

      $default = '';
      if (!empty($entity_element['data-entity-uuid']) && module_exists('uuid')) {
        $entity = entity_uuid_load($entity_element['data-entity-type'], array($entity_element['data-entity-uuid']));
        $entity = reset($entity);
      }
      else {
        $entity = entity_load_single($entity_element['data-entity-type'], $entity_element['data-entity-id']);
      }

      if ($entity) {
        $default = entity_label($entity_element['data-entity-type'], $entity);
      }

      $form['attributes']['data-entity-id'] = array(
        '#type' => 'textfield',
        '#title' => $label,
        '#default_value' => $default,
        '#autocomplete_path' => 'entity-embed/autocomplete-entity/' . $filter_format->format . '/' . $embed_button->name,
        '#required' => TRUE,
        '#description' => t('Type label and pick the right one from suggestions. Note that the unique ID will be saved.'),
      );

      // Integrate with the Media module to support selecting files to embed.
      if ($entity_element['data-entity-type'] == 'file' && module_exists('media')) {
        $form['attributes']['data-entity-id'] = array(
          '#type' => 'media',
          '#title' => $label,
          '#default_value' => $entity_element['data-entity-id'],
          '#required' => TRUE,
          '#description' => t('Select a file to embed using the media browser.'),
          '#media_options' => array(
            'global' => array(
              'types' => isset($embed_button->entity_type_bundles) ? $embed_button->entity_type_bundles : array(),
            ),
          ),
        );
      }

      $form['attributes']['data-entity-uuid'] = array(
        '#type' => 'value',
        '#title' => $entity_element['data-entity-uuid'],
      );
      $form['actions'] = array(
        '#type' => 'actions',
      );
      $form['actions']['save_modal'] = array(
        '#attributes' => array(
          'class' => array('button--primary'),
        ),
        '#type' => 'submit',
        '#value' => t('Next'),
        // No regular submit-handler. This form only works via JavaScript.
        '#submit' => array(),
        '#ajax' => array(
          'callback' => 'submitSelectForm',
          'event' => 'click',
        ),
      );
      break;

    case 'embed':
      $form['#title'] = t('Edit embedded entity');

      if (!empty($entity_element['data-entity-uuid']) && module_exists('uuid')) {
        $entity = entity_uuid_load($entity_element['data-entity-type'], array($entity_element['data-entity-uuid']));
        $entity = reset($entity);
      }
      else {
        $entity = entity_load_single($entity_element['data-entity-type'], $entity_element['data-entity-id']);
      }

      $text = t('Preview selected entity');
      if ($entity) {
        $text = entity_label($entity_element['data-entity-type'], $entity);
      }

      $uri = entity_uri($entity_element['data-entity-type'], $entity);

      $form['entity'] = array(
        '#type' => 'item',
        '#title' => t('Selected entity'),
        '#markup' => l($text, $uri['path'], $uri['options'] + array('attributes' => array('target' => '_blank'))),
      );
      $form['attributes']['data-entity-type'] = array(
        '#type' => 'value',
        '#value' => $entity_element['data-entity-type'],
      );
      $form['attributes']['data-entity-id'] = array(
        '#type' => 'value',
        '#value' => $entity_element['data-entity-id'],
      );
      $form['attributes']['data-entity-uuid'] = array(
        '#type' => 'value',
        '#value' => $entity_element['data-entity-uuid'],
      );

      $formatter_options = array();
      $available_plugins = array();

      // Retrieve a list of formatters appropriate for the currently selected
      // entity type.
      $formatters = entity_embed_get_entity_field_formatters($entity_element['data-entity-type']);

      if (!empty($formatters)) {
        foreach ($formatters as $formatter => $info) {
          $available_plugins[$info['module'] . ':' . $formatter] = $info;
          $formatter_options[$info['module'] . ':' . $formatter] = $info['label'];

          // With the File Entity module enabled, file entities require some
          // special handling. Unlike standard field formatters which specify a
          // list of compatible entity types and will work with any entity as
          // long as it is the correct type, file formatters may only support
          // files with specific MIME types.
          if ($entity_element['data-entity-type'] == 'file' && module_exists('file_entity')) {
            if (isset($info['file formatter']['mime types'])) {
              if (!file_entity_match_mimetypes($info['file formatter']['mime types'], $entity->filemime)) {
                // Remove the incompatible formatter.
                unset($formatter_options[$info['module'] . ':' . $formatter]);
              }
            }
          }

          // With the File Image Formatters module enabled, image field
          // formatters will appear twice.
          if ($info['module'] == 'file_image_formatters' && module_exists('file_image_formatters')) {
            // Remove the duplicate formatter.
            unset($formatter_options[$info['module'] . ':' . $formatter]);
          }
        }
      }

      // The 'Rendered entity' formatter can not be used for files unless
      // the file_entity module is available.
      if (!empty($formatter_options['entityreference:entityreference_entity_view']) && $entity_element['data-entity-type'] == 'file' && !module_exists('file_entity')) {
        unset($formatter_options['entityreference:entityreference_entity_view']);
      }

      // Build the list of allowed Entity Embed Display plugins.
      $allowed_plugins = $embed_button->display_plugins;

      // If list of allowed options is empty, it means that all plugins are
      // allowed. Else, take the intersection of allowed and available
      // plugins.
      $display_plugin_options = empty($allowed_plugins) ? $available_plugins : array_intersect_key($available_plugins, $allowed_plugins);

      // If the currently selected display is not in the available options,
      // use the first from the list instead. This can happen if an alter
      // hook customizes the list based on the entity.
      if (!isset($display_plugin_options[$entity_element['data-entity-embed-display']])) {
        $entity_element['data-entity-embed-display'] = key($display_plugin_options);
      }

      // The default Entity Embed Display plugin has been deprecated by the
      // rendered entity field formatter.
      if ($entity_element['data-entity-embed-display'] === 'default') {
        $entity_element['data-entity-embed-display'] = 'entityreference:entityreference_entity_view';
      }

      // @todo: Abstract formatters into 'display plugins'.
      $form['attributes']['data-entity-embed-display'] = array(
        '#type' => 'select',
        '#title' => t('Display as'),
        '#options' => $formatter_options,
        '#default_value' => $entity_element['data-entity-embed-display'],
        '#required' => TRUE,
        '#ajax' => array(
          'callback' => 'updatePluginConfigurationForm',
          'wrapper' => 'data-entity-embed-settings-wrapper',
          'effect' => 'fade',
        ),
        // Hide the selection if only one option is available.
        '#access' => count($display_plugin_options) > 1,
      );
      $form['attributes']['data-entity-embed-settings'] = array(
        '#type' => 'container',
        '#prefix' => '<div id="data-entity-embed-settings-wrapper">',
        '#suffix' => '</div>',
      );
      $form['attributes']['data-embed-button'] = array(
        '#type' => 'value',
        '#value' => $embed_button->name,
      );
      $form['attributes']['data-entity-label'] = array(
        '#type' => 'value',
        '#value' => $embed_button->admin_title,
      );

      $plugin_id = !empty($values['attributes']['data-entity-embed-display']) ? $values['attributes']['data-entity-embed-display'] : $entity_element['data-entity-embed-display'];

      // Split the formatter into separate parts.
      $formatter_parts = explode(':', $plugin_id);
      $formatter_module = $formatter_parts[0];
      $formatter_type = $formatter_parts[1];

      // Retrieve the settings appropriate for the currently selected formatter.
      if (!empty($formatter_type)) {
        if (is_string($entity_element['data-entity-embed-settings'])) {
          $entity_element['data-entity-embed-settings'] = drupal_json_decode($entity_element['data-entity-embed-settings']);
        }

        $form['attributes']['data-entity-embed-settings'] += entity_embed_field_info_formatter_settings($entity_element['data-entity-type'], $entity, $formatter_type, $entity_element['data-entity-embed-settings'], $form, $form_state);
      }

      $filters = filter_list_format($filter_format->format);

      // When Editor's editor_align is being used, the text editor may offer the
      // ability to change the alignment.
      if (isset($entity_element['data-align']) && isset($filters['editor_align']) && $filters['editor_align']->status) {
        $form['attributes']['data-align'] = array(
          '#title' => t('Align'),
          '#type' => 'radios',
          '#options' => array(
            'none' => t('None'),
            'left' => t('Left'),
            'center' => t('Center'),
            'right' => t('Right'),
          ),
          '#default_value' => $entity_element['data-align'] === '' ? 'none' : $entity_element['data-align'],
          '#wrapper_attributes' => array('class' => array('container-inline')),
          '#attributes' => array('class' => array('container-inline')),
          '#parents' => array('attributes', 'data-align'),
        );
      }

      // @todo Re-add caption attribute.
      $form['actions'] = array(
        '#type' => 'actions',
      );
      $form['actions']['back'] = array(
        '#type' => 'submit',
        '#value' => t('Back'),
        // No regular submit-handler. This form only works via JavaScript.
        '#submit' => array(),
        '#ajax' => array(
          'callback' => 'goBack',
          'event' => 'click',
        ),
      );
      $form['actions']['save_modal'] = array(
        '#attributes' => array(
          'class' => array('button--primary'),
        ),
        '#type' => 'submit',
        '#value' => t('Embed'),
        // No regular submit-handler. This form only works via JavaScript.
        '#submit' => array(),
        '#ajax' => array(
          'callback' => 'submitEmbedForm',
          'event' => 'click',
        ),
      );
      break;
  }

  return $form;
}

/**
 * Form validation handler for entity_embed_dialog_form().
 *
 * @see entity_embed_dialog_form()
 */
function entity_embed_dialog_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  switch ($form_state['step']) {
    case 'select':
      if ($entity_type = $values['attributes']['data-entity-type']) {
        $title = trim($values['attributes']['data-entity-id']);

        $entity_info = entity_get_info($entity_type);

        // Prevent errors if the entity type has no label key.
        if (empty($entity_info['entity keys']['label'])) {
          form_set_error('data-entity-id', t('Unable to find label for @type entity @id.', array('@type' => $entity_type, '@id' => $title)));
        }

        // The entity ID may be either the ID (integer) of the entity or the
        // entity's title (string).
        if (is_numeric($title)) {
          $entities = entity_load($entity_type, array($title));
        }
        else {
          $entities = entity_load($entity_type, FALSE, array($entity_info['entity keys']['label'] => array($title)));
        }

        $entity = reset($entities);

        if (!empty($entity)) {
          list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

          if (!entity_access('view', $entity_type, $entity)) {
            form_set_error('data-entity-id', t('Unable to access @type entity @id.', array('@type' => $entity_type, '@id' => $title)));
          }
          else {
            form_set_value($form['attributes']['data-entity-id'], $id, $form_state);

            if (isset($entity->uuid)) {
              form_set_value($form['attributes']['data-entity-uuid'], $entity->uuid, $form_state);
            }
            else {
              form_set_value($form['attributes']['data-entity-uuid'], '', $form_state);
            }

            // Ensure that at least one Entity Embed Display plugin is present
            // before proceeding to the next step. Rasie an error otherwise.
            $embed_button = $form['#embed_button'];
            $allowed_plugins = $embed_button->display_plugins;
            $available_plugins = entity_embed_get_entity_field_formatters($entity_type);

            $display_plugin_options = empty($allowed_plugins) ? $available_plugins : array_intersect_key($available_plugins, $allowed_plugins);

            // If no plugin is available after taking the intersection,
            // raise error. Also log an exception.
            if (empty($display_plugin_options)) {
              form_set_error('data-entity-id', t('No display options available for the selected entity. Please select another entity.'));
              watchdog('entity_embed', 'No display options available for "@type:" entity "@id" while embedding using button "@button". Please ensure that at least one Entity Embed Display plugin is allowed for this embed button which is available for this entity.', array('@type' => $entity_type, '@id' => $id, '@button' => $embed_button->admin_title), WATCHDOG_WARNING);
            }
          }
        }
        else {
          form_set_error('data-entity-id', t('Unable to load @type entity @id.', array('@type' => $entity_type, '@id' => $title)));
        }
      }
      break;

    case 'embed':
        // @todo: Display plugin validation.
      break;
  }
}

/**
 * Form submission handler to update the plugin configuration form.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $form_state
 *   An associative array containing the current state of the form.
 */
function updatePluginConfigurationForm(&$form, $form_state) {
  return $form['attributes']['data-entity-embed-settings'];
}

/**
 * Form submission handler to go back to the previous step of the form.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $form_state
 *   An associative array containing the current state of the form.
 */
function goBack(&$form, $form_state) {
  $form_state['step'] = 'select';
  $form_state['rebuild'] = TRUE;
  $rebuild_form = drupal_rebuild_form('entity_embed_dialog_form', $form_state, $form);
  unset($rebuild_form['#prefix'], $rebuild_form['#suffix']);
  $status_messages = array('#theme' => 'status_messages');
  $output = drupal_render($rebuild_form);
  drupal_process_attached($rebuild_form);
  $output = '<div>' . drupal_render($status_messages) . $output . '</div>';
  $commands[] = ajax_command_html('#entity-embed-dialog-form', $output);

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Form submission handler that selects an entity and display embed settings.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $form_state
 *   An associative array containing the current state of the form.
 */
function submitSelectForm(&$form, $form_state) {
  // Display errors in form, if any.
  if (form_get_errors()) {
    unset($form['#prefix'], $form['#suffix']);
    $status_messages = array('#theme' => 'status_messages');
    $output = drupal_render($form);
    $output = '<div>' . drupal_render($status_messages) . $output . '</div>';
    $commands[] = ajax_command_html('#entity-embed-dialog-form', $output);
  }
  else {
    $form_state['step'] = 'embed';
    $form_state['rebuild'] = TRUE;
    $rebuild_form = drupal_rebuild_form('entity_embed_dialog_form', $form_state, $form);
    unset($rebuild_form['#prefix'], $rebuild_form['#suffix']);
    $status_messages = array('#theme' => 'status_messages');
    $output = drupal_render($rebuild_form);
    drupal_process_attached($rebuild_form);
    $output = '<div>' . drupal_render($status_messages) . $output . '</div>';
    $commands[] = ajax_command_html('#entity-embed-dialog-form', $output);
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Form submission handler embeds selected entity in WYSIWYG.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $form_state
 *   An associative array containing the current state of the form.
 */
function submitEmbedForm(&$form, $form_state) {
   // @todo: Display plugin submission.
  $values = $form_state['values'];

  // Display errors in form, if any.
  if (form_get_errors()) {
    unset($form['#prefix'], $form['#suffix']);
    $status_messages = array('#theme' => 'status_messages');
    $output = drupal_render($form);
    $output = '<div>' . drupal_render($status_messages) . $output . '</div>';
    $commands[] = ajax_command_html('#entity-embed-dialog-form', $output);
  }
  else {
    // Serialize entity embed settings to JSON string.
    if (!empty($values['attributes']['data-entity-embed-settings'])) {
      $values['attributes']['data-entity-embed-settings'] = json_encode($values['attributes']['data-entity-embed-settings'], JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_AMP | JSON_HEX_QUOT);
    }

    $commands[] = editor_command_editor_dialog_save($values);
    $commands[] = dialog_command_close_modal_dialog();
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}
