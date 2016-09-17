<?php

/**
 * @file
 * Contains page callbacks and related functions for the admin UI.
 */

/**
 * Form displaying an overview over all searches available for autocompletion.
 *
 * @param SearchApiIndex $index
 *   The index for which autocompletion searches should be configured.
 *
 * @see search_api_autocomplete_admin_overview_submit()
 * @see search_api_autocomplete_admin_overview_submit_delete()
 * @ingroup forms
 */
function search_api_autocomplete_admin_overview(array $form, array &$form_state, SearchApiIndex $index) {
  $form = array();
  $form_state['index'] = $index;
  $index_id = $index->machine_name;

  $available_suggesters = search_api_autocomplete_suggesters_for_index($index);
  if (!$available_suggesters) {
    $args = array(
      '@feature' => 'search_api_autocomplete',
      '@service_classes_url' => url('https://www.drupal.org/node/1254698#service-classes'),
    );
    drupal_set_message(t('There are currently no suggester plugins installed that support this index. To solve this problem, you can either:<ul><li>move the index to a server which supports the "@feature" feature (see the <a href="@service_classes_url">available service class</a>)</li><li>or install a module providing a new suggester plugin that supports this index</li></ul>', $args), 'error');
    if (search_api_autocomplete_search_load_multiple(FALSE, array('index_id' => $index_id))) {
      $form['description'] = array(
        '#type' => 'item',
        '#title' => t('Delete autocompletion settings'),
        '#description' => t("If you won't use autocompletion with this index anymore, you can delete all autocompletion settings associated with it. " .
            "This will delete all autocompletion settings on this index. Settings on other indexes won't be influenced."),
      );
      $form['button'] = array(
        '#type' => 'submit',
        '#value' => t('Delete autocompletion settings'),
        '#submit' => array('search_api_autocomplete_admin_overview_submit_delete'),
      );
    }
    return $form;
  }

  $form['#tree'] = TRUE;
  $types = search_api_autocomplete_get_types();
  $searches = search_api_autocomplete_search_load_multiple(FALSE, array('index_id' => $index_id));
  $show_status = FALSE;
  foreach ($types as $type => $info) {
    if (empty($info['list searches'])) {
      continue;
    }
    $t_searches = $info['list searches']($index);
    if (empty($t_searches)) {
      $t_searches = array();
    }
    foreach ($t_searches as $id => $search) {
      if (isset($searches[$id])) {
        $types[$type]['searches'][$id] = $searches[$id];
        $show_status |= $searches[$id]->hasStatus(ENTITY_IN_CODE);
        unset($searches[$id]);
      }
      else {
        $search += array(
          'machine_name' => $id,
          'index_id' => $index_id,
          'suggester_id' => key($available_suggesters),
          'type' => $type,
          'enabled' => 0,
          'options' => array(),
        );
        $search['options'] += array(
          'results' => TRUE,
          'fields' => array(),
        );
        $types[$type]['searches'][$id] = entity_create('search_api_autocomplete_search', $search);
      }
    }
  }
  foreach ($searches as $id => $search) {
    $type = isset($types[$search->type]) ? $search->type : '';
    $types[$type]['searches'][$id] = $search;
    $types[$type]['unavailable'][$id] = TRUE;
    $show_status |= $search->hasStatus(ENTITY_IN_CODE);
  }
  $base_path = 'admin/config/search/search_api/index/' . $index_id . '/autocomplete/';
  foreach ($types as $type => $info) {
    if (empty($info['searches'])) {
      continue;
    }
    if (!$type) {
      $info += array(
        'name' => t('Unavailable search types'),
        'description' => t("The modules providing these searches were disabled or uninstalled. If you won't use them anymore, you can delete their settings."),
      );
    }
    elseif (!empty($info['unavailable'])) {
      $info['description'] .= '</p><p>' . t("The searches marked with an asterisk (*) are currently not available, possibly because they were deleted. If you won't use them anymore, you can delete their settings.");
    }
    $form[$type] = array(
      '#type' => 'fieldset',
      '#title' => $info['name'],
    );
    if (!empty($info['description'])) {
      $form[$type]['#description'] = '<p>' . $info['description'] . '</p>';
    }
    $form[$type]['searches']['#theme'] = 'tableselect';
    $form[$type]['searches']['#header'] = array();
    if ($show_status) {
      $form[$type]['searches']['#header']['status'] = t('Status');
    }
    $form[$type]['searches']['#header'] += array(
      'name' => t('Name'),
      'operations' => t('Operations'),
    );
    $form[$type]['searches']['#empty'] = '';
    $form[$type]['searches']['#js_select'] = TRUE;
    foreach ($info['searches'] as $id => $search) {
      $form[$type]['searches'][$id] = array(
        '#type' => 'checkbox',
        '#default_value' => $search->enabled,
        '#parents' => array('searches', $id),
      );
      $unavailable = !empty($info['unavailable'][$id]);
      if ($unavailable) {
        $form[$type]['searches'][$id]['#default_value'] = FALSE;
        $form[$type]['searches'][$id]['#disabled'] = TRUE;
      }
      $form_state['searches'][$id] = $search;
      $options = &$form[$type]['searches']['#options'][$id];
      if ($show_status) {
        $options['status'] = isset($search->status) ? theme('entity_status', array('status' => $search->status)) : '';;
      }
      $options['name'] = $search->name;
      if ($unavailable) {
        $options['name'] = '* ' . $options['name'];
      }
      $items = array();
      if (!$unavailable && !empty($search->id)) {
        $items[] = l(t('edit'), $base_path . $id . '/edit');
      }
      if (!empty($search->status) && ($search->hasStatus(ENTITY_CUSTOM))) {
        $title = $search->hasStatus(ENTITY_IN_CODE) ? t('revert') : t('delete');
        $items[] = l($title, $base_path . $id . '/delete');
      }
      if ($items) {
        $variables = array(
          'items' => $items,
          'attributes' => array('class' => array('inline')),
        );
        $options['operations'] = theme('item_list', $variables);
      }
      else {
        $options['operations'] = '';
      }
      unset($options);
    }
  }

  if (!element_children($form)) {
    $form['message']['#markup'] = '<p>' . t('There are currently no searches known for this index.') . '</p>';
  }
  else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }

  return $form;
}

