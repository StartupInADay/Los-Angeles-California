<?php

/**
 * @file
 * Documentation for Editor module APIs.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define text editors, such as WYSIWYGs or toolbars to assist with text input.
 *
 * Text editors are bound to an individual text format. When a format is
 * activated in a 'text_format' element, the text editor associated with the
 * format should be activated on the text area.
 *
 * @return array
 *   An associative array of editors, whose keys are internal editor names,
 *   which should be unique and therefore prefixed with the name of the module.
 *   Each value is an associative array describing the editor, with the
 *   following elements (all are optional except as noted):
 *   - title: (required) A human readable name for the editor.
 *   - settings callback: The name of a function that returns configuration
 *     form elements for the editor. See hook_editor_EDITOR_settings() for
 *     details.
 *   - default settings: An associative array containing default settings for
 *     the editor, to be applied when the editor has not been configured yet.
 *   - js settings callback: The name of a function that returns configuration
 *     options that should be added to the page via JavaScript for use on the
 *     client side. See hook_editor_EDITOR_js_settings() for details.
 *
 * @see filter_example.module
 * @see hook_filter_info_alter()
 */
function hook_editor_info() {
  $editors['myeditor'] = array(
    'title' => t('My Editor'),
    'settings callback' => '_myeditor_settings',
    'default settings' => array(
      'enable_toolbar' => TRUE,
      'toolbar_buttons' => array('bold', 'italic', 'underline', 'link', 'image'),
      'resizeable' => TRUE,
    ),
    'js settings callback' => '_myeditor_js_settings',
  );

  return $editors;
}

/**
 * Perform alterations on editor definitions.
 *
 * @param array $editors
 *   Array of information on editors exposed by hook_editor_info()
 *   implementations.
 */
function hook_editor_info_alter(&$editors) {
  $editors['some_other_editor']['title'] = t('A different name');
}

/**
 * Perform alterations on the JavaScript settings that are added for filters.
 *
 * Note that changing settings here only affects the client side behavior of the
 * filter. To affect the filter globally both on the client side and server
 * side, use hook_filter_info_alter().
 *
 * @param array $settings
 *   All the settings that will be added to the page via drupal_add_js() for
 *   the text formats to which a user has access.
 */
function hook_filter_js_settings_alter(&$settings) {
  $settings['full_html']['allowed_tags'][] = 'strong';
  $settings['full_html']['allowed_tags'][] = 'em';
  $settings['full_html']['allowed_tags'][] = 'img';
}

/**
 * @} End of "addtogroup hooks".
 */
