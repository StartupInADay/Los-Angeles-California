<?php
/**
 * @file
 * sbp_service_center.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function sbp_service_center_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_more_contact_info|node|labp_service_center|form';
  $field_group->group_name = 'group_more_contact_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'labp_service_center';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Additional contact info',
    'weight' => '10',
    'children' => array(
      0 => 'field_email',
      1 => 'field_link',
      2 => 'field_phone',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-more-contact-info field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_more_contact_info|node|labp_service_center|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Additional contact info');

  return $field_groups;
}
