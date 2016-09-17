<?php

/**
 * @file
 * Editor CKEditor integration for the entity_embed module.
 */

/**
 * Implements hook_editor_ckeditor_plugins().
 */
function entity_embed_editor_ckeditor_plugins() {
  $plugins = array(
    'drupalentity' => array(
      'path' => drupal_get_path('module', 'entity_embed') . '/js/plugins/drupalentity',
      'file' => 'plugin.js',
      'required_html' => array(
        array(
          'tags' => array('drupal-entity'),
          'attributes' => array('data-entity-type', 'data-entity-id', 'data-entity-uuid', 'data-view-mode', 'data-entity-embed-display', 'data-entity-embed-settings', 'data-entity-label', 'data-embed-button'),
        ),
      )
    ),
  );

  // Add user-defined buttons.
  $embed_buttons = entity_embed_embed_button_load_all();

  $buttons = array();

  foreach ($embed_buttons as $embed_button) {
    $fid = NULL;

    // Use a custom icon when available.
    if (!empty($embed_button->button_icon_fid)) {
      $fid = $embed_button->button_icon_fid;
    }

    $icon = _entity_embed_button_image($fid);

    $buttons[$embed_button->name] = array(
      'label' => check_plain($embed_button->admin_title),
      'image' => $icon,
    );
  }

  $plugins['drupalentity']['buttons'] = $buttons;

  return $plugins;
}

/**
 * Implements hook_editor_ckeditor_css_alter().
 */
function entity_embed_editor_ckeditor_css_alter(array &$css, $format) {
  // Add styles for handling contextual links.
  if (module_exists('contextual')) {
    $css[] = drupal_get_path('module', 'contextual') . '/contextual.css';
  }
}
