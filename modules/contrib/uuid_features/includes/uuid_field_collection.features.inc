<?php
/**
 * @file
 * Features hooks for the uuid_field_collection features component.
 */

/**
 * Implements hook_features_export_options().
 */
function uuid_field_collection_features_export_options() {
  $options = array();
  $all_ids = array();

  // Only allow enabled bundles to be exported.
  $enabled_bundles = variable_get('uuid_features_entity_field_collection_item', FALSE);
  if ($enabled_bundles) {
    $efq_field_collection_items = new EntityFieldQuery();
    $efq_field_collection_items->entityCondition('entity_type', 'field_collection_item');
    $efq_field_collection_items->entityCondition('bundle', $enabled_bundles);
    $result = $efq_field_collection_items->execute();

    if (!empty($result['field_collection_item'])) {
      $all_ids = array_keys($result['field_collection_item']);
    }

    $items = entity_load('field_collection_item', $all_ids);

    foreach ($items as $item) {
      $info = $item->instanceInfo();

      $options[$item->uuid] = t('@type: @title', array(
        '@type' => $info['label'],
        '@title' => $item->defaultLabel(),
      ));
    }
  }

  drupal_alter('uuid_field_collection_features_export_options', $options);
  return $options;
}

/**
 * Implements hook_features_export().
 */
function uuid_field_collection_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['field_collection'] = 'field_collection';
  $export['dependencies']['uuid'] = 'uuid';
  $export['dependencies']['uuid_features'] = 'uuid_features';

  foreach ($data as $uuid) {
    $export['features']['uuid_field_collection'][$uuid] = $uuid;
  }
  return array();
}

/**
 * Implements hook_features_export_render().
 */
function uuid_field_collection_features_export_render($module, $data) {
  $translatables = $code = array();

  $code[] = '  $field_collections = array();';
  $code[] = '';
  foreach ($data as $uuid) {
    $field_collections = entity_uuid_load('field_collection_item', array($uuid), array(), TRUE);
    if (!count($field_collections)) {
      continue;
    }

    $field_collection = reset($field_collections);
    // Clone entity to avoid changes by reference.
    $export = clone $field_collection;

    // Do not export ids.
    unset($export->item_id);
    unset($export->revision_id);

    uuid_features_file_field_export($export, 'field_collection_item');

    $entity_type = 'field_collection_item';
    drupal_alter('uuid_entity_features_export_render', $entity_type, $export, $field_collection);

    $export_output = features_var_export($export, '  ');
    $export_output = str_replace("entity_import('field_collection_item', '", "", $export_output);
    $export_output = str_replace("}')", "}", $export_output);
    $export_output = json_decode($export_output, TRUE);

    $code[] = '  $field_collections[] = ' . features_var_export($export_output, '  ') . ';';
  }

  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }
  $code[] = '  return $field_collections;';
  $code = implode("\n", $code);

  return array('uuid_features_default_field_collections' => $code);
}

/**
 * Implements hook_features_revert().
 */
function uuid_field_collection_features_revert($module) {
  uuid_field_collection_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function uuid_field_collection_features_rebuild($module) {
  // Make sure all referenced entities exist.
  taxonomy_features_rebuild($module);
  field_features_rebuild($module);
  uuid_node_features_rebuild($module);
  cache_clear_all();

  $field_collections = module_invoke($module, 'uuid_features_default_field_collections');
  if (!empty($field_collections)) {
    foreach ($field_collections as $field_collection_item) {
      try {
        $fields = field_info_instances('field_collection_item', $field_collection_item['field_name']);
        foreach ($fields as $field_name => $field_instance_config) {
          $field_config = field_info_field($field_name);
          if (isset($field_collection_item[$field_name])) {
            foreach ($field_collection_item[$field_name] as $lang => $field_data) {
              foreach ($field_data as $delta => $value) {
                if ($field_config['type'] == 'entityreference' || $field_config['type'] == 'taxonomy_term_reference') {
                  $columns = array_keys($field_config['columns']);
                  $first_column = reset($columns);
                  if (isset($field_config['settings']['target_type'])) {
                    $target_entity = $field_config['settings']['target_type'];
                  }
                  elseif ($field_config['type'] == 'taxonomy_term_reference') {
                    $target_entity = 'taxonomy_term';
                  }
                  else {
                    throw new Exception('Unknown target.');
                  }
                  $ids = entity_get_id_by_uuid($target_entity, array($value[$first_column]));
                  $field_collection_item[$field_name][$lang][$delta][$first_column] = $ids ? reset($ids) : UUID_FEATURES_DEFAULT_FALLBACK_ID;
                }
              }
            }
          }
        }

        $field_collection_item['original'] = TRUE;
        $entity = entity_import('field_collection_item', json_encode($field_collection_item));
        // Hack to ignore missing host entities.
        /* @var FieldCollectionItemEntity $entity */
        $entity->is_new = FALSE;

        uuid_features_file_field_import($entity, 'field_collection_item');

        $entity->save(TRUE);
      }
      catch (Exception $e) {

      }
    }
  }
  uuid_node_features_rebuild($module);
}
