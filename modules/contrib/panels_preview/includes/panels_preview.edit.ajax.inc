<?php

/**
 * Implements hook_menu()
 */
function panels_preview_menu() {
  $items = array();
  $items['system/panels-preview'] = array(
    'title' => 'AHAH callback',
    'page callback' => 'panels_preview_ajax_form_callback',
    'delivery callback' => 'ajax_deliver',
    'access callback' => TRUE,
    'theme callback' => 'ajax_base_page_theme',
    'type' => MENU_CALLBACK,
    'file path' => 'includes',
    'file' => 'form.inc',
  );
  return $items;
}

/**
 * Replaces system/ajax for pane configuration preview callback to work with
 * ctools multi step form.
 */
function panels_preview_ajax_form_callback() {
  list($form, $form_state, $form_id, $form_build_id, $commands) = ajax_get_form();
  ctools_include('content');
  ctools_include('modal');

  if (isset($form_state['no_redirect']) && !isset($form_state['form_info'])) {
    $form_state += array(
      're_render' => FALSE,
      'no_redirect' => !empty($form_state['ajax']),
     );
     $form = drupal_build_form($form_state['build_info']['form_id'], $form_state);
  }
  else {
    // These ensure the class files/form definations are included.
    $content_type = ctools_get_content_type($form_state['pane']->type);
    $subtype = ctools_content_get_subtype($content_type, $form_state['subtype_name']);

    $form = ctools_content_form($form_state['op'], $form_state['form_info'], $form_state, $form_state['plugin'], $form_state['subtype_name'], $form_state['conf'], $form_state['step']);
  }

  if (!empty($form_state['triggering_element'])) {
    $callback = $form_state['triggering_element']['#ajax']['callback'];
  }
  if (!empty($callback) && function_exists($callback)) {
    return $callback($form, $form_state);
  }
}

/**
 * Implements hook_menu_alter().
 */
function panels_preview_menu_alter(&$items) {
  // Include entity_field.inc from ctools to ensure that files can be properly removed
  // from within the panelizer edit forms.
  $items['file/ajax']['file'] = 'entity_field.inc';
  $items['file/ajax']['file path'] = drupal_get_path('module', 'ctools') . '/plugins/content_types/entity_context';
}

/**
 * Content panes should not use default system/ajax. Use our own for now.
 */
function _panels_preview_add_path_to_ajax($element) {
  if (!empty($element['#ajax']) && !isset($element['#ajax']['path'])) {
    $element['#ajax']['path'] = 'system/panels-preview';
  }
  foreach (element_children($element) as $key) {
    _panels_preview_add_path_to_ajax($element[$key]);
  }
}

/**
 * Ajax callback that just returns the rendered preview.
 */
function panels_preview_ajax_update_preview($form, $form_state) {
  if (isset($form_state['values']) && isset($form['#panels_preview_preview_info']['configuration'])) {
    $form['#panels_preview_preview_info']['configuration'] = $form_state['values'] + $form['#panels_preview_preview_info']['configuration'];
  }
  return panels_preview_form_post_render_preview('', $form);
}
