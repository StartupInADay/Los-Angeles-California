<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('View Node'),
  'description' => t('Places a node view in a specified menu location.  Requires a node context to be passed to it.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'content form' => 'context_admin_node_view_menu_content_form',
  'content form submit' => 'context_admin_node_view_menu_content_form_submit',
  'render' => 'context_admin_node_view_menu_render_page',
);

function context_admin_node_view_menu_content_form($form, $form_state) {
  $conf = $form_state['conf'];
  if (module_exists('panels')) {
    $form['context_admin_use_node_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Render the node using the page manager node override.'),
      '#description' => t('Enable node/%node in page manager. Unchecked renders with the node templating system.'),
      '#default_value' => isset($conf['context_admin_use_node_view']) ? $conf['context_admin_use_node_view'] : NULL,
    );
  }
  return $form;
}

function context_admin_node_view_menu_content_form_submit($form, &$form_state) {
  if (isset($form_state['values']['context_admin_use_node_view'])) {
    $form_state['conf']['context_admin_use_node_view'] = $form_state['values']['context_admin_use_node_view'];
  }
}

function context_admin_node_view_menu_render_page($handler, $contexts, $args, $test = TRUE) {
  $node = $contexts[$handler->conf['submitted_context']]->data;
  $use_panels = FALSE;
  if (isset($handler->conf['context_admin_use_node_view'])) {
    $use_panels = $handler->conf['context_admin_use_node_view'];
  }
  if ($use_panels && module_exists('panels')) {
    // Render form using Page Manager's node/%node/edit page
    // Requires node/%node/edit page to be enabled.  Check for this and disable checkbox above?  FIXME
    ctools_include("plugins");
    $plugin = ctools_get_plugins("page_manager", "tasks", "node_view");
    return page_manager_node_view_page($node);
  }
  return node_view($node);
}
