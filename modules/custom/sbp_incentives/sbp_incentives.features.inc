<?php
/**
 * @file
 * sbp_incentives.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function sbp_incentives_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "panelizer" && $api == "panelizer") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function sbp_incentives_node_info() {
  $items = array(
    'incentives' => array(
      'name' => t('Incentives'),
      'base' => 'node_content',
      'description' => t('Business Portal Incentives pages'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
