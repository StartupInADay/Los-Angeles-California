<?php

/**
 * @file personalize.admin.campaign.inc
 * Includes the multi-page campaign form wizard.
 */

/**
 ********************************************************************
 *
 * F O R M  G E N E R A T I O N
 *
 ********************************************************************
 */

/**
 * Drupal form callback to generate the full campaign wizard.
 */
function personalize_campaign_wizard($form, &$form_state, $agent_data = NULL, $selected = NULL) {
  module_load_include('inc', 'personalize', 'personalize.admin');

  // We have to specify the include file so as not to lose it during rendering from ajax.
  // @see personalize_agent_form_ajax_submit()
  // @see drupal_retrieve_form():734
  $form_state['build_info']['files'] = array(
    drupal_get_path('module', 'personalize') . '/personalize.admin.campaign.inc',
  );

  // Set the default step.
  $selected = empty($selected) ? 'variations' : $selected;

  // Determine the data for the current campaign.
  if (empty($agent_data)) {
    if (isset($form_state['values']['agent'])) {
      $agent_data = personalize_agent_load($form_state['values']['agent']);
    }
    else {
      $agent_data = new stdClass();
      if (isset($_GET['personalize_agent_type']) && $plugin = personalize_get_agent_type($_GET['personalize_agent_type'])) {
        $agent_data->plugin = $plugin['name'];
      }
      $form_state['storage']['step'] = 'base';
    }
  }
  else {
    // If the form is being reloaded, reload the agent data in case it changed.
    if (isset($form_state['triggering_element'])) {
      $agent_data = personalize_agent_load($agent_data->machine_name);
    }
    $form['agent'] = array(
      '#type' => 'value',
      '#value' => $agent_data->machine_name,
    );
  }
  // Set this as the current campaign context.
  if (isset($agent_data->machine_name)) {
    personalize_set_campaign_context($agent_data->machine_name);
  }

  $form['#agent'] = $agent_data;
  $agent_instance = !empty($agent_data->machine_name) ? personalize_agent_load_agent($agent_data->machine_name) : NULL;

  // This property can be adjusted by any sub-form to prevent the submit action.
  $form_state['storage']['allow_submit'] = TRUE;

  // Read the current step from the parameters unless the form process has
  // already started.
  $current_step = isset($form_state['storage']['step']) ? $form_state['storage']['step'] : $selected;
  if ($current_step !== 'base') {
    // Make sure that the current step requested is applicable to the agent.
    $agent_steps = _personalize_campaign_wizard_steps($agent_instance);
    if (empty($agent_steps[$current_step])) {
      $current_step = 'variations';
    }
  }
  $form_state['storage']['step'] = $current_step;

  $form['#attached']['library'][] = array('personalize', 'admin.campaign');

  // Add the top level campaign base information.
  $header_form = personalize_campaign_wizard_base($form, $form_state, $agent_data, $agent_instance);
  if ($header_form === FALSE) {
    return $form;
  }
  $form += $header_form;

  // Create the campaign before being able to add variations/goals/etc.
  if (empty($agent_data->machine_name)) {
    return $form;
  }

  // Add the form process bar (includes navigation and other campaign
  // level actions).
  $form += personalize_campaign_wizard_process_bar($form, $form_state, $agent_data, $agent_instance);

  // Add the current sub-form.
  $function = 'personalize_campaign_wizard_' . $form_state['storage']['step'];
  if (function_exists($function)) {
    $form += $function($form, $form_state, $agent_data, $agent_instance);
  }

  // Add the form action buttons.
  $form += personalize_campaign_wizard_action($form, $form_state, $agent_data, $agent_instance);
  return $form;
}

/**
 * Generate the multi-step form navigation display.
 *
 * @param array $form
 *   The current form array.
 * @param array $form_state
 *   The current drupal form state
 * @param stdClass $agent_data
 *   The current campaign data.
 * @param PersonalizeAgentInterface $agent_instance
 *   The agent that defines the functionality for the campaign (if set).
 *
 * @return array
 *   The form array for the navigation of the multi-step form.
 */
function personalize_campaign_wizard_process_bar($form, &$form_state, $agent_data, $agent_instance) {
  $form = array();
  $form['override_step'] = array(
    '#type' => 'hidden',
  );
  $form['process_bar'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'personalize-campaign-wizard-process-bar',
    ),
    '#theme' => 'personalize_wizard_process_bar',
    '#current' => $form_state['storage']['step'],
    '#base_url' => empty($agent_data->machine_name) ? 'admin/structure/personalize/add' : 'admin/structure/personalize/manage/' . $agent_data->machine_name,
  );

  // Navigation buttons.
  $form['process_bar']['navigation'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('personalize-wizard-navigation'),
    ),
    '#options' => _personalize_campaign_wizard_steps($agent_instance),
  );

  // Campaign-specific actions.
  $form['process_bar']['actions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('personalize-wizard-process-bar-actions'),
    ),
  );
  if (!empty($agent_data->machine_name)) {
    $destination = 'admin/structure/personalize/manage/' . $agent_data->machine_name . '/' . $form_state['storage']['step'];
    // NOTE: The button is hidden and activated via JavaScript when
    // clicking the dropbutton link of the same class name.  This allows us
    // to have button/form functionality that is called from the links in a
    // ctools_dropbutton.
    $form['process_bar']['actions']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#attributes' => array(
        'class' => array(
          'personalize-wizard-save',
          'element-hidden',
        ),
      ),
    );

    // Note that the link has a data attribute to point to the class of the
    // button that should be activated.  This is generalized so that any
    // combination of link/button can be used within the actions bar.
    $links = array(
      array(
        'title' => t('Save'),
        'href' => '#',
        'attributes' => array(
          'data-personalize-action' => 'personalize-wizard-save',
        ),
        'external' => TRUE, // Prevents url-encoding of #
      ),
    );

    // Change campaign status.  Note that this status change form will not be
    // run through hook_form_FORMID_alter.
    // @todo: Examine this more closely as it prevents other module's hooks
    // from being utilized.
    $form['process_bar']['actions']['status'] = personalize_status_change_form($form, $form_state, $agent_data, FALSE, FALSE);
    // Assign the submit handler to the button to bypass full form submission.
    $form_state['build_info']['files'][] = drupal_get_path('module', 'personalize') . '/personalize.admin.inc';
    foreach (element_children($form['process_bar']['actions']['status']['status_wrapper']) as $form_element) {
      $button = &$form['process_bar']['actions']['status']['status_wrapper'][$form_element];
      $button['#submit'] = array('personalize_status_change_form_submit');
      $button['#attributes']['class'] = array('element-hidden', 'personalize-wizard-status-' . $button['#personalize_next_status']);

      // Add a link to the drop-down button.
      $links[] = array(
        'title' => $button['#value'],
        'href' => '#',
        'attributes' => array(
          'data-personalize-action' => 'personalize-wizard-status-' . $button['#personalize_next_status'],
        ),
      );
    }

    // Allow other modules to add action links to the process bar.
    drupal_alter('personalize_campaign_action_links', $links, $agent_data, $destination);
    $form['process_bar']['actions']['action_links'] = array(
      '#markup' => theme('links__ctools_dropbutton', array(
        'title' => t('Operations'),
        'links' => $links,
        'class' => array('personalize-wizard-action-links'),
      )),
    );
    // ctools calls drupal_add_js() and drupal_add_css() to load assets. Unfortunately,
    //   1) drupal_add_*() don't get called when page is cached, and;
    //   2) when there is a form validation error, the cached form is served.
    // In this special case, dropbuttons are loaded without their supporting js and css.
    // The fix is to manually configure #attached to ensure the assets are loaded, regardless.
    if (count($links) > 1) {
      $form['process_bar']['actions']['action_links']['#attached']['js'][] = drupal_get_path('module', 'ctools') . '/js/dropbutton.js';
      $form['process_bar']['actions']['action_links']['#attached']['css'][] = drupal_get_path('module', 'ctools') . '/css/dropbutton.css';
    }
    $form['process_bar']['actions']['action_links']['#attached']['css'][] = drupal_get_path('module', 'ctools') . '/css/button.css';
  }

  return $form;
}

/**
 * Generate the action buttons for the current step of the form.
 *
 * @param array $form
 *   The current form array.
 * @param array $form_state
 *   The current drupal form state
 * @param stdClass $agent_data
 *   The current campaign data.
 * @param PersonalizeAgentInterface $agent_instance
 *   The agent that defines the functionality for the campaign (if set).
 *
 * @return array
 *   The form array for the actions of the multi-step form.
 */