/**
 * Submit callback for search_api_autocomplete_admin_overview().
 *
 * @see search_api_autocomplete_admin_overview()
 */
function search_api_autocomplete_admin_overview_submit(array $form, array &$form_state) {
  $msg = t('The settings have been saved.');
  foreach ($form_state['values']['searches'] as $id => $enabled) {
    $search = $form_state['searches'][$id];
    if ($search->enabled != $enabled) {
      $change = TRUE;
      if (!empty($search->is_new)) {
        $options['query']['destination'] = $_GET['q'];
        $options['fragment'] = 'module-search_api_autocomplete';
        $vars['@perm_url'] = url('admin/people/permissions', $options);
        $msg = t('The settings have been saved. Please remember to set the <a href="@perm_url">permissions</a> for the newly enabled searches.', $vars);
      }
      $search->enabled = $enabled;
      $search->save();
    }
  }
  drupal_set_message(empty($change) ? t('No values were changed.') : $msg);
}

/**
 * Submit callback for search_api_autocomplete_admin_overview(), when all
 * settings for the index should be deleted.
 *
 * @see search_api_autocomplete_admin_overview()
 */
function search_api_autocomplete_admin_overview_submit_delete(array $form, array &$form_state) {
  $index = $form_state['index'];
  $ids = array_keys(search_api_autocomplete_search_load_multiple(FALSE, array('index_id' => $index->machine_name)));
  if ($ids) {
    entity_delete_multiple('search_api_autocomplete_search', $ids);
    drupal_set_message(t('All autocompletion settings stored for this index were deleted.'));
  }
  else {
    drupal_set_message(t('There were no settings to delete.'), 'warning');
  }
  $form_state['redirect'] = 'admin/config/search/search_api/index/' . $index->machine_name;
}

