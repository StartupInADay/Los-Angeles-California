<?php
/**
 * @file
 * sbp_hero.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function sbp_hero_defaultconfig_features() {
  return array(
    'sbp_hero' => array(
      'field_default_fields' => 'field_default_fields',
    ),
  );
}