function personalize_campaign_wizard_action($form, &$form_state, $agent_data, $agent_instance) {
  // Determine the preview text to show with the button based on the next step
  // for this agent instance.
  $steps = _personalize_campaign_wizard_steps($agent_instance);
  $next_step = _personalize_campaign_wizard_next_step($form_state, $agent_instance);
  if (empty($next_step)) {
    return $form;
  }
  $current_info = $steps[$form_state['storage']['step']];
  $next_info = $steps[$next_step];

  if (empty($next_info) || empty($current_info['button_value'])) {
    return $form;
  }

  $form['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array(
      'class' => array(
        'personalize-wizard-actions',
      ),
    ),
  );
  // TRICKY: Don't name this "submit" or it does crazy things to JS form
  // submission.
  $button_text = $current_info['button_value'];
  if ($form_state['storage']['step'] !== $next_step && !empty($next_info['preview_message'])) {
    $button_text .= ': ' . $next_info['preview_message'];
  }
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#name' => 'wizard_submit',
    '#value' => $button_text,
    '#disabled' => !($form_state['storage']['allow_submit']),
    '#attributes' => array(
      'class' => array('action-item-primary-active'),
    ),
  );
  if (isset($current_info['next_status'])) {
    $form['actions']['submit']['#personalize_next_status'] = $current_info['next_status'];
  }

  return $form;
}

/**
 ********************************************************************
 *
 * S U B F O R M S
 *
 ********************************************************************
 */

/**
 * Generates the top-level campaign information shown above each sub-form.
 *
 * @param array $form
 *   The current form array.
 * @param array $form_state
 *   The current drupal form state
 * @param stdClass $agent_data
 *   The current campaign data.
 * @param PersonalizeAgentInterface $agent_instance
 *   The agent that defines the functionality for the campaign (if set).
 *
 * @return array|bool
 *   Returns the form array to include for the campaign header form or FALSE
 *   if there is an error that prevents form display.
 */
function personalize_campaign_wizard_base($form, &$form_state, $agent_data, $agent_instance) {
  $form = array();
  // Base form is shown for all steps so the values are the main values.
  $values = isset($form_state['values']) ? $form_state['values'] : array();
  $is_new = empty($agent_data->machine_name);

  // Make sure we have at least one agent type available.
  $agent_types = personalize_get_agent_types();
  $agent_type_options = $agent_type_form_options = array();
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

  $form['header'] = array(
    '#markup' => theme('html_tag', array(
      'element' => array(
        '#tag' => 'h2',
        '#value' => $is_new ? t('Create personalization') : t('Edit personalization'),
      ),
    )),
  );

  $form['agent_basic_info'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  if (!$is_new) {
    $form['agent_basic_info']['machine_name'] = array(
      '#type' => 'value',
      '#value' => $agent_data->machine_name,
    );
    $form['agent_basic_info']['overview'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('personalize-campaign-overview'),
      ),
    );
    $status_map = personalize_get_agent_status_map();
    $status = personalize_agent_get_status($agent_data->machine_name);
    $form['agent_basic_info']['overview']['status'] = array(
      '#markup' => '<div>' . t('Status: @status', array('@status' => $status_map[$status])) . '</div>',
    );
  }
  if (empty($values['agent_basic_info']['title'])) {
    $title = isset($agent_data->label) ? $agent_data->label : '';
  }
  else {
    $title = $values['agent_basic_info']['title'];
  }
  $form['agent_basic_info']['title'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => $title,
    '#required' => TRUE,
  );
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
    );
  }
  if ($is_new) {
    $form['agent_basic_info']['create'] = array(
      '#type' => 'submit',
      '#value' => t('Create and edit'),
      '#disabled' => empty($agent_type_options),
      '#attributes' => array(
        'class' => array('action-item-primary-active'),
      ),
    );
  }

  return $form;
}

/**
 * The subform for add/editing variations.
 *
 * @param array $form
 *   The current form array.
 * @param array $form_state
 *   The current drupal form state
 * @param stdClass $agent_data
 *   The current campaign data.
 * @param PersonalizeAgentInterface $agent_instance
 *   The agent that defines the functionality for the campaign (if set).
 */
function personalize_campaign_wizard_variations($form, &$form_state, $agent_data, $agent_instance = NULL) {
  // Load the data used throughout form.
  $option_sets = personalize_option_set_load_by_agent($agent_data->machine_name);
  $campaign_page = "admin/structure/personalize/manage/{$agent_data->machine_name}";
  $is_running = personalize_agent_get_status($agent_data->machine_name) == PERSONALIZE_STATUS_RUNNING;

  $form['variations'] = array(
    '#type' => 'container',
    '#tree' => FALSE,
    '#theme' => 'personalize_wizard_section',
  );
  $form['variations']['title'] = array(
    '#type' => 'container',
    '#title' => t('Variation sets'),
    '#instructions' => t('A variation set is a collection of related content for a website object, from which content items are selected for display to each visitor based on the personalization\'s settings and the visitors\' contexts.'),
    '#theme' => 'personalize_wizard_section_title',
  );
  $form['variations']['title']['summary'] = array(
    '#markup' => personalize_get_create_new_links_dropbutton($campaign_page),
  );
  // Determine option set labels.
  $counter = 1;
  // Tricky: key is not necessarily from 0.
  // Setting value on existing object causes unwanted recursion so labels
  // are saved within their own object.
  $option_sets_labels = array();
  foreach($option_sets as $option_set) {
    $option_sets_labels[$option_set->osid] = theme('personalize_admin_enumerated_item', array(
      'enum' => t('Set @delta', array('@delta' => $counter)),
      'title' => isset($option_set->label) ? check_plain($option_set->label) : t('Option Set @id', array('@id' => $option_set->osid)),
    ));
    $counter++;
  }

  // Containing element for all option sets.
  $form['variations']['option_sets'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('personalize-wizard-variation-sets'),
    ),
  );

  $section = &$form['variations']['option_sets'];
  foreach ($option_sets as $option_set) {
    $option_set_plugin = personalize_get_option_set_type($option_set->plugin);
    $replace_option_set_id = drupal_strtolower('personalize-option-set-' . $option_set->osid);

    // Show this open if it was just refreshed via Ajax.
    $section['option_set_' . $option_set->osid] = array(
      '#tree' => TRUE,
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('personalize-option-set'),
        'id' => $replace_option_set_id, // Used for AJAX replace.
      ),
    );
    $option_set_winner = isset($form_state['winners']['option_set_' . $option_set->osid]) ? $form_state['winners']['option_set_' . $option_set->osid] : $option_set->winner;
    $section['option_set_' . $option_set->osid]['winner'] = array(
      '#type' => 'value',
      '#value' => $option_set_winner,
    );
    // Header information.
    $edit_link = module_invoke($option_set_plugin['module'], 'personalize_edit_link', $option_set);
    $delete_link = module_invoke($option_set_plugin['module'], 'personalize_delete_link', $option_set);
    $section['option_set_' . $option_set->osid]['summary'] = array(
      '#type' => 'markup',
      '#markup' => theme('personalize_wizard_variations_header', array(
        'variation_title' => $option_sets_labels[$option_set->osid],
        'variation_count' => count($option_set->options),
        'edit_link' => !empty($edit_link) ? l('edit', $edit_link, array('query' => array('destination' => $campaign_page))) : '',
        'delete_link' => !empty($delete_link) ? l('delete', $delete_link, array('query' => array('destination' => $campaign_page))) : '',
      )),
      '#theme_wrappers' => array('container'),
    );

    $variant_number = 1;
    foreach ($option_set->options as $option) {
      $is_winner = FALSE;
      $is_control = $variant_number == 1;
      // Determine container classes based on campaign status and winner.
      $classes = array('personalize-admin-content-item', 'personalize-content-variation', 'clearfix');
      if ($is_running) {
        $classes[] = 'personalize-content-variation-running';
      }
      else {
        $classes[] = 'personalize-content-variation-stopped';
        // The winner is either previously selected, or the control option.
        if ((!empty($option_set_winner) && $option_set_winner == $option['option_id']) ||
          (empty($option_set_winner) && $is_control)) {
          $is_winner = TRUE;
          $classes[] = 'personalize-content-variation-winner';
        }
      }
      $section['option_set_' . $option_set->osid]['options'][$option['option_id']] = array(
        '#tree' => TRUE,
        '#type' => 'container',
        '#attributes' => array(
          'class' => $classes,
        ),
      );
      $section['option_set_' . $option_set->osid]['options'][$option['option_id']]['basic'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('personalize-variation-row'),
        ),
      );
      $preview_link = '';
      if (!empty($option_set->preview_link)) {
        $preview_link = l('Preview', $option_set->preview_link, array(
          'attributes' => array('target' => 'preview'),
          'query' => array(PERSONALIZE_PRESELECTION_PARAM => personalize_stringify_osid($option_set->osid) . '--' . $option['option_id']),
        ));
      }
      $suffix = !empty($preview_link) ? '" ' . $preview_link : '"';
      if ($is_control) {
        $suffix .= '<span class="personalize-content-variation-control">' . t('Control') . '</span>';
      }
      if ($is_winner) {
        $suffix .= '<span class="personalize-content-variation-winner">' . t('Fallback/Winner') . '</span>';
      }

      $heading = theme('personalize_admin_enumerated_item', array(
        'enum' => t('V@delta', array('@delta' => $variant_number)),
        'title' => check_plain($option['option_label']),
        'title_prefix' => '"',
        'title_suffix' => $suffix,
      ));
      $section['option_set_' . $option_set->osid]['options'][$option['option_id']]['basic']['heading'] = array(
        '#markup' => $heading,
      );
      if (!$is_running & !$is_winner) {
        $section['option_set_' . $option_set->osid]['options'][$option['option_id']]['basic']['winner'] = array(
          '#prefix' => '<div class="personalize-option-set-winner">',
          '#suffix' => '</div>',
          '#type' => 'submit',
          '#tag' => 'button',
          '#text' => t('Set as Fallback/Winner'),
          '#value' => 'winner_' . $option_set->osid . '_' . $option['option_id'],
          '#theme_wrappers' => array('personalize_html_tag'),
          '#attributes' => array(
            // The ID is necessary for the AJAX replace to function correctly.
            'id' => drupal_strtolower('edit-option-sets-option-set-' . $option_set->osid . '-options-' . $option['option_id'] . '-basic-winner'),
            'class' => array('personalize-add-link'),
            'title' => t('Click here to set as winning variation.')
          ),
          '#submit' => array('personalize_campaign_wizard_ajax_variations_winner_submit'),
          '#ajax' => array(
            'callback' => 'personalize_campaign_wizard_ajax_variations_callback',
            'wrapper' => $replace_option_set_id,
            'effect' => 'fade',
            'progress' => array(
              'message' => ' ',
              'type' => 'throbber',
            )
          ),
        );
      }
      $variant_number++;
    } // end of loop through variations.

    // Add the advanced options.
    $section['option_set_' . $option_set->osid]['advanced'] = personalize_campaign_wizard_variations_advanced($form, $form_state, $option_set);
  } // End loop through option sets.

  // Add agent level advanced settings.
  $form['variations']['advanced'] = personalize_campaign_wizard_variations_advanced_agent($form, $form_state, $agent_data, $agent_instance);
  return $form;
}

