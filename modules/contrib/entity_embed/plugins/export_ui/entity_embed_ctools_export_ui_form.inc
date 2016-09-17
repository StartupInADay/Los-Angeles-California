<?php

/**
 * @file
 * Builds the ctools export UI page for configuring embed buttons.
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'entity_embed',

  'access' => 'administer embed buttons',

  'menu' => array(
    'menu prefix' => 'admin/config/content',
    'menu item' => 'embed-button',
    'menu title' => 'Entity embed',
    'menu description' => 'Administer entity embed buttons.',
  ),

  'title singular' => t('button configuration'),
  'title plural' => t('button configurations'),
  'title singular proper' => t('Button configuration'),
  'title plural proper' => t('Button configurations'),

  'handler' => array(
    'class' => 'ctools_export_ui_entity_embed',
    'parent' => 'ctools_export_ui',
  ),

  'form' => array(
    'settings' => 'entity_embed_ctools_export_ui_form',
    'submit' => 'entity_embed_ctools_export_ui_form_submit',
  ),
);

/**
 * Define the preset add/edit form.
 */
function entity_embed_ctools_export_ui_form(&$form, &$form_state) {
  // Without javascript we need an extra "Choose" button, which is hidden with
  // CSS when we have javascript enabled.
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'entity_embed') . '/css/entity_embed.admin.css',
  );

  $preset = $form_state['item'];

  // Determine the currently selected entity type.
  $selected = isset($form_state['values']['entity_type']) ? $form_state['values']['entity_type'] : $preset->entity_type;

  $options = array();
  $entity_types = entity_get_info();

  foreach ($entity_types as $entity_type => $info) {
    $options[$entity_type] = $info['label'];
  }

  $form['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Entity type'),
    '#default_value' => $preset->entity_type,
    '#options' => $options,
    '#empty_option' => t('- Select an entity type -'),
    '#description' => t('The type of entity allowed to be embedded using the button.'),
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'entity_embed_dependent_entity_type_bundles_callback',
    ),
  );

  // The user must select an entity type before the entity type bundles can be
  // determined.
  $form['select_entity_type'] = array(
    '#type' => 'submit',
    '#value' => t('Choose'),
    '#attributes' => array(
      'class' => array('next-button'),
    ),
  );

  $bundle_options = array();
  $formatter_options = array();

  if ($selected) {
    $entity_type = entity_get_info($selected);

    // If the entity has bundles, allow option to restrict to bundle(s).
    if (!empty($entity_type['bundles'])) {
      foreach ($entity_type['bundles'] as $bundle_id => $bundle_info) {
        $bundle_options[$bundle_id] = $bundle_info['label'];
      }
    }

    // Retrieve a list of formatters appropriate for the currently selected
    // entity type.
    $formatters = entity_embed_get_entity_field_formatters($selected);

    if (!empty($formatters)) {
      foreach ($formatters as $formatter => $info) {
        $formatter_options[$info['module'] . ':' . $formatter] = $info['label'];
      }
    }
  }

  if (isset($options[$selected])) {
    $title = t('@entity_type bundles', array('@entity_type' => $options[$selected]));
  }
  else {
    $title = t('Bundles');
  }

  $form['entity_type_bundles'] = array(
    '#type' => 'checkboxes',
    '#title' => $title,
    '#default_value' => $preset->entity_type_bundles,
    '#description' => t('The selected entity type only contains one bundle which is always allowed.'),
    '#prefix' => '<div id="bundle-entity-type-wrapper">',
    '#suffix' => '</div>',
  );

  // Alert the user that they must choose an entity_type before selecting
  // entity_type_bundles.
  if (empty($form_state['values']['entity_type'])) {
    $form['entity_type_bundles']['#description'] = t('You must choose an entity type before selecting its bundles.');
  }

  // The selection is hidden if there's just one option, since that's always
  // going to be allowed.
  if (count($bundle_options) > 1) {
    $form['entity_type_bundles']['#options'] = $bundle_options;
    $form['entity_type_bundles']['#description'] = t('Select one or more bundles to restrict embedding to. If none are selected, all are allowed.');
  }

  $form['button_icon_fid'] = array(
    '#type' => 'managed_file',
    '#title' => t('Button icon'),
    '#default_value' => $preset->button_icon_fid,
    '#description' => t('The icon to be used for the button in the CKEditor toolbar. The default Entity icon will be displayed if a custom icon is not available.'),
    '#upload_location' => file_default_scheme() . '://entity_embed/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
      'file_validate_image_resolution' => array('32x32', '16x16'),
    ),
  );

  // @todo: Abstract formatters into 'display plugins'.
  $form['display_plugins'] = array(
    '#type' => 'checkboxes',
    '#default_value' => $preset->display_plugins ?: array(),
    '#prefix' => '<div id="display-plugins-wrapper">',
    '#suffix' => '</div>',
  );

  // The selection is hidden if there's just one option, since that's always
  // going to be allowed.
  if (count($formatter_options) > 1) {
    // Allow option to limit Entity Embed Display plugins.
    $form['display_plugins'] += array(
      '#title' => t('Allowed Entity Embed Display plugins'),
      '#options' => $formatter_options,
      '#description' => t('If none are selected, all are allowed. Note that these are the plugins which are allowed for this entity type, all of these might not be available for the selected entity.'),
    );
  }

  // Set options to an empty array if it hasn't been set so far.
  if (!isset($form['entity_type_bundles']['#options'])) {
    $form['entity_type_bundles']['#options'] = array();
  }
  if (!isset($form['display_plugins']['#options'])) {
    $form['display_plugins']['#options'] = array();
  }
}

/**
 * Submit function for entity_embed_ctools_export_ui_form().
 */
function entity_embed_ctools_export_ui_form_submit($form, &$form_state) {
  // Filter out any unselected checkboxes.
  if (isset($form_state['values']['entity_type_bundles'])) {
    $form_state['values']['entity_type_bundles'] = array_filter($form_state['values']['entity_type_bundles']);
  }
  if (isset($form_state['values']['display_plugins'])) {
    $form_state['values']['display_plugins'] = array_filter($form_state['values']['display_plugins']);
  }

  // Now handle the case of the next, previous, and submit buttons.
  // only submit will result in actual submission, all others rebuild.
  switch ($form_state['triggering_element']['#value']) {
    case t('Save'):
      // Submit: We're done.
      return;
  }
  // 'Choose' or anything else will cause rebuild of the form and present
  // it again.
  $form_state['rebuild'] = TRUE;
}

/**
 * Replaces the entity_type_bundles and submit button form elements.
 *
 * @return array
 *   An array of ajax commands which replace the entity_type_bundles and submit
 *   button form elemenets.
 */
function entity_embed_dependent_entity_type_bundles_callback($form, $form_state) {
  $commands = array();

  // Update options for entity type bundles.
  $commands[] =  ajax_command_replace("#bundle-entity-type-wrapper", render($form['entity_type_bundles']));

  // Update options for display plugins.
  $commands[] =  ajax_command_replace("#display-plugins-wrapper", render($form['display_plugins']));

  // Update submit button.
  $commands[] =  ajax_command_replace("#save-button", render($form['buttons']['submit']));

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}
