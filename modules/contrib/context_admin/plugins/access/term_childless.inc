<?php
//$Id: term_childless.inc,v 1.1 2010/03/12 17:51:58 eclipsegc Exp $

/**
 * @file
 * Provides access handler based upon the lack of child terms for the term we're on.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Taxonomy: term childless"),
  'description' => t('Control access based upon the lack of children on the current term.'),
  'callback' => 'context_admin_term_childless_access_check',
  'default' => array('menu' => array()),
  'settings form' => 'context_admin_term_childless_access_settings',
  'settings form submit' => 'context_admin_term_childless_access_settings_submit',
  'summary' => 'context_admin_term_childless_access_summary',
  'required context' => new ctools_context_required(t('Term'), array('taxonomy_term', 'term', 'terms')),
);

function context_admin_term_childless_access_settings($form, &$form_state, $conf) {
  $form['settings']['childless'] = array(
    '#type' => 'markup',
    '#value' => '<p>'. t('Provides a access check against the current term to make sure it is not the parent of any other terms.') .'</p>',
  );
  return $form;
}

function context_admin_term_childless_access_settings_submit(&$form, &$form_state) {

}

/**
 * Check for access.
 */
function context_admin_term_childless_access_check($conf, $context) {
  // Per the example in node type in core ctools
  if (empty($context) || empty($context->data) || empty($context->data->tid)) {
    return FALSE;
  }

  $query = db_select('taxonomy_term_hierarchy', 'th')
    ->fields('th', array('tid'))
    ->condition('parent', $context->data->tid, '=');
  $result = $query->execute()->fetchObject();
  if ($result) {
    return FALSE;
  }
  else {
    return TRUE;
  }
  return FALSE;
}

/**
 * Provide a summary description based upon the checked node_types.
 */
function context_admin_term_childless_access_summary($conf, $context) {
  return t('@identifier is a term with no children"', array('@identifier' => $context->identifier));
}
