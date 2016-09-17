<?php
/**
 * @file
 * sbp_taxonomy.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function sbp_taxonomy_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_term_definitions_grouping'.
  $field_bases['field_term_definitions_grouping'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_term_definitions_grouping',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'terms_definitions_grouping',
          'parent' => 0,
        ),
      ),
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  return $field_bases;
}
