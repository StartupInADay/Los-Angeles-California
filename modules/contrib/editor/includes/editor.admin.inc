<?php

/**
 * @file
 * Replaces the core Filter module administration pages.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Adds editor support to the text format add/edit form.
 */
function editor_form_filter_admin_format_form_alter(&$form, &$form_state, $form_id) {
  // Retrieve the current format from the build info arguments.
  $format = $form_state['build_info']['args'][0];

  editor_format_ensure_additional_properties($format);

  $is_fallback = ($format->format == filter_fallback_format());
  $editors = editor_get_editors();

  if (isset($form_state['editor_info'])) {
    $editor_info = $form_state['editor_info'];
  }
  else {
    $editor_info = ($format->editor && isset($editors[$format->editor])) ? $editors[$format->editor] : NULL;
  }

  $form_state['format'] = $format;
  $form_state['editor_info'] = $editor_info;
  $form['#validate'] = array();
  $form['#submit'] = array();
  $form['#tree'] = TRUE;
  $form['#attached']['library'][] = array('filter', 'filter.admin');

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $format->name,
    '#required' => TRUE,
    '#weight' => -20,
  );
  $form['format'] = array(
    '#type' => 'machine_name',
    '#required' => TRUE,
    '#default_value' => $format->format,
    '#maxlength' => 255,
    '#disabled' => !empty($format->format),
    '#machine_name' => array(
      'exists' => 'filter_format_exists',
    ),
    '#weight' => -19,
  );

  // Build the list of all available editors.
  $editor_options = array('' => t('None'));

  foreach ($editors as $editor_name => $editor) {
    $editor_options[$editor_name] = $editor['label'];
  }

  // Associate an editor with this format.
  $editor_info = $form_state['editor_info'];

  if ($editor_info) {
    // Load the associated editor callbacks file, if any.
    if (!empty($editor_info['file'])) {
      $filepath = $editor_info['file'];
      $extension = substr($filepath, strrpos($filepath, '.') + 1);
      $filepath = substr($filepath, 0, strrpos($filepath, '.'));
      form_load_include($form_state, $extension, $editor_info['module'], $filepath);
    }
  }

  $form['editor'] = array(
    '#weight' => -9,
  );
  $form['editor']['editor'] = array(
    '#type' => 'select',
    '#title' => t('Text editor'),
    '#options' => $editor_options,
    '#empty_option' => t('None'),
    '#default_value' => $format->editor ? $format->editor : '',
    '#ajax' => array(
      'trigger_as' => array('name' => 'editor_configure'),
      'callback' => 'editor_admin_format_editor_ajax',
      'wrapper' => 'editor-settings-wrapper',
    ),
    '#parents' => array('editor'),
  );
  $form['editor']['configure'] = array(
    '#type' => 'submit',
    '#name' => 'editor_configure',
    '#value' => t('Configure editor'),
    '#limit_validation_errors' => array(array('editor')),
    '#submit' => array('editor_admin_format_editor_submit'),
    '#ajax' => array(
      'callback' => 'editor_admin_format_editor_ajax',
      'wrapper' => 'editor-settings-wrapper',
    ),
    '#attributes' => array('class' => array('js-hide')),
    '#parents' => array('editor_configure'),
  );

  // If there aren't any options (other than "None"), disable the select list.
  if (empty($editor_options)) {
    $form['editor']['editor']['#disabled'] = TRUE;
    $form['editor']['editor']['#description'] = t('This option is disabled because no modules that provide a text editor are currently enabled.');
  }

  $form['editor_settings'] = array(
    '#tree' => TRUE,
    '#weight' => -8,
    '#type' => 'container',
    '#id' => 'editor-settings-wrapper',
  );

  // Populate editor defaults.
  if (!empty($editor_info['default settings'])) {
    $format->editor_settings += $editor_info['default settings'];
  }

  // Add editor-specific validation and submit handlers.
  if (!empty($editor_info['settings callback'])) {
    $function = $editor_info['settings callback'];
    $form['editor_settings'] = array_merge($function($form, $form_state, $format), $form['editor_settings']);
    $form['editor_settings']['#parents'] = array('editor_settings');
  }

  // Add user role access selection.
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#options' => array_map('check_plain', user_roles()),
    '#disabled' => $is_fallback,
  );

  if ($is_fallback) {
    $form['roles']['#description'] = t('All roles for this text format must be enabled and cannot be changed.');
  }

  if (!empty($format->format)) {
    // If editing an existing text format, pre-select its current permissions.
    $form['roles']['#default_value'] = array_keys(filter_get_roles_by_format($format));
  }
  elseif ($admin_role = variable_get('user_admin_role', 0)) {
    // If adding a new text format and the site has an administrative role,
    // pre-select that role so as to grant administrators access to the new
    // text format permission by default.
    $form['roles']['#default_value'] = array($admin_role);
  }

  // Retrieve available filters and load all configured filters for existing
  // text formats.
  $all_filter_info = filter_get_filters();
  $filters = !empty($format->format) ? filter_list_format($format->format) : array();

  // Prepare filters for form sections.
  foreach ($all_filter_info as $name => $filter_info) {
    // Create an empty filter object for new/unconfigured filters.
    if (!isset($filters[$name])) {
      $filters[$name] = (object) array(
        'format' => $format->format,
        'module' => $filter_info['module'],
        'name' => $name,
        'status' => 0,
        'weight' => $filter_info['weight'],
        'settings' => isset($filter_info['default settings']) ? $filter_info['default settings'] : array(),
      );
    }
  }

  $form['#filters'] = $filters;

  // Filter status.
  $form['filters']['status'] = array(
    '#type' => 'item',
    '#title' => t('Enabled filters'),
    '#prefix' => '<div id="filters-status-wrapper">',
    '#suffix' => '</div>',
  );

  foreach ($all_filter_info as $name => $filter_info) {
    $form['filters']['status'][$name] = array(
      '#type' => 'checkbox',
      '#title' => $filter_info['title'],
      '#default_value' => $filters[$name]->status,
      '#parents' => array('filters', $name, 'status'),
      '#description' => $filter_info['description'],
      '#weight' => $filter_info['weight'],
    );
  }

  // Filter order (tabledrag).
  $form['filters']['order'] = array(
    '#type' => 'item',
    '#title' => t('Filter processing order'),
    '#theme' => 'filter_admin_format_filter_order',
  );

  foreach ($all_filter_info as $name => $filter_info) {
    $form['filters']['order'][$name]['filter'] = array(
      '#markup' => $filter_info['title'],
    );
    $form['filters']['order'][$name]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $filter_info['title'])),
      '#title_display' => 'invisible',
      '#delta' => 50,
      '#default_value' => $filters[$name]->weight,
      '#parents' => array('filters', $name, 'weight'),
    );
    $form['filters']['order'][$name]['#weight'] = $filters[$name]->weight;
  }

  // Filter settings.
  $form['filter_settings_title'] = array(
    '#type' => 'item',
    '#title' => t('Filter settings'),
  );
  $form['filter_settings'] = array(
    '#type' => 'vertical_tabs',
  );

  foreach ($all_filter_info as $name => $filter) {
    if (isset($filter['settings callback']) && function_exists($filter['settings callback'])) {
      $function = $filter['settings callback'];

      // Pass along stored filter settings and default settings, but also the
      // format object and all filters to allow for complex implementations.
      $defaults = (isset($filter['default settings']) ? $filter['default settings'] : array());
      $settings_form = $function($form, $form_state, $filters[$name], $format, $defaults, $filters);

      if (!empty($settings_form)) {
        $form['filters']['settings'][$name] = array(
          '#type' => 'fieldset',
          '#title' => $filter['title'],
          '#parents' => array('filters', $name, 'settings'),
          '#weight' => $filter['weight'],
          '#group' => 'filter_settings',
        );
        $form['filters']['settings'][$name] += $settings_form;
      }
    }
  }

  $form['#validate'][] = 'editor_admin_format_form_validate';
  $form['#submit'][] = 'editor_admin_format_form_submit';

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
}

