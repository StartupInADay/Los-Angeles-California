<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('Edit Node Path'),
  'description' => t('Places a node path edit form on a specified menu location.  Requires a node context to be passed to it.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'content form' => 'context_admin_node_path_alias_content_form',
  'content form submit' => 'context_admin_node_path_alias_content_form_submit',
  'render' => 'context_admin_node_path_alias_render_page',
);

function context_admin_node_path_alias_content_form($form, $form_state) {
  return $form;
}

function context_admin_node_path_alias_content_form_submit($form, &$form_state) {
}

function context_admin_node_path_alias_render_page($handler, $contexts, $args, $test = TRUE) {
  $node = $contexts[$handler->conf['submitted_context']]->data;
  return drupal_get_form('context_admin_node_path_alias_form', $node);
}

function context_admin_node_path_alias_form($form, $form_state) {
  $form['#node'] = $form_state['build_info']['args'][0];
  // We just want path.module's form element for nodes, so we pretend to be one
  // and call the alter directly.
  path_form_node_form_alter($form, $form_state);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 100,
  );
  return $form;
}

function context_admin_node_path_alias_form_submit($form, &$form_state) {
  $node = $form['#node'];
  $node->path = $form_state['values']['path'];
  $node->path['pathauto'] = 0;
  node_save($node);
}
