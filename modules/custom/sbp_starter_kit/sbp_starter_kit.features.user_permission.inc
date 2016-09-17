<?php
/**
 * @file
 * sbp_starter_kit.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function sbp_starter_kit_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer panelizer node sbp_starter_kit content'.
  $permissions['administer panelizer node sbp_starter_kit content'] = array(
    'name' => 'administer panelizer node sbp_starter_kit content',
    'roles' => array(),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_starter_kit context'.
  $permissions['administer panelizer node sbp_starter_kit context'] = array(
    'name' => 'administer panelizer node sbp_starter_kit context',
    'roles' => array(),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_starter_kit defaults'.
  $permissions['administer panelizer node sbp_starter_kit defaults'] = array(
    'name' => 'administer panelizer node sbp_starter_kit defaults',
    'roles' => array(),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_starter_kit layout'.
  $permissions['administer panelizer node sbp_starter_kit layout'] = array(
    'name' => 'administer panelizer node sbp_starter_kit layout',
    'roles' => array(),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_starter_kit overview'.
  $permissions['administer panelizer node sbp_starter_kit overview'] = array(
    'name' => 'administer panelizer node sbp_starter_kit overview',
    'roles' => array(),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_starter_kit settings'.
  $permissions['administer panelizer node sbp_starter_kit settings'] = array(
    'name' => 'administer panelizer node sbp_starter_kit settings',
    'roles' => array(),
    'module' => 'panelizer',
  );

  // Exported permission: 'create sbp_starter_kit content'.
  $permissions['create sbp_starter_kit content'] = array(
    'name' => 'create sbp_starter_kit content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any sbp_starter_kit content'.
  $permissions['delete any sbp_starter_kit content'] = array(
    'name' => 'delete any sbp_starter_kit content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own sbp_starter_kit content'.
  $permissions['delete own sbp_starter_kit content'] = array(
    'name' => 'delete own sbp_starter_kit content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any sbp_starter_kit content'.
  $permissions['edit any sbp_starter_kit content'] = array(
    'name' => 'edit any sbp_starter_kit content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own sbp_starter_kit content'.
  $permissions['edit own sbp_starter_kit content'] = array(
    'name' => 'edit own sbp_starter_kit content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
