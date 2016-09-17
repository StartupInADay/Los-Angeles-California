<?php

// Plugin definition
$plugin = array(
  // is a 'context' handler type, meaning it supports the API of the
  // context handlers provided by ctools context plugins.
  'handler type' => 'context',
  'visible' => TRUE, // may be added up front.

  // Administrative fields.
  'title' => t('Context Admin'),
  'admin summary' =>'context_admin_menu_context_admin_summary',
  'admin title' => 'context_admin_menu_context_title',
  'admin css' => array(
    context_admin_get_path('css/context_admin.css'),
  ),
  'operations' => array(
    'settings' => array(
      'title' => t('General'),
      'description' => t('Change general settings about this variant.'),
      'form' => 'context_admin_menu_context_edit_settings',
    ),
    'criteria' => array(
      'title' => t('Selection rules'),
      'description' => t('Control the criteria used to decide whether or not this variant is used.'),
      'ajax' => FALSE,
      'form' => array(
        'order' => array(
          'form' => t('Selection rules'),
        ),
        'forms' => array(
          'form' => array(
            'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
            'form id' => 'ctools_context_handler_edit_criteria',
          ),
        ),
      ),
    ),
    'context' => array(
      'title' => t('Contexts'),
      'ajax' => FALSE,
      'description' => t('Add additional context objects to this variant that can be used by the content.'),
      'form' => array(
        'order' => array(
          'form' => t('Context'),
        ),
        'forms' => array(
          'form' => array(
            'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
            'form id' => 'ctools_context_handler_edit_context',
          ),
        ),
      ),
    ),
    'content' => array(
      'title' => t('Content'),
      'description' => t('Select the sort of content this particular administration menu item will contain.'),
      'ajax' => FALSE,
      'form' => array(
        'order' => array(
          'form' => t('Content'),
        ),
        'forms' => array(
          'form' => array(
            'form id' => 'context_admin_menu_context_edit_content',
            'no blocks' => TRUE,
          ),
        ),
      ),
    ),
    'content_settings' => array(
      'title' => t('Content Setting'),
      'ajax' => FALSE,
      'form' => array(
        'order' => array(
          'form' => t('Content Settings'),
        ),
        'forms' => array(
          'form' => array(
            'form id' => 'context_admin_menu_context_edit_content_settings',
            'no blocks' => TRUE,
          ),
        ),
      ),
    ),
  ),

  // Callback to render the data.
  'render' => 'context_admin_menu_context_render',

  // Various callbacks for operations performed on the handler to ensure
  // related data is updated properly.
  'save' => 'context_admin_menu_context_save',
  'delete' => 'context_admin_menu_context_delete',
  'export' => 'context_admin_menu_context_export',
  'clone' => 'context_admin_menu_context_clone',

  'add features' => array(
    'criteria' => t('Selection rules'),
    'context' => t('Contexts'),
  ),

  // Where to go when finished.
  'add finish' => 'content',

  'required forms' => array(
    'content' => t('Content'),
    'content_settings' => t('Content Settings'),
  ),

  'edit forms' => array(
    'settings' => t('General'),
    'criteria' => t('Selection rules'),
    'context' => t('Contexts'),
    'content' => t('Content'),
    'content_settings' => t('Content Settings'),
  ),
  'forms' => array(
    'settings' => array(
      'form id' => 'context_admin_menu_context_edit_settings',
    ),
    'content' => array(
      'form id' => 'context_admin_menu_context_edit_content',
      'no blocks' => TRUE,
    ),
    'content_settings' => array(
      'form id' => 'context_admin_menu_context_edit_content_settings',
    ),
    'context' => array(
      'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
      'form id' => 'ctools_context_handler_edit_context',
    ),
    'criteria' => array(
      'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
      'form id' => 'ctools_context_handler_edit_criteria',
    ),
  ),
  'default conf' => array(
    'title' => t('Contextual Administration'),
    'no_blocks' => FALSE,
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  ),
);

function context_admin_menu_context_title($handler, $task, $subtask) {
  if (isset($handler->conf['title'])) {
    return check_plain($handler->conf['title']);
  }
  else {
    return t('Contextual Administration');
  }
}

