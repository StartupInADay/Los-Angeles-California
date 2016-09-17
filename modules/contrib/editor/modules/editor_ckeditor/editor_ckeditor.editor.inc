<?php

/**
 * @file
 * Editor hooks implemented by the Editor CKEditor module.
 */

/**
 * Implements hook_editor_info().
 */
function editor_ckeditor_editor_info() {
  $editors['ckeditor'] = array(
    'label' => t('CKEditor'),
    'library' => array('editor_ckeditor', 'drupal.ckeditor'),
    'default settings' => array(
      'toolbar' => array(
        array(
          array(
            'name' => 'Formatting',
            'items' => array('Bold', 'Italic'),
          ),
          array(
            'name' => 'Linking',
            'items' => array('Link', 'Unlink'),
          ),
          array(
            'name' => 'Alignment',
            'items' => array('JustifyLeft', 'JustifyCenter', 'JustifyRight'),
          ),
          array(
            'name' => 'Lists',
            'items' => array('BulletedList', 'NumberedList'),
          ),
          array(
            'name' => 'Media',
            'items' => array('Blockquote', 'DrupalImage'),
          ),
          array(
            'name' => 'Tools',
            'items' => array('Source', 'Maximize'),
          ),
        ),
      ),
      'format_list' => array('p', 'h1', 'h2', 'h3', 'h4', 'h5', 'h6'),
      'style_list' => array(),
    ),
    'file' => 'includes/editor_ckeditor.admin.inc',
    'settings callback' => 'editor_ckeditor_settings_form',
    'js settings callback' => 'editor_ckeditor_get_settings',
  );

  return $editors;
}
