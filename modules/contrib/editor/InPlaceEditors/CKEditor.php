<?php

/**
 * @file
 * Defines the "ckeditor" in-place editor.
 *
 * @see Drupal 8's \Drupal\editor\Plugin\InPlaceEditor\Editor.
 */

module_load_include('php', 'quickedit', 'includes/QuickEditInPlaceEditorInterface');

/**
 * Defines the CKEditor in-place editor.
 */
class CKEditor implements QuickEditInPlaceEditorInterface {

  /**
   * Implements QuickEditInPlaceEditorInterface::isCompatible().
   *
   * @see Drupal 8's \Drupal\editor\Plugin\quickedit\editor\Editor::isCompatible().
   */
  public function isCompatible(array $instance, array $items) {
    $field = field_info_field($instance['field_name']);

    // This editor is incompatible with multivalued fields.
    if ($field['cardinality'] != 1) {
      return FALSE;
    }
    // This editor is compatible with processed ("rich") text fields; but only
    // if there is a currently active text format, and that text format has an
    // associated CKEditor profile.
    elseif (!empty($instance['settings']['text_processing'])) {
      $format_id = $items[0]['format'];

      if ($format = filter_format_load($format_id)) {
        editor_format_ensure_additional_properties($format);

        if ($format->editor == 'ckeditor') {
          return TRUE;
        }
      }

      return FALSE;
    }
  }

  /**
   * Implements QuickEditInPlaceEditorInterface::getMetadata().
   *
   * @see Drupal 8's \Drupal\editor\Plugin\quickedit\editor\Editor::getMetadata().
   */
  public function getMetadata(array $instance, array $items) {
    $format_id = $items[0]['format'];
    $metadata['format'] = $format_id;
    $metadata['formatHasTransformations'] = (bool) count(array_intersect(array(FILTER_TYPE_TRANSFORM_REVERSIBLE, FILTER_TYPE_TRANSFORM_IRREVERSIBLE), filter_get_filter_types_by_format($format_id)));
    return $metadata;
  }

  /**
   * Implements QuickEditInPlaceEditorInterface::getAttachments().
   *
   * @see Drupal 8's \Drupal\editor\Plugin\quickedit\editor\Editor::getAttachments().
   */
  public function getAttachments() {
    global $user;

    // Get a list of formats to which the current user has access.
    $formats = filter_formats($user);

    // Get filter configuration information for the available formats.
    $settings = editor_get_attached($formats);

    // Also include editor.module's formatted text editor.
    $settings['library'][] = array('editor', 'quickedit.inPlaceEditor.formattedText');
    $settings['js'][] = array(
      'type' => 'setting',
      'data' => array(
        'quickedit' => array(
          'ckeditor' => array(
            'basePath' => base_path() . drupal_get_path('module', 'editor_ckeditor') . '/lib/ckeditor/',
          ),
        ),
      ),
    );

    return $settings;
  }

}