/**
 * Helper subform for the advanced section of an option set.
 */
function personalize_campaign_wizard_variations_advanced(&$form, &$form_state, $option_set) {
  $option_set_plugin = personalize_get_option_set_type($option_set->plugin);
  $advanced = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Advanced options for @option_set', array(
      '@option_set' => $option_set->label,
    )),
  );
  $advanced['label'] = array(
    '#type' => 'textfield',
    '#title' => 'Name',
    '#description' => 'Shown in the menu when you are viewing content variations on a page.',
    '#default_value' => isset($option_set->label) ? $option_set->label : '',
  );
  $advanced['decision_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Decision Name',
    '#description' => 'By default, this decision will be named after the content variation ID. You can use this if you want to make one conceptual decision across multiple content variations such as a special offer which manifests in different places on the site.',
    '#default_value' => isset($option_set->decision_name) ? $option_set->decision_name : '',
  );
  $advanced['stateful'] = array(
    '#type' => 'checkbox',
    '#title' => t('Shareable'),
    '#description' => 'Will display what the original visitor saw, not another variation.',
    '#default_value' => isset($option_set->stateful) ? $option_set->stateful : 0,
  );
  $advanced['preview_link'] = array(
    '#type' => 'textfield',
    '#title' => 'Preview link',
    '#description' => t('Enter the internal Drupal path on your site to a page containing this content variation set. Enter &lt;front&gt; to link to the front page.'),
    '#default_value' => !empty($option_set->preview_link) ? $option_set->preview_link : '',
    '#element_validate' => array('visitor_actions_form_element_path_validate'),
    '#allow_external' => FALSE,
  );
  // Display options to select the executor if supported.
  $executors = personalize_get_executors();
  $supported_executors = module_invoke($option_set_plugin['module'], 'personalize_get_executor_options');
  if (count($supported_executors) == 1) {
    $advanced['executor'] = array(
      '#type' => 'value',
      '#value' => array_shift(array_keys($supported_executors)),
    );
  }
  else {
    if (count($supported_executors) > 1) {
      $default_executor = '';
      $options = array();
      foreach ($supported_executors as $executor_name => &$supported_options) {
        // Allow the supported declarations to overwrite the default displays.
        $supported_options += $executors[$executor_name];
        $options[$executor_name] = $supported_options['title'];
        if (isset($supported_options['default']) && $supported_options['default'] === TRUE) {
          $default_executor = $executor_name;
        }
      }

      if (isset($option_set->executor) && isset($options[$option_set->executor])) {
        $default_executor = $option_set->executor;
      }
      if (empty($default_executor)) {
        reset($supported_executors);
        $default_executor = key($supported_executors);
      }

      $advanced['executor'] = array(
        '#type' => 'radios',
        '#title' => t('Rendering'),
        '#options' => $options,
        '#default_value' => $default_executor,
        '#title_display' => 'invisible',
      );
      // Add descriptions to the executor options
      foreach ($supported_executors as $executor_name => $options) {
        $advanced['executor'][$executor_name] = array(
          '#description' => $options['description'] . theme('personalize_admin_info_details', $options),
        );
      }
    }
  }
  return $advanced;
}

/**
 * Helper function to generate the advanced settings form for agent-level
 * settings.
 *
 * @param array $form
 *   The current form array.
 * @param array $form_state
 *   The current drupal form state
 * @param stdClass $agent_data
 *   The current campaign data.
 * @param PersonalizeAgentInterface $agent_instance
 *   The agent that defines the functionality for the campaign (if set).
 * @return array
 *   The advanced settings form element.
 */
function personalize_campaign_wizard_variations_advanced_agent($form, &$form_state, $agent_data, $agent_instance = NULL) {
  $element = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
  );
  $element['cache_decisions'] = array(
    '#type' => 'checkbox',
    '#title' => 'Cache decisions made by this agent?',
    '#description' => t('Check this if decisions made by this agent should stick, so that they don\'t have to be made again every time variation sets are rendered. This depends on the availability of local storage in the visitor\'s browser'),
    '#default_value' => isset($agent_data->data['cache_decisions']) ? $agent_data->data['cache_decisions'] : 1
  );
  return $element;
}

/**
 * Subform for campaign targeting.
 *
 * @param array $form
 *   The current form array.
 * @param array $form_state
 *   The current drupal form state
 * @param stdClass $agent_data
 *   The current campaign data.
 * @param PersonalizeAgentInterface $agent_instance
 *   The agent that defines the functionality for the campaign (if set).
 */
