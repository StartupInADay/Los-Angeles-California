<?php

/**
 * @file visitor_actions.admin.inc
 * Provides functions needed for the admin UI.
 */


/**
 * Menu callback for the action edit page.
 *
 * @param $action
 *   The action to edit
 *
 * @return array|mixed
 *   The action edit form.
 */
function visitor_actions_edit($action) {
  if (!$action) {
    drupal_not_found();
    return NULL;
  }
  return drupal_get_form('visitor_actions_form', NULL, FALSE, $action);
}

/**
 * Menu callback for displaying a list of all existing custom actions.
 */
function visitor_actions_list() {

  $header = array(
    array('data' => t('Action name')),
    array('data' => t('Operations'), 'colspan' => 2),
  );
  $rows = array();

  foreach (visitor_actions_custom_load_multiple() as $action) {
    $tablerow = array(
      array('data' => check_plain($action['label'])),
      array('data' => l(t('Edit'), 'admin/structure/visitor_actions/manage/'. $action['machine_name'] . '/edit')),
      array('data' => l(t('Delete'), 'admin/structure/visitor_actions/manage/'. $action['machine_name'] . '/delete')),
    );
    $rows[] = $tablerow;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No actions available.'), 'colspan' => 3));
  }

  $build = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'visitor_actions'),
  );
  return $build;
}

/**
 * Form for creating a new action.
 */
