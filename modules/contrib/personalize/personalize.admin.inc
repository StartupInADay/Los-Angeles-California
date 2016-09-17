<?php

/**
 * @file personalize.admin.inc
 * Provides functions needed for the admin UI.
 */

/**
 * Admin form for configuring personalization backends.
 */
function personalize_admin_form($form, &$form_state) {
  $form['#attached']['css'][] = drupal_get_path('module', 'personalize') . '/css/personalize.admin.css';
  $form['cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cache storage'),
    '#tree' => FALSE,
    '#attributes' => array(
      'class' => array('personalize-admin-cache-settings'),
    ),
  );
  $form['cache']['personalize_local_caching_storage'] = array(
    '#type' => 'select',
    '#title' => t('Clear decision cache'),
    '#options' => array(
      'session' => t('at end of session'),
      'local' => t('after ...'),
    ),
    '#default_value' => variable_get('personalize_local_caching_storage', 'session'),
  );
  $form['cache']['personalize_local_caching_expiration'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('personalize_local_caching_expiration', 30),
    '#field_suffix' => t('minutes'),
    '#size' => 5,
    '#states' => array(
      'visible' => array(
        ':input[name="personalize_local_caching_storage"]' => array('value' => 'local'),
      ),
    ),
  );
  $form['personalize_use_admin_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use admin mode'),
    '#description' => t('When this box is checked, users with the "Manage personalized content" permission will not trigger any personalization activity such as requests for decisions or the sending of goals.'),
    '#default_value' => variable_get('personalize_use_admin_mode', TRUE),
  );
  $form['personalize_enable_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),
    '#description' => t('Select this check box to display Acquia Lift diagnostic information in certain browser consoles. !learnmore.', array('!learnmore' => l(t('Learn more'), 'https://docs.acquia.com/lift/drupal/debug', array('attributes' => array('target' => '_blank'))))),
    '#default_value' => variable_get('personalize_enable_debug_mode', FALSE),
  );
  // Add visitor context configuration.
  $settings = variable_get('personalize_visitor_context_disabled', array());
  $form['personalize_visitor_context_disabled'] = personalize_admin_build_visitor_context_select($settings, FALSE);
  $form['personalize_visitor_context_disabled']['#title'] = t('Disallowed visitor context items');
  $form['personalize_visitor_context_disabled']['#description'] = t('Select which visitor context items should never show up when configuring visitor context for personalizations.');

  // Add our own submit handler to run BEFORE settings form handler.
  $form['#submit'][] = 'personalize_admin_form_submit';

  return system_settings_form($form);
}

/**
 * Validation callback for the settings form.
 */
function personalize_admin_form_validate($form, &$form_state) {
  // Perform some slight re-organization of the values because the visitor context
  // settings need to be shuffled around.
  if (isset($form_state['values']['personalize_visitor_context_disabled'])) {
    $form_state['values']['personalize_visitor_context_disabled'] = personalize_admin_convert_visitor_context_form_values($form_state['values']['personalize_visitor_context_disabled']);
  }
}

/**
 * Submit callback for settings form.
 */
function personalize_admin_form_submit($form, &$form_state) {
  // Clear the visitor context cache settings if the allowed contexts changed.
  // This submit callback is called BEFORE the system_settings_form callback.
  if (isset($form_state['values']['personalize_visitor_context_disabled']) && $form_state['values']['personalize_visitor_context_disabled'] != variable_get('personalize_visitor_context_disabled')) {
    personalize_visitor_context_expiration_clear();
  }
}

/**
 * Form for changing the status of an agent.
 *
 * @param stdClass $agent_data
 *   The standard data for the agent to change.
 * @param bool $combine_status_choice
 *   True if multiple statuses should be combined into a single select input.
 *   False if each choice should get its own submit button.
 * @param bool $forward_only
 *   (Optional) When set to true, only transitions that move the campaign
 *   status forward to the next level will be included.  Transitions to undo
 *   a prior transition change (e.g., unarchive, unschedule) will not be
 *   included.
 * @return array
 *   Array representing the status change form.
 */
function personalize_status_change_form($form, &$form_state, $agent_data, $combine_status_choice = TRUE, $forward_only = TRUE) {
  $agent_status = personalize_agent_get_status($agent_data->machine_name);
  $form = array();
  $form['agent_name'] = array(
    '#type' => 'value',
    '#value' => $agent_data->machine_name,
  );
  $form['status_wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('personalize-status-change'),
    ),
    '#tree' => FALSE,
  );
  $allowed_statuses = personalize_allowed_status_transitions($agent_status, $agent_data, $forward_only);
  if ($combine_status_choice) {
    // Present a dropdown of all available next statuses when multiple allowed.
    if (count($allowed_statuses) === 1) {
      $keys = array_keys($allowed_statuses);
      // Just present a single button to change the status.
      $form['status_wrapper']['status'] = array(
        '#type' => 'value',
        '#value' => reset($keys),
      );
      $submit_text = reset($allowed_statuses);
    }
    else {
      $form['status_wrapper']['status'] = array(
        '#type' => 'select',
        '#options' => $allowed_statuses,
        '#default_value' => '',
        '#empty_option' => t('--Select--'),
      );
      $submit_text = t('Change');
    }

    $form['status_wrapper']['submit'] = array(
      '#type' => 'submit',
      '#value' => $submit_text
    );
  }
  else {
    // Show a button for each status change.
    foreach ($allowed_statuses as $next_status => $transition_label) {
      $form['status_wrapper']['submit_' . $next_status] = array(
        '#type' => 'submit',
        '#value' => $transition_label,
        '#personalize_next_status' => $next_status,
      );
    }
  }
  return $form;
}

/**
 * Submit callback for the status change form.
 */
function personalize_status_change_form_submit(&$form, &$form_state) {
  if (isset($form_state['values']['status'])) {
    $status = $form_state['values']['status'];
  }
  else if (isset($form_state['triggering_element']['#personalize_next_status'])) {
    $status = $form_state['triggering_element']['#personalize_next_status'];
  }
  if (empty($status)) {
    return;
  }
  $agent_name = $form_state['values']['agent_name'];
  if ($status == PERSONALIZE_STATUS_COMPLETED) {
    // Redirect to a confirm form as this has more serious implications than
    // other status changes.
    $form_state['redirect'] = array('admin/structure/personalize/manage/'. $agent_name .'/complete');
    return;
  }
  // Otherwise just change the status as requested.
  if (!personalize_agent_set_status($agent_name, $status)) {
    return;
  }
  $status_map = personalize_get_agent_status_map();
  drupal_set_message(t('The personalization status has been set to @status', array('@status' => $status_map[$status])));
  if (request_path() === 'admin/structure/personalize') {
    $form_state['redirect'] = array('admin/structure/personalize', array('fragment' => 'personalization-group-' . $status));
  }
}

/**
 * Returns a visitor context select box.
 *
 * @param $settings
 *   The existing visitor context settings, needed for default values.
 * @param $exclude_disallowed
 *   (optional) Exclude context options disallowed by admin settings.
 * @param $agent_data
 *   (optional) Object representing an existing agent if this is an edit form.
 * @return mixed
 *   An array representing a visitor_context dropdown form element or FALSE
 *     if no context options are available to show
 */