/**
 * Form for editing the autocompletion settings for a search.
 *
 * @param SearchApiAutocompleteSearch $search
 *   The search whose settings should be edited.
 *
 * @ingroup forms
 *
 * @see search_api_autocomplete_admin_search_edit_validate()
 * @see search_api_autocomplete_admin_search_edit_submit()
 */
function search_api_autocomplete_admin_search_edit(array $form, array &$form_state, SearchApiAutocompleteSearch $search) {
  drupal_set_title(t('Configure autocompletion for %search', array('%search' => $search->name)), PASS_THROUGH);

  // If this is a re-build (i.e., most likely an AJAX call due to a new
  // suggester being selected), prepare the suggester sub-form state
  // accordingly.
  $selected_suggester_id = $search->getSuggesterId();
  if (!empty($form_state['values']['suggester_id'])) {
    $selected_suggester_id = $form_state['values']['suggester_id'];
    // Don't let submitted values for a different suggester influence another
    // suggester's form.
    if ($selected_suggester_id != $form_state['values']['old_suggester_id']) {
      unset($form_state['values']['options']['suggester_configuration']);
      unset($form_state['input']['options']['suggester_configuration']);
    }
  }

  $form_state['search'] = $search;
  $form_state['type'] = $type = search_api_autocomplete_get_types($search->type);
  if (!$type) {
    drupal_set_message(t('No information about the type of this search was found.'), 'error');
    return array();
  }
  $form['#tree'] = TRUE;
  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => $search->enabled,
  );
  $form['suggester_id'] = array(
    '#type' => 'radios',
    '#title' => t('Suggester plugin'),
    '#description' => t('Choose the suggester implementation to use.'),
    '#options' => array(),
    '#required' => TRUE,
    '#default_value' => $selected_suggester_id,
    '#ajax' => array(
      'callback' => 'search_api_autocomplete_suggester_ajax_callback',
      'wrapper' => 'search-api-suggester-options',
    ),
  );

  foreach (search_api_autocomplete_suggesters_for_index($search->index()) as $suggester_id => $definition) {
    // Load the suggester plugin. If the suggester is unchanged from the one on
    // the saved version of the search, use the saved configuration.
    $configuration = array();
    if ($suggester_id == $search->getSuggesterId() && isset($search->options['suggester_configuration'])) {
      $configuration = $search->options['suggester_configuration'];
    }
    $suggester = search_api_autocomplete_suggester_load($suggester_id, $search, $configuration);
    if (!$suggester) {
      continue;
    }

    // Add the suggester to the suggester options.
    $form['suggester_id']['#options'][$suggester_id] = $suggester->label();

    // Then, also add the configuration form for the selected suggester.
    if ($suggester_id != $selected_suggester_id) {
      continue;
    }
    $suggester_form_state = &search_api_autocomplete_get_plugin_form_state($form_state);
    $suggester_form = $suggester->buildConfigurationForm(array(), $suggester_form_state);
    if ($suggester_form) {
      $form['options']['suggester_configuration'] = $suggester_form;
      $form['options']['suggester_configuration']['#type'] = 'fieldset';
      $form['options']['suggester_configuration']['#title'] = t('Configure the %suggester suggester plugin', array('%suggester' => $suggester->label()));
      $form['options']['suggester_configuration']['#description'] = $suggester->getDescription();
      $form['options']['suggester_configuration']['#collapsible'] = TRUE;
    }
    else {
      $form['options']['suggester_configuration']['#type'] = 'item';
    }
    $form['options']['suggester_configuration']['#prefix'] = '<div id="search-api-suggester-options">';
    $form['options']['suggester_configuration']['#suffix'] = '</div>';
  }
  $form['options']['suggester_configuration']['old_suggester_id'] = array(
    '#type' => 'hidden',
    '#value' => $selected_suggester_id,
    '#tree' => FALSE,
  );

  // If there is only a single plugin available, hide the "suggester" option.
  if (count($form['suggester_id']['#options']) == 1) {
    $form['suggester_id'] = array(
      '#type' => 'value',
      '#value' => key($form['suggester_id']['#options']),
    );
  }

  $form['options']['min_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum length of keywords for autocompletion'),
    '#description' => t('If the entered keywords are shorter than this, no autocomplete suggestions will be displayed.'),
    '#default_value' => isset($search->options['min_length']) ? $search->options['min_length'] : 1,
    '#validate' => array('element_validate_integer_positive'),
  );
  $form['options']['results'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display result count estimates'),
    '#description' => t('Display the estimated number of result for each suggestion. ' .
      'This option might not have an effect for some servers or types of suggestion.'),
    '#default_value' => !empty($search->options['results']),
  );

  $custom_form = empty($form['options']['custom']) ? array() : $form['options']['custom'];
  if (!empty($type['config form']) && function_exists($type['config form']) && is_array($custom_form = $type['config form']($custom_form, $form_state, $search))) {
    $form['options']['custom'] = $custom_form;
  }

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['submit_button_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Search button selector'),
    '#description' => t('<a href="@jquery_url">jQuery selector</a> to identify the search button in the search form. Use the ID attribute of the "Search" submit button to prevent issues when another button is present (e.g., "Reset"). The selector is evaluated relative to the form. The default value is ":submit".', array('@jquery_url' => 'https://api.jquery.com/category/selectors/')),
    '#default_value' => isset($search->options['submit_button_selector']) ? $search->options['submit_button_selector'] : ':submit',
    '#required' => TRUE,
    '#parents' => array('options', 'submit_button_selector'),
  );
  $form['advanced']['autosubmit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable auto-submit'),
    '#description' => t('When enabled, the search form will automatically be submitted when a selection is made by pressing "Enter".'),
    '#default_value' => isset($search->options['autosubmit']) ? $search->options['autosubmit'] : TRUE,
    '#parents' => array('options', 'autosubmit'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form AJAX handler for search_api_autocomplete_admin_search_edit().
 */
function search_api_autocomplete_suggester_ajax_callback(array $form, array &$form_state) {
  return $form['options']['suggester_configuration'];
}

/**
 * Validate callback for search_api_autocomplete_admin_search_edit().
 *
 * @see search_api_autocomplete_admin_search_edit()
 * @see search_api_autocomplete_admin_search_edit_submit()
 */
function search_api_autocomplete_admin_search_edit_validate(array $form, array &$form_state) {
  $values = &$form_state['values'];
  // Call the config form validation method of the selected suggester plugin,
  // but only if it was the same plugin that created the form.
  if ($values['suggester_id'] == $values['old_suggester_id']) {
    $configuration = array();
    if (!empty($values['options']['suggester_configuration'])) {
      $configuration = $values['options']['suggester_configuration'];
    }
    $suggester = search_api_autocomplete_suggester_load($values['suggester_id'], $form_state['search'], $configuration);
    $suggester_form = $form['options']['suggester_configuration'];
    unset($suggester_form['old_suggester_id']);
    $suggester_form_state = &search_api_autocomplete_get_plugin_form_state($form_state);
    $suggester->validateConfigurationForm($suggester_form, $suggester_form_state);
  }

  if (!empty($form_state['type']['config form'])) {
    $f = $form_state['type']['config form'] . '_validate';
    if (function_exists($f)) {
      $custom_form = empty($form['options']['custom']) ? array() : $form['options']['custom'];
      $f($custom_form, $form_state, $values['options']['custom']);
    }
  }
}

/**
 * Submit callback for search_api_autocomplete_admin_search_edit().
 *
 * @see search_api_autocomplete_admin_search_edit()
 * @see search_api_autocomplete_admin_search_edit_validate()
 */
function search_api_autocomplete_admin_search_edit_submit(array $form, array &$form_state) {
  $values = &$form_state['values'];
  if (!empty($form_state['type']['config form'])) {
    $f = $form_state['type']['config form'] . '_submit';
    if (function_exists($f)) {
      $custom_form = empty($form['options']['custom']) ? array() : $form['options']['custom'];
      $f($custom_form, $form_state, $values['options']['custom']);
    }
  }

  $search = $form_state['search'];
  $search->enabled = $values['enabled'];
  $search->suggester_id = $values['suggester_id'];

  $form_state['redirect'] = 'admin/config/search/search_api/index/' . $search->index_id . '/autocomplete';

  // Take care of custom options that aren't changed in the config form.
  if (!empty($search->options['custom'])) {
    if (!isset($values['options']['custom'])) {
      $values['options']['custom'] = array();
    }
    $values['options']['custom'] += $search->options['custom'];
  }

  // Allow the suggester to decide how to save its configuration. If the user
  // has disabled JS in the browser, or AJAX didn't work for some other reason,
  // a different suggester might be selected than that which created the config
  // form. In that case, we don't call the form submit method, save empty
  // configuration for the plugin and stay on the page.
  if ($values['suggester_id'] == $values['old_suggester_id']) {
    $configuration = array();
    if (!empty($values['options']['suggester_configuration'])) {
      $configuration = $values['options']['suggester_configuration'];
    }
    $suggester = search_api_autocomplete_suggester_load($values['suggester_id'], $search, $configuration);
    $suggester_form = $form['options']['suggester_configuration'];
    unset($suggester_form['old_suggester_id']);
    $suggester_form_state = &search_api_autocomplete_get_plugin_form_state($form_state);
    $suggester->submitConfigurationForm($suggester_form, $suggester_form_state);
    $values['options']['suggester_configuration'] = $suggester->getConfiguration();
  }
  else {
    $values['options']['suggester_configuration'] = array();
    $form_state['redirect'] = NULL;
    drupal_set_message(t('The used suggester plugin has changed. Please review the configuration for the new plugin.'), 'warning');
  }

  $search->options = $values['options'];

  $search->save();
  drupal_set_message(t('The autocompletion settings for the search have been saved.'));
}

/**
 * Returns a new form state for the suggester configuration sub-form.
 *
 * @param array $form_state
 *   The original form state.
 *
 * @return array
 *   A form state for the sub-form.
 */
function &search_api_autocomplete_get_plugin_form_state(array &$form_state) {
  $sub_form_state = &$form_state['suggester_form_state'];

  foreach (array('values', 'input') as $key) {
    if (!isset($form_state[$key]['options']['suggester_configuration'])) {
      $form_state[$key]['options']['suggester_configuration'] = array();
    }
    $sub_form_state[$key] = &$form_state[$key]['options']['suggester_configuration'];
  }

  foreach (array('rebuild', 'rebuild_info', 'redirect') as $key) {
    $sub_form_state[$key] = &$form_state[$key];
  }

  return $sub_form_state;
}

/**
 * Form for deleting the autocompletion settings of a search.
 *
 * @param SearchApiAutocompleteSearch $search
 *   The search whose settings should be deleted.
 *
 * @see search_api_autocomplete_admin_search_delete_submit()
 * @ingroup forms
 */
function search_api_autocomplete_admin_search_delete(array $form, array &$form_state, SearchApiAutocompleteSearch $search) {
  $form_state['search'] = $search;
  return confirm_form(
    $form,
    t('Do you really want to delete the autocompletion settings for search %search?', array('%search' => $search->name)),
    'admin/config/search/search_api/index/' . $search->index_id . '/autocomplete'
  );
}

/**
 * Submit callback for search_api_autocomplete_admin_search_delete().
 *
 * @see search_api_autocomplete_admin_search_delete()
 */
function search_api_autocomplete_admin_search_delete_submit(array $form, array &$form_state) {
  $form_state['search']->delete();
  drupal_set_message(t('The autocompletion settings for the search have been deleted.'));
  $form_state['redirect'] = 'admin/config/search/search_api/index/' . $form_state['search']->index_id . '/autocomplete';
}
