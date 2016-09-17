<?php
/**
 * @file
 * sbp_taxonomy.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function sbp_taxonomy_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'taxonomy_term-style_template-field_style_class'.
  $field_instances['taxonomy_term-style_template-field_style_class'] = array(
    'bundle' => 'style_template',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Please supply a unique class name that corresponds to your theme\'s style templates. These classes will prefix a container.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'quickedit' => array(
            'editor' => 'plain_text',
          ),
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'featured' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_style_class',
    'label' => 'Style Class',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-terms_definitions-field_term_definitions_grouping'.
  $field_instances['taxonomy_term-terms_definitions-field_term_definitions_grouping'] = array(
    'bundle' => 'terms_definitions',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'featured' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_term_definitions_grouping',
    'label' => 'Term & Definitions Grouping',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 3,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Please supply a unique class name that corresponds to your theme\'s style templates. These classes will prefix a container.');
  t('Style Class');
  t('Term & Definitions Grouping');

  return $field_instances;
}