function context_admin_menu_context_admin_summary($handler, $task, $subtask, $page, $show_title = TRUE) {
  $task_name = page_manager_make_task_name($task['name'], $subtask['name']);
  $output = '';

  ctools_include('context');
  ctools_include('context-task-handler');

  $plugin = page_manager_get_task_handler($handler->handler);

  // Get the operations
  $operations = page_manager_get_operations($page);

  // Get operations for just this handler.
  $operations = $operations['handlers']['children'][$handler->name]['children']['actions']['children'];
  $args = array('handlers', $handler->name, 'actions');
  $rendered_operations = page_manager_render_operations($page, $operations, array(), array('class' => array('actions')), 'actions', $args);

  $object = ctools_context_handler_get_task_object($task, $subtask, $handler);
  $context = ctools_context_load_contexts($object, TRUE);

  $access = ctools_access_group_summary(!empty($handler->conf['access']) ? $handler->conf['access'] : array(), $context);
  if ($access) {
    $access = t('This contextual administration menu will be visible if @conditions.', array('@conditions' => $access));
  }
  else {
    $access = t('This contextual administration menu will always be visible.');
  }

  $rows = array();

  $type = $handler->type == t('Default') ? t('In code') : $handler->type;
  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('Storage')),
    array('class' => t('page-summary-data'), 'data' => $type),
    array('class' => t('page-summary-operation'), 'data' => ''),
  );

  if (!empty($handler->disabled)) {
    $link = l(t('Enable'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'actions', 'enable')));
    $text = t('Disabled');
  }
  else {
    $link = l(t('Disable'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'actions', 'disable')));
    $text = t('Enabled');
  }

  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('Status')),
    array('class' => t('page-summary-data'), 'data' => $text),
    array('class' => t('page-summary-operation'), 'data' => $link),
  );

  $link = l(t('Edit'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'criteria')));
  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('Selection rule')),
    array('class' => t('page-summary-data'), 'data' => $access),
    array('class' => t('page-summary-operation'), 'data' => $link),
  );

  $content_link = ' [' . l(t('Edit'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'content'))) . ']';

  $info = theme('table', array('header' => array(), 'rows' => $rows, 'attributes' => array('class' => 'page-manager-handler-summary')));

  $contexts = theme('ctools_context_list_no_table', array('object' => $object));
  if (empty($contexts)) {
    $contexts = t('This contextual administration menu has no contexts.');
  }

  $title = $handler->conf['title'];
  if ($title != t('Context Admin')) {
    $title = t('Contex Admin: @title', array('@title' => $title));
  }

  $output .= '<div class="clear-block">';
  if ($show_title) {
    $output .= '<div class="handler-title clear-block">';
    $output .= '<div class="actions handler-actions">' . $rendered_operations['actions'] . '</div>';
    $output .= '<span class="title-label">' . $title . '</span>';
  }

  $output .= '</div>';
  $output .= $info;
  $output .= '</div>';
  return $output;
}

function context_admin_menu_context_edit_settings(&$form, &$form_state) {
  $conf = $form_state['handler']->conf;
  $form['conf']['title'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['title'],
    '#title' => t('Administrative title'),
    '#description' => t('Administrative title of this variant.'),
  );

  $form['conf']['no_blocks'] = array(
    '#type' => 'checkbox',
    '#default_value' => $conf['no_blocks'],
    '#title' => t('Disable Drupal blocks/regions'),
    '#description' => t('Check this to have the page disable all regions displayed in the theme. Note that some themes support this setting better than others. If in doubt, try with stock themes to see.'),
  );

  $form['conf']['css_id'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#default_value' => $conf['css_id'],
    '#title' => t('CSS ID'),
    '#description' => t('The CSS ID to apply to this page'),
  );

  $form['conf']['css'] = array(
    '#type' => 'textarea',
    '#title' => t('CSS code'),
    '#description' => t('Enter well-formed CSS code here; this code will be embedded into the page, and should only be used for minor adjustments; it is usually better to try to put CSS for the page into the theme if possible. This CSS will be filtered for safety so some CSS may not work.'),
    '#default_value' => $conf['css'],
  );
}

function context_admin_menu_context_edit_content($form, &$form_state) {
  ctools_include('context-task-handler');
  if (!isset($form_state['handler_id'])) {
    $handler = $form_state['page']->new_handler;
  }
  else {
    $handler = $form_state['page']->handlers[$form_state['handler_id']];
  }
  $contexts = ctools_context_handler_get_all_contexts($form_state['page']->task, $form_state['page']->subtask, $handler);
  $plugins = context_admin_get_contextual_plugins($contexts);
  
  if (isset($handler->conf['context_admin_options'])) {
    $default =  $handler->conf['context_admin_options'];
  }
  else {
    $default = NULL;
  }

  $options = array();
  foreach ($plugins as $key => $plugin) {
    $options[$key] = $plugin['title'];
  }
  $form['context_admin_options'] = array(
    '#title' => t('Choose a contextual administration type'),
    '#type' => 'radios',
    '#options' => $options,
    '#required' => TRUE,
    '#default_value' => $default,
  );
  return $form;
}

