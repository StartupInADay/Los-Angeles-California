<?php

/**
 * @file
 * Quick Edit hooks implemented by the Quick Edit module.
 */

/**
 * Implements hook_quickedit_editor_info().
 *
 * @see Drupal 8's \Drupal\quickedit\Plugin\InPlaceEditorManager.
 * @see Drupal 8's \Drupal\quickedit\Plugin\InPlaceEditorInterface.
 */
function quickedit_quickedit_editor_info() {
  $editors = array();
  $path = drupal_get_path('module', 'quickedit') . '/InPlaceEditors';
  $editors['form'] = array(
    'file' => $path . '/formEditor.php',
    'class' => 'FormEditor',
  );
  $editors['plain_text'] = array(
    'file' => $path . '/plainTextEditor.php',
    'class' => 'PlainTextEditor',
  );
  if (module_exists('ckeditor')) {
    $editors['ckeditor'] = array(
      'alternativeTo' => array('plain_text'),
      'file' => $path . '/CKEditor.php',
      'class' => 'CKEditor',
    );
  }
  return $editors;
}
