<?php

include_once('panels_preview.edit.ajax.inc');
include_once('panels_preview.edit.ctools_entity.inc');
include_once('panels_preview.edit.form_alter.inc');
include_once('panels_preview.edit.fpp.inc');
include_once('panels_preview.edit.panels_style.inc');
include_once('panels_preview.edit.stylizer.inc');
include_once('panels_preview.edit.views.inc');

/**
 * Recursively parse form elements to add special autosubmit handling on a per field-type basis.
 */
function panels_preview_autosubmit_configure(&$element) {
  if (!empty($element['#type'])) {
    switch ($element['#type']) {
      case 'textfield':
        // Special handling for autosubmit.
        if (!empty($element['#autocomplete_path'])) {
          $element['#attributes']['class'][] = 'ctools-auto-submit-exclude panels-preview-autocomplete-autosubmit';
        }
        else {
          $element['#attributes']['class'][] = 'ctools-auto-submit-exclude panels-preview-textfield-autosubmit';
        }
        break;
      case 'text_format':
        $element['#attributes']['class'][] = 'ctools-auto-submit-exclude panels-preview-textarea-autosubmit';
        break;
      default:
        break;
    }
  }

  $children = element_children($element);

  if (!empty($children) && is_array($children)) {
    foreach ($children as $child) {
      panels_preview_autosubmit_configure($element[$child]);
    }
  }
}

/**
 * Add the preview to the form output.
 *
 * It is done here so the form is fully processed.
 */
function panels_preview_form_post_render_preview($output, $form) {
  extract($form['#panels_preview_preview_info']);

  // If no preview type was specified, render the pane as normal.
  if (empty($preview_callback)) {
    $preview_callback = 'ctools_content_render';
  }
  $content = $preview_callback($pane->type, $pane->subtype, $configuration, $keywords, $args, $context);

  // Create the fieldset with appropriate content.
  $preview = array(
    '#type' => 'fieldset',
    '#title' => 'Preview',
    '#attributes' => array(
      'id' => 'panels-preview-form-widget-preview',
      'class' => array('widget-preview', 'widget-preview-single'),
    ),
    '#collapsible' => FALSE,
    '#weight' => -100,
  );
  if (!empty($content)) {
    $preview['preview']['#markup'] = (!empty($style['render pane'])) ? theme($style['render pane'], array('content' => $content, 'pane' => $pane, 'display' => $display, 'style' => $style, 'settings' => $pane->style['settings'])) : theme('panels_pane', array('content' => $content, 'pane' => $pane, 'display' => $display));
  }
  else {
    $preview['preview']['#markup'] = t('[no preview]');
  }
  return  drupal_render($preview) . $output;
}

/**
 * Helper function to get view modes
 *
 * @param (optional) string $entity_type
 *   The type of entity for which to load view modes.
 */
function panels_preview_view_mode_options($entity_type = 'node') {
  $entity_info = entity_get_info($entity_type);
  $hidden_view_modes = array_filter(array_map('trim', explode("\n", variable_get('panels_preview_hidden_view_mode_options', PANELS_PREVIEW_HIDDEN_VIEW_MODE_OPTIONS))));

  $options = array();
  if (!empty($entity_info['view modes'])) {
    foreach ($entity_info['view modes'] as $mode => $settings) {
      if (!in_array($mode, $hidden_view_modes)) {
        $options[$mode] = $settings['label'];
      }
    }
  }
  return $options;
}

/**
 * Custom submit handler to save panels pane configuration for styling.
 */
function panels_preview_ctools_content_type_modal_submit(&$form, &$form_state) {
  $move = array('view_settings', 'header_type', 'view_mode', 'widget_title');
  foreach ($move as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}