function visitor_actions_form($form, &$form_state, $actionable_element = NULL, $isAjax = FALSE, $action = NULL) {
  $form['event'] = $form['identifier'] = array(
    '#tree' => TRUE
  );

  $form['options'] = array(
    '#tree' => TRUE,
  );
  ctools_include('plugins');
  if ($actionable_element === NULL) {
    if (isset($form_state['values']['title'])) {
      $action = visitor_actions_convert_form_values_to_action($form_state['values']);
    }
    elseif ($action === NULL) {
      $action = array();
    }
    // We'll need a dropdown of available "actionable_elements" and form
    // elements for each one.
    $elements = visitor_actions_get_actionable_element_types();
    $element_options = $element_form_options = $event_element = array();
    foreach ($elements as $info) {
      // Add this option to the options for the "actionable element" dropdown.
      $element_options[$info['type']] = $info['type'];
      visitor_actions_get_form_options($info['type'], $form, TRUE, $action);
    }
    ksort($element_options);
    $form['actionable_element'] = array(
      '#type' => 'select',
      '#title' => t('Actionable Element'),
      '#options' => array('' => '('. t('Please select...').')') + $element_options,
      '#default_value' => isset($action['plugin']) ? $action['plugin'] : '',
      '#description' => t('Choose which actionable element to use.'),
      '#weight' => -7,
    );
  }
  else {
    $plugins = ctools_get_plugins('visitor_actions', 'actionable_element', $actionable_element);
    if (empty($plugins)) {
      // Unrecognized plugin, just redirect to the normal form.
      drupal_goto('admin/structure/visitor_actions/add');
    }
    // No need for a dropdown, just set the actionable_element as a value element
    // and build the other form elements as required by the chosen plugin.
    $form['actionable_element'] = array(
      '#type' => 'value',
      '#value' => $actionable_element,
    );
    visitor_actions_get_form_options($actionable_element, $form, FALSE);
  }
  $form['#visitor_action'] = $action;
  $form['title'] = array(
    '#title' => t('Title'),
    '#description' => t('The administrative title of this action.'),
    '#type' => 'textfield',
    '#weight' => -9,
    '#required' => TRUE,
    '#default_value' => isset($action['label']) ? $action['label'] : '',
  );

  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => 64,
    '#machine_name' => array(
      'exists' => 'visitor_actions_action_name_exists',
      'source' => array('title'),
      'replace_pattern' => '[^a-z0-9_]+',
      'replace' => '_',
    ),
    '#description' => t('A unique machine-readable name for this action. It must only contain lowercase letters, numbers, and underscores.'),
    '#weight' => -8,
  );
  if (isset($action['machine_name'])) {
    $form['machine_name']['#default_value'] = $action['machine_name'];
    $form['machine_name']['#disabled'] = TRUE;
    $form['machine_name']['#value'] = $action['machine_name'];
  }
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['pages'] = array(
    '#type' => 'textarea',
    '#element_validate' => array('visitor_actions_form_element_path_validate'),
    '#title' => t('Pages'),
    '#allow_dynamic' => TRUE,
    '#allow_external' => FALSE,
    '#default_value' => isset($_GET['path']) ? urldecode($_GET['path']) : (isset($action['pages']) ? $action['pages'] : ''),
    '#description' => t("Specify pages by using one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page. If the URL's language prefix is skipped, all languages of the same URL will be selected.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );
  $form['advanced']['reusable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reusable'),
    '#default_value' => isset($action['limited_use']) ? !$action['limited_use'] : 1,
    '#description' => t("Select this check box to make the action available for general use."),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit_form'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form_state['isAjax'] = $isAjax;

  return $form;
}

/**
 * Constructs the plugin-specific form elements for the action form.
 *
 * @param $actionable_element
 *   The name of the actionable_element plugin.
 * @param $form
 *   The form array to add the elements to.
 * @param bool $needs_states
 *   Whether to use #states to hide elements.
 */
function visitor_actions_get_form_options($actionable_element, &$form, $needs_states = TRUE, $action = array()) {
  $visible_state = array('visible' => array(':input[name="actionable_element"]' => array('value' => $actionable_element)));
  $required_state = array('required' => array(':input[name="actionable_element"]' => array('value' => $actionable_element)));

  if ($class = ctools_plugin_load_class('visitor_actions', 'actionable_element', $actionable_element, 'handler')) {
    $event_options = array();
    $event_option_types = call_user_func(array($class, 'getActionOptions'));
    // Actions are divided into server-side and client-side
    foreach ($event_option_types as $type => $options) {
      foreach ($options as $option => $label) {
        // Whichever action is selected we need to know whether it's a server-
        // side or a client-side action, so we can set this when saving the
        // action.
        $event_options[$type . '::' . $option] = $label;
      }
    }
    $form['event'][$actionable_element] = array(
      '#title' => t('Event'),
      '#type' => 'select',
      '#options' => $event_options,
    );
    if ($needs_states) {
      $form['event'][$actionable_element]['#states'] = array_merge($visible_state, $required_state);
    }

    $implements = class_implements($class);
    if (isset($implements['IdentifiableActionableElementInterface'])) {
      $form['identifier'][$actionable_element] = array(
        '#title' => call_user_func(array($class, 'getIdentifierLabel')),
        '#type' => 'textfield',
        '#maxlength' => 500,
      );
      if ($needs_states) {
        $form['identifier'][$actionable_element]['#states'] = array_merge($visible_state, $required_state);
      }
    }
    // Get the plugin's advanced options form elements
    $options = call_user_func_array(array($class, 'optionsForm'), array($action));
    $form['options'][$actionable_element] = array(
      '#tree' => TRUE,
    );
    foreach ($options as $name => $element) {
      $form['options'][$actionable_element][$name] = $element;
      if ($needs_states) {
        $states = $visible_state;
        // Add element visible states to any existing states for this option.
        if (isset($form['options'][$actionable_element][$name]['#states'])) {
          foreach($form['options'][$actionable_element][$name]['#states'] as $property => $state) {
            $states[$property] = isset($states[$property]) ? array_merge($states[$property], $state) : $state;
          }
        }
        else {
          $states = $visible_state;
        }
        if (isset($element['#required'])) {
          // Add element required state to any existing states for the option.
          foreach($required_state as $property => $state) {
           $states[$property] = isset($states[$property]) ? array_merge($states[$property], $state) : $state;
          }
          unset($form['options'][$actionable_element][$name]['#required']);
        }
        $form['options'][$actionable_element][$name]['#states'] = $states;
      }
    }
    // Add default values if we're editing an action of this type.
    if (isset($action['plugin']) && $action['plugin'] == $actionable_element) {
      $form['event'][$actionable_element]['#default_value'] = implode('::', array(($action['client_side'] ? 'client' : 'server'), $action['event']));
      $form['identifier'][$actionable_element]['#default_value'] = $action['identifier'];
    }
  }
}

/**
 * Validation callback for the action creation form.
 */
function visitor_actions_form_validate($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  if ($op == t('Save')) {
    $plugin = $form_state['values']['actionable_element'];
    // Allow the plugin to validate the form values.
    ctools_include('plugins');
    if ($class = ctools_plugin_load_class('visitor_actions', 'actionable_element', $plugin, 'handler')) {
      call_user_func_array(array($class, 'validate'), array($form_state['values']));
    }

    if (isset($form_state['values']['identifier'][$plugin])) {
      // Validating a CSS selector is generally beyond what REGEX can handle,
      // and using the Drupal cleaning scripts all update < > to entities
      // so just make sure that there are no tags.
      $re = "/<\\/?[a-z][a-z0-9]*[^<>]*>|<!--.*?-->/im";
      $matches = array();
      $matched = preg_match_all($re, $form_state['values']['identifier'][$plugin], $matches);
      if ($matched > 0) {
        form_set_error('identifier[' . $plugin . ']', t('The selector cannot contain tags.'));
      }
    }
  }
}

/**
 * Submit handler for the action creation form.
 */
function visitor_actions_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  if ($op == t('Save')) {
    $action = visitor_actions_convert_form_values_to_action($form_state['values']);
    // Allow the plugin to modify the action before saving.
    ctools_include('plugins');
    if ($class = ctools_plugin_load_class('visitor_actions', 'actionable_element', $action['plugin'], 'handler')) {
      $action = call_user_func_array(array($class, 'actionPresave'), array($action));
    }
    if (visitor_actions_save_action($action)) {
      drupal_set_message(t('The action !action was saved.', array('!action' => l($action['label'], "admin/structure/visitor_actions/manage/{$action['machine_name']}/edit"))));
      $form_state['redirect'] = 'admin/structure/visitor_actions';
    }
  }
}

