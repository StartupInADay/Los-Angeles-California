<?php
/**
 * @file
 * sbp_sponsors.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function sbp_sponsors_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function sbp_sponsors_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_fe_nodequeue_export_fields().
 */
function sbp_sponsors_fe_nodequeue_export_fields() {
  $nodequeues = array();

  // Exported nodequeues: sponsors_list
  $nodequeues['sponsors_list'] = array(
    'name' => 'sponsors_list',
    'title' => 'Sponsors List',
    'subqueue_title' => '',
    'size' => 0,
    'link' => '',
    'link_remove' => '',
    'owner' => 'nodequeue',
    'show_in_ui' => 1,
    'show_in_tab' => 1,
    'show_in_links' => 0,
    'reference' => 0,
    'reverse' => 0,
    'insert_at_front' => 0,
    'i18n' => 0,
    'subqueues' => 1,
    'types' => array(
      0 => 'business_portal_sponsors',
    ),
    'roles' => array(),
    'count' => 0,
  );

  return $nodequeues;
}

/**
 * Implements hook_image_default_styles().
 */
function sbp_sponsors_image_default_styles() {
  $styles = array();

  // Exported image style: sponsors_logo_70x70.
  $styles['sponsors_logo_70x70'] = array(
    'label' => 'Sponsors Logo(70x70)',
    'effects' => array(
      1 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 70,
          'height' => 70,
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
function sbp_sponsors_node_info() {
  $items = array(
    'business_portal_sponsors' => array(
      'name' => t('Business Portal Sponsors'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Sponsor Name'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
