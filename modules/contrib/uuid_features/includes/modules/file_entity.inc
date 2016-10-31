<?php
/**
 * @file
 * uuid features hooks on behalf of the file entity module.
 */

/**
 * Implements hook_uuid_entity_features_export_render_alter().
 */
function file_entity_uuid_entity_features_export_render_alter($entity_type, &$export, &$entity) {
  list($entity_id, $revision_id, $export_bundle) = entity_extract_ids($entity_type, $entity);

  // Handle referenced file entities in fields.
  $supported_fields = array_map('trim', explode(',', variable_get('uuid_features_file_supported_fields', 'file, image')));
  $bundles = variable_get('uuid_features_file_' . $entity_type, array());
  if (in_array($export_bundle, $bundles)) {
    $fields = field_info_instances($entity_type, $export_bundle);
    foreach ($fields as $field_name => $field_instance) {
      $field_info = field_info_field($field_name);
      if (!in_array($field_info['type'], $supported_fields) || empty($export->{$field_name})) {
        continue;
      }
      // Ensure file entity is properly processed for the export.
      foreach ($export->{$field_name} as $lang_code => $field_values) {
        foreach ($field_values as $i => $field_value) {
          if (!empty($field_value)) {
            $file_entity_type = 'file';
            $file_entity = (object) $field_value;
            $export_file_entity = clone $file_entity;
            drupal_alter('uuid_entity_features_export_render', $file_entity_type, $export_file_entity, $file_entity);
            $export->{$field_name}[$lang_code][$i] = (array) $export_file_entity;
          }
        }
      }
    }
  }

  // Handle file entities itself.
  if ($entity_type == 'file') {
    $info = entity_get_info($entity_type);
    // Ensure we don't export a revision key.
    if (!empty($info['entity keys']['revision']) && isset($export->{$info['entity keys']['revision']})) {
      unset($export->{$info['entity keys']['revision']});
    }
    unset(
      $export->created,
      $export->changed
    );
  }
}

/**
 * Implements hook_uuid_entity_features_rebuild_alter().
 */
function file_entity_uuid_entity_features_rebuild_alter($entity_type, &$entity) {
  list($entity_id, $revision_id, $import_bundle) = entity_extract_ids($entity_type, $entity);

  // Handle referenced file entities in fields.
  // Get all fields from this bundle.
  $fields = field_info_instances($entity_type, $import_bundle);
  $supported_fields = array_map('trim', explode(',', variable_get('uuid_features_file_supported_fields', 'file, image')));

  foreach ($fields as $field_name => $field_instance) {
    if (isset($entity->{$field_name})) {
      // Load field info to check the type.
      $info = field_info_field($field_name);
      if (in_array($info['type'], $supported_fields) && !empty($entity->{$field_name})) {
        // Ensure file entity is properly processed for the import.
        foreach ($entity->{$field_name} as $lang_code => $field_values) {
          foreach ($field_values as $i => $field_value) {
            $file_entity_type = 'file';
            $import_file_entity = (object) $field_value;
            drupal_alter('uuid_entity_features_rebuild', $file_entity_type, $import_file_entity);
            $entity->{$field_name}[$lang_code][$i] = (array) $import_file_entity;
          }
        }
      }
    }
  }
}
