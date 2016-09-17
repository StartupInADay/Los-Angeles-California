<?php

/**
 * @file personalize_elements.admin.inc
 * Provides functions needed for the admin UI.
 */


/**
 * Menu callback for the element edit page.
 *
 * @param $element
 *   The element to edit
 *
 * @return array|mixed
 *   The element edit form.
 */
function personalize_elements_edit($element) {
  if (!$element) {
    drupal_not_found();
    return NULL;
  }
  return drupal_get_form('personalize_elements_form', NULL, FALSE, $element);
}

/**
 * Menu callback for displaying a list of all existing custom elements.
 */
function personalize_elements_list() {
  $header = array(
    array('data' => t('Variation set name')),
    array('data' => t('Operations'), 'colspan' => 2),
  );
  $rows = array();
  foreach (personalize_option_set_load_by_type('elements') as $option_set) {
    $tablerow = array(
      array('data' => check_plain($option_set->label)),
      array('data' => l(t('Edit'), 'admin/structure/personalize/variations/personalize-elements/manage/'. $option_set->osid . '/edit')),
      array('data' => l(t('Delete'), 'admin/structure/personalize/variations/personalize-elements/manage/'. $option_set->osid . '/delete')),
    );
    $rows[] = $tablerow;
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No elements available.'), 'colspan' => 3));
  }
  $build = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'personalize_elements'),
  );
  return $build;
}

/**
 * Form for creating a new action.
 */
