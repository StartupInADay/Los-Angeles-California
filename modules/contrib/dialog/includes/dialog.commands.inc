<?php

/**
 * @file
 * AJAX commands.
 */

/**
 * Creates a Drupal Ajax 'close dialog' command.
 *
 * @param string $selector
 *   A CSS selector string of the dialog to close.
 * @param bool $persist
 *   (optional) Whether to persist the dialog in the DOM or not.
 *
 * @return array
 *   An array suitable for use with the ajax_render() function.
 */
function dialog_command_close_dialog($selector = NULL, $persist = FALSE) {
  return array(
    'command' => 'closeDialog',
    'selector' => $selector ? $selector : '#drupal-modal',
    'persist' => $persist,
  );
}

/**
 * Creates a Drupal Ajax 'close modal dialog' command.
 *
 * @param bool $persist
 *   (optional) Whether to persist the dialog in the DOM or not.
 *
 * @return array
 *   An array suitable for use with the ajax_render() function.
 */
function dialog_command_close_modal_dialog($persist = FALSE) {
  return dialog_command_close_dialog('#drupal-modal', $persist);
}

/**
 * Creates a Drupal Ajax 'open dialog' command.
 *
 * @param string $selector
 *   The selector of the dialog.
 * @param string $title
 *   The title of the dialog.
 * @param string|array $content
 *   The content that will be placed in the dialog, either a render array
 *   or an HTML string.
 * @param array $dialog_options
 *   (optional) Options to be passed to the dialog implementation. Any
 *   jQuery UI option can be used. See http://api.jqueryui.com/dialog.
 * @param array|null $settings
 *   (optional) Custom settings that will be passed to the Drupal behaviors
 *   on the content of the dialog. If left empty, the settings will be
 *   populated automatically from the current request.
 *
 * @return array
 *   An array suitable for use with the ajax_render() function.
 */
function dialog_command_open_dialog($selector, $title, $content, array $dialog_options = array(), $settings = NULL) {
  // Add the library for handling the dialog in the response.
  drupal_add_library('dialog', 'drupal.dialog.ajax');

  $dialog_options += array('title' => $title);

  if (is_array($content)) {
    $html = drupal_render($content);
    $content = $html;
  }

  // For consistency ensure the modal option is set to TRUE or FALSE.
  $dialog_options['modal'] = isset($dialog_options['modal']) && $dialog_options['modal'];

  return array(
    'command' => 'openDialog',
    'selector' => $selector,
    'settings' => $settings,
    'data' => $content,
    'dialogOptions' => $dialog_options,
  );
}

/**
 * Creates a Drupal Ajax 'open modal dialog' command.
 *
 * The modal dialog differs from the normal modal provided by
 * dialog_command_open_dialog in that a modal prevents other interactions
 * on the page until the modal has been completed. Drupal provides a built-in
 * modal for this purpose, so no selector needs to be provided.
 *
 * @param string $title
 *   The title of the dialog.
 * @param string|array $content
 *   The content that will be placed in the dialog, either a render array
 *   or an HTML string.
 * @param array $dialog_options
 *   (optional) Settings to be passed to the dialog implementation. Any
 *   jQuery UI option can be used. See http://api.jqueryui.com/dialog.
 * @param array|null $settings
 *   (optional) Custom settings that will be passed to the Drupal behaviors
 *   on the content of the dialog. If left empty, the settings will be
 *   populated automatically from the current request.
 *
 * @return array
 *   An array suitable for use with the ajax_render() function.
 */
function dialog_command_open_modal_dialog($title, $content, array $dialog_options = array(), $settings = NULL) {
  $dialog_options['modal'] = TRUE;

  return dialog_command_open_dialog('#drupal-modal', $title, $content, $dialog_options, $settings);
}

/**
 * Creates a Drupal Ajax 'set dialog option' command.
 *
 * @param string $selector
 *   The selector of the dialog whose title will be set. If set to an empty
 *   value, the default modal dialog will be selected.
 * @param string $option_name
 *   The name of the option to set. May be any jQuery UI dialog option.
 *   See http://api.jqueryui.com/dialog.
 * @param mixed $option_value
 *   The value of the option to be passed to the dialog.
 *
 * @return array
 *   An array suitable for use with the ajax_render() function.
 */
function dialog_command_set_dialog_option($selector, $option_name, $option_value) {
  return array(
    'command' => 'setDialogOption',
    'selector' => $selector ? $selector : '#drupal-modal',
    'optionName' => $option_name,
    'optionValue' => $option_value,
  );
}

/**
 * Creates a Drupal Ajax 'set dialog title' command.
 *
 * @param string $selector
 *   The selector of the dialog whose title will be set. If set to an empty
 *   value, the default modal dialog will be selected.
 * @param string $title
 *   The title that will be set on the dialog.
 *
 * @return array
 *   An array suitable for use with the ajax_render() function.
 */
function dialog_command_set_dialog_title($selector, $title) {
  return dialog_command_set_dialog_option($selector, 'title', $title);
}

/**
 * Provides an AJAX command to redirect the page.
 *
 * @param string $url
 *   The URL that will be loaded into window.location. This should be a full
 *   URL, one that has already been run through the url() function.
 * @return array
 *   An array suitable for use with the ajax_render() function.
 */
function dialog_command_redirect($url) {
  return array(
    'command' => 'redirect',
    'url' => $url,
  );
}

/**
 * Provides an AJAX command to reload the page.
 *
 * @return array
 *   An array suitable for use with the ajax_render() function.
 */
function dialog_command_reload() {
  return array(
    'command' => 'reload',
  );
}