function personalize_admin_build_visitor_context_select($settings, $exclude_disallowed = TRUE, $agent_data = NULL) {
  $groups = personalize_get_grouped_context_options($agent_data, $exclude_disallowed);
  // We'll need an array of default values.
  $selected = array();
  // Count the number of items in total so we can determine the correct size of
  // the multi-select element.
  $group_count = 0;
  foreach ($groups as $group) {
    $group_count++;
    if (is_array($group)) {
      $group_count = $group_count + count($group);
      foreach ($group as $option_name => $friendly_name) {
        list($plugin_name, $code) = explode(PERSONALIZE_TARGETING_ADMIN_SEPARATOR, $option_name);
        // Build up our default values based on the settings passed in.
        if (isset($settings[$plugin_name][$code])) {
          $selected[$option_name] = $option_name;
        }
      }
    }
  }
  if ($group_count === 0) {
    return FALSE;
  }
  $element = array(
    '#type' => 'select',
    '#title' => t('Visitor contexts'),
    '#options' => $groups,
    '#multiple' => TRUE,
    '#default_value' => $selected,
    '#size' => $group_count > 0 ? $group_count : 1,
  );
  return $element;
}

/**
 * Returns the available context options, sorted into groups.
 *
 * The array returned is suitable for use in a select element.
 *
 * @param null $agent
 *   An object representing an agent, if these settings apply to an agent.
 * @param bool $exclude_disallowed
 *   Whether to exclude any contexts that have been set as disallowed.
 * @param array $exclude_plugins
 *   An array of plugins to be excluded from the list.
 * @return array
 *   An associative array of available contexts where the keys are group names
 *   and each value is an associative array  of contexts with context name and
 *   friendly name as key and value respectively.
 */
function personalize_get_grouped_context_options($agent = NULL, $exclude_disallowed = TRUE, $exclude_plugins = array()) {
  ctools_include('plugins');
  $disallowed = $exclude_disallowed ? variable_get('personalize_visitor_context_disabled', array()) : array();
  $groups = array('Miscellaneous' => array());
  $contexts = personalize_get_visitor_contexts();
  foreach ($contexts as $plugin_name => $plugin_info) {
    if (in_array($plugin_name, $exclude_plugins)) {
      continue;
    }
    if ($class = ctools_plugin_load_class('personalize', 'visitor_context', $plugin_name, 'handler')) {
      if (!empty($agent)) {
        if (!call_user_func(array($class, 'allowedFromAgent'), $agent)) {
          continue;
        }
      }
      $context_options = call_user_func(array($class, 'getOptions'));
      // Organize the elements according to groups.
      foreach ($context_options as $code => $info) {
        if (isset($disallowed[$plugin_name][$code])) {
          continue;
        }
        $option_name = $plugin_name . PERSONALIZE_TARGETING_ADMIN_SEPARATOR . $code;
        if (isset($info['group'])) {
          $group = $info['group'];
          if (!isset($groups[$group])) {
            $groups[$group] = array();
          }
          $groups[$group][$option_name] = $info['name'];
        }
        else {
          $groups['Miscellaneous'][$option_name] = $info['name'];
        }
      }
    }
  }
  // Move the Miscellaneous group to the end.
  $misc = $groups['Miscellaneous'];
  unset($groups['Miscellaneous']);
  if (!empty($misc)) {
    $groups['Miscellaneous'] = $misc;
  }
  return $groups;
}

/**
 * Converts visitor context form values into the correct structure.
 *
 * @param $values
 *   The values that were submitted in the form.
 * @return array
 *   An array of properly structure visitor context settings.
 */
function personalize_admin_convert_visitor_context_form_values($values) {
  $visitor_context = array();
  // Visitor context form values are grouped by category. We want to
  // save them grouped by the plugin that provided them.
  $selected = array_filter($values);
  foreach ($selected as $name => $value) {
    // Element names are in the form {$plugin_name}__{$element_name}
    list($plugin_name, $element_name) = explode(PERSONALIZE_TARGETING_ADMIN_SEPARATOR, $name);
    if (!isset($visitor_context[$plugin_name])) {
      $visitor_context[$plugin_name] = array();
    }
    $visitor_context[$plugin_name][$element_name] = $element_name;
  }

  return $visitor_context;
}

/**
 * Menu callback for displaying a list of all agents.
 */