function personalize_elements_form($form, &$form_state, $isAjax = FALSE, $option_set = NULL) {
  global $base_url;

  $path = drupal_get_path('module', 'personalize_elements');
  $form = array(
    '#attached' => array(
      'css' => array(
        $path . '/css/personalize_elements.admin.css',
        $path . '/css/personalize_elements.admin.theme.css',
       ),
      'js' => array(
        $path . '/js/personalize_elements.admin.js',
      ),
    ),
  );

  if (!$option_set) {
    $option_set = new stdClass();
  }
  $options = array();
  if (isset($form_state['options'])) {
    $options = $form_state['options'];
  }
  elseif (isset($option_set->options)) {
    foreach ($option_set->options as $option) {
      $options[] = array(
        'option_id' => $option['option_id'],
        'option_label' => $option['option_label'],
        'personalize_elements_content' => isset($option['personalize_elements_content']) ? $option['personalize_elements_content'] : ''
      );
    }
  }
  if (!empty($option_set->osid)) {
    $form['osid'] = array(
      '#type' => 'value',
      '#value' => $option_set->osid,
    );
    $form['agent_select'] = array(
      '#type' => 'value',
      '#value' => $option_set->agent,
    );
  }
  else {
    $current_agent = isset($option_set->agent) ? $option_set->agent : '';
    $form += personalize_get_agent_selection_form($current_agent);
  }

  $form['title'] = array(
    '#title' => t('Variation set name'),
    '#description' => t('The administrative title of this content variation set.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($option_set->label) ? $option_set->label : '',
  );

  $variation_type_options = array();
  $selector_states = array();
  $variation_types = module_invoke_all('personalize_elements_variation_types', TRUE, FALSE);
  foreach ($variation_types as $type => $info) {
    // Show only active types unless it is the current type of an existing
    // option set.
    if ($info['active'] || (!empty($option_set->data['personalize_elements_type']) && $option_set->data['personalize_elements_type'] === $type)) {
      $variation_type_options[$type] = $info['label'];
      if ($info['needs_selector']) {
        $selector_states[] = array('select[name="variation_type"]' => array("value" => $type));
      }
    }
  }

  $form['variation_type'] = array(
    '#title' => t('Operation'),
    '#type' => 'select',
    '#options' => array('' => t('-- Select operation --')) + $variation_type_options,
    '#default_value' => isset($option_set->data['personalize_elements_type']) ? $option_set->data['personalize_elements_type'] : '',
  );

  $form['details'] = array(
    '#type' => 'container',
    '#tree' => FALSE,
    '#states' => array(
      'invisible' => array(
        ':input[name="variation_type"]' => array('value' => ''),
      ),
    ),
  );

  if (!empty($selector_states)) {
    $form['details']['selector'] = array(
      '#prefix' => '<div class="form-item-help-link">',
      '#suffix' => '</div>',
      '#title' => t('Element') . ' ' . '<span class="secondary">' . t('(selector)' . '</span>'),
      '#field_prefix' => l(t('What should I put here?'), PERSONALIZE_ELEMENTS_HELP_URL_SELECTOR, array('attributes' => array('target' => '_blank'))),
      '#description' => t('Enter the selector that identifies the element or elements that you want to personalize'),
      '#type' => 'textfield',
      '#default_value' => isset($option_set->data['personalize_elements_selector']) ? $option_set->data['personalize_elements_selector'] : '',
    );
    $form['details']['selector']['#states'] = array(
      'visible' => $selector_states,
      'required' => $selector_states,
    );
  }

  // If the "Add another" button was clicked, we need to increment the number of
  // options by one.
  $num_options = count($options);
  if (empty($options) || (isset($form_state['num_options']) && $form_state['num_options'] > $num_options)) {
    $index = isset($options[0]['option_label']) && $options[0]['option_label'] == PERSONALIZE_CONTROL_OPTION_LABEL ? $num_options - 1 : $num_options;
    $options[] = array(
      'option_label' => personalize_generate_option_label($index),
      'content' => '',
      'is_new' => TRUE,
      'option_id' => '',
    );
  }
  $show_control = TRUE;
  // If we are currently in a form process where the user has deselected the
  // add control checkbox, then don't show it.
  if (isset($form_state['values']) && empty($form_state['values']['add_control_option'])) {
    $show_control = FALSE;
  }
  // If this is an edit form and there is no control in the options loaded,
  // then don't show the control.
  else if (isset($option_set->osid) && $options[0]['option_label'] !== PERSONALIZE_CONTROL_OPTION_LABEL) {
    $show_control = FALSE;
  }
  // If the first option is not already the control, and then add a control
  // option.  This option is always available to show on the display in response
  // to the add control option checkbox.
  if (count($options) == 0 || $options[0]['option_label'] !== PERSONALIZE_CONTROL_OPTION_LABEL) {
    array_unshift($options, array(
      'option_label' => PERSONALIZE_CONTROL_OPTION_LABEL,
      'personalize_elements_content' => '',
      'option_id' => PERSONALIZE_CONTROL_OPTION_ID,
    ));
  }
  $form_state['num_options'] = count($options);
  $form_state['options'] = $options;

  // Add a wrapper for the options and Add another option button.
  $form['details']['options_wrapper'] = array(
    '#tree' => FALSE,
    '#title' => t('Options'),
  );

  $form['details']['options_wrapper']['options'] = array(
    '#tree' => TRUE,
    '#theme_wrappers' => array('container'),
    '#attributes' => array('id' => 'personalize-elements-options'),
  );

  $form['details']['options_wrapper']['options_more'] = array(
    '#type' => 'submit',
    '#prefix' => '<span class="personalize-elements-add-link-prefix"></span>',
    '#value' => t('Add variation'),
    '#attributes' => array(
      'class' => array('personalize-add-link'),
      'title' => t('Click here to add more goals.')
    ),
    '#attributes' => array(
      'class' => array('personalize-elements-add-option'),
      'title' => t('Click here to add more variations.')
    ),
    '#weight' => 1,
    '#submit' => array('personalize_elements_add_option_submit'),
    '#ajax' => array(
      'callback' => 'personalize_elements_add_option_ajax_callback',
      'wrapper' => 'personalize-elements-options',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(),
  );

  // Add current variations to the form.
  foreach ($options as $delta => $option) {
    $option_form = array(
      '#tree' => TRUE,
      '#type' => 'container',
    );
    $option_form['option_id'] = array(
      '#type' => 'value',
      '#value' => $option['option_id'],
    );
    $option_form['option_label'] = array(
      '#prefix' => '<div class="personalize-elements-option-label-element">',
      '#suffix' => '</div>',
      '#type' => 'textfield',
      '#title' => t('Variation label'),
      '#size' => 20,
      '#default_value' => !empty($option['option_label']) ? check_plain($option['option_label']) : '',
    );
    $option_form['personalize_elements_content'] = array(
      '#prefix' => '<div class="personalize-elements-option-content-element">',
      '#suffix' => '</div>',
      '#type' => 'textarea',
      '#rows' => 2,
      '#title' => t('Content'),
      '#title_display' => 'invisible',
      '#default_value' => !empty($option['personalize_elements_content']) ? $option['personalize_elements_content'] : '',
    );
    // Add markup to differentiate a control.
    if ($option['option_label'] === PERSONALIZE_CONTROL_OPTION_LABEL) {
      // If the form is re-submitted via Ajax (e.g., Add another) then the
      // control option will appear even if the add checkbox has been unchecked.
      // Adding the "element-hidden" class will hide the element, but still allow
      // it to show in response to state changes which use jQuery show()/hide().
      if (!$show_control) {
        $option_form['#attributes']['class'][] = 'element-hidden';
      }
      $option_form['#attributes']['class'][] = 'personalize-elements-control';
      $option_form['option_label']['#attributes']['disabled'] = 'disabled';
      $option_form['personalize_elements_content']['#attributes']['disabled'] = 'disabled';
      $option_form['personalize_elements_content']['#default_value'] = t('Display original content without personalization.');
      $option_form['#states'] = array(
        'visible' => array(
          ':input[name="add_control_option"]' => array('checked' => TRUE),
        ),
      );
    }
    // Add markup to differentiate newly created options.
    if (empty($option_set->osid) || (isset($option['is_new']) && $option['is_new'] === TRUE)) {
      $option_form['option_label']['#attributes']['class'][] = 'personalize-elements-add-option-label';
      $option_form['personalize_elements_content']['#attributes']['class'][] = 'personalize-elements-add-content';
    }
    $form['details']['options_wrapper']['options'][$delta] = $option_form;
  }

  $form['details']['pages_all'] = array(
    '#type' => 'radios',
    '#title' => t('Execute on'),
    '#options' => array(
      '0' => t('Selected pages'),
      '1' => t('All pages'),
    ),
    '#default_value' => isset($option_set->data['pages']) && empty($option_set->data['pages']) ? 1 : 0,
  );

  $form['details']['pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#title_display' => 'invisible',
    '#field_prefix' => $base_url . base_path(),
    '#default_value' => isset($_GET['path']) ? urldecode($_GET['path']) : (isset($option_set->data['pages']) ? $option_set->data['pages'] : ''),
    '#description' => t("Specify pages by using one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page. If the URL's language prefix is skipped, all languages of the same URL will be selected.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#states' => array(
      'disabled' => array(
        ':input[name="pages_all"]' => array('value' => '1'),
      )
    )
  );

  $form['details']['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
  );

  $form['details']['advanced']['add_control_option'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use a control variation based on your website's original content."),
    '#default_value' => $show_control,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit_form'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#states' => array(
      'disabled' => array(
        ':input[name="variation_type"]' => array('value' => ''),
      ),
    ),
  );

  $form_state['isAjax'] = $isAjax;

  return $form;
}

/**
 * Submit handler for the "Add another" button.
 */
function personalize_elements_add_option_submit($form, &$form_state) {
  // Increment the number of option element sets to be rendered.
  $form_state['num_options']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for the "Add another" button.
 */
function personalize_elements_add_option_ajax_callback($form, &$form_state) {
  return $form['details']['options_wrapper']['options'];
}

/**
 * Validation callback for the element creation/edit form.
 */
function personalize_elements_form_validate($form, &$form_state) {
  $values = &$form_state['values'];
  if ($values['op'] == t('Save')) {
    if (empty($values['variation_type'])) {
      form_set_error('variation_type', t('You must choose an operation'));
    }
    $needs_control_option = isset($values['add_control_option']) && $values['add_control_option'];
    $has_control_option = $values['options'][0]['option_label'] == PERSONALIZE_CONTROL_OPTION_LABEL;
    // If the option set needs a control option and doesn't yet have one, then
    // one will be added on submit so only 1 additional option is required,
    // otherwise a minimum of 2 options must be supplied.
    $num_required = 2;
    if ($needs_control_option && !$has_control_option) {
      $num_required = 1;
    }
    // If an option has an empty label and empty content, it will be removed so
    // we need to count how many non-empty options there are.
    $non_empty_options = 0;

    $changed = 0;
    // Remove empty options.
    foreach ($values['options'] as $i => &$option) {
      // First, filter out any disallowed html tags.
      $filtered = personalize_elements_filter_personalize_element_option($values['variation_type'], $option['personalize_elements_content']);
      if ($filtered !== $option['personalize_elements_content']) {
        $changed++;
      }
      $option['personalize_elements_content'] = $filtered;
      if ($option['option_label'] == PERSONALIZE_CONTROL_OPTION_LABEL) {
        if ($needs_control_option) {
          $non_empty_options++;
        }
        continue;
      }
      if (!empty($option['personalize_elements_content'])) {
        $non_empty_options++;
        if (empty($option['option_label'])) {
          form_set_error('options[' . $i . '][option_label', t('Please enter a label for each option'));
        }
      }
    }
    if ($non_empty_options < $num_required) {
      form_set_error('', t('You must have at least 2 variations for a content variation set.'));
    }
    if ($changed) {
      drupal_set_message(t('Disallowed HTML tags were removed from @variations.', array('@variations' => format_plural($changed, '1 variation', '@count variations', array('@count' => $changed)))), 'warning');
    }
    if ($values['pages_all'] == 0) {
      $form['details']['pages']['#allow_external'] = FALSE;
      $form['details']['pages']['#allow_dynamic'] = TRUE;
      visitor_actions_form_element_path_validate($form['details']['pages'], $form_state);
    }

    if (!personalize_elements_selector_validate($form_state['values']['selector'])) {
      form_set_error('selector', t('The selector cannot contain tags.'));
    }

  }
}

/**
 * Submit handler for the element creation/edit form.
 */
function personalize_elements_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  if ($op == t('Save')) {
    $option_set = personalize_elements_convert_form_values_to_option_set($form_state['values']);
    // @todo This section should really be the responsibility of personalize module, figure
    //   out a way to not have to add this to every submit callback of a form that uses
    //   the embedded campaign creation form.
    if ($form_state['values']['agent_select'] == PERSONALIZE_NEW_AGENT_FORM_VALUE) {
      if ($new_agent = personalize_save_agent_from_form_values($form_state['values'])) {
        $option_set->agent = $new_agent->machine_name;
      }
      else {
        drupal_set_message('There was a problem saving the new personalization', 'error');
        return;
      }
    }
    personalize_option_set_save($option_set);
    drupal_set_message(t('The variation set was saved.'));
    $form_state['redirect'] = 'admin/structure/personalize/manage/' . $option_set->agent . '/variations';
  }
}

/**
 * Ajax callback for the "Add another" button.
 */
function personalize_elements_option_ajax_callback($form, &$form_state) {
  return $form['option_wrapper']['options'];
}

/**
 * Submit handler for the "Add another" button.
 */
function personalize_elements_option_more_submit($form, &$form_state) {
  // Increment the number of option element sets to be rendered.
  $form_state['num_options']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Converts the values submitted in the form to an element array.
 *
 * @param $values
 *   The form values.
 * @return array
 *   An array that can be saved as an element to the db.
 */
function personalize_elements_convert_form_values_to_option_set($values) {
  $options = $values['options'];
  // Remove empty options.
  foreach ($options as $i => $option) {
    if (empty($option['personalize_elements_content'])) {
      unset($options[$i]);
    }
  }
  // Re-key the array in case some items were removed.
  $options = array_values($options);
  if ($options[0]['option_label'] === PERSONALIZE_CONTROL_OPTION_LABEL) {
    // This will get added back on if needed, after the other options have
    // been processed.
    $control_option = array_shift($options);
    // Reset the content to empty.
    $control_option['personalize_elements_content'] = '';
  }
  if (isset($values['add_control_option']) && $values['add_control_option']) {
    if (!isset($control_option)) {
      $control_option = array('option_label' => PERSONALIZE_CONTROL_OPTION_LABEL, 'option_id' => PERSONALIZE_CONTROL_OPTION_ID, 'personalize_elements_content' => '');
    }
    // Auto-generation of option IDs needs to happen *before* we add the control
    // option as it should not affect the numbering scheme.
    $options = personalize_ensure_unique_option_ids($options);
    // Now add (back) the control option.
    array_unshift($options, $control_option);
  }

  $option_set = array(
    'label' => $values['title'],
    'plugin' => 'elements',
    'agent' => $values['agent_select'],
    'executor' => 'personalizeElements',
    'options' => $options,
    'data' => array(
      'personalize_elements_selector' => isset($values['selector']) ? $values['selector'] : '',
      'personalize_elements_type' => $values['variation_type'],
      'pages' => isset($values['pages_all']) && $values['pages_all'] == 1 ? '' : $values['pages'],
    )
  );
  if (isset($values['osid'])) {
    $option_set['osid'] = $values['osid'];
  }
  return (object) $option_set;
}

/**
 * Form for deleting a personalized element.
 */
function personalize_elements_element_delete($form, $form_state, $option_set) {
  $form['osid'] = array('#type' => 'hidden', '#value' => $option_set->osid);
  $form['title'] = array('#type' => 'hidden', '#value' => $option_set->label);
  $form['agent'] = array('#type' => 'value', '#value' => $option_set->agent);
  return confirm_form($form, t('Are you sure you want to delete the element %title?', array('%title' => $option_set->label)), 'admin/structure/personalize/manage/' . $option_set->agent . '/variations', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for personalized element deletion form.
 */
function personalize_elements_element_delete_submit($form, &$form_state) {
  personalize_option_set_delete($form_state['values']['osid']);
  drupal_set_message(t('The personalized element %name has been removed.', array('%name' => $form_state['values']['title'])));
  $form_state['redirect'] = 'admin/structure/personalize/manage/' . $form_state['values']['agent'] . '/variations';
}

/**
 * Form for deleting an option within a personalized element.
 */
function personalize_elements_option_delete($form, $form_state, $option_set, $option_id) {
  $option_label = $option_id;
  foreach($option_set->options as $option) {
    if ($option['option_id'] === $option_id) {
      $option_label = $option['option_label'];
      break;
    }
  }
  $form['osid'] = array('#type' => 'hidden', '#value' => $option_set->osid);
  $form['option_id'] = array('#type' => 'hidden', '#value' => $option_id);
  $form['title'] = array('#type' => 'hidden', '#value' => $option_set->label);
  $form['option_label'] = array('#type' => 'hidden', '#value' => $option_label);
  return confirm_form($form, t('Are you sure you want to delete variation %option_label from the %title personalized element?', array('%option_label' => $option_label, '%title' => $option_set->label)), 'admin/structure/personalize/variations/personalize-blocks/manage/' . $option_set->osid, '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for personalize_elements_option_delete().
 */
function personalize_elements_option_delete_submit($form, &$form_state) {
  $option_set = personalize_option_set_load($form_state['values']['osid']);
  foreach($option_set->options as $i => $option) {
    if ($option['option_id'] === $form_state['values']['option_id']) {
      unset($option_set->options[$i]);
      break;
    }
  }
  personalize_option_set_save($option_set);
  drupal_set_message(t('The variation %option_label has been removed from the %title personalized element.', array('%option_label' => $form_state['values']['option_label'], '%title' => $form_state['values']['title'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/personalize/manage/' . $option_set->agent . '/variations';
}

/**
 * Menu callback for the configuration form.
 */
function personalize_elements_configuration_form($form, &$form_state) {
  $form['personalize_elements_allowed_html'] = array(
    '#title' => t('Allowed HTML tags'),
    '#description' => t('Some Personalize Elements operations involve replacing existing HTML with new HTML, or appending/prepending HTML. Tags other than those included here will be filtered out.'),
    '#type' => 'textarea',
    '#default_value' => variable_get('personalize_elements_allowed_html', _personalize_elements_get_default_allowed_tags()),
  );
  return system_settings_form($form);
}

/**
 * Returns the HTML tags that are enabled by default.
 */
function _personalize_elements_get_default_allowed_tags() {
  return '<p> <div> <img> <a> <br> <caption> <col> <em> <strong> <cite> <blockquote> <code> <ul> <ol> <li> <dl> <dt> <dd> <h1> <h2> <h3> <h4> <h5> <h6> <pre> <span> <table> <tbody> <th> <tr>';
}

/**
 * Filters out any invalid content from a personalize elements option.
 *
 * @param string $variation_type
 *   The type of variation for the option's option set.
 * @param string $option_content
 *   The user-entered option content to be filtered.
 * @return string
 *   The filtered option content.
 */
function personalize_elements_filter_personalize_element_option($variation_type, $option_content) {
  if (user_access('use advanced personalize elements features')) {
    return $option_content;
  }
  $allowed_html = variable_get('personalize_elements_allowed_html', _personalize_elements_get_default_allowed_tags());
  $allowed_tags = preg_split('/\s+|<|>/', $allowed_html, -1, PREG_SPLIT_NO_EMPTY);

  $filtered = $option_content;
    if ($variation_type !== 'runJS') {
      $filtered = personalize_sanitize_string($filtered, $allowed_tags);
    }
  return $filtered;
}

/**
 * Validate the selector for a personalized element.
 *
 * Validating a CSS selector is generally beyond what REGEX can handle,
 * and using the Drupal cleaning scripts all update < > to entities
 * so just make sure that there are no tags.
 *
 * @param string $selector
 *   The selector string to validate
 * @return boolean
 *   True if the selector validates successfully, false if error.
 */
function personalize_elements_selector_validate($selector) {
  $re = "/<\\/?[a-z][a-z0-9]*[^<>]*>|<!--.*?-->/im";
  $matches = array();
  $matched = preg_match_all($re, $selector, $matches);
  return $matched == 0;
}
