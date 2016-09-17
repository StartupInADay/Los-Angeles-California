<?php

/**
 * hook_node_revision_filters()
 *
 * Define the filters to display on the node revision page
 *
 * @return array
 *  The form element to use
 */
function hook_node_revision_filters() {
  // Example from the state flow module
  $filters = array();
  if ($states = state_flow_get_all_states()) {
    $options = array_combine(array_keys($states), array_keys($states));
  } else {
    $options = array();
  }
  array_unshift($options, '[any]');
  $filters['state'] = array(
    'form' => array(
      '#type' => 'select',
      '#title' => t('State'),
      '#options' => $options,
    ),
  );

  return $filters;
}


/**
 * Implements hook_query_node_revision_alter()
 *
 * Then implement a hook to alter the query
 *
 * Example from state_flow module
 */
function hook_query_node_revision_alter(QueryAlterableInterface $query) {
  // Get the filter form the session
  $filters = $query->getMetaData('filters');
  if ($filter = isset($filters['state']) ? $filters['state'] : NULL) {
    $query->join('node_revision_states', 'nrs', 'nr.vid = nrs.vid');
    $query->condition('nrs.state', $filter);
  }
}

/**
 * Implements hook_node_revision_operations().
 *
 * What operations can occur on the node revision
 *
 * @return array
 *  label
 *  callback
 *  callback arguments
 */
function hook_node_revision_operations() {
  // Example from the state_flow module
  $operations = array();

  $events = state_flow_get_all_events();

  foreach ($events as $event) {
    $operations["change_state_$event"] = array(
      'label' => t('Transition Action: @event', array('@event' => $event)),
      'callback' => 'state_flow_node_revision_operation_change_state',
      'callback arguments' => array('args' => array('event' => $event)),
    );
  }

  return $operations;
}

/**
 * Get the status of a revision
 *
 * @param $node
 *  THe node revision object
 *
 * @return string
 *  The display label of the status
 */
function hook_node_revision_status($node) {
  return (!state_flow_load_state_machine($node)->ignore()) ?
    state_flow_load_state_machine($node)->get_label_for_current_state() :
    '- None -';
}
