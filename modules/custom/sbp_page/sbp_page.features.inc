<?php
/**
 * @file
 * sbp_page.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function sbp_page_ctools_plugin_api($module = NULL, $api = NULL) {
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
function sbp_page_node_info() {
  $items = array(
    'sbp_basic_page' => array(
      'name' => t('Business Portal Basic Page'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
