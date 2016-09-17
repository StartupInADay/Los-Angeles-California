<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('Edit Node'),
  'description' => t('Places a node edit form in a specified location.  Requires a node context to be passed to it.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'content form' => 'context_admin_node_edit_menu_content_form',
  'content form submit' => 'context_admin_node_edit_menu_content_form_submit',
  'render' => 'context_admin_node_edit_menu_render_page',
);


function context_admin_node_edit_menu_content_form($form, $form_state) {
  $conf = $form_state['conf'];
  if (module_exists('panels')) {
    if (isset($conf['context_admin_use_node_edit'])) {
      $node_edit = $conf['context_admin_use_node_edit'];
    }
    else {
      $node_edit = NULL;
    }

    $form['context_admin_use_node_edit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Render the Edit form using panels and page manager'),
      '#description' => t('Enable node/%node/edit in page manager. Unchecked renders Edit form with Drupal\'s node templating system.'),
      '#default_value' => $node_edit,
    );
  }
  return $form;
}

function context_admin_node_edit_menu_content_form_submit($form, &$form_state) {
  if (isset($form_state['values']['context_admin_use_node_edit'])) {
    $form_state['conf']['context_admin_use_node_edit'] = $form_state['values']['context_admin_use_node_edit'];
  }
}

function context_admin_node_edit_menu_render_page($handler, $contexts, $args, $test = TRUE) {
  $node = $contexts[$handler->conf['submitted_context']]->data;
  $use_panels = FALSE;
  if (isset($handler->conf['context_admin_use_node_edit'])) {
    $use_panels = $handler->conf['context_admin_use_node_edit'];
  }
  if ($use_panels && module_exists('panels')) {
    // Render form using Page Manager's node/%node/edit page
    // Requires node/%node/edit page to be enabled.  Check for this and disable checkbox above?  FIXME
    ctools_include("plugins");
    $plugin = ctools_get_plugins("page_manager", "tasks", "node_edit");
    global $user;
    $type_name = node_type_get_name($node);
    drupal_set_title(t('<em>Edit @type</em> @title', array('@type' => $type_name, '@title' => $node->title)), PASS_THROUGH);
    return page_manager_node_edit($node);
  }
  module_load_include('inc', 'node', 'node.pages');
  return node_page_edit($node);
}
