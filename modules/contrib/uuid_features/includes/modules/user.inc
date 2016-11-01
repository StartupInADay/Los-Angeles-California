<?php
/**
 * @file
 * uuid features hooks on behalf of the user collection module.
 */

/**
 * Implements hook_uuid_node_features_export_render_alter().
 */
function user_uuid_node_features_export_render_alter(&$export, &$node, $module) {
  $uuids = entity_get_uuid_by_id('user', array($node->uid));
  $uuid = reset($uuids);
  if ($uuid) {
    $export->user_uuid = $uuid;
  }
  unset($export->uid);
  // Ensure we don't export the name or uid property which could lead to a user
  // override due to the handling in node_submit().
  unset($export->name);

  // Remove other properties handled solely by user module.
  unset($export->data, $export->picture);
}

/**
 * Implements hook_uuid_node_features_rebuild_alter().
 */
function user_uuid_node_features_rebuild_alter(&$node, $module) {
  if (isset($node->user_uuid) && uuid_is_valid($node->user_uuid)) {
    $uids = entity_get_id_by_uuid('user', array($node->user_uuid));
    $uid = reset($uids);
    if ($uid) {
      $node->uid = $uid;
    }
    else {
      $node->uid = 1;
    }
  }
  else {
    $node->uid = 1;
  }
}