function personalize_campaign_wizard_targeting($form, &$form_state, $agent_data, $agent_instance = NULL) {
  if (empty($agent_instance) || !($agent_instance instanceof PersonalizeExplicitTargetingInterface)) {
    return array(
      'unsupported' => array(
        '#type' => 'container',
        'message' => array(
          '#markup' => t('This agent does not support targeting.'),
        ),
      ),
    );
  }
  // Get the enabled targeting values for this agent.
  $targeting_values = personalize_get_targeting_options_for_agent($agent_data);
  if (empty($targeting_values)) {
    return array(
      'unsupported' => array(
        '#type' => 'container',
        'message' => array(
          '#markup' => t('Either your website does not have any enabled targeting contexts, or there are no targeting contexts available for your personalization. Enable one or more applicable Context modules (such as the <a href="!module_url">Personalize URL Context</a> module) to allow for context targeting.',
            array(
              '!module_url' => url('admin/modules', array(
                'fragment' => 'personalization',
              )),
            )
          ),
        ),
      ),
    );
  }

  $form['targeting'] = array(
    '#type' => 'container',
    '#tree' => FALSE,
    '#theme' => 'personalize_wizard_section',
  );
  $form['targeting']['title'] = array(
    '#type' => 'container',
    '#title' => t('Targeting'),
    '#instructions' => t('Determine which of the variations in your personalizations are displayed to specific groups of website visitors.'),
    '#theme' => 'personalize_wizard_section_title',
  );

  // Load the data used throughout form.
  $option_sets = personalize_option_set_load_by_agent($agent_data->machine_name);
  $is_running = personalize_agent_get_status($agent_data->machine_name) == PERSONALIZE_STATUS_RUNNING;

  if (empty($option_sets)) {
    $form['targeting']['error'] = array(
      '#markup' => t('You must <a href="!variations_url">create variation sets</a> before targeting.', array(
        '!variations_url' => url('admin/structure/personalize/manage/' . $agent_data->machine_name . '/variations'),
      )),
    );
    return $form;
  }

  // Containing element for all option sets.
  $form['targeting']['option_sets'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('personalize-wizard-variation-sets'),
    ),
  );

  $section = &$form['targeting']['option_sets'];
  if (empty($option_sets)) {
    return $form;
  }

  $targeting_support = $agent_instance->explicitTargetingSupportMultiple();

  foreach ($option_sets as $option_set) {
    $replace_option_set_id = drupal_strtolower('personalize-option-set-' . $option_set->osid);

    // Show this open if it was just refreshed via Ajax.
    $section['option_set_' . $option_set->osid] = array(
      '#tree' => TRUE,
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('personalize-option-set'),
        'id' => $replace_option_set_id, // Used for AJAX replace.
      ),
    );
    $option_set_winner = isset($form_state['winners']['option_set_' . $option_set->osid]) ? $form_state['winners']['option_set_' . $option_set->osid] : $option_set->winner;
    $section['option_set_' . $option_set->osid]['winner'] = array(
      '#type' => 'value',
      '#value' => $option_set_winner,
    );
    // Header information.
    $section['option_set_' . $option_set->osid]['summary'] = array(
      '#type' => 'markup',
      '#markup' => theme('personalize_wizard_variations_header', array(
        'variation_title' => $option_set->label,
        'variation_count' => count($option_set->options),
      )),
      '#theme_wrappers' => array('container'),
    );

    $variant_number = 1;
    $targeting = personalize_get_targeting_for_options($option_set);
    foreach ($option_set->options as $option) {
      // Determine container classes based on campaign status.
      $classes = array('personalize-admin-content-item', 'personalize-content-variation', 'clearfix');
      if ($is_running) {
        $classes[] = 'personalize-content-variation-running';
      }
      else {
        $classes[] = 'personalize-content-variation-stopped';
      }
      $section['option_set_' . $option_set->osid]['options'][$option['option_id']] = array(
        '#tree' => TRUE,
        '#type' => 'container',
        '#attributes' => array(
          'class' => $classes,
        ),
      );
      $section['option_set_' . $option_set->osid]['options'][$option['option_id']]['basic'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('personalize-variation-row'),
        ),
      );

      $heading = theme('personalize_admin_enumerated_item', array(
        'enum' => t('V@delta', array('@delta' => $variant_number)),
        'title' => check_plain($option['option_label']),
        'title_prefix' => '"',
        'title_suffix' => '"',
      ));
      $section['option_set_' . $option_set->osid]['options'][$option['option_id']]['basic']['heading'] = array(
        '#markup' => $heading,
      );
      if ($targeting_support && !empty($targeting_values)) {
        $section['option_set_' . $option_set->osid]['options'][$option['option_id']]['targeting'] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('personalize-variation-row'),
          ),
        );
        $section['option_set_' . $option_set->osid]['options'][$option['option_id']]['targeting']['enable_explicit_targeting'] = array(
          '#title' => t('Show to visitors with specific traits'),
          '#type' => 'checkbox',
          '#default_value' => !empty($targeting[$option['option_id']]) ? 1 : 0,
          '#parents' => array('option_sets', 'option_set_' . $option_set->osid, 'options', $option['option_id'],'enable_explicit_targeting'),
        );
        $states = array(
          'visible' => array(
            ':input[name="option_sets[option_set_' . $option_set->osid . '][options][' . $option['option_id'] . '][enable_explicit_targeting]"]' => array('checked' => TRUE),
          ),
        );
        $parents = array('option_sets', 'option_set_' . $option_set->osid, 'options', $option['option_id']);
        $section['option_set_' . $option_set->osid]['options'][$option['option_id']]['explicit_targeting'] = personalize_targeting_support_form_elements($option_set, $option, $targeting_values, $targeting_support, $states, $parents, $form_state);
      }
      $variant_number++;
    } // end of loop through variations.

    switch ($targeting_support) {
      case PersonalizeExplicitTargetingInterface::EXPLICIT_TARGETING_MULTIPLE_AND:
        $section['option_set_' . $option_set->osid]['explicit_targeting_explanation'] = array(
          '#type' => 'markup',
          '#markup' => t('You can add explicit targeting for this content variation. If you choose multiple contexts for an option, then the visitor must have all specified contexts for the rule to apply.')

        );
        break;
      case PersonalizeExplicitTargetingInterface::EXPLICIT_TARGETING_MULTIPLE_OR:
        $section['option_set_' . $option_set->osid]['explicit_targeting_explanation'] = array(
          '#type' => 'markup',
          '#markup' => t('You can add explicit targeting for this content variation. If you choose multiple contexts for an option, then the rule will apply if the visitor has any of the specified contexts.')
        );
        break;
    }
  } // End loop through option sets.
  return $form;
}


/**
 * The subform for add/editing goals.
 *
 * @param array $form
 *   The current form array.
 * @param array $form_state
 *   The current drupal form state
 * @param stdClass $agent_data
 *   The current campaign data.
 * @param PersonalizeAgentInterface $agent_instance
 *   The agent that defines the functionality for the campaign (if set).
 */
