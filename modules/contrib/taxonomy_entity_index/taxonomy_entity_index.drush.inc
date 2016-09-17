<?php
/**
 * @file
 * Clear all caches
 */

/**
 * Implementation of hook_drush_command().
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing each command.
 */
function taxonomy_entity_index_drush_command() {
  $items = array();

  $items['taxonomy-entity-index-rebuild'] = array(
    'description' => "Rebuild taxonomy entity index",
    'examples' => array(
      'drush taxonomy-entity-index-rebuild',
    ),
    'aliases' => array('tei-rebuild'),
  );

  return $items;
}

/**
 * Clear cache raw command callback.
 */
function drush_taxonomy_entity_index_rebuild() {
  $entity_info = entity_get_info();
  $operations = array();

  foreach($entity_info as $type => $info) {
    $operations[] = array('taxonomy_entity_index_reindex_entity_type', array($type));
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'taxonomy_entity_index_finished',
  );

  batch_set($batch);

  // Process the batch.
  $batch =& batch_get();
  $batch['progressive'] = FALSE;
  batch_process();
}
