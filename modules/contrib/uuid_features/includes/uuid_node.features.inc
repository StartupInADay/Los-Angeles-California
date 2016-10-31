<?php
/**
 * @file
 * Features hooks for the uuid_node features component.
 */

/**
 * Implements hook_features_export_options().
 */
function uuid_node_features_export_options() {
  $options = array();

  // Check what content types are enabled for uuid features export.
  $enabled_types = variable_get('uuid_features_entity_node', FALSE);
  if (!empty($enabled_types)) {
    $types = node_type_get_names();
    $query = db_select('node', 'n');
    $query->fields('n', array('nid', 'title', 'type', 'uuid'))
      ->condition('type', $enabled_types)
      ->orderBy('type')
      ->orderBy('title');
    $nodes = $query->execute()->fetchAll();
    foreach ($nodes as $node) {
      $options[$node->uuid] = t('@type: @title', array(
        '@type' => $types[$node->type],
        '@title' => $node->title,
      ));
    }
  }

  drupal_alter('uuid_node_features_export_options', $options);
  return $options;
}

/**
 * Implements hook_features_export().
 */
function uuid_node_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['uuid_features'] = 'uuid_features';

  $nids = entity_get_id_by_uuid('node', $data);
  foreach ($nids as $uuid => $nid) {
    // Load the existing node, with a fresh cache.
    $node = node_load($nid, NULL, TRUE);

    $export['features']['uuid_node'][$uuid] = $uuid;
    $pipe['node'][$node->type] = $node->type;

    // drupal_alter() normally supports just one byref parameter. Using
    // the __drupal_alter_by_ref key, we can store any additional parameters
    // that need to be altered, and they'll be split out into additional params
    // for the hook_*_alter() implementations.
    $data = &$export;
    $data['__drupal_alter_by_ref']['pipe'] = &$pipe;
    $entity_type = 'node';
    drupal_alter('uuid_entity_features_export', $entity_type, $data, $node);
    drupal_alter('uuid_node_features_export', $data, $node, $module);
    unset($data['__drupal_alter_by_ref']);
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function uuid_node_features_export_render($module, $data) {
  $translatables = $code = array();

  $code[] = '  $nodes = array();';
  $code[] = '';
  $nids = entity_get_id_by_uuid('node', $data);
  foreach ($nids as $uuid => $nid) {
    // Only export the node if it exists.
    if ($nid === FALSE) {
      continue;
    }
    // Attempt to load the node, using a fresh cache.
    $node = node_load($nid, NULL, TRUE);
    if (empty($node)) {
      continue;
    }
    if (!empty($node->path)) {
      $node->pathauto_perform_alias = FALSE;
    }
    // Clone entity to avoid changes by reference.
    $export = clone $node;

    // Use date instead of created, in the same format used by
    // node_object_prepare().
    $export->date = format_date($export->created, 'custom', 'Y-m-d H:i:s O');

    // Don't cause conflicts with nid/vid/revision_timestamp/changed fields.
    uuid_features_file_field_export($export, 'node');

    $entity_type = 'node';
    drupal_alter('uuid_entity_features_export_render', $entity_type, $export, $node);
    drupal_alter('uuid_node_features_export_render', $export, $node, $module);

    unset($export->revision_uid);
    unset($export->revision_timestamp);
    unset($export->nid);
    unset($export->vid);
    unset($export->vuuid);
    unset($export->changed);
    unset($export->last_comment_timestamp);
    unset($export->last_comment_id);
    unset($export->last_comment_name);
    unset($export->last_comment_uid);
    unset($export->cid);

    $code[] = '  $nodes[] = ' . features_var_export($export) . ';';
  }

  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }
  $code[] = '  return $nodes;';
  $code = implode("\n", $code);
  return array('uuid_features_default_content' => $code);
}

/**
 * Implements hook_features_revert().
 */
function uuid_node_features_revert($module) {
  uuid_node_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 *
 * Rebuilds nodes based on UUID from code defaults.
 */
function uuid_node_features_rebuild($module) {
  // Import the terms first.
  uuid_term_features_rebuild($module);

  $nodes = module_invoke($module, 'uuid_features_default_content');
  if (!empty($nodes)) {
    module_load_include('inc', 'node', 'node.pages');

    $tmp = array();
    foreach ($nodes as $key => $data) {
      if (isset($data['og_group_ref'])) {
        $tmp[] = $data;
        unset($nodes[$key]);
      }
    }

    $nodes = array_merge($nodes, $tmp);
    return uuid_node_features_rebuild_nodes($nodes);
  }
  return TRUE;
}

/**
 * Runs the node import multiple times to resolve dependencies.
 *
 * We might need several runs of ths function to resolve the dependencies
 * created by reference fields. Those can only be resolved if the target node
 * already exists.
 *
 * @param array $nodes
 *   The nodes to process.
 * @param int $max_nesting
 *   Maximal nesting level.
 * @param int $nesting_level
 *   Current nesting level.
 *
 * @return bool
 *   TRUE if all nodes could be restored.
 */
function uuid_node_features_rebuild_nodes($nodes, $max_nesting = 5, $nesting_level = 0) {
  // Max nesting level hit.
  if ($max_nesting < $nesting_level) {
    watchdog('UUID Features', 'Unable to restore nodes. Max nesting level reached.', array(), WATCHDOG_ERROR);
    return FALSE;
  }
  $second_run_nodes = array();
  foreach ($nodes as $data) {
    try {
      $node = (object) $data;
      $node->uid = 0;
      node_object_prepare($node);

      // Find the matching UUID, with a fresh cache.
      $nids = entity_get_id_by_uuid('node', array($node->uuid));
      if (isset($nids[$node->uuid])) {
        $nid = array_key_exists($node->uuid, $nids) ? $nids[$node->uuid] : FALSE;
        $existing = node_load($nid, NULL, TRUE);
        if (!empty($existing)) {
          $node->nid = $existing->nid;
          $node->vid = $existing->vid;
        }
      }

      $entity_type = 'node';
      drupal_alter('uuid_entity_features_rebuild', $entity_type, $node, $data);
      drupal_alter('uuid_node_features_rebuild', $node, $module);

      $node = node_submit($node);
      uuid_features_file_field_import($node, 'node');
      node_save($node);
    }
    catch (Exception $e) {
      $second_run_nodes[] = $data;
    }
  }
  if (!empty($second_run_nodes)) {
    return uuid_node_features_rebuild_nodes($second_run_nodes, $max_nesting, ++$nesting_level);
  }
  return TRUE;
}