function personalize_campaign_wizard_goals($form, &$form_state, $agent_data, $agent_instance = NULL) {
  if (empty($agent_instance) || !($agent_instance instanceof PersonalizeAgentGoalInterface)) {
    return array(
      '#markup' => t('This agent does not support goals.'),
    );
  }

  $goal_entities = personalize_goal_load_by_conditions(array('agent' => $agent_data->machine_name));
  $goals = $exclude = array();
  $all_actions = visitor_actions_get_actions();
  foreach ($goal_entities as $obj) {
    $goals[] = (array) $obj;
  }

  foreach ($goals as $i => $goal) {
    if (!isset($all_actions[$goal['action']])) {
      // The action for this goal no longer exists - don't include it in the form
      // and set a warning that it will be removed upon save.
      drupal_set_message(t('At least one of your goals no longer has an action associated with it. Saving this form will cause the goal to be removed from the personalization.'), 'warning', FALSE);
      unset($goals[$i]);
    }
    else {
      // Any action already set as a goal on this campaign should be excluded
      // from the action dropdown.
      $exclude[$goal['action']] = $goal['action'];
    }
  }

  if (empty($goals)) {
    $goals[] = array('action' => '', 'value' => 1);
  }

  // If the "Add another" button was clicked, we need to add goals to get up
  // to the number indicated.
  $num_goals = count($goals);
  if (isset($form_state['num_goals']) && $form_state['num_goals'] > $num_goals) {
    while ($num_goals < $form_state['num_goals']) {
      $goals[] = array('action' => '', 'value' => 1, 'collapsed' => FALSE, 'classes' => array('personalize-goal-add'));
      $num_goals++;
    }
  }
  $form_state['num_goals'] = count($goals);

  $goal_options = personalize_goals_options(TRUE);
  // For new goals, we exclude actions already used.
  $new_goal_options = array_diff_key($goal_options, $exclude);
  // Add the "add new action in context" option if visitor_actions_ui
  // is enabled.
  if (module_exists('visitor_actions_ui')) {
    $link = url('admin/structure/visitor_actions/add-in-context', array(
      'query' => array('destination' => "admin/structure/personalize/manage/{$agent_data->machine_name}/goals"),
      'attributes' => array(
        'class' => array('personalize-add-context', 'button'),
      ),
    ));
    $new_goal_options[$link] = t('[Add new action in context...]');
  }

  // Begin form generation.
  $form = array();
  // Assemble the form in nested containers to allow for easier styling.
  $form['goals'] = array(
    '#type' => 'container',
    '#tree' => FALSE,
    '#attributes' => array(
      'id' => 'personalize-goals-container',
    ),
    '#theme' => 'personalize_wizard_section',
  );

  $form['goals']['title'] = array(
    '#type' => 'container',
    '#title' => t('Goals'),
    '#instructions' => t('Create your personalization\'s goals, which are the website actions that you want your visitors to take. Completed goals for a variation increase its chance of being the winning variation for a group of visitors.'),
    '#theme' => 'personalize_wizard_section_title',
  );
  $form['goals']['title']['title_summary'] = array(
    '#prefix' => '<span class="personalize-add-link-prefix"></span>',
    '#type' => 'submit',
    '#value' => t('Add goal'),
    '#attributes' => array(
      'class' => array('personalize-add-link'),
      'title' => t('Click here to add more goals.')
    ),
    '#submit' => array('personalize_campaign_wizard_ajax_goal_add_submit'),
    '#ajax' => array(
      'callback' => 'personalize_campaign_wizard_ajax_goals_callback',
      'wrapper' => 'personalize-goals',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('goals')),
  );

  $goal_labels = array();
  // Use a counter variable as the delta is affected by adding/removing goals.
  $counter = 1;
  foreach($goals as $delta => $goal) {
    if (empty($goal['action'])) {
      $title = t('New goal');
    }
    else {
      $title = empty($goal_options[$goal['action']]) ? check_plain($goal['action']) : check_plain($goal_options[$goal['action']]);
    }
    // Save the display goal title for use later
    $goal_labels[$delta] = theme('personalize_admin_enumerated_item', array(
      'enum' => t('Goal @delta', array('@delta' => ($counter))),
      'title' => $title,
    ));
    $counter++;
  }

  // Now handle the goals input display.
  $section = &$form['goals'];
  $section['all_goals'] = array(
    '#tree' => TRUE,
    '#theme_wrappers' => array('container'),
    '#attributes' => array(
      'id' => 'personalize-goals',
      'class' => array('personalize-wizard-goals', 'personalize-goals-wrapper'),
    ),
  );

  foreach ($goals as $delta => $goal) {
    $goal_fieldset_classes = array('personalize-admin-collapsed-content', 'personalize-goal');
    if (isset($goal['classes']) && is_array($goal['classes'])) {
      $goal_fieldset_classes = array_merge($goal_fieldset_classes, $goal['classes']);
    }
    // The goal id should match loaded goals from the database for jumping to
    // a specific goal on page load.  This will not be available after an AJAX
    // call, but that doesn't matter in the use case of initial page linkages.
    $section['all_goals'][$delta] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => isset($goal['collapsed']) ? $goal['collapsed'] : TRUE,
      '#title' => $goal_labels[$delta],
      '#tree' => TRUE,
      '#attributes' => array(
        'class' => $goal_fieldset_classes,
        'id' => isset($goal['id']) ? "personalize-goal-{$goal['id']}" : "personalize-goal-$delta",
      )
    );
    $options = $goal['action'] == '' ? $new_goal_options : $goal_options;

    if (!empty($goal['action']) && empty($options[$goal['action']])) {
      if (isset($all_actions[$goal['actions']])) {
        // The current goal was excluded from the list of options because it is
        // a limited use visitor action, just add it separately here.
        $options = array_merge(array($goal['action'] => $all_actions[$goal['action']]['label']), $options);
      }
    }
    $section['all_goals'][$delta]['goal_id'] = array(
      '#type' => 'value',
      '#value' => isset($goal['id']) ? $goal['id'] : '',
    );
    $section['all_goals'][$delta]['action_name'] = array(
      '#type' => 'select',
      '#title' => t('Action'),
      '#options' => $options,
      '#default_value' => $goal['action'],
      '#empty_option' => t('Select an action'),
      '#attributes' => array(
        'class' => array('personalize-goal-action'),
      ),
    );
    $section['all_goals'][$delta]['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#size' => 5,
      '#default_value' => $goal['value']
    );
    $section['all_goals'][$delta]['remove'] = array(
      '#type' => 'submit',
      '#tag' => 'button',
      '#text' => t('Remove'),
      '#value' => 'remove_' . $delta,
      '#theme_wrappers' => array('personalize_html_tag'),
      '#attributes' => array(
        // The ID is necessary for the AJAX replace to function correctly. It
        // is fragile to declare it like this, essentially hard-coding the
        // #parents, but I know of no other way to do this programmatically.
        'id' => 'edit-all-goals-' . $delta . '-remove',
        'data-personalize-goal-id' => empty($goal['id']) ? '' : $goal['id'],
        'data-personalize-goal-index' => $delta,
        'class' => array('personalize-delete-goal', 'form-submit',),
        'title' => t('Delete the goal.'),
        // Adding the name and value attributes allows for targeting within
        // simpletest.
        'name' => 'edit-goals-' . $delta . '-remove',
        'value' => t('Remove'),
      ),
      '#submit' => array('personalize_campaign_wizard_ajax_goal_remove_submit'),
      '#ajax' => array(
        'callback' => 'personalize_campaign_wizard_ajax_goals_callback',
        'wrapper' => 'personalize-goals',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#limit_validation_errors' => array(array('goals')),
    );
  }
  return $form;
}

/**
 * Subform for campaign scheduling.
 *
 * @param array $form
 *   The current form array.
 * @param array $form_state
 *   The current drupal form state
 * @param stdClass $agent_data
 *   The current campaign data.
 * @param PersonalizeAgentInterface $agent_instance
 *   The agent that defines the functionality for the campaign (if set).
 */
function personalize_campaign_wizard_scheduling($form, &$form_state, $agent_data, $agent_instance = NULL) {
  $agent_end = $default_end = personalize_agent_get_stop_date($agent_data->machine_name);
  $agent_start = $default_start = personalize_agent_get_start_date($agent_data->machine_name);

  // Default the start date to one month and the end date to one month.
  if ($agent_start == 0) {
    $default_start = strtotime('+1 month');
  }
  if ($agent_end == 0) {
    $default_end = strtotime('+1 month');
  }

  $form['scheduling'] = array(
    '#type' => 'container',
    '#tree' => FALSE,
  );
  $form['scheduling']['title'] = array(
    '#type' => 'container',
    '#title' => t('Scheduling'),
    '#instructions' => t('Control the settings that define how and when your personalization will begin and end.'),
    '#theme' => 'personalize_wizard_section_title',
  );

  $agent_start_date = array('year' => date('Y', $default_start), 'month' => intval(date('m', $default_start)), 'day' => date('d', $default_start));
  $form['scheduling']['campaign_start'] = array(
    '#type' => 'radios',
    '#title' => t('Personalization start date'),
    '#options' => array(
      'none' => t('Start manually'),
      'specified' => t('Start on selected date'),
    ),
    '#default_value' => $agent_start == 0 ? 'none' : 'specified',
  );
  $form['scheduling']['campaign_start_date'] = array(
    '#type' => 'date',
    '#default_value' => $agent_start_date,
    '#states' => array(
      'visible' => array(
        ':input[name="campaign_start"]' => array('value' => 'specified'),
      ),
    ),
  );
  $agent_end_date = array('year' => date('Y', $default_end), 'month' => intval(date('m', $default_end)), 'day' => date('d', $default_end));
  $form['scheduling']['campaign_end'] = array(
    '#type' => 'radios',
    '#title' => t('Personalization end date'),
    '#options' => array(
      'none' => t('End manually'),
      'specified' => t('End on selected date'),
    ),
    '#default_value' => $agent_end == 0 ? 'none' : 'specified',
  );
  $form['scheduling']['campaign_end_date'] = array(
    '#type' => 'date',
    '#default_value' => $agent_end_date,
    '#states' => array(
      'visible' => array(
        ':input[name="campaign_end"]' => array('value' => 'specified'),
      ),
    ),
  );
  return $form;
}

/**
 * Subform for campaign detail review.
 *
 * @param array $form
 *   The current form array.
 * @param array $form_state
 *   The current drupal form state
 * @param stdClass $agent_data
 *   The current campaign data.
 * @param PersonalizeAgentInterface $agent_instance
 *   The agent that defines the functionality for the campaign (if set).
 */