/**
 * A copy of filter_admin_format_form_validate() which adds support for editors.
 *
 * @see editor_admin_format_form_submit()
 * @see filter_admin_format_form_validate()
 */
function editor_admin_format_form_validate($form, &$form_state) {
  $format_format = trim($form_state['values']['format']);
  $format_name = trim($form_state['values']['name']);

  // Ensure that the values to be saved later are exactly the ones validated.
  form_set_value($form['format'], $format_format, $form_state);
  form_set_value($form['name'], $format_name, $form_state);

  $result = db_query("SELECT format FROM {filter_format} WHERE name = :name AND format <> :format", array(':name' => $format_name, ':format' => $format_format))->fetchField();
  if ($result) {
    form_set_error('name', t('Text format names must be unique. A format named %name already exists.', array('%name' => $format_name)));
  }
}

/**
 * AJAX callback for updating the editor settings elements().
 */
function editor_admin_format_editor_ajax($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace('#editor-settings-wrapper', drupal_render($form['editor_settings']));

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Element submission handler for configuring an editor.
 *
 * Sets the editor_info property so that the form can be AJAX-updated.
 */
function editor_admin_format_editor_submit($form, &$form_state) {
  $editor_name = $form_state['values']['editor'];
  $editors = editor_get_editors();

  if ($editor_name && isset($editors[$editor_name])) {
    $form_state['editor_info'] = $editors[$editor_name];
  }
  else {
    $form_state['editor_info'] = array();
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * A copy of filter_admin_format_form_submit() which adds support for editors.
 *
 * @see editor_admin_format_form_validate()
 * @see filter_admin_format_form_submit()
 */
function editor_admin_format_form_submit($form, &$form_state) {
  // Remove unnecessary values.
  form_state_values_clean($form_state);

  unset($form_state['values']['filter_settings']);
  unset($form_state['values']['actions']);

  // Add the submitted form values to the text format, and save it.
  $format = $form_state['format'];

  foreach ($form_state['values'] as $key => $value) {
    $format->$key = $value;
  }

  // If not saving an editor, do not save any settings.
  if (!$format->editor) {
    $format->editor_settings = array();
  }

  $status = filter_format_save($format);

  // Save user permissions.
  if ($permission = filter_permission_name($format)) {
    foreach ($format->roles as $rid => $enabled) {
      user_role_change_permissions($rid, array($permission => $enabled));
    }
  }

  switch ($status) {
    case SAVED_NEW:
      drupal_set_message(t('Added text format %format.', array('%format' => $format->name)));
      break;

    case SAVED_UPDATED:
      drupal_set_message(t('Updated text format %format.', array('%format' => $format->name)));
      break;
  }

  $form_state['redirect'] = 'admin/config/content/formats';
}
