<?php
/**
 * @file
 * sbp_starter_kit.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function sbp_starter_kit_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_checklist|node|sbp_starter_kit|form';
  $field_group->group_name = 'group_checklist';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sbp_starter_kit';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Checklist',
    'weight' => '3',
    'children' => array(
      0 => 'field_checklist_description',
      1 => 'field_checklist_items',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-checklist field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_checklist|node|sbp_starter_kit|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_guide|node|sbp_starter_kit|form';
  $field_group->group_name = 'group_guide';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sbp_starter_kit';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Summary',
    'weight' => '0',
    'children' => array(
      0 => 'field_guide_description',
      1 => 'field_guide_image',
      2 => 'field_guide_pdf',
      3 => 'field_guide_title',
      4 => 'field_permit_pdf',
      5 => 'field_permit_image',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Summary',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-guide field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_guide|node|sbp_starter_kit|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_permits|node|sbp_starter_kit|form';
  $field_group->group_name = 'group_permits';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sbp_starter_kit';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Permits',
    'weight' => '4',
    'children' => array(
      0 => 'field_permit_description',
      1 => 'field_permit_section',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-permits field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_permits|node|sbp_starter_kit|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Checklist');
  t('Permits');
  t('Summary');

  return $field_groups;
}