function personalize_campaign_wizard_review($form, &$form_state, $agent_data, $agent_instance = NULL) {
  $form['review'] = array(
    '#type' => 'container',
    '#theme' => 'personalize_wizard_section',
  );
  $form['review']['summary_column'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('personalize-wizard-column'),
    ),
  );
  $form['review']['summary_column']['summary'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Overview'),
  );

  // Get summary data for option sets.
  $option_sets = personalize_option_set_load_by_agent($agent_data->machine_name);
  $count_option_sets = count($option_sets);
  $items = array();
  foreach ($option_sets as $option_set) {
    $items[] = $option_set->label . ' (' . $option_set->plugin . ')';
  }

  $form['review']['summary_column']['summary']['option_sets'] = array(
    '#markup' => theme('personalize_wizard_summary_count', array(
      'count' => $count_option_sets,
      'details' => theme('item_list', array(
        'title' => format_plural($count_option_sets, 'Variation set', 'Variation sets'),
        'items' => $items,
      )),
    )),
  );
  // Get summary data for goals.
  if ($agent_instance instanceof PersonalizeAgentGoalInterface) {
    $goals = personalize_goal_load_by_conditions(array('agent' => $agent_data->machine_name));
    $count_goals = count($goals);
    $actions = visitor_actions_get_actions();
    $items = array();
    foreach ($goals as $goal) {
      $items[] = isset($actions[$goal->action]) ? $actions[$goal->action]['label'] : $goal->action;
    }

    $form['review']['summary_column']['summary']['goals'] = array(
      '#markup' => theme('personalize_wizard_summary_count', array(
        'count' => $count_goals,
        'details' => theme('item_list', array(
          'title' => format_plural($count_goals, 'Goal', 'Goals'),
          'items' => $items,
        )),
      )),
    );
  }
  $form['review']['summary_column']['summary']['scheduling'] = array(
    '#markup' => theme('personalize_wizard_summary_schedule', array(
      'start_date' => personalize_agent_get_start_date($agent_data->machine_name),
      'stop_date' => personalize_agent_get_stop_date($agent_data->machine_name),
      'started_date' => empty($agent_data->started) ? 0 : $agent_data->started,
    )),
  );

  // Get an errors that would prevent submitting the agent.
  $warnings = personalize_verify_agent($agent_data->machine_name, FALSE);
  $start_date = personalize_agent_get_start_date($agent_data->machine_name);
  $end_date = personalize_agent_get_stop_date($agent_data->machine_name);
  if (($start_date > 0 && $start_date < time()) ||
    ($end_date > 0 && $end_date < time())) {
    $warnings[] = t('The scheduled dates for this personalization have already passed.  In the <a href="!schedule_url">When</a> section, update or remove the date information.', array(
      '!schedule_url' => url('admin/structure/personalize/manage/' . $agent_data->machine_name . '/scheduling'),
    ));
  }
  $form['review']['messages'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('personalize-wizard-column'),
    ),
  );
  if (!empty($warnings)) {
    $form_state['storage']['allow_submit'] = FALSE;
    $form['review']['messages']['warnings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Let\'s fix this'),
      '#collapsible' => FALSE,
    );
    $form['review']['messages']['warnings']['details'] = array(
      '#markup' => theme('item_list', array(
        'items' => $warnings,
        'attributes' => array(
          'class' => array('personalize-wizard-review-warnings'),
        ),
      )),
    );
  }
  return $form;
}

/**
 ********************************************************************
 *
 * I N S T R U C T I O N S  &  H E L P
 *
 ********************************************************************
 */


/**
 ********************************************************************
 *
 * A J A X  C A L L B A C K S
 *
 ********************************************************************
 */

/**
 * Ajax submit callback to set the winning variation within an option set.
 */
