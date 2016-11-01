<?php
/**
 * @file
 * uuid features hooks on behalf of the field collection module.
 */

/**
 * Implements hook_uuid_entity_features_export_render_alter().
 */
function field_collection_uuid_entity_features_export_render_alter($entity_type, &$export, &$entity) {
  $fields = uuid_features_get_field_items_iterator($export, $entity_type, 'field_collection');
  uuid_features_set_uuid_references($fields, 'field_collection_item');
}

/**
 * Implements hook_uuid_entity_features_rebuild_alter().
 */
function field_collection_uuid_entity_features_rebuild_alter($entity_type, &$entity, $data) {
  $fields = uuid_features_get_field_items_iterator($entity, $entity_type, 'field_collection');
  uuid_features_fetch_uuid_references($fields, 'field_collection_item', array(
    'item_id' => 'value',
    'revision_id' => 'revision_id',
  ));
}
