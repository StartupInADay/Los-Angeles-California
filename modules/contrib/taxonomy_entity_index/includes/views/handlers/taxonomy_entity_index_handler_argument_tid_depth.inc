<?php

/**
 * @file
 * Handler to filter all entities which has a certain term with depth.
 */

class taxonomy_entity_index_handler_argument_tid_depth extends views_handler_argument_term_node_tid_depth {

  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->base_table_info = views_fetch_data($this->table);
  }


  function query($group_by = FALSE) {
    $this->ensure_my_table();

    if (!empty($this->options['break_phrase'])) {
      $tids = new stdClass();
      $tids->value = $this->argument;
      $tids = views_break_phrase($this->argument, $tids);
      if ($tids->value == array(-1)) {
        return FALSE;
      }

      if (count($tids->value) > 1) {
        $operator = 'IN';
      }
      else {
        $operator = '=';
      }

      $tids = $tids->value;
    }
    else {
      $operator = "=";
      $tids = $this->argument;
    }
    // Now build the subqueries.
    $subquery = db_select('taxonomy_entity_index', 'tn');
    $subquery->addField('tn', 'entity_id');
    if (isset($this->base_table_info['table']['entity type'])) {
      $subquery->condition('entity_type', $this->base_table_info['table']['entity type']);
    }
    $where = db_or()->condition('tn.tid', $tids, $operator);
    $last = "tn";

    if ($this->options['depth'] > 0) {
      $subquery->leftJoin('taxonomy_term_hierarchy', 'th', "th.tid = tn.tid");
      $last = "th";
      foreach (range(1, abs($this->options['depth'])) as $count) {
        $subquery->leftJoin('taxonomy_term_hierarchy', "th$count", "$last.parent = th$count.tid");
        $where->condition("th$count.tid", $tids, $operator);
        $last = "th$count";
      }
    }
    elseif ($this->options['depth'] < 0) {
      foreach (range(1, abs($this->options['depth'])) as $count) {
        $subquery->leftJoin('taxonomy_term_hierarchy', "th$count", "$last.tid = th$count.parent");
        $where->condition("th$count.tid", $tids, $operator);
        $last = "th$count";
      }
    }

    $subquery->condition($where);

    $this->query->add_where(0, "$this->table_alias.$this->real_field", $subquery, 'IN');
  }
}
