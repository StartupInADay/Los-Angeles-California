<?php

/**
 * @file
 * AJAX commands.
 */

/**
 * Creates a Drupal Ajax 'editor dialog save' command.
 *
 * @param string $values
 *   The values that should be passed to the form constructor in Drupal.
 *
 * @return array
 *   An array suitable for use with the ajax_render() function.
 */
function editor_command_editor_dialog_save($values) {
  return array(
    'command' => 'editorDialogSave',
    'values' => $values,
  );
}
