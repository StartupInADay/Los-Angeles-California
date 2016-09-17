<?php
/**
 * @file
 * sbp_service_center.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function sbp_service_center_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "panelizer" && $api == "panelizer") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function sbp_service_center_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function sbp_service_center_image_default_styles() {
  $styles = array();

  // Exported image style: medium_landscape.
  $styles['medium_landscape'] = array(
    'label' => 'Medium Landscape (190 x 140)',
    'effects' => array(
      1 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 190,
          'height' => 140,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function sbp_service_center_node_info() {
  $items = array(
    'labp_service_center' => array(
      'name' => t('Service Center'),
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