function personalize_agent_list($agents = NULL, $allow_edit = TRUE, $allow_alter = TRUE) {
  $status_map = personalize_get_agent_status_map();

  $header = array(
    array('data' => t('Name')),
    array('data' => t('Type')),
    array('data' => t('Change status')),
    array('data' => t('Operations'), 'colspan' => 4),
  );
  if ($agents === NULL) {
    $agents = personalize_agent_load_multiple(array(), array(), FALSE, TRUE, 'label');
  }

  if ($allow_alter) {
    // Allow other modules to alter the list of agents being displayed here.
    drupal_alter('personalize_agent_list', $agents);
  }

  if (empty($agents)) {
    return array(
      'no_agents' => array(
        '#markup' => '<div>' . t('No personalizations available.') . '</div>'
      )
    );
  }

  $sorted_agents = array(
    PERSONALIZE_STATUS_NOT_STARTED => array(),
    PERSONALIZE_STATUS_SCHEDULED => array(),
    PERSONALIZE_STATUS_RUNNING => array(),
    PERSONALIZE_STATUS_PAUSED => array(),
    PERSONALIZE_STATUS_COMPLETED => array()
  );

  foreach ($agents as $agent) {
    $agent_status = personalize_agent_get_status($agent->machine_name);
    $sorted_agents[$agent_status][] = $agent;
  }

  $build = array(
    'personalizations_list' => array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => 'personalize-personalizations-list',
      ),
      'navigation_bar' => array(),
    ),
  );
  $navigation_bar = array();
  foreach ($sorted_agents as $status => $agents) {
    if (empty($agents)) {
      continue;
    }
    // Notice the 'href', 'fragment', and 'external' here are made specific to theme an anchor link.
    $navigation_bar['personalization-group-' . $status] = array(
      'title' => '<h3>' . $status_map[$status] . '</h3>',
      'href' => '',
      'fragment' => 'personalization-group-' . $status,
      'external' => TRUE,
      'html' => TRUE,
    );
    $description = '';
    switch($status) {
      case PERSONALIZE_STATUS_NOT_STARTED:
        $description = t('This table lists personalizations that have never been started.');
        break;
      case PERSONALIZE_STATUS_SCHEDULED:
        $description = t('This table lists personalizations that are scheduled to start in the future.');
        break;
      case PERSONALIZE_STATUS_RUNNING:
        $description = t('This table lists personalizations that are displaying personalized content to website visitors for each variation set.');
        break;
      case PERSONALIZE_STATUS_PAUSED:
        $description = t('This table lists personalizations that use personalization JavaScript to display the fallback/winner variation for each variation set.');
        break;
      case PERSONALIZE_STATUS_COMPLETED:
        $description = t('This table lists personalizations that use PHP (whenever possible) to display the fallback/winner variation for each variation set, which can cause variations to display more quickly than those in paused personalizations.');
        break;
    }
    $build['personalizations_list']['personalization_group_' . $status] = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => 'personalization-group-' . $status,
      ),
    );
    $build['personalizations_list']['personalization_group_' . $status]['description'] = array(
      '#markup' => '<p>' . $description . '</p>'
    );
    $rows = array();
    foreach ($agents as $agent) {
      // The ability to delete depends on the storage type and on whether the campaign has started
      // or contains option sets.
      $can_delete = personalize_delete_agent_access($agent);
      $delete_link = '';
      // Determine storage
      if ($can_delete) {
        switch ($agent->export_type) {
          case EXPORT_IN_DATABASE | EXPORT_IN_CODE:
            $delete_link = l(t('Revert'), 'admin/structure/personalize/manage/'. $agent->machine_name .'/delete');
            break;
          case EXPORT_IN_DATABASE:
            $delete_link = l(t('Delete'), 'admin/structure/personalize/manage/'. $agent->machine_name .'/delete');
            break;
        }
      }

      $status_change_form = drupal_get_form("personalize_change_status_{$agent->machine_name}_form", $agent);
      // @todo We are ignoring the "storage" info and the clone/export functionality
      //   for now. Revisit once we can provide full support for clone/export.
      $plugin = personalize_agent_load_agent($agent->machine_name);
      $tablerow = array(
        array('data' => check_plain($agent->label)),
        array('data' => $agent->plugin),
        array('data' => drupal_render($status_change_form)),
        //array('data' => $storage),
        array('data' => $allow_edit ? l(t('Edit'), 'admin/structure/personalize/manage/'. $agent->machine_name . '/edit') : ''),
        array('data' => $delete_link),
      );
      // Show the scheduled start date for campaigns that aren't running yet.
      if ($status === PERSONALIZE_STATUS_NOT_STARTED || $status == PERSONALIZE_STATUS_SCHEDULED) {
        $start_date = personalize_agent_get_start_date($agent->machine_name);
        array_splice($tablerow, 1, 0, array('data' => $start_date > 0 ? format_date($start_date, 'custom', 'M d, Y') : ''));
      }
      // Remove the status change for a completed campaign.
      if ($status === PERSONALIZE_STATUS_COMPLETED) {
        array_splice($tablerow, 2, 1);
      }

      $rows[] = $tablerow;
    }
    // Add the scheduled start date in to the "not started" table headers.
    $build_header = $header;
    if ($status === PERSONALIZE_STATUS_NOT_STARTED || $status == PERSONALIZE_STATUS_SCHEDULED) {
      array_splice($build_header, 1, 0, array('data' => t('Scheduled start')));
    }
    if ($status === PERSONALIZE_STATUS_COMPLETED) {
      array_splice($build_header, 2, 1);
    }

    $build['personalizations_list']['personalization_group_' . $status]['table'] = array(
      '#theme' => 'table',
      '#header' => $build_header,
      '#rows' => $rows,
      '#attributes' => array('id' => 'personalize'),
    );
  }
  $build['personalizations_list']['navigation_bar']['#markup'] = theme('links', array('links' => $navigation_bar));
  $build['#attached']['library'][] = array('personalize', 'admin.campaign.list');

  return $build;
}

/**
 * Form for setting the status of an agent to "Completed".
 *
 * @todo This duplicates code in personalize_status_toggle_message - refactor
 *   it out to its own function.
 */
function personalize_agent_complete_form($form, &$form_state, $agent) {
  // Display information about the "winner" that will be shown for each option
  // set in this agent.
  $option_sets = personalize_option_set_load_by_agent($agent->machine_name);
  $theme_variables = array(
    'option_sets' => array(),
    'option_message' => '',
    'alert_message' => '',
    'agent_name' => $agent->machine_name,
  );
  foreach($option_sets as $option_set) {
    $v = 1;

    // No winner selected, so first item is shown as the control item.
    if (empty($option_set->winner)) {
      $winner = $option_set->options[0];
      $winner_type = t('Control');
    }
    // Show the selected winning option.
    else {
      foreach($option_set->options as $option) {
        if ($option_set->winner == $option['option_id']) {
          $winner = $option;
          $winner_type = t('Winner');
          break;
        }
        $v++;
      }
    }
    // Generate the message shown for each 'winning' option.
    $theme_variables['option_sets'][] = array(
      'label' => (isset($option_set->label) ? check_plain($option_set->label) : t('Option @id', array('@id' => $option_set->osid))),
      'counter' => 'V' . $v,
      'winner_label' => $winner['option_label'],
      'alert' => (!empty($winner['targeting_features']) ? t('Show @v to specific users will not apply when paused.', array('@v' => 'V' . $v)) : ''),
      'winner_type' => $winner_type,
    );
  }
  if (!empty($theme_variables['option_sets'])) {
    $theme_variables['option_message'] = t('All visitors will see the following @variations:', array('@variations' => format_plural(count($option_sets), 'variation', 'variations')));
  }
  $form['message'] = array(
    '#markup' => theme('personalize_campaign_status_update', $theme_variables)
  );
  $form['machine_name'] = array('#type' => 'hidden', '#value' => $agent->machine_name);
  $form['title'] = array('#type' => 'hidden', '#value' => $agent->label);
  return confirm_form($form, t('Are you sure you want to stop the agent %title?', array('%title' => $agent->label)), 'admin/structure/personalize', '', t('Complete'), t('Cancel'));
}

/**
 * Submit handler for agent completion form.
 */
function personalize_agent_complete_form_submit($form, &$form_state) {
  personalize_agent_set_status($form_state['values']['machine_name'], PERSONALIZE_STATUS_COMPLETED);
  drupal_set_message(t('The campaign %name has been stopped.', array('%name' => $form_state['values']['title'])));
  $form_state['redirect'] = 'admin/structure/personalize';
}

/**
 * Menu callback for displaying a list of campaign goals.
 */
function personalize_goals_list($form, &$form_state) {
  $goals = personalize_goal_load_multiple();
  $actions = personalize_goals_options();

  $header = array(
    array('data' => t('Goal')),
    array('data' => t('Personalization')),
    array('data' => t('Value')),
    array('data' => t('Edit')),
    array('data' => t('Delete')),
  );

  $rows = array();
  foreach($goals as $goal) {
    $campaign = personalize_agent_load($goal->agent);
    $rows[] = array(
      $actions[$goal->action],
      check_plain($campaign->label),
      $goal->value,
      l(t('edit'), 'admin/structure/personalize/manage/' . $goal->agent . '/goals', array('query' => array('goal' => $goal->id))),
      l(t('delete'), 'admin/structure/personalize/goals/' . $goal->id . '/delete'),
    );
  }

  $form['results'] = array(
    '#markup' => theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(
        'id' => 'personalize-goals',
      ),
    )),
  );
  return $form;
}

