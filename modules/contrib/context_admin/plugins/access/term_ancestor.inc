<?php
//$Id: term_ancestor.inc,v 1.1 2010/06/25 22:53:32 eclipsegc Exp $

/**
 * @file
 * Plugin to provide access control based upon a term's parent tid.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Taxonomy: term ancestor"),
  'description' => t('Control access based upon the a term, or group of terms being in the ancestry of the currently viewed term.'),
  'callback' => 'context_admin_term_ancestor_access_check',
  'default' => array('menu' => array()),
  'settings form' => 'context_admin_term_ancestor_access_settings',
  'settings form submit' => 'context_admin_term_ancestor_access_settings_submit',
  'summary' => 'context_admin_term_ancestor_access_summary',
  'required context' => new ctools_context_required(t('Term'), array('taxonomy_term', 'term', 'terms')),
  'restrictions' => 'context_admin_term_ancestor_access_restrictions',
);

/**
 * Settings form for the 'by node_type' access plugin
 */
function context_admin_term_ancestor_access_settings($form, &$form_state, $conf) {
  $form['settings']['ancestors'] = array(
    '#title' => t('Term ancestor(s)'),
    '#type' => 'textfield',
    '#description' => t('Enter a numeric value for the ancestor(s) of the term you wish to utilize.  Multiple terms may be comma separated.'),
    '#default_value' => $conf['ancestors'],
    '#required' => TRUE,
  );
  $form['settings']['and_or'] = array(
    '#title' => t('All or One'),
    '#type' => 'radios',
    '#options' => array(
      'and' => t('All'),
      'or' => t('One'),
    ),
    '#description' => t('Choose whether all terms must be an ancestor of the current term, or if just one of them needs to be an ancestor.'),
    '#default_value' => $conf['and_or'],
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Compress the node_types allowed to the minimum.
 */
function context_admin_term_ancestor_access_settings_submit(&$form, &$form_state) {

}

/**
 * Check for access.
 */
function context_admin_term_ancestor_access_check($conf, $context) {
  // Per the example in node type in core ctools
  if (empty($context) || empty($context->data) || empty($context->data->tid)) {
    return FALSE;
  }

  $ancestors = explode(',', $conf['ancestors']);
  foreach ($ancestors as $key => $ancestor) {
    $ancestors[$key] = trim($ancestor);
  }

  if ($conf['and_or'] == 'or') {
    return context_admin_term_ancestor_access_check_or($context->data->tid, $ancestors);
  }
  elseif($conf['and_or'] == 'and') {
    return context_admin_term_ancestor_access_check_and($context->data->tid, $ancestors);
  }

  if (empty($conf['ancestors'])) {
    return FALSE;
  }

  return FALSE;
}

function context_admin_term_ancestor_access_check_or($term, $ancestors) {
  $query = db_select( 'taxonomy_term_hierarchy', 'th')
    ->fields('th', array('parent'))
    ->condition('tid', $term, '=');
  $result = $query->execute()->fetchObject();
  if ($result->parent) {
    if (in_array($result->parent, $ancestors)) {
      return TRUE;
    }
    else {
      return context_admin_term_ancestor_access_check_or($result->parent, $ancestors);
    }
  }
  return FALSE;
}

function context_admin_term_ancestor_access_check_and($term, $ancestors) {
  $query = db_select( 'taxonomy_term_hierarchy', 'th')
    ->fields('th', array('parent'))
    ->condition('tid', $term, '=');
  $result = $query->execute()->fetchObjecT();
  if ($result->parent) {
    if (in_array($result->parent, $ancestors)) {
      $key = array_search($result->parent, $ancestors);
      unset($ancestors[$key]);
      if (!empty($ancestors)) {
        return context_admin_term_ancestor_access_check_and($result->parent, $ancestors);
      }
      else {
        return TRUE;
      }
    }
    else {
      return context_admin_term_ancestor_access_check_and($result->parent, $ancestors);
    }
  }
  return FALSE;
}

/**
 * Inform the UI that we've eliminated a bunch of possibilities for this
 * context.
 */
function context_admin_term_ancestor_access_restrictions($conf, &$context) {
  if (!is_array($conf['ancestors'])) {
    $conf['ancestors'] = array($conf['ancestors']);
  }
  if (isset($context->restrictions['ancestors'])) {
    $context->restrictions['ancestors'] = array_unique(array_merge($context->restrictions['ancestors'], array_keys(array_filter($conf['ancestors']))));
  }
  else {
    $context->restrictions['ancestors'] = array_keys(array_filter($conf['ancestors']));
  }
}

/**
 * Provide a summary description based upon the checked node_types.
 */
function context_admin_term_ancestor_access_summary($conf, $context) {
  if (!isset($conf['ancestors'])) {
    return t('No parent context has been selected for @identifier', array('@identifier' => $context->identifier));
  }

  return t('@identifier is a descendant of term "@ancestor"', array('@ancestor' => $conf['ancestors'], '@identifier' => $context->identifier));
}
