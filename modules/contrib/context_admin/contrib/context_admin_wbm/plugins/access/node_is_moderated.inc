<?php
/**
 * @file
 * Provides access handler based upon workbench_moderation for the node type.
 */

$plugin = array(
  'title' => t("Node: is workbench_moderation moderated"),
  'callback' => 'context_admin_wbm_node_is_moderated_access_check',
  'default' => array('menu' => array()),
  'settings form' => 'context_admin_wbm_node_is_moderated_access_settings',
  'settings form submit' => 'context_admin_wbm_node_is_moderated_access_settings_submit',
  'summary' => 'context_admin_wbm_node_is_moderated_access_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Plugin configuration form.
 *
 * @param $form
 * @param $form_state
 * @param $conf
 *
 * @return mixed
 */
function context_admin_wbm_node_is_moderated_access_settings($form, &$form_state, $conf) {
  return $form;
}

/**
 * Plugin configuration form submission
 *
 * @param $form
 * @param $form_state
 */
function context_admin_wbm_node_is_moderated_access_settings_submit(&$form, &$form_state) {
}

/**
 * Check for access.
 */
function context_admin_wbm_node_is_moderated_access_check($conf, $context) {
  // Per the example in node type in core ctools
  if (empty($context) || empty($context->data) || empty($context->data->type)) {
    return FALSE;
  }

  return workbench_moderation_node_type_moderated($context->data->type);
}

/**
 * Provide a summary description based upon the checked node_types.
 */
function context_admin_wbm_node_is_moderated_access_summary($conf, $context) {
  return t('@identifier is a moderated node type', array('@identifier' => $context->identifier));
}