/**
 * Form creation function to delete a specified goal.
 *
 * @param array $goal
 *   The goal to be deleted.
 */
function personalize_admin_goal_delete($form, &$form_state, $goal) {
  $options = personalize_goals_options();
  $campaign = personalize_agent_load($goal->agent);
  if (personalize_agent_get_status($campaign->machine_name) == PERSONALIZE_STATUS_RUNNING) {
    $description = t('The %goal goal will no longer be tracked in the currently running %campaign personalization.', array(
      '%campaign' => $campaign->label,
      '%goal' => $options[$goal->action]
    ));
  }
  else {
    $description = t('The %goal goal will no longer be tracked when the %campaign personalization is started.', array(
      '%campaign' => $campaign->label,
      '%goal' => $options[$goal->action]
    ));
  }
  $form['goal'] = array(
    '#type' => 'value',
    '#value' => $goal,
  );
  return confirm_form($form, t('Are you sure you want to delete %goal from %campaign?', array(
    '%goal' => $options[$goal->action],
    '%campaign' => $campaign->label,
  )), 'admin/structure/personalize/manage/' . $goal->agent . '/goals', $description);
}

/**
 * Form submission handler to delete a specific goal.
 */
function personalize_admin_goal_delete_submit($form, &$form_state) {
  personalize_goal_delete($form_state['values']['goal']->id);
  $form_state['redirect'] = 'admin/structure/personalize/manage/' . $form_state['values']['goal']->agent . '/goals';
}

/**
 * Update messaging when the user changes the status of a campaign.
 */
function personalize_status_toggle_message($agent_name, $current_status) {
  if ($current_status == PERSONALIZE_STATUS_RUNNING || $current_status == PERSONALIZE_STATUS_NOT_STARTED) {
    return;
  }
  $theme_variables = array(
    'option_sets' => array(),
    'option_message' => '',
    'alert_message' => '',
    'agent_name' => $agent_name,
  );
  $option_sets = personalize_option_set_load_by_agent($agent_name);
  foreach($option_sets as $option_set) {
    $v = 1;

    // No winner selected, so first item is shown as the control item.
    if (empty($option_set->winner)) {
      $winner = $option_set->options[0];
      $winner_type = t('Control');
    }
    // Show the selected winning option.
    else {
      foreach($option_set->options as $option) {
        if ($option_set->winner == $option['option_id']) {
          $winner = $option;
          $winner_type = t('Winner');
        }
        $v++;
      }
    }
    // Generate the message shown for each 'winning' option.
    $theme_variables['option_sets'][] = array(
      'label' => (isset($option_set->label) ? check_plain($option_set->label) : t('Option @id', array('@id' => $option_set->osid))),
      'counter' => 'V' . $v,
      'winner_label' => $winner['option_label'],
      'alert' => (!empty($winner['targeting_features']) ? t('Show @v to specific users will not apply when paused.', array('@v' => 'V' . $v)) : ''),
      'winner_type' => $winner_type,
    );
  }
  if (!empty($theme_variables['option_sets'])) {
    $theme_variables['option_message'] = t('All visitors will see the following variations:');
  }
  drupal_set_message(theme('personalize_campaign_status_update', $theme_variables));
}

/**
 * Builds the basic form for agent creation.
 *
 * @param $agent_data
 *   Object representing an existing agent if this is an edit form.
 * @param $include_machine_name_field
 *   Whether to include the machine name field in the form. (This form can be
 *   embedded, with #states controlling whether it is visible or not, in which
 *   case the machine_name field doesn't work.)
 * @param $parent_array
 *   The #array_parents of the new form if nested.
 */
function personalize_agent_build_basic_form($agent_data = NULL, $include_machine_name_field = TRUE, $parent_array = NULL) {
  $form = array();
  $form['agent_basic_info'] = array(
    '#tree' => TRUE
  );
  if ($agent_data === NULL) {
    $agent_data = new stdClass();
  }
  $form['#agent'] = $agent_data;
  // Make sure we have at least one agent type available.
  $agent_types = personalize_get_agent_types();
  $agent_type_options = array();
  foreach ($agent_types as $name => $info) {
    if ($class = ctools_plugin_load_class('personalize', 'agent_type', $name, 'handler')) {
      // Add this option to the options for the "agent type" dropdown.
      $agent_type_options[$name] = $name;
    }
  }
  if (empty($agent_type_options)) {
    drupal_set_message(t('You don\'t have any agent types enabled. Please enable the personalize_target module or another module that provides an agent type.'), 'error');
    return FALSE;
  }
  ksort($agent_type_options);
  $form['agent_basic_info']['title'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => isset($agent_data->label) ? $agent_data->label : '',
    '#weight' => -9,
    '#required' => TRUE,
  );
  if ($include_machine_name_field) {
    // Define the parent array to the source input.
    if (empty($parent_array)) {
      $parent_array = array();
    }
    $parent_array[] = 'agent_basic_info';
    $parent_array[] = 'title';
    $form['agent_basic_info']['machine_name'] = array(
      '#type' => 'machine_name',
      '#maxlength' => PERSONALIZE_MACHINE_NAME_MAXLENGTH,
      '#machine_name' => array(
        'exists' => 'personalize_agent_machine_name_exists',
        'source' => $parent_array,
        'replace_pattern' => '[^a-z0-9-]+',

        'replace' => '-',
      ),
      '#description' => t('A unique machine-readable name for this agent. It must only contain lowercase letters, numbers, and hyphens.'),
      '#weight' => -8,
    );
    if (!empty($agent_data->machine_name)) {
      $form['agent_basic_info']['machine_name']['#default_value'] = $agent_data->machine_name;
      $form['agent_basic_info']['machine_name']['#disabled'] = TRUE;
      $form['agent_basic_info']['machine_name']['#value'] = $agent_data->machine_name;
    }
  }

  if (!empty($agent_data->plugin)) {
    // It is not possible to change the type of an agent.
    $form['agent_basic_info']['agent_type'] = array(
      '#type' => 'value',
      '#value' => $agent_data->plugin,
    );
  }
  elseif (count($agent_type_options) < 2) {
    // No need to show a dropdown if there's only one available plugin.
    $form['agent_basic_info']['agent_type'] = array(
      '#type' => 'hidden',
      '#value' => key($agent_type_options),
    );
  }
  else {
    $form['agent_basic_info']['agent_type'] = array(
      '#type' => 'select',
      '#title' => t('Agent Type'),
      '#options' => $agent_type_options,
      '#default_value' => '',
      '#description' => t('Choose which type of agent to create.'),
      '#weight' => -7,
    );
  }

  return $form;
}

/**
 * Builds the form elements needed for adding explicit targeting to an option.
 *
 * @param $option_set
 *   The option set this explicit targeting config is for.
 * @param $option
 *   The individual option this explicit targeting config is for.
 * @param $targeting_values
 *   The available context values.
 * @param $targeting_support
 *   How multiple targeting features can be handled, i.e. using AND, OR or either
 *   of these, in which case a dropdown is provided for the user to select.
 * @param $states
 *   An array to use as the states property for these elements.
 * @param $parents
 *   The array of parents of these elements within the larger form
 *
 * @return array
 *   An array representing the explicit targeting portion of the form.
 */
