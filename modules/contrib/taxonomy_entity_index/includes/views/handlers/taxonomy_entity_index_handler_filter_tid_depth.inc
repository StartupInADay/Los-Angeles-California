<?php
/**
 * Define an alias for future additions.
 */
class taxonomy_entity_index_handler_filter_tid_depth extends views_handler_filter_term_node_tid_depth {

  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->base_table_info = views_fetch_data($this->table);
  }

  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);
    $form['depth']['#description'] = t('The depth will match entities tagged with terms in the hierarchy. For example, if you have the term "fruit" and a child term "apple", with a depth of 1 (or higher) then filtering for the term "fruit" will get entities that are tagged with "apple" as well as "fruit". If negative, the reverse is true; searching for "apple" will also pick up nodes tagged with "fruit" if depth is -1 (or lower).');
  }

  function query() {
    // If no filter values are present, then do nothing.
    if (count($this->value) == 0) {
      return;
    }
    elseif (count($this->value) == 1) {
      // Somethis $this->value is an array with a single element so convert it.
      if (is_array($this->value)) {
        $this->value = current($this->value);
      }
      $operator = '=';
    }
    else {
      $operator = 'IN';# " IN (" . implode(', ', array_fill(0, sizeof($this->value), '%d')) . ")";
    }

    // The normal use of ensure_my_table() here breaks Views.
    // So instead we trick the filter into using the alias of the base table.
    // See http://drupal.org/node/271833
    // If a relationship is set, we must use the alias it provides.
    if (!empty($this->relationship)) {
      $this->table_alias = $this->relationship;
    }
    // If no relationship, then use the alias of the base table.
    elseif (isset($this->query->table_queue[$this->query->base_table]['alias'])) {
      $this->table_alias = $this->query->table_queue[$this->query->base_table]['alias'];
    }
    // This should never happen, but if it does, we fail quietly.
    else {
      return;
    }

    // Now build the subqueries.
    $subquery = db_select('taxonomy_entity_index', 'tei');
    $subquery->addField('tei', 'entity_id');
    if (isset($this->base_table_info['table']['entity type'])) {
      $subquery->condition('entity_type', $this->base_table_info['table']['entity type']);
    }
    $where = db_or()->condition('tei.tid', $this->value, $operator);
    $last = "tei";

    if ($this->options['depth'] > 0) {
      $subquery->leftJoin('taxonomy_term_hierarchy', 'th', "th.tid = tei.tid");
      $last = "th";
      foreach (range(1, abs($this->options['depth'])) as $count) {
        $subquery->leftJoin('taxonomy_term_hierarchy', "th$count", "$last.parent = th$count.tid");
        $where->condition("th$count.tid", $this->value, $operator);
        $last = "th$count";
      }
    }
    elseif ($this->options['depth'] < 0) {
      foreach (range(1, abs($this->options['depth'])) as $count) {
        $subquery->leftJoin('taxonomy_term_hierarchy', "th$count", "$last.tid = th$count.parent");
        $where->condition("th$count.tid", $this->value, $operator);
        $last = "th$count";
      }
    }

    $subquery->condition($where);
    $this->query->add_where(0, "$this->table_alias.$this->real_field", $subquery, 'IN');
  }
}
