<?php
/**
 * @file
 * sbp_terms_definitions.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function sbp_terms_definitions_defaultconfig_features() {
  return array(
    'sbp_terms_definitions' => array(
      'strongarm' => 'strongarm',
    ),
  );
}

/**
 * Implements hook_defaultconfig_strongarm().
 */
function sbp_terms_definitions_defaultconfig_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'metatag_enable_taxonomy_term__terms_definitions';
  $strongarm->value = TRUE;
  $export['metatag_enable_taxonomy_term__terms_definitions'] = $strongarm;

  return $export;
}