function personalize_targeting_support_form_elements($option_set, $option, $targeting_values, $targeting_support, $states, $parents, &$form_state) {
  $attributes = array(
    'class' => array('personalize-variation-row'),
  );

  $osid = $option_set->osid;
  $form = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#attributes' => $attributes,
    '#states' => $states,
  );

  // This is the portion of the form that will be replace when the "add new" or "remove
  // context" links are clicked.
  $main_wrapper_id = drupal_strtolower('personalize-targeting-osid-' . $osid . '-' . $option['option_id']);

  $form['mapping'] = array(
    '#tree' => TRUE,
    '#theme_wrappers' => array('container'),
    '#attributes' => $attributes,
  );
  $form['mapping']['contexts'] = array(
    '#type' => 'container',
    '#states' => $states,
    '#attributes' => array(
      'id' => $main_wrapper_id,
    ),
  );

  // Get an array of targeting rules per option id.
  $targeting_for_options = personalize_get_targeting_for_options($option_set);
  // Load from the existing form if passed first, otherwise, load from option
  // set.
  $mappings = array();
  if (isset($form_state['values']['option_sets']['option_set_' . $osid]['options'][$option['option_id']]['explicit_targeting']['mapping']['contexts'])) {
    foreach($form_state['values']['option_sets']['option_set_' . $osid]['options'][$option['option_id']]['explicit_targeting']['mapping']['contexts'] as $delta => $context) {
      if (empty($context['context'])) {
        continue;
      }
      list($plugin_name, $context_option) = explode(PERSONALIZE_TARGETING_ADMIN_SEPARATOR, $context['context']);
      // Important: preserve the delta passed through the form as it is used
      // to determine the item to delete when "remove" is clicked.
      $mappings[$delta] = array(
        'plugin' => $plugin_name,
        'context' => $context_option,
        'operator' => $context['value']['operator'],
        'match' => $context['value']['match'],
      );
    }
  }
  else if (isset($targeting_for_options[$option['option_id']])) {
    $rule = $targeting_for_options[$option['option_id']];
    if (isset($rule['targeting_features'])) {
      foreach ($rule['targeting_features'] as $feature) {
        if (isset($rule['targeting_rules'][$feature])) {
          $mappings[] = $rule['targeting_rules'][$feature];
        }
      }
    }
  }

  // If the "Remove" button was clicked for a context, we need to remove that context
  // from the form.
  if (isset($form_state['to_remove']['option_set_' . $osid][$option['option_id']])) {
    unset($mappings[$form_state['to_remove']['option_set_' . $osid][$option['option_id']]]);
    unset($form_state['to_remove']['option_set_' . $osid][$option['option_id']]);
    $form_state['num_contexts']['option_set_' . $osid][$option['option_id']]--;
  }

  // Make sure there is at least an empty context.
  if (empty($mappings)) {
    $mappings[] = array(
      'context' => '',
      'operator' => 'equals',
      'match' => '',
      'plugin' => '',
    );
  }

  // If the "Add another" button was clicked, we need to add contexts to get up
  // to the number indicated.
  $num_contexts = count($mappings);
  if (isset($form_state['num_contexts']['option_set_' . $osid][$option['option_id']]) && $form_state['num_contexts']['option_set_' . $osid][$option['option_id']] > $num_contexts) {
    while ($num_contexts < $form_state['num_contexts']['option_set_' . $osid][$option['option_id']]) {
      $mappings[] = array(
        'context' => '',
        'operator' => 'equals',
        'match' => '',
        'plugin' => '',
      );
      $num_contexts++;
    }
  }

  $form_state['num_contexts']['option_set_' . $osid][$option['option_id']] = count($mappings);
  foreach ($mappings as $delta => $mapping) {
    $wrapper_id =  $osid . '-' . $option['option_id'] . '-' . $delta;
    $form['mapping']['contexts'][$delta] = personalize_explicit_targeting_mapping_element($mapping, $targeting_values, $wrapper_id);

    // Add a "remove" button for this context.
    // NOTE: ajax.js expects the ID of the element to match the element's name
    // even when a different selector is passed.
    $form['mapping']['contexts'][$delta]['remove'] = array(
      '#prefix' => '<div class="personalize-remove-context">',
      '#suffix' => '</div>',
      '#type' => 'submit',
      '#tag' => 'button',
      '#text' => t('Remove'),
      '#value' => 'remove_' . $osid . '_' . $option['option_id'] . '_' . $delta,
      '#theme_wrappers' => array('personalize_html_tag'),
      '#attributes' => array(
        'class' => array('personalize-delete-context', 'form-submit',),
        'title' => t('Delete this context.'),
        'id' => drupal_strtolower('edit-option-sets-option-set-' . trim($osid, '- ') . '-options-' . trim($option['option_id'], ' -') . '-explicit-targeting-mapping-contexts-' . $delta . '-remove'),
        'name' => 'option-sets[option-set-' . trim($osid, '- ') . '][options][' . trim($option['option_id'], ' -') . '][explicit-targeting][mapping][contexts][' . $delta . '][remove]'
      ),
      '#submit' => array('personalize_targeting_remove_context_submit'),
      '#ajax' => array(
        'callback' => 'personalize_targeting_context_ajax_callback',
        'wrapper' => $main_wrapper_id,
        'effect' => 'fade',
      ),
    );
  }
  // Create an "add new context" link.
  $form['add_new'] = array(
    '#prefix' => '<span class="personalize-add-link-prefix"></span>',
    '#type' => 'submit',
    '#tag' => 'button',
    '#text' => t('Add context'),
    '#value' => 'add_context_' . $osid . '_' . $option['option_id'],
    '#theme_wrappers' => array('personalize_html_tag'),
    '#attributes' => array(
      'id' => drupal_strtolower('edit-option-sets-option-set-' . trim($osid, ' -') . '-options-' . trim($option['option_id'], ' -') . '-explicit-targeting-add-new'),
      'class' => array('personalize-add-link'),
      'title' => t('Click here to add more contexts.'),
      'name' => 'option_sets[option_set_' . trim($osid) . '][options][' . trim($option['option_id']) . '][explicit_targeting][add_new]',
    ),
    '#submit' => array('personalize_targeting_add_context_submit'),
    '#ajax' => array(
      'callback' => 'personalize_targeting_context_ajax_callback',
      'wrapper' => $main_wrapper_id,
      'effect' => 'fade',
    ),
  );
  $strategy_parents = $parents;
  foreach (array('explicit_targeting', 'strategy') as $parent) {
    $strategy_parents[] = $parent;
  }
  // And/Or targeting selection.
  if ($targeting_support == PersonalizeExplicitTargetingInterface::EXPLICIT_TARGETING_MULTIPLE_BOTH) {
    $form['strategies'] = array(
      '#type' => 'container',
      '#attributes' => $attributes,
      '#states' => $states,
    );

    // Add radio buttons so the user can select how multiple features for an option
    // should be treated.
    $default_strategy = 'OR';
    if (isset($targeting_for_options[$option['option_id']]) && isset($targeting_for_options[$option['option_id']]['targeting_strategy'])) {
      $default_strategy = $targeting_for_options[$option['option_id']]['targeting_strategy'];
    }
    $form['strategies']['strategy'] = array(
      '#type' => 'select',
      '#multiple' => FALSE,
      '#field_prefix' => t('Visitor must have '),
      '#field_suffix' => t(' of the specified contexts'),
      '#description' => t('Choose how multiple contexts should be applied to options. Choose "any" if the rule should apply if the user has any of the contexts. Choose "all" if the rule should apply only if the user has all of the contexts.'),
      '#options' => array(
        'OR' => 'any',
        'AND' => 'all'
      ),
      '#default_value' => $default_strategy,
      '#parents' => $strategy_parents,
    );
  }
  else {
    // Send the strategy as a value so it can be used when checking for features
    // that have been assigned to multiple options.
    $form['strategy'] = array(
      '#type' => 'value',
      '#value' => PersonalizeExplicitTargetingInterface::EXPLICIT_TARGETING_MULTIPLE_OR ? 'OR' : 'AND',
      '#parents' => $strategy_parents,
    );
  }
  return $form;
}