function personalize_campaign_wizard_ajax_variations_winner_submit($form, &$form_state) {
  list($prefix, $osid, $option_id) = explode('_', $form_state['triggering_element']['#value']);
  $form_state['winners']['option_set_' . $osid] = $option_id;
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback to return a portion of the variations form.
 */
function personalize_campaign_wizard_ajax_variations_callback($form, &$form_state) {
  list($prefix, $osid, $option_id) = explode('_', $form_state['triggering_element']['#value']);
  return $form['variations']['option_sets']['option_set_' . $osid];
}

/**
 * Ajax callback for the add goal and remove goal buttons.
 */
function personalize_campaign_wizard_ajax_goals_callback($form, $form_state) {
  return $form['goals']['all_goals'];
}

/**
 * Submit handler for the "Add Goal" button.
 */
function personalize_campaign_wizard_ajax_goal_add_submit($form, &$form_state) {
  // Increment the number of goal elements to be rendered.
  $form_state['num_goals']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "Remove Goal" button.
 */
function personalize_campaign_wizard_ajax_goal_remove_submit($form, &$form_state) {
  // Get the goal delta for the clicked button.
  $goal_id = $form_state['triggering_element']['#attributes']['data-personalize-goal-id'];
  $delta = $form_state['triggering_element']['#attributes']['data-personalize-goal-index'];
  if (!empty($goal_id)) {
    personalize_goal_delete($goal_id);
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for changing data within the process bar.
 */
function personalize_campaign_wizard_ajax_process_bar_callback($form, &$form_state) {
  return $form['process_bar'];
}

/**
 ********************************************************************
 *
 * V A L I D A T I O N
 *
 ********************************************************************
 */

/**
 * Validate handler for wizard form.
 */
function personalize_campaign_wizard_validate($form, &$form_state) {
  $step_name = $form_state['storage']['step'];
  // Do not validate if the form is disabled.
  if (!empty($form[$step_name]['#disabled'])) {
    return;
  }

  module_load_include('inc', 'personalize', 'personalize.admin');

  // Call the validation step for the appropriate subform.
  $function = 'personalize_campaign_wizard_validate_' . $step_name;
  if (function_exists($function)) {
    $function($form, $form_state);
  }
}

/**
 * Validation callback for targeting form.
 */
function personalize_campaign_wizard_validate_targeting($form, &$form_state) {
  $form_values = &$form_state['values']['targeting'];
  if (empty($form_values)) {
    return;
  }

  $need_form_state_cache_clear = FALSE;
  foreach ($form_values as $option_set_id => $values) {
    if (!empty($form_values[$option_set_id]['options'])) {
      foreach ($form_values[$option_set_id]['options'] as $option_name => $option) {
        if (isset($option['enable_explicit_targeting']) && $option['enable_explicit_targeting'] == 0) {
          $form_values[$option_set_id]['options'][$option_name]['explicit_targeting']['mapping'] = array();
          $need_form_state_cache_clear = TRUE;
        }
      }
    }
  }

  // Drupal caches form_state before validators are triggered, so flush it
  if ($need_form_state_cache_clear) {
    cache_clear_all('form_state_' . $form_state['values']['form_build_id'], 'cache_form');
  }

}

/**
 * Validation for advanced settings form for a particular option set.
 *
 * @param array $form_values
 *   An array of form state values for each option set.
 * @param string $parents
 *   The string representation of the parents to the form values used to target
 *   error messages, e.g. variations][option_sets][option_set_1]
 */
function personalize_campaign_wizard_validate_variations_advanced($form_values, $parents) {
  if (!empty($form_values['advanced']['preview_link'])) {
    if ($form_values['advanced']['preview_link'] != '<front>' && drupal_lookup_path('source', $form_values['advanced']['preview_link']) === FALSE && drupal_valid_path($form_values['advanced']['preview_link']) === FALSE) {
      form_set_error($parents . '[advanced][preview_link', t('The preview link for "@option_set" must be a valid internal Drupal path.', array(
        '@option_set' => $form_values['advanced']['label'],
      )));
    }
  }
}

/**
 * Custom validation callback for scheduling form.
 */
function personalize_campaign_wizard_validate_scheduling($form, &$form_state) {
  $values = $form_state['values'];
  $start_date = 0;
  $end_date = 0;
  if (isset($values['campaign_start']) && $values['campaign_start'] === 'specified') {
    $start_date = strtotime($values['campaign_start_date']['year'] . '-' . $values['campaign_start_date']['month'] . '-' . $values['campaign_start_date']['day']);
    if ($start_date < time()) {
      form_set_error('campaign_start_date', t('When provided, the start date for the personalization must be in the future.'));
    }
  }
  if (isset($values['campaign_end']) && $values['campaign_end'] === 'specified') {
    $end_date = strtotime($values['campaign_end_date']['year'] . '-' . $values['campaign_end_date']['month'] . '-' . $values['campaign_end_date']['day']);
    if ($end_date < time()) {
      form_set_error('campaign_end_date', t('When provided, the end date for the personalization must be in the future.'));
    }
  }
  if ($start_date > 0 && $end_date > 0) {
    if ($end_date <= $start_date) {
      form_set_error('campaign_end_date', t('When provided, the end date for the personalization must be after the start date for the personalization.'));
    }
  }
}

/**
 ********************************************************************
 *
 * S U B M I S S I O N
 *
 ********************************************************************
 */

/**
 * Submit handler for wizard form to save current values to the appropriate
 * step.
 */
function personalize_campaign_wizard_submit($form, &$form_state) {
  module_load_include('inc', 'personalize', 'personalize.admin');

  // Call the base level submit handler.
  $agent = personalize_campaign_wizard_submit_base($form, $form_state);
  personalize_set_campaign_context($agent->machine_name);
  if ($form_state['storage']['step'] === 'base') {
    // Redirect to the full wizard view with variations selected.
    $form_state['redirect'] = 'admin/structure/personalize/manage/' . $agent->machine_name . '/variations';
    return;
  }

  // Call the submit handler that is specific to the current step.
  $function = 'personalize_campaign_wizard_submit_' . $form_state['storage']['step'];
  if (function_exists($function)) {
    $function($form, $form_state, $agent);
  }
  // Save the campaign.
  if (!personalize_agent_save($agent)) {
    drupal_set_message(t('There was a problem saving the personalization.', 'error'));
  }

  // If a new status was indicated, then update the agent status.
  if (!empty($form_state['triggering_element']['#personalize_next_status']) && empty($form_state['values']['override_step'])) {
    personalize_agent_set_status($agent->machine_name, $form_state['triggering_element']['#personalize_next_status']);
  }

  // Rebuild the form to show the next step.
  $agent_instance = personalize_agent_load_agent($agent->machine_name);
  _personalize_campaign_wizard_rebuild($form_state, $agent_instance);
}

/**
 * Submit handler for the base campaign-level information.
 *
 * This submit handler is responsible for retrieving the base agent object
 * for any other changes to modify prior to saving.
 *
 * @return stdClass
 *   The agent/campaign class that is being modified.
 */
function personalize_campaign_wizard_submit_base($form, &$form_state) {
  if (isset($form_state['values']['agent_basic_info']['machine_name'])) {
    $agent = personalize_agent_load($form_state['values']['agent_basic_info']['machine_name']);
    $agent->label = $form_state['values']['agent_basic_info']['title'];
  }
  else {
    $agent = _personalize_agent_from_form_values($form_state['values']['agent_basic_info']);
  }

  // If it is a new campaign, save the basic information so that it can be
  // added to in the other subform submit handlers.
  if (empty($agent->machine_name)) {
    $agent->data['cache_decisions'] = 0;
    $agent->machine_name = personalize_generate_machine_name($agent->label, 'personalize_agent_machine_name_exists');
    $agent = personalize_agent_save($agent);
  }
  return $agent;
}

/**
 * Submit handler for the variations section.
 */
function personalize_campaign_wizard_submit_variations($form, &$form_state, &$agent_data) {
  $form_values = isset($form_state['values']['option_sets']) && is_array($form_state['values']['option_sets']) ? $form_state['values']['option_sets'] : array();
  foreach ($form_values as $option_set_id => $values) {
    $osid = (int) str_replace('option_set_', '', $option_set_id);
    if ($option_set = personalize_option_set_load($osid)) {
      $option_set->label = $values['advanced']['label'];
      $option_set->stateful = $values['advanced']['stateful'];
      if (isset($values['advanced']['executor'])) {
        $option_set->executor = $values['advanced']['executor'];
      }
      if (isset($values['advanced']['decision_name'])) {
        $option_set->decision_name = personalize_generate_machine_name($values['advanced']['decision_name']);
      }
      if (isset($values['advanced']['preview_link'])) {
        $option_set->preview_link = $values['advanced']['preview_link'];
      }
      if (isset($values['winner'])) {
        $option_set->winner = $values['winner'];
      }
      personalize_campaign_wizard_submit_variations_advanced($values, $option_set);
      if (!isset($values['options'])) {
        personalize_option_set_save($option_set);
        continue;
      }
      personalize_option_set_save($option_set);
    }
  }
  // Update any advanced agent-level settings.
  personalize_campaign_wizard_submit_variations_advanced_agent($form, $form_state, $agent_data);
}

/**
 * Submit helper for advanced settings within the variations subform.
 *
 * Update any advanced options on the option set.
 *
 * @param array $form_values
 *   The form state values that include the "advanced" subform.
 * @param stdClass $option_set
 *   The option set class to get the new values.
 */
function personalize_campaign_wizard_submit_variations_advanced($form_values, &$option_set) {
  $option_set->label = $form_values['advanced']['label'];
  $option_set->stateful = $form_values['advanced']['stateful'];
  if (isset($form_values['advanced']['executor'])) {
    $option_set->executor = $form_values['advanced']['executor'];
  }
  if (isset($form_values['advanced']['decision_name'])) {
    $option_set->decision_name = personalize_generate_machine_name($form_values['advanced']['decision_name']);
  }
  if (isset($form_values['advanced']['preview_link'])) {
    $option_set->preview_link = $form_values['advanced']['preview_link'];
  }
}

/**
 * Submit handler for variations page advanced agent settings.
 */
function personalize_campaign_wizard_submit_variations_advanced_agent($form, &$form_state, &$agent_data) {
  // Decision caching advanced setting.
  $agent_data->data['cache_decisions'] = $form_state['values']['cache_decisions'];
}

/**
 * Submit handler for campaign targeting.
 */
function personalize_campaign_wizard_submit_targeting($form, &$form_state, &$agent_data) {
  $agent_instance = personalize_agent_load_agent($agent_data->machine_name);
  $form_values = isset($form_state['values']['option_sets']) && is_array($form_state['values']['option_sets']) ? $form_state['values']['option_sets'] : array();
  foreach ($form_values as $option_set_id => $values) {
    $osid = (int) str_replace('option_set_', '', $option_set_id);
    if ($option_set = personalize_option_set_load($osid)) {
      // Keep track of which explicit targeting features have already been
      // designated as each can only be specified for one option.
      // Keep track of feature strings that have already been designated to options,
      // which options they have been designated to, and whether there's a rule
      // associated with each feature.
      $designated_features = $targeting = array();
      $target_weight = 0;
      if (empty($values['options'])) {
        continue;
      }
      foreach ($values['options'] as $option_id => $settings) {
        $feature_values = $rules = array();
        if (isset($settings['explicit_targeting']) && !empty($settings['explicit_targeting']['mapping'])) {
          foreach ($settings['explicit_targeting']['mapping']['contexts'] as $delta => $context_values) {
            if ($context_values['context'] == '') {
              continue;
            }
            list($plugin_name, $context_name) = explode(PERSONALIZE_TARGETING_ADMIN_SEPARATOR, $context_values['context']);
            $context_values['match'] = $context_values['value']['match'];
            $context_values['operator'] = $context_values['value']['operator'];
            unset($context_values['value'], $context_values['remove']);
            // Generate a value code based on the operator used.
            $value = personalize_targeting_generate_value_code($context_values['match'], $context_values['operator']);
            // Create a feature string for this context value that can be consumed
            // by the agent that will be using it.
            $feature_string = $agent_instance->convertContextToFeatureString($context_name, $value);
            $feature_values[] = $feature_string;
            // Save the actual rule information as this is what will be used
            // for evaluating it.
            $rules[$feature_string] = $context_values;
            // Override the context to split it into plugin and context parts.
            $rules[$feature_string]['context'] = $context_name;
            $rules[$feature_string]['plugin'] = $plugin_name;
          }
          foreach ($feature_values as $feature) {
            // Mark this feature string as already designated.
            if (!in_array($feature, $designated_features)) {
              $designated_features[] = $feature;
            }
            else {
              drupal_set_message(t('You have set the same targeting feature for more than one option. Depending on whether features are AND\'d or OR\'d together for the different options, and on how the decision agent evaluates explicit targeting rules, this may not produce the desired effect.'), 'warning');
            }
          }
          if (!empty($feature_values)) {
            $rule = array(
              'option_id' => $option_id,
              'targeting_features' => $feature_values,
              'targeting_rules' => $rules,
              'weight' => $target_weight,
            );
            // If a strategy has been specified for how to use the targeting features,
            // then store this as well.
            if (isset($values['options'][$option_id]['explicit_targeting']['strategy'])) {
              $rule['targeting_strategy'] = $values['options'][$option_id]['explicit_targeting']['strategy'];
            }
            $targeting['target_' . ($target_weight + 1)] = $rule;
            $target_weight++;
          }
        }
      }

      $option_set->targeting = $targeting;
      personalize_option_set_save($option_set);
    }
  }
}

/**
 * Submit handler for the goals section.
 */
function personalize_campaign_wizard_submit_goals($form, &$form_state, &$agent_data) {
  // Update the goals for this agent.
  $form_values = $form_state['values']['all_goals'];

  // Check if any goals have been removed during editing.
  $saved_goals = $existing_goals = array();
  foreach (personalize_goal_load_by_conditions(array('agent' => $agent_data->machine_name)) as $goal) {
    // We can only have one goal with a given action per agent, so
    // we key the array of existing goals by action name.
    $existing_goals[$goal->action] = $goal->id;
  }
  foreach ($form_values as $goal) {
    if (!empty($goal['action_name'])) {
      try {
        personalize_goal_save($agent_data->machine_name, $goal['action_name'], $goal['value']);

        // Add this to the array of saved goals so that at the end
        // we know that anything in $existing_goals but not in $saved_goals
        // needs to be deleted.
        $saved_goals[$goal['action_name']] = isset($existing_goals[$goal['action_name']]) ? $existing_goals[$goal['action_name']] : 'new';
      }
      catch (Exception $e) {
        drupal_set_message($e->getMessage(), 'error');
      }
    }
  }
  // The difference between existing goals and saved goals are the ones
  // that need to be deleted.
  $to_delete = array_diff_key($existing_goals, $saved_goals);
  foreach ($to_delete as $goal_id) {
    personalize_goal_delete($goal_id);
  }
  // Redraw the form without any additional new goals included.
  unset($form_state['num_goals']);
}

/**
 * Submit handler for the scheduling section.
 */
function personalize_campaign_wizard_submit_scheduling($form, &$form_state, &$agent_data) {
  // Grab the original start value.
  $current_start = personalize_agent_get_start_date($agent_data->machine_name);

  // Submit the new values.
  $start_date = $end_date = 0;
  $values = $form_state['values'];
  if (!empty($values['campaign_start'])) {
    // Set the start date.
    if ($values['campaign_start'] === 'specified') {
      $start_date = strtotime($values['campaign_start_date']['year'] . '-' . $values['campaign_start_date']['month'] . '-' . $values['campaign_start_date']['day']);
    }
    personalize_agent_set_start_date($agent_data->machine_name, $start_date);
  }
  if (!empty($values['campaign_end'])) {
    // Set the end date.
    if ($values['campaign_end'] === 'specified') {
      $end_date = strtotime($values['campaign_end_date']['year'] . '-' . $values['campaign_end_date']['month'] . '-' . $values['campaign_end_date']['day']);
    }
    personalize_agent_set_stop_date($agent_data->machine_name, $end_date);
  }

  // Show a message if the start schedule changed.
  if ($current_start != $start_date) {
    drupal_set_message(t('The scheduled start date has been saved. Your personalization will not be automatically started until it has been finalized by pressing "Start on @start_date" in the <a href="!review_url">review screen</a>.', array(
      '@start_date' => _personalize_campaign_wizard_date($start_date),
      '!review_url' => url('admin/structure/personalize/manage/' . $agent_data->machine_name . '/review'),
    )));
  }
}

/**
 ********************************************************************
 *
 * H E L P E R  F U N C T I O N S
 *
 ********************************************************************
 */

/**
 * Formats a date for presentation consistently throughout the wizard.
 */
function _personalize_campaign_wizard_date($timestamp) {
  return date('M j, Y', $timestamp);
}

/**
 * Generate an array of steps for the wizard based on the type of agent shown.
 *
 * @param stdClass $agent_instance
 *   (Optional) The agent instance for the current agent.
 * @return array
 *   An array of steps with the following keys:
 *   - label: The label to display for the step
 *   - button_value: The text to display for the step within a button
 *   - preview_message: (Optional) Any text to give a quick preview of the step
 *   - next_status: (Optional) The status that should be applied along with
 *     completing this step.
 */
function _personalize_campaign_wizard_steps(PersonalizeAgentInterface $agent_instance = NULL) {
  $status = empty($agent_instance) ? PERSONALIZE_STATUS_NOT_STARTED : personalize_agent_get_status($agent_instance->getMachineName());
  $not_running = $status == PERSONALIZE_STATUS_NOT_STARTED || $status == PERSONALIZE_STATUS_PAUSED;

  $review_button = t('Save');
  $review_next_status = '';
  if ($not_running) {
    if (!empty($agent_instance) && $start_date = personalize_agent_get_start_date($agent_instance->getMachineName())) {
      $review_button = t('Start on @date', array(
        '@date' => _personalize_campaign_wizard_date($start_date),
      ));
      $review_next_status = PERSONALIZE_STATUS_SCHEDULED;
    }
    else {
      $review_button = t('Start');
      $review_next_status = PERSONALIZE_STATUS_RUNNING;
    }
  }

  $next = t('Next');
  $steps = array(
    'variations' => array(
      'label' => t('What'),
      'description' => t('Create variation sets of personalized content for visitors'),
      'button_value' => $next,
    ),
    'goals' => array(
      'label' => t('Why'),
      'description' => t('Create goals to gauge the effectiveness of variation sets'),
      'button_value' => $next,
      'preview_message' => t('Goals for these variations'),
    ),
    'targeting' => array(
      'label' => t('Who'),
      'description' => t('Determine how variations are displayed to visitor segments'),
      'button_value' => $next,
      'preview_message' => t('Who\'s going to see what'),
    ),
    'scheduling' => array(
      'label' => t('When'),
      'description' => t('Control how and when personalizations start and end'),
      'button_value' => $next,
      'preview_message' => t('When should this personalization run'),
    ),
    'review' => array(
      'label' => t('Review'),
      'description' => t('Examine an overview of your personalization'),
      'button_value' => $review_button,
      'preview_message' => t('Review personalization settings'),
    )
  );

  // Add any status changes that should happen upon completion of the review
  // step.
  if (!empty($review_next_status)) {
    $steps['review']['next_status'] = $review_next_status;
  }
  // Remove any steps that aren't applicable to the current agent.
  if (empty($agent_instance) || !($agent_instance instanceof PersonalizeAgentGoalInterface)) {
    unset($steps['goals']);
  }
  if (empty($agent_instance) || !($agent_instance instanceof PersonalizeExplicitTargetingInterface)) {
    unset($steps['targeting']);
  }
  drupal_alter('personalize_wizard_steps', $steps, $agent_instance);
  return $steps;
}

/**
 * Determine the next step in the wizard based on the type of agent shown.
 *
 * @param array $form_state
 *   The current form state.
 * @param stdClass $agent_instance
 *   The agent instance for the current agent.
 * @return string
 *   The next step if there is a further step.  If the current step is the last
 *   step then it is returned.
 */
function _personalize_campaign_wizard_next_step($form_state, PersonalizeAgentInterface $agent_instance) {
  // If the user is just clicking the standard "save" button then they should
  // stay on the same sub-form.
  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#value'] === t('Save')) {
    return $form_state['storage']['step'];
  }
  // Determine the next step to show based on the steps available to this agent.
  $agent_steps = array_keys(_personalize_campaign_wizard_steps($agent_instance));
  $index = array_search($form_state['storage']['step'], $agent_steps);
  $index++;
  return $index < count($agent_steps) ? $agent_steps[$index] : $agent_steps[$index-1];
}

/**
 * Rebuild the form for the next step.
 *
 * @param array $form_state
 *   The current form state.
 * @param stdClass $agent_instance
 *   The agent instance for the current agent.
 */
function _personalize_campaign_wizard_rebuild(&$form_state, PersonalizeAgentInterface $agent_instance) {
  // If an override was specified, then use it as long as it is valid.
  if (!empty($form_state['values']['override_step'])) {
    $steps = _personalize_campaign_wizard_steps($agent_instance);
    if (!empty($steps[$form_state['values']['override_step']])) {
      $form_state['redirect'] = 'admin/structure/personalize/manage/' . $agent_instance->getMachineName() . '/' . $form_state['values']['override_step'];
      $form_state['rebuild'] = FALSE;
      return;
    }
  }
  // Otherwise check to see if the save button was pressed.
  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#value'] === t('Save')) {
    // Use the redirect action to clear out any "new" item forms.
    // Specify the step to ensure remaining on the current step.
    $form_state['redirect'] = 'admin/structure/personalize/manage/' . $agent_instance->getMachineName() . '/' . $form_state['storage']['step'];
    $form_state['rebuild'] = FALSE;
    return;
  }
  // Default behavior is to just move on to the next step in the wizard.
  $form_state['storage']['step'] = _personalize_campaign_wizard_next_step($form_state, $agent_instance);
  $form_state['rebuild'] = TRUE;
}