function context_admin_menu_context_edit_content_submit(&$form, &$form_state) {
  $form_state['handler']->conf['context_admin_options'] = $form_state['values']['context_admin_options'];
}

function context_admin_menu_context_edit_content_settings($form, &$form_state) {
  ctools_include('context-task-handler');
  if (!isset($form_state['handler_id'])) {
    $handler = $form_state['page']->new_handler;
  }
  else {
    $handler = $form_state['page']->handlers[$form_state['handler_id']];
  }
  $contexts = ctools_context_handler_get_all_contexts($form_state['page']->task, $form_state['page']->subtask, $handler);
  $plugin = context_admin_get_plugins('context_admin', array('id' => $handler->conf['context_admin_options']));
  $form_state['conf'] = $handler->conf;

  $callback = isset($handler->conf['context_admin_options']) ? $handler->conf['context_admin_options'] : NULL;
  $submitted_context = isset($handler->conf['submitted_context']) ? $handler->conf['submitted_context'] : array();

  if (!empty($plugin['required context']) && is_array($contexts)) {
    $form['context'] = ctools_context_selector($contexts, $plugin['required context'], $submitted_context);
  }
  if (isset($callback) && $callback = ctools_plugin_load_function('context_admin', 'context_admin', $callback, 'content form')) {
    $form = $callback($form, $form_state, $contexts);
  }
  return $form;
}

/**
 * Since each administration type provides its own settings, the submission function depends on
 * the type's submission function to manage the object-cache for the individual items that are
 * pertinant to it.
 */

function context_admin_menu_context_edit_content_settings_submit($form, &$form_state) {
  $plugin = $form_state['handler']->conf['context_admin_options'];

  if ($callback = ctools_plugin_load_function('context_admin', 'context_admin', $plugin, 'content form submit')) {
    $callback($form, $form_state);
  }
  if (isset($form_state['values']['context'])) {
    $form_state['conf']['submitted_context'] = $form_state['values']['context'];
  }
  $form_state['handler']->conf = $form_state['conf'];
}

function context_admin_menu_context_save(&$handler, $update) {
  $plugin = $handler->conf['context_admin_options'];
  if ($callback = ctools_plugin_load_function('context_admin', 'context_admin', $plugin, 'save')) {
    $callback($handler, $update);
  }
}

function context_admin_menu_context_delete(&$handler) {
  $plugin = $handler->conf['context_admin_options'];
  if ($callback = ctools_plugin_load_function('context_admin', 'context_admin', $plugin, 'delete')) {
    $callback($handler);
  }
}

function context_admin_menu_context_clone(&$handler) {
  $plugin = $handler->conf['context_admin_options'];
  if ($callback = ctools_plugin_load_function('context_admin', 'context_admin', $plugin, 'clone')) {
    $callback($handler);
  } 
}

function context_admin_menu_context_export(&$handler, $indent) {
  $plugin = $handler->conf['context_admin_options'];
  if ($callback = ctools_plugin_load_function('context_admin', 'context_admin', $plugin, 'export')) {
    $callback($handler, $indent);
  } 
}

function context_admin_menu_context_render($handler, $base_contexts, $args, $test = TRUE) {
  // Go through arguments and see if they match.
  ctools_include('context');
  ctools_include('context-task-handler');
  // Add my contexts
  $contexts = ctools_context_handler_get_handler_contexts($base_contexts, $handler);
  // Test.
  if ($test && !ctools_context_handler_select($handler, $contexts)) {
    return;
  }

  if (isset($handler->handler)) {
    ctools_context_handler_pre_render($handler, $contexts, $args);
  }

  page_manager_get_current_page(array(
    'name' => $handler->name,
    'task' => $handler->task,
    'subtask' => $handler->subtask,
    'contexts' => $contexts,
    'arguments' => $args,
    'handler' => $handler,
  ));
  $plugin = $handler->conf['context_admin_options'];
  if ($callback = ctools_plugin_load_function('context_admin', 'context_admin', $plugin, 'render')) {
    $info['content'] = $callback($handler, $contexts, $args, $test);
  }
  else {
    $info['content'] = t('An error occurred with your context admin handler.  No callback was defined.');
  }
  return $info;
}

