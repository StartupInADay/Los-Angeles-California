<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('Workbench Moderation Node Draft'),
  'description' => t('Places a workbench_moderation node draft on a custom page.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'content form' => 'context_admin_wbm_workbench_moderation_node_draft_content_form',
  'content form submit' => 'context_admin_wbm_workbench_moderation_node_draft_content_form_submit',
  'render' => 'context_admin_wbm_workbench_moderation_node_draft_render_page',
);

/**
 * The plugin configuration form.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function context_admin_wbm_workbench_moderation_node_draft_content_form($form, $form_state) {
  $conf = $form_state['conf'];
  $form['panelizer_edit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Panelizer Edits'),
    '#description' => t('Allow panelizer edits to be performed on the view of this revision page.'),
    '#default_value' => !empty($conf['panelizer_edit']) ? $conf['panelizer_edit'] : FALSE,
  );
  return $form;
}

/**
 * Configuration form submission.
 *
 * @param $form
 * @param $form_state
 */
function context_admin_wbm_workbench_moderation_node_draft_content_form_submit($form, &$form_state) {
  if (isset($form_state['values']['panelizer_edit'])) {
    $form_state['conf']['panelizer_edit'] = $form_state['values']['panelizer_edit'];
  }
}

/**
 * The plugin's render callback
 *
 * @param $handler
 * @param $contexts
 * @param $args
 * @param bool $test
 *
 * @return array
 */
function context_admin_wbm_workbench_moderation_node_draft_render_page($handler, $contexts, $args, $test = TRUE) {
  $node = $contexts[$handler->conf['submitted_context']]->data;
  module_load_include('inc', 'workbench_moderation', 'workbench_moderation.node');
  $page = page_manager_get_current_page();
  context_admin_get_current_page($page);
  return workbench_moderation_node_view_draft($node);
}
