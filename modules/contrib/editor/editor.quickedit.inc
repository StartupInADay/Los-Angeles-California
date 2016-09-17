<?php

/**
 * @file
 * Quick Edit hooks implemented by the Editor module.
 */

/**
 * Implements hook_quickedit_editor_info().
 *
 * @see Drupal 8's \Drupal\quickedit\Plugin\InPlaceEditorManager.
 * @see Drupal 8's \Drupal\quickedit\Plugin\InPlaceEditorInterface.
 */
function editor_quickedit_editor_info() {
  $path = drupal_get_path('module', 'editor') . '/InPlaceEditors';

  return array(
    'ckeditor' => array(
      'alternativeTo' => array('plain_text'),
      'file' => $path . '/CKEditor.php',
      'class' => 'CKEditor',
    )
  );
}
