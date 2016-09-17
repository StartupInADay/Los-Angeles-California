<?php
/**
 * Contains workbench_moderation_node_edit.inc.
 */

$plugin = array(
  'title' => t('Workbench Moderation Edit Node'),
  'description' => t('Places a workbench_moderation node edit form in a specified location.  Requires a node context to be passed to it.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'content form' => 'context_admin_wbm_workbench_moderation_node_edit_menu_content_form',
  'content form submit' => 'context_admin_wbm_workbench_moderation_node_edit_menu_content_form_submit',
  'render' => 'context_admin_wbm_workbench_moderation_node_edit_menu_render_page',
);


/**
 * There's no configuration for this plugin type, so we just return the form.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function context_admin_wbm_workbench_moderation_node_edit_menu_content_form($form, $form_state) {
  return $form;
}

/**
 * Placeholder so that there's something to call.
 *
 * @param array $form
 * @param array $form_state
 */
function context_admin_wbm_workbench_moderation_node_edit_menu_content_form_submit($form, &$form_state) {
}

/**
 * The render callback for the plugin.
 *
 * @param $handler
 * @param $contexts
 * @param $args
 * @param bool $test
 *
 * @return array
 */
function context_admin_wbm_workbench_moderation_node_edit_menu_render_page($handler, $contexts, $args, $test = TRUE) {
  $node = $contexts[$handler->conf['submitted_context']]->data;
  return workbench_moderation_node_edit_page_override($node);
}
