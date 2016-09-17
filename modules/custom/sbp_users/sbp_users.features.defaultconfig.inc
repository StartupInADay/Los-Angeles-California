<?php
/**
 * @file
 * sbp_users.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function sbp_users_defaultconfig_features() {
  return array(
    'labp_users' => array(
      'user_default_permissions' => 'user_default_permissions',
    ),
  );
}

/**
 * Implements hook_defaultconfig_user_default_permissions().
 */
function sbp_users_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer panelizer node landing layout'.
  $permissions['administer panelizer node landing layout'] = array(
    'name' => 'administer panelizer node landing layout',
    'roles' => array(
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panels layouts'.
  $permissions['administer panels layouts'] = array(
    'name' => 'administer panels layouts',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'panels',
  );

  return $permissions;
}