/**
 * Helper function to generate an element for the selection of context to apply
 * to a rule.
 *
 * @param array $mapping
 *   The mapping for the context plugin and values to match.  Mapping contains:
 *   - plugin: the context plugin responsible for providing this data.
 *   - context: the specific context to match
 *   - operator: the operator that defines the type of match
 *   - match: the value to match (if appropriate to the operator)
 * @param array $targeting_values
 *   The available context values.
 * @param string $wrapper_id
 *   A unique id that can be used within this element for AJAX purposes.
 */
function personalize_explicit_targeting_mapping_element($mapping, $targeting_values, $wrapper_id) {
  // @todo Cache the context options, value types, and operator options as this
  // function can be called multiple times per form.
  $context_options = array('' => '-- ' . t('Select a context') . ' --');
  $value_types = array();
  foreach ($targeting_values as  $key => $info) {
    $option_key = $info['visitor_context'] . PERSONALIZE_TARGETING_ADMIN_SEPARATOR . $key;
    $key_label = isset($info['friendly name']) ? $info['friendly name'] : $key;
    $context_options[$option_key] = $key_label;
    $value_types[$option_key] = $info['value type'];
  }

  $wrapper_id = 'operator-dropdown-replace-' . $wrapper_id;
  $selected_plugin_context = $mapping['plugin'] . PERSONALIZE_TARGETING_ADMIN_SEPARATOR . $mapping['context'];
  $selected_context = $mapping['context'];
  $selected_operator = $mapping['operator'];
  $selected_match = $mapping['match'];
  $operator_options = array(
    'string' => array(
      'equals' => t('equals'),
      'contains' => t('contains'),
      'starts' => t('starts with'),
      'ends' => t('ends with')
    ),
    'number' => array(
      'equals' => t('equals'),
      'numgt' => t('greater than'),
      'numlt' => t('less than')
    )
  );
  $element = array(
    '#prefix' => '<div class="personalize-target-wrapper">',
    '#suffix' => '</div>',
    'context' => array(
      '#type' => 'select',
      '#title' => t('Context'),
      '#options' => $context_options,
      '#default_value' => empty($selected_plugin_context) ? '' : $selected_plugin_context,
      '#ajax' => array(
        'event' => 'change',
        'callback' => 'personalize_explicit_targeting_context_callback',
        'wrapper' => $wrapper_id,
        'progress' => array(
          'type' => 'none',
        ),
      ),
      '#attributes' => array(
        'class' => array('personalize-targeting-context'),
      ),
    ),
  );
  $element['value'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div id="' . $wrapper_id. '" class="personalize-target-value">',
    '#suffix' => '</div>'
  );
  $value_type = isset($value_types[$selected_plugin_context]) ? $value_types[$selected_plugin_context] : 'string';
  // We use different form elements depending on whether there is a predefined list
  // of possible values or not.
  switch ($value_type) {
    case 'predefined':
      $element['value']['operator'] = array(
        '#type' => 'value',
        '#value' => 'equals',
      );
      $element['value']['match'] = array(
        '#type' => 'select',
        '#options' => $targeting_values[$selected_context]['values'],
        '#title' => 'Value',
        '#default_value' => $selected_match,
      );
      break;
    case 'boolean':
      $element['value']['operator'] = array(
        '#type' => 'value',
        '#value' => 'equals',
      );
      $off_label = isset($targeting_values[$selected_context]['off_label']) ? $targeting_values[$selected_context]['off_label'] : 'No';
      $on_label = isset($targeting_values[$selected_context]['on_label']) ? $targeting_values[$selected_context]['on_label'] : 'Yes';
      $element['value']['match'] = array(
        '#type' => 'select',
        '#options' => array(1 => $on_label, 0 => $off_label),
        '#title' => 'Value',
        '#default_value' => $selected_match,
      );
      break;
    default:
      $element['value']['operator'] = array(
        '#type' => 'select',
        '#title' => t('Operator'),
        '#options' => $operator_options[$value_type],
        '#default_value' => $selected_operator
      );
      $element['value']['match'] = array(
        '#type' => 'textfield',
        '#title' => 'Value',
        '#size' => $value_type == 'number' ? 6 : 30,
        '#default_value' => $selected_match,
      );
      break;
  }
  return $element;
}

/**
 * Ajax callback for the add context and remove context buttons.
 */
function personalize_targeting_context_ajax_callback($form, $form_state) {
  $parents = $form_state['triggering_element']['#array_parents'];
  $section = $form;
  $pos = array_search('explicit_targeting', $parents);
  for ($i = 0; $i <= $pos; $i++) {
    $section = $section[$parents[$i]];
  }
  return $section['mapping']['contexts'];
}

/**
 * Submit handler for the "Add Context" button.
 */
