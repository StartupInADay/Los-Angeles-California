<?php
/**
 * @file
 * sbp_wizard.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function sbp_wizard_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function sbp_wizard_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function sbp_wizard_node_info() {
  $items = array(
    'sbp_wizard_screen' => array(
      'name' => t('Business Portal Wizard Screen'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Screen ID'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
