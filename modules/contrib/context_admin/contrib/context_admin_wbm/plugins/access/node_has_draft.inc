<?php
/**
 * @file
 * Provides access handler based upon workbench_moderation for the node type.
 */

$plugin = array(
  'title' => t("Node: has workbench_moderation draft"),
  'callback' => 'context_admin_wbm_node_has_draft_access_check',
  'default' => array('menu' => array()),
  'settings form' => 'context_admin_wbm_node_has_draft_access_settings',
  'settings form submit' => 'context_admin_wbm_node_has_draft_access_settings_submit',
  'summary' => 'context_admin_wbm_node_has_draft_access_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Plugin configuration form
 *
 * @param $form
 * @param $form_state
 * @param $conf
 *
 * @return mixed
 */
function context_admin_wbm_node_has_draft_access_settings($form, &$form_state, $conf) {
  return $form;
}

/**
 * Plugin configuration form submission
 *
 * @param $form
 * @param $form_state
 */
function context_admin_wbm_node_has_draft_access_settings_submit(&$form, &$form_state) {
}

/**
 * Check for access.
 */
function context_admin_wbm_node_has_draft_access_check($conf, $context) {
  // Per the example in node type in core ctools
  if (empty($context) || empty($context->data) || empty($context->data->type)) {
    return FALSE;
  }

  $node = $context->data;
  $draft = workbench_moderation_node_current_load($node);
  return $node->vid != $draft->vid;
}

/**
 * Provide a summary description based upon the checked node_types.
 */
function context_admin_wbm_node_has_draft_access_summary($conf, $context) {
  return t('@identifier has a moderated draft', array('@identifier' => $context->identifier));
}
