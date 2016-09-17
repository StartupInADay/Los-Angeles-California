<?php

/**
 * Implements hook_views_data_alter().
 */
function taxonomy_entity_index_views_data_alter(&$data) {
  $entity_info = taxonomy_entity_index_entity_views_integrable($data);
  $data['taxonomy_entity_index']['table']['group']  = t('Taxonomy Entity Index');
  foreach($entity_info as $type => $info) {
    if (isset($data[$info['base table']]) && isset($info['entity keys']['id'])) {
      $base_table = $info['base table'];

      // Join on all entity tables.
      $data['taxonomy_entity_index']['table']['join'][$base_table] = array(
        'left_field' => $info['entity keys']['id'],
        'field' => 'entity_id',
      );

      $data[$base_table]['taxonomy_entity_index_tid_depth'] = array(
        'help' => t('Display content if it has the selected taxonomy terms, or children of the selected terms. Due to additional complexity, this has fewer options than the versions without depth.'),
        'group' => t('Taxonomy Entity Index'),
        'real field' => $info['entity keys']['id'],
        'argument' => array(
          'title' => t('Has taxonomy term ID on @entity_type (with depth and indexed in taxonomy_entity_index)', array('@entity_type' => $info['label'])),
          'handler' => 'taxonomy_entity_index_handler_argument_tid_depth',
          'accept depth modifier' => TRUE,
        ),
        'filter' => array(
          'title' => t('Has taxonomy terms on @entity_type  (with depth and indexed in taxonomy_entity_index)', array('@entity_type' => $info['label'])),
          'handler' => 'taxonomy_entity_index_handler_filter_tid_depth',
        ),
      );

      $data[$base_table]['taxonomy_entity_index_entity_tid'] = array(
        'group' => t('Taxonomy Entity Index'),
        'title' => t('Taxonomy terms on @entity_type', array('@entity_type' => $info['label'])),
        'help' => t('Relate @entity_type to taxonomy terms. This relationship will cause duplicated records if there are multiple terms.', array('@entity_type' => $info['label'])),
        'relationship' => array(
          'real field' => $info['entity keys']['id'],
          'label' => t('terms'),
          'base' => 'taxonomy_entity_index',
          'base field' => 'entity_id',
          'extra' => array(
            array(
              'field' => 'entity_type',
              'operator' => '=',
              'value' => $type,
            ),
          ),
        ),
        'field' => array(
          'title' => t('All taxonomy terms on @entity_type', array('@entity_type' => $info['label'])),
          'help' => t('Display all taxonomy terms associated with a @entity_type from specified vocabularies.', array('@entity_type' => $info['label'])),
          'handler' => 'taxonomy_entity_index_handler_field_tid',
          'no group by' => TRUE,
        ),
      );
    }
  }

  $data['taxonomy_term_data']['table']['join']['taxonomy_entity_index'] = array(
    // links directly to taxonomy_term_data via tid
    'left_field' => 'tid',
    'field' => 'tid',
  );
  $data['taxonomy_term_hierarchy']['table']['join']['taxonomy_entity_index'] = array(
    // links directly to taxonomy_term_hierarchy via tid
    'left_field' => 'tid',
    'field' => 'tid',
  );
}
