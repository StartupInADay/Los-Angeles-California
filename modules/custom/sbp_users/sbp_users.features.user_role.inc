<?php
/**
 * @file
 * sbp_users.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function sbp_users_user_default_roles() {
  $roles = array();

  // Exported role: Editor.
  $roles['Editor'] = array(
    'name' => 'Editor',
    'weight' => 7,
    'machine_name' => 'editor',
  );

  return $roles;
}