function personalize_targeting_add_context_submit($form, &$form_state) {
  $parents = $form_state['clicked_button']['#parents'];
  $option_set = $parents[1];
  $option_id = $parents[3];
  // Increment the number of goal elements to be rendered.
  $form_state['num_contexts'][$option_set][$option_id]++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "Remove Context" button.
 */
function personalize_targeting_remove_context_submit($form, &$form_state) {
  $parents = $form_state['clicked_button']['#parents'];
  // The parents are option_sets, option_set_[osid], options, [option_id],
  // explicit_targeting, mapping, contexts, [delta]. At least if we only depend
  // on the position of the immediate parent of each element we need, that makes
  // this a little less vulnerable to future chnages made to the form structure.
  $option_set_pos = array_search('option_sets', $parents) + 1;
  $option_id_pos = array_search('options', $parents) + 1;
  $delta_pos = array_search('contexts', $parents) + 1;
  $option_set = $parents[$option_set_pos];
  $option_id = $parents[$option_id_pos];
  $delta = $parents[$delta_pos];
  $form_state['to_remove'][$option_set][$option_id] = $delta;
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for the explicit targeting context dropdown.
 *
 * Returns the operator and value portion of the form, which are different
 * depending on what was selected in the context dropdown.
 */
function personalize_explicit_targeting_context_callback($form, &$form_state) {
  // Don't show messages in the small portion of the form returned.
  drupal_get_messages(NULL, TRUE);
  // We need to return the section of the form corresponding to one level up from
  // the dropdown that triggered the ajax, so use the array parents to find that
  // section.
  $parents = $form_state['triggering_element']['#array_parents'];
  $section = $form;
  for ($i = 0; $i < count($parents) -1; $i++) {
    $section = $section[$parents[$i]];
  }
  return $section['value'];
}

/**
 * Helper function to return targeting values for a particular visitor context
 * plugin and agent.
 *
 * This function returns the results in an array suitable for fixed targeting.
 *
 * @param string $plugin_name
 *   The name of the plugin to get targeting values from.
 * @param stdClass $agent_data
 *   The agent data for the current campaign.
 * @param array $selected_context
 *   (Optional) An array of pre-selected contexts for the agent in order to
 *   limit the returned values.  Only pass if limit is desired.
 * @return array
 *   An associative array keyed by context name, with an array of information about the
 *   possible values for that context. The 'value type' key in this array indicates what
 *   type of value this field can hold and is one of 'predefined', 'string' or 'number'. If
 *   the value type is 'predefined', then there must be a 'values' array with the list of
 *   predefined possible values. For example:
 *     array(
 *       'some_context' => array(
 *         'label' => 'Some Context',
 *         'type' => 'string',
 *         'visitor_context' => 'Plugin name',
 *        ),
 *       'some_other_context' => array(
 *         'label' => 'Some Other Context',
 *         'type' => 'predefined',
 *         'visitor_context' => 'Plugin name',
 *         'values' => array(
 *           'some_value' => 'Some Value',
 *           'some_other_value' => 'Some Other Value',
 *         ),
 *       ),
 *     )
 */
function _personalize_get_targeting_values($plugin_name, $agent_data, $limit_to_context = NULL) {
  ctools_include('plugins');
  $values = array();
  $agent_instance = personalize_agent_load_agent($agent_data->machine_name);
  $limit = TRUE;
  if (!isset($limit_to_context)) {
    $limit = FALSE;
    $limit_to_context = array();
  }

  if ($class = ctools_plugin_load_class('personalize', 'visitor_context', $plugin_name, 'handler')) {
    // Make sure this agent is allowed to use this context.
    if (!call_user_func(array($class, 'allowedFromAgent'), $agent_data)) {
      return $values;
    }
    if ($plugin = call_user_func_array(array($class, 'create'), array($agent_instance, array_keys($limit_to_context)))) {
      // Get the values and add in the source visitor context plugin name.
      $values = $plugin->getPossibleValues($limit);

      array_walk($values, function (&$item, $key, $visitor_context_plugin) {
        $item['visitor_context'] = $visitor_context_plugin;
      }, $plugin_name);
    }
  }
  return $values;
}

/**
 * Gets all targeting options that can be selected for an agent.
 *
 * @param stdClass $agent_data
 *   The personalize agent plugin data.
 * @return array
 *   An associative array keyed by context name, with an array of information
 *   about the possible values for that context.
 */
function personalize_get_targeting_options_for_agent($agent_data) {
  $contexts = personalize_get_visitor_contexts();
  $value_options = array();
  foreach ($contexts as $plugin_name => $plugin_info) {
    $value_options = array_merge($value_options, _personalize_get_targeting_values($plugin_name, $agent_data));
  }
  return $value_options;
}

/**
 * Clone agent.
 */
function personalize_agent_clone($agent) {
  unset($agent->machine_name);
  $agent->label = '';
  return drupal_get_form('personalize_agent_form', 'clone', $agent);
}

/**
 * Form for deleting an agent.
 */
function personalize_agent_delete_form($form, $form_state, $agent) {
  $form['machine_name'] = array('#type' => 'hidden', '#value' => $agent->machine_name);
  $form['title'] = array('#type' => 'hidden', '#value' => $agent->label);
  return confirm_form($form, t('Are you sure you want to delete the agent %title?', array('%title' => $agent->label)), 'admin/structure/personalize', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for agent deletion form.
 */
function personalize_agent_delete_form_submit($form, &$form_state) {
  personalize_agent_delete($form_state['values']['machine_name']);
  drupal_set_message(t('The agent %name has been removed.', array('%name' => $form_state['values']['title'])));
  $form_state['redirect'] = 'admin/structure/personalize';
}

/**
 * Export form for agents.
 */
function personalize_agent_export_form($form, &$form_state, $agent) {
  ctools_include('export');
  // Generate export code
  $code = '$items = array();' ."\n";
  $code .= ctools_export_object('personalize_agent', $agent, '');
  $code .= '$items["'. $agent->machine_name .'"] = $personalize_agent;' ."\n";
  $code .= 'return $items;';

  // Create form
  $form = array();
  $form['export'] = array(
    '#type' => 'textarea',
    '#default_value' => $code,
    '#rows' => substr_count($code, "\n") + 1,
    '#resizable' => FALSE,
    '#description' => t('Place this code in your module\'s implementation of <code>hook_personalize_default_agents()</code> to provide it as a default agent.'),
  );
  $form['done'] = array(
    '#type' => 'submit',
    '#value' => t('Done'),
  );
  $form['#redirect'] = 'admin/structure/personalize';

  return $form;
}

/**
 * Helper function to convert the admin form data into an agent object.
 */
function _personalize_agent_from_form_values($values) {
  $agent_type = $values['agent_type'];
  $agent = new stdClass();
  $agent->label = $values['title'];
  $agent->plugin = $agent_type;
  if (isset($values['machine_name'])) {
    $agent->machine_name = $values['machine_name'];
  }
  return $agent;
}

/**
 * =======================================================================
 *  M V T  M A N A G E M E N T
 * =======================================================================
 */

/**
 * Lists all MVTs
 */
function personalize_mvt_list() {

  $header = array(
    array('data' => t('Test')),
    array('data' => t('Operations'), 'colspan' => 3),
  );
  $rows = array();

  foreach (personalize_mvt_load_all() as $mvt) {

    $tablerow = array(
      array('data' => check_plain($mvt->label)),
      array('data' => l(t('Edit'), 'admin/structure/personalize/mvt/manage/'. $mvt->machine_name .'/edit')),
      array('data' => l(t('Export'), 'admin/structure/personalize/mvt/manage/'. $mvt->machine_name .'/export')),
      array('data' => l(t('Delete'), 'admin/structure/personalize/mvt/manage/'. $mvt->machine_name .'/delete')),
    );
    $rows[] = $tablerow;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No MVTS available.'), 'colspan' => 4));
  }

  $build = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'personalize'),
  );
  return $build;
}

/**
 * Form for editing an existing MVT.
 */
function personalize_mvt_form($form, &$form_state, $mvt) {
  $form = personalize_mvt_form_elements($mvt, $mvt->agent);
  $form['actions']['submit_form'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#attributes' => array(
      'class' => array('action-item-primary-active'),
    ),
  );
  return $form;
}

/**
 * Returns the actual form elements using for creating or editing an MVT.
 *
 * See the above two forms.
 *
 * @param $mvt
 *   An object representing the existing MVT or null if this is a creation form.
 * @param $agent
 *   The name of the agent for which this MVT is being added / edited.
 * @param bool $include_machine_name_field
 *   Whether to include the machine name field.
 * @return array
 *   An array of form elements.
 */
function personalize_mvt_form_elements($mvt = NULL, $agent) {
  if (empty($mvt)) {
    $mvt = new stdClass();
  }
  $form = array();
  $form['mvt_basic_info'] = array(
    '#tree' => TRUE
  );
  $form['mvt_basic_info']['label'] = array(
    '#title' => t('Administrative title'),
    '#type' => 'textfield',
    '#default_value' => isset($mvt->label) ? check_plain($mvt->label) : '',
    '#weight' => -9,
    '#required' => TRUE,
  );

  if (!empty($mvt->machine_name)) {
    $form['mvt_basic_info']['machine_name'] = array(
      '#type' => 'value',
      '#value' => $mvt->machine_name,
    );
  }

  $form['mvt_basic_info']['agent_select'] = array(
    '#type' => 'value',
    '#value' => $agent,
  );

  $option_sets = personalize_option_set_load_by_agent($agent);
  $option_set_options = array();
  foreach ($option_sets as $osid => $option_set) {
    $option_set_options[$osid] = check_plain($option_set->label);
  }
  $default = isset($mvt->option_sets) ? array_keys($mvt->option_sets) : array();
  $form['mvt_basic_info']['option_sets'] = array(
    '#type' => 'select',
    '#title' => t('Option Sets'),
    '#multiple' => TRUE,
    '#options' => $option_set_options,
    '#default_value' => $default,
  );
  $form['mvt_basic_info']['stateful'] = array(
    '#type' => 'checkbox',
    '#title' => t('Shareable'),
    '#description' => t('If a visitor shares the URL the receiver will see what the sharer saw, not a personalized variation.'),
    '#default_value' => isset($mvt->stateful) ? $mvt->stateful : 0,
  );
  return $form;
}

/**
 * Submit handler for the MVT add/edit form.
 */
function personalize_mvt_form_submit($form, &$form_state) {
  personalize_mvt_save_from_form_values($form_state['values']['mvt_basic_info']);
}

/**
 * Form for deleting an MVT.
 */
function personalize_mvt_delete_form($form, $form_state, $mvt) {
  $form['machine_name'] = array('#type' => 'hidden', '#value' => $mvt->machine_name);
  $form['title'] = array('#type' => 'hidden', '#value' => $mvt->label);
  return confirm_form($form, t('Are you sure you want to delete the multivariate test %title?', array('%title' => $mvt->label)), 'admin/structure/personalize/mvt', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for MVT deletion form.
 */
function personalize_mvt_delete_form_submit($form, &$form_state) {
  personalize_mvt_delete($form_state['values']['machine_name']);
  drupal_set_message(t('The MVT %name has been removed.', array('%name' => $form_state['values']['title'])));
  $form_state['redirect'] = 'admin/structure/personalize/mvt';
}

/**
 * =======================================================================
 *  E N D  M V T  R E L A T E D
 * =======================================================================
 */

/**
 * =======================================================================
 *  A J A X   C A L L B A C K S
 * =======================================================================
 */

/**
 * AJAX callback for getting the active campaign context.
 */
function personalize_get_campaign_context_callback() {
  $ret = array(
    'personalize_campaign' => personalize_get_campaign_context(),
  );
  drupal_json_output($ret);
}

/**
 * AJAX callback for setting the active campaign context.
 *
 * @param object $agent_data
 *   A Personalize Agent object.
 */
function personalize_set_campaign_context_callback($agent_data) {
  if (isset($agent_data)) {
    $name = $agent_data->machine_name;
    // Set the SESSION variable personalize_campaign.
    personalize_set_campaign_context($name);
    // Return a response with this campaign name to the caller.
    $ret = array(
      'personalize_campaign' => $name,
    );
    drupal_json_output($ret);
  }
  else {
    throw new Exception('No personalization was specified.');
  }
}

/**
 * =======================================================================
 *  E N D   A J A X   C A L L B A C K S
 * =======================================================================
 */

/**
 * Landing page for configuring agents.
 */
function personalize_admin_page() {
  return l(t('manage agents'), 'admin/content/personalize/agents');
}

/**
 * Retrieves a list of existing actions as goal options.
 *
 * @param boolean $exclude_limited_use
 *   Whether to exclude those goals that have been defined as limited use.
 * @return array
 *   An associative array with goal names as keys and labels as values.
 */
function personalize_goals_options($exclude_limited_use = FALSE) {
  $actions = visitor_actions_get_actions($exclude_limited_use);
  $options = array();
  foreach($actions as $name => $info) {
    $options[$name] = isset($info['label']) ? personalize_sanitize_string($info['label']) : $name;
  }
  return $options;
}

/**
 * Maps operators onto 2-character codes.
 *
 * These are only used to differentiate a string that needs to be
 * evaluated from a string holding a value to be matched exactly.
 *
 * @return array
 *
 */
function personalize_targeting_operator_map() {
  return array(
    'contains' => 'sc',
    'starts' => 'ss',
    'ends' => 'se',
    'numgt' => 'ng',
    'numlt' => 'nl',
  );
}

/**
 * Generates the 'value' string to be used when creating the feature
 * string for explicit targeting.
 *
 * If an operator other than 'equals' is passed in, the value is prefixed
 * with a string representing the operator.
 *
 * @param $value_match
 * @param $operator
 * @return string
 */
function personalize_targeting_generate_value_code($value_match, $operator) {
  if ($operator == 'equals') {
    return $value_match;
  }
  $operator_map = personalize_targeting_operator_map();
  if (!isset($operator_map[$operator])) {
    return $value_match;
  }
  return $operator_map[$operator] . PERSONALIZE_TARGETING_OP_SEPARATOR . $value_match;
}

/**
 * Returns the targeting that has been defined for the options in an option set.
 *
 * @param $option_set
 *   The option set to get targeting info for.
 * @return array
 *   An associative array, keyed by option_id, whose values are arrays of target-
 *   ing info for the option in question. If any option does not have targeting
 *   set up, it will not be included in the array.
 */
function personalize_get_targeting_for_options($option_set) {
  // Get an array of targeting rules per option id.
  $targeting_for_options = array();
  if (!empty($option_set->targeting)) {
    foreach ($option_set->targeting as $name => $rule) {
      if (isset($rule['option_id'])) {
        $targeting_for_options[$rule['option_id']] = $rule;
        $targeting_for_options[$rule['option_id']]['name'] = $name;
      }
    }
  }
  return $targeting_for_options;
}