/**
 * Converts the values submitted in the form to an action array.
 *
 * @param $values
 *   The form values.
 * @return array
 *   An array that can be saved as an action to the db.
 */
function visitor_actions_convert_form_values_to_action($values) {
  $plugin = $values['actionable_element'];
  list($type, $event) = explode('::', $values['event'][$plugin]);
  $action = array(
    'label' => $values['title'],
    'machine_name' => $values['machine_name'],
    'plugin' => $plugin,
    'client_side' => $type == 'client' ? 1 : 0,
    'identifier' => isset($values['identifier'][$plugin]) ? $values['identifier'][$plugin] : '',
    'event' => $event,
    'pages' => $values['pages'],
    // The data array gets populated with the plugin-specific
    // info for the action.
    'data' => isset($values['options'][$plugin]) ? $values['options'][$plugin] : array(),
    'limited_use' => $values['reusable'] ? 0 : 1
  );
  return $action;
}

/**
 * Returns whether the given action name already exists.
 *
 * @param $name
 * @return mixed
 */
function visitor_actions_action_name_exists($name) {
  $exists = db_query_range('SELECT 1 FROM {visitor_actions_actions} WHERE machine_name = :name', 0, 1, array(':name' => $name))->fetchField();
  return $exists;
}


/**
 * Form for deleting an action.
 */
function visitor_actions_delete_form($form, $form_state, $action) {
  $form['machine_name'] = array('#type' => 'hidden', '#value' => $action['machine_name']);
  $form['title'] = array('#type' => 'hidden', '#value' => $action['label']);
  return confirm_form($form, t('Are you sure you want to delete the action %title?', array('%title' => $action['label'])), 'admin/structure/visitor_actions', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for action deletion form.
 */
function visitor_actions_delete_form_submit($form, &$form_state) {
  visitor_actions_delete_action($form_state['values']['machine_name']);
  drupal_set_message(t('The action %name has been removed.', array('%name' => $form_state['values']['title'])));
  $form_state['redirect'] = 'admin/structure/visitor_actions';
}
