<?php
/**
 * @file
 * sbp_resources_incentives.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function sbp_resources_incentives_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'facetapi:block_cache:search_api@resources_incentives';
  $strongarm->value = -1;
  $export['facetapi:block_cache:search_api@resources_incentives'] = $strongarm;

  return $export;
}
