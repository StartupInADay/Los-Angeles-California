<?php
/**
 * @file
 * sbp_wizard.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function sbp_wizard_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer panelizer node sbp_wizard_screen defaults'.
  $permissions['administer panelizer node sbp_wizard_screen defaults'] = array(
    'name' => 'administer panelizer node sbp_wizard_screen defaults',
    'roles' => array(),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer taxonomy_term business_portal_wizard_sections defaults'.
  $permissions['administer panelizer taxonomy_term business_portal_wizard_sections defaults'] = array(
    'name' => 'administer panelizer taxonomy_term business_portal_wizard_sections defaults',
    'roles' => array(),
    'module' => 'panelizer',
  );

  // Exported permission: 'create sbp_wizard_screen content'.
  $permissions['create sbp_wizard_screen content'] = array(
    'name' => 'create sbp_wizard_screen content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any sbp_wizard_screen content'.
  $permissions['delete any sbp_wizard_screen content'] = array(
    'name' => 'delete any sbp_wizard_screen content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own sbp_wizard_screen content'.
  $permissions['delete own sbp_wizard_screen content'] = array(
    'name' => 'delete own sbp_wizard_screen content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete terms in business_portal_wizard_sections'.
  $permissions['delete terms in business_portal_wizard_sections'] = array(
    'name' => 'delete terms in business_portal_wizard_sections',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit any sbp_wizard_screen content'.
  $permissions['edit any sbp_wizard_screen content'] = array(
    'name' => 'edit any sbp_wizard_screen content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own sbp_wizard_screen content'.
  $permissions['edit own sbp_wizard_screen content'] = array(
    'name' => 'edit own sbp_wizard_screen content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit terms in business_portal_wizard_sections'.
  $permissions['edit terms in business_portal_wizard_sections'] = array(
    'name' => 'edit terms in business_portal_wizard_sections',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'taxonomy',
  );

  return $permissions;
}
