<?php
/**
 * @file
 * sbp_sponsors.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function sbp_sponsors_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_business_portal_sponsors';
  $strongarm->value = 0;
  $export['comment_anonymous_business_portal_sponsors'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_business_portal_sponsors';
  $strongarm->value = '0';
  $export['comment_business_portal_sponsors'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_business_portal_sponsors';
  $strongarm->value = 1;
  $export['comment_default_mode_business_portal_sponsors'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_business_portal_sponsors';
  $strongarm->value = '50';
  $export['comment_default_per_page_business_portal_sponsors'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_business_portal_sponsors';
  $strongarm->value = 1;
  $export['comment_form_location_business_portal_sponsors'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_business_portal_sponsors';
  $strongarm->value = '1';
  $export['comment_preview_business_portal_sponsors'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_business_portal_sponsors';
  $strongarm->value = 1;
  $export['comment_subject_field_business_portal_sponsors'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__business_portal_sponsors';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'metatags' => array(
          'weight' => '5',
        ),
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '4',
        ),
        'rabbit_hole' => array(
          'weight' => '3',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__business_portal_sponsors'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_business_portal_sponsors';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_business_portal_sponsors'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_business_portal_sponsors';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_business_portal_sponsors'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_business_portal_sponsors';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_business_portal_sponsors'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_business_portal_sponsors';
  $strongarm->value = '1';
  $export['node_preview_business_portal_sponsors'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_business_portal_sponsors';
  $strongarm->value = 0;
  $export['node_submitted_business_portal_sponsors'] = $strongarm;

  return $export;
}
