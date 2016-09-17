<?php

/**
 * @file visitor_actions.rules.inc
 * Provides hooks and classes for integration with Rules module.
 */

/**
 * Implements hook_rules_action_info().
 */
function visitor_actions_rules_action_info() {
  return array(
    'trigger_visitor_action' => array(
      'label' => t('Trigger a visitor action'),
      'parameter' => array(
        'action_name' => array(
          'type' => 'text',
          'label' => t('Action name'),
          'description' => t('The name of the action to trigger.')
        ),
        'action_value' => array(
          'type' => 'integer',
          'label' => t('Action value'),
          'description' => t('The optional value of the action.'),
          'optional' => TRUE,
          'allow null' => TRUE,
        ),
      ),
      'group' => t('visitor_actions'),
      'base' => 'visitor_actions_rule_triggered',
    ),
  );
}

/**
 * Fetches all rules that have been defined with the visitor_actions rules plugin.
 *
 * @return An array of action definitions, keyed by action name.
 */
function visitor_actions_get_rules() {
  // How the f do we cache this?
  $rules_visitor_actions = array();
  $rules = entity_load_multiple_by_name('rules_config');
  foreach ($rules as $name => $rule) {
    if (in_array('visitor_actions', $rule->dependencies)) {
      $actions = $rule->actions();
      foreach ($actions as $action) {
        $element_name = $action->getElementName();
        if ($element_name == 'trigger_visitor_action') {
          $rules_visitor_actions[$action->settings['action_name']] = array(
            'type' => 'server_side',
            'default_value' => $action->settings['action_value']
          );
        }
      }
    }
  }
  return $rules_visitor_actions;
}

/**
 * Sends a goal to an agent, invoked by a rules action.
 *
 * @param string $agent
 *   The name of the agent to send the goal to.
 * @param $goal_name
 *   The name of the goal to send.
 * @param null $goal_value
 *   The value of the goal to send.
 */
function visitor_actions_rule_triggered($action_name, $action_value = NULL) {
  $context = array();
  if (is_numeric($action_value)) {
    $context['value'] = $action_value;
  }
  visitor_actions_trigger_event($action_name, $context);
}

