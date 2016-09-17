<?php

/**
 * @file
 * Plugin to provide access control based upon existence in a particular menu.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Node: Menu Item Depth"),
  'description' => t('Control access based upon the depth within a particular menu of this node.'),
  'callback' => 'context_admin_node_menu_depth_access_check',
  'default' => array('menu' => array()),
  'settings form' => 'context_admin_node_menu_depth_access_settings',
  'settings form submit' => 'context_admin_node_menu_depth_access_settings_submit',
  'summary' => 'context_admin_node_menu_depth_access_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'restrictions' => 'context_admin_node_menu_depth_access_restrictions',
);

function context_admin_node_menu_depth_get_menus() {
  $custom_menus = menu_get_menus();
  foreach ($custom_menus as $name => $title) {
    $options[$name] = $title;
  }
  return $options;
}

/**
 * Settings form for the 'by node_type' access plugin
 */
function context_admin_node_menu_depth_access_settings($form, $form_state, $conf) {
  $options = context_admin_node_menu_depth_get_menus();

  $form['settings']['menu'] = array(
    '#title' => t('Menu'),
    '#type' => 'select',
    '#options' => $options,
    '#description' => t('Only the selected menu will be valid.'),
    '#default_value' => $conf['menu'],
  );
  $form['settings']['menu_depth'] = array(
    '#title' => t('Depth'),
    '#type' => 'select',
    '#options' => array(
      'p1' => '0',
      'p2' => '1',
      'p3' => '2',
      'p4' => '3',
      'p5' => '4',
      'p6' => '5',
      'p7' => '6',
      'p8' => '7',
      'p9' => '8',
    ),
    '#description' => t('The maximum depth from which this menu item is accessible.'),
    '#default_value' => $conf['menu_depth'],
  );
  return $form;
}

/**
 * Compress the node_types allowed to the minimum.
 */
function context_admin_node_menu_depth_access_settings_submit(&$form, &$form_state) {

}

/**
 * Check for access.
 */
function context_admin_node_menu_depth_access_check($conf, $context) {
  // Per the example in node type in core ctools
  if (empty($context) || empty($context->data) || empty($context->data->nid)) {
    return FALSE;
  }

  //$results = db_query("SELECT mlid, p1, p2, p3, p4, p5, p6, p7, p8, p9 FROM {menu_links} WHERE link_path = 'node/%d' AND module = 'menu' AND menu_name = '%s'", $context->data->nid, $conf['menu']);
  $query = db_select('menu_links', 'ml')
    ->fields('ml', array('mlid', 'p1', 'p2', 'p3', 'p4', 'p5', 'p6', 'p7', 'p8', 'p9'))
    ->condition('link_path', 'node/' . $context->data->nid, '=')
    ->condition('module', 'menu', '=')
    ->condition('menu_name', $conf['menu'], '=');
  $result = $query->execute()->fetchObject();
  if ($result && $result->mlid != $result->{$conf['menu_depth']}) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Inform the UI that we've eliminated a bunch of possibilities for this
 * context.
 */
function context_admin_node_menu_depth_access_restrictions($conf, &$context) {
  if (isset($context->restrictions['menu'])) {
    $context->restrictions['menu'] = array_unique(array_merge($context->restrictions['menu'], array_keys(array_filter($conf['menu']))));
  }
  else {
    $context->restrictions['menu'] = $conf['menu'];
  }
}

/**
 * Provide a summary description based upon the checked node_types.
 */
function context_admin_node_menu_depth_access_summary($conf, $context) {
  if (!isset($conf['menu'])) {
    return t('No menu context has been selected for @identifier', array('@identifier' => $context->identifier));
  }

  return t('@identifier is in the "@menu" menu', array('@menu' => $conf['menu'], '@identifier' => $context->identifier));
}
