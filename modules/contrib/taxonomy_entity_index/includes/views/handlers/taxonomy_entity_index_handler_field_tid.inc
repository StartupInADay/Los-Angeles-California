<?php

/**
 * @file
 * Field handler to display all taxonomy terms of an entity.
 */

class taxonomy_entity_index_handler_field_tid extends views_handler_field_term_node_tid {
  /**
   * Stores the base table information.
   */

  var $base_table_info = NULL;

  /**
   * Stores the entity info of the base table.
   */
  var $entity_get_info = NULL;


  function init(&$view, &$options) {
    parent::init($view, $options);
    // Reset the variables which are set by the parent class.
    unset($this->additional_fields['nid']);

    $this->base_table_info = views_fetch_data($this->table);
    $this->entity_info = entity_get_info($this->base_table_info['table']['entity type']);
    $this->additional_fields['entity_id'] = array('table' => $this->entity_info['base table'], 'field' => $this->entity_info['entity keys']['id']);
  }

  function pre_render(&$values) {
    $this->field_alias = $this->aliases['entity_id'];
    $entity_ids = array();
    foreach ($values as $result) {
      if (!empty($result->{$this->field_alias})) {
        $entity_ids[] = $result->{$this->field_alias};
      }
    }

    if ($entity_ids) {
      $query = db_select('taxonomy_term_data', 'td');
      $query->innerJoin('taxonomy_entity_index', 'tei', 'td.tid = tei.tid');
      $query->innerJoin('taxonomy_vocabulary', 'tv', 'td.vid = tv.vid');
      $query->fields('td');
      $query->addField('tei', 'entity_id', 'entity_id');
      $query->addField('tei', 'entity_type', 'entity_type');
      $query->addField('tei', 'revision_id', 'revision_id');
      $query->addField('tv', 'name', 'vocabulary');
      $query->addField('tv', 'machine_name', 'vocabulary_machine_name');
      $query->orderby('td.weight');
      $query->orderby('td.name');
      $query->condition('tei.entity_id', $entity_ids);
      $query->condition('tei.entity_type', $this->base_table_info['table']['entity type']);
      $query->addTag('term_access');
      $vocabs = array_filter($this->options['vocabularies']);
      if (!empty($this->options['limit']) && !empty($vocabs)) {
        $query->condition('tv.machine_name', $vocabs);
      }
      $result = $query->execute();

      foreach ($result as $term) {
        $this->items[$term->entity_id][$term->tid]['name'] = check_plain($term->name);
        $this->items[$term->entity_id][$term->tid]['tid'] = $term->tid;
        $this->items[$term->entity_id][$term->tid]['vocabulary_machine_name'] = check_plain($term->vocabulary_machine_name);
        $this->items[$term->entity_id][$term->tid]['vocabulary'] = check_plain($term->vocabulary);

        if (!empty($this->options['link_to_taxonomy'])) {
          $this->items[$term->entity_id][$term->tid]['make_link'] = TRUE;
          $this->items[$term->entity_id][$term->tid]['path'] = 'taxonomy/term/' . $term->tid;
        }
      }
    }
  }
}
