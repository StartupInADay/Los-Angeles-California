<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
if (module_exists('node_reference')) {
  $plugin = array(
    'title' => t('Create Node with auto node reference'),
    'description' => t('Creates a node with an automatic reference back to its parent.'),
    'required context' => new ctools_context_required(t('Node'), 'node'),
    'content form' => 'context_admin_noderef_create_node_content_form',
    'content form submit' => 'context_admin_noderef_create_node_content_form_submit',
    'render' => 'context_admin_noderef_create_node_render_page',
    'form alter' => 'context_admin_noderef_create_node_form_alter',
  );
}

function context_admin_noderef_create_node_content_form($form, &$form_state, $contexts = array()) {
  $conf = $form_state['conf'];
  ctools_include('dependent');
  if (isset($conf['context_admin_options_items'])) {
    $default = $conf['context_admin_options_items'];
  }
  else {
    $default = NULL;
  }
  if (isset($conf['context_admin_content_types'])) {
    $type_fields = $conf['context_admin_content_types'];
  }
  else {
    $type_fields = NULL;
  }
  if (isset($conf['context_admin_autoforward'])) {
    $forward = $conf['context_admin_autoforward'];
  }
  else {
    $forward = NULL;
  }
  if (isset($conf['context_admin_custom_redirect'])) {
    $custom_redirect = $conf['context_admin_custom_redirect'];
  }
  else {
    $custom_redirect = NULL;
  }
  $node_entities = entity_get_info('node');
  $types = field_info_instances('node');
  $options = array();
  $fields = array();
  if ($types) {
    foreach ($types as $type => $field_instances) {
      foreach ($field_instances as $field_name => $field) {
        $field_data = field_read_field($field_name);
        if ($field_data['type'] == 'node_reference') {
          $fields[$type][$field_name] = $field['label'];
          $options[$type] = $node_entities['bundles'][$type]['label'];
        }
      }
    }
  }
  if ($options) {
    $form['context_admin'] = array(
      '#type' => 'fieldset',
      '#title' => t('Node Creation/Reference Options'),
      '#tree' => TRUE,
    );
    $form['context_admin']['context_admin_options_items'] = array(
      '#type' => 'radios',
      '#title' => t('Select the node type you would like to create'),
      '#required' => TRUE,
      '#options' => $options,
      '#default_value' => $default,
    );
    foreach ($fields as $key => $field_group) {
      $form['context_admin']['content_types'][$key] = array(
        '#type' => 'radios',
        '#title' => t('Available Reference Fields'),
        '#description' => t('Choose a reference field from the available fields'),
        '#options' => $field_group,
        '#process' => array('ctools_dependent_process', 'form_process_radios'),
        '#dependency' => array('radio:context_admin[context_admin_options_items]' => array($key)),
        '#prefix' => '<div id="edit-context-admin-content-types-'. str_replace('_', '-', $key) .'-wrapper"><div>',
        '#suffix' => '</div></div>',
        '#default_value' => isset($type_fields[$key]) ? $type_fields[$key] : NULL,
      );
    }
    $form['context_admin_autoforward'] = array(
      '#type' => 'checkbox',
      '#title' => t('Forward the user back to the node they were on before they created this node.'),
      '#default_value' => $forward,
    );

    $form['context_admin_custom_redirect'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom redirect path'),
      '#default_value' => $custom_redirect,
      '#description' => t('Define a custom path to redirect to after the node creation. This path will be translated with the node tokens of the parent node. Note: This overrides auto forwarding back to the original node.'),
    );
    $rows = array();
    foreach ($contexts as $context) {
      foreach (ctools_context_get_converters('%' . check_plain($context->keyword) . ':', $context) as $keyword => $title) {
        $rows[] = array(
          check_plain($keyword),
          t('@identifier: @title', array('@title' => $title, '@identifier' => $context->identifier)),
        );
      }
    }

    $header = array(t('Keyword'), t('Value'));
    $form['display_title']['contexts'] = array(
      '#type' => 'fieldset',
      '#title' => t('Substitutions'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#value' => theme('table', array('header' => $header, 'rows' => $rows)),
    );
  }
  else {
    drupal_set_message('There are no node reference fields setup on any existing node types. Please add a node reference field to a node type and try again.', 'error');
  }

  if (module_exists('panels')) {
    if (isset($conf['context_admin_use_node_edit'])) {
      $node_edit = $conf['context_admin_use_node_edit'];
    }
    else {
      $node_edit = NULL;
    }

    $form['context_admin_use_node_edit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Render the Add form using panels and page manager'),
      '#description' => t('Enable node/%node/edit in page manager. Unchecked renders Add form with Drupal\'s node templating system. This will prevent redirects from working.'),
      '#default_value' => $node_edit,
    );
  }

  return $form;
}

function context_admin_noderef_create_node_content_form_submit($form, &$form_state) {
  if (isset($form_state['values']['context_admin'])) {
    $form_state['conf']['context_admin_options_items'] = $form_state['values']['context_admin']['context_admin_options_items'];
    unset($form_state['conf']['context_admin_content_types']);
    $form_state['conf']['context_admin_content_types'][$form_state['conf']['context_admin_options_items']] = $form_state['values']['context_admin']['content_types'][$form_state['conf']['context_admin_options_items']];
    $form_state['conf']['context_admin_field'] = $form_state['values']['context_admin']['content_types'][$form_state['conf']['context_admin_options_items']];
    $form_state['conf']['context_admin_autoforward'] = $form_state['values']['context_admin_autoforward'];
    $form_state['conf']['context_admin_custom_redirect'] = $form_state['values']['context_admin_custom_redirect'];
  }
  if (isset($form_state['values']['context_admin_use_node_edit'])) {
    $form_state['conf']['context_admin_use_node_edit'] = $form_state['values']['context_admin_use_node_edit'];
  }
}

function context_admin_noderef_create_node_render_page($handler, $base_contexts, $args, $test = TRUE) {
  module_load_include('inc', 'node', 'node.pages');
  $type = $handler->conf['context_admin_options_items'];
  $fields = array(
    $handler->conf['context_admin_field'] => array(
      'language' => LANGUAGE_NONE,
      'key' => 'nid',
      'values' => array(
        $base_contexts[$handler->conf['submitted_context']]->data->nid
      ),
    ),
  );
  $use_panels = FALSE;
  if (isset($handler->conf['context_admin_use_node_edit'])) {
    $use_panels = $handler->conf['context_admin_use_node_edit'];
  }

  return context_admin_node_add_wrapper($type, $fields, $use_panels);
}

function context_admin_noderef_create_node_form_alter(&$form, &$form_state, $form_id, $page) {
  switch($form_id) {
    case 'context_admin_node_form_wrapper':
      if ($form['#node']->type == $page['handler']->conf['context_admin_options_items']) {
        $form[$page['handler']->conf['context_admin_field']]['#access'] = FALSE;
        $form[$page['handler']->conf['context_admin_field']]['und']['#required'] = FALSE;

        if ($page['handler']->conf['context_admin_autoforward'] || $page['handler']->conf['context_admin_custom_redirect']) {
          $form['page_context'] = array(
            '#type' => 'value',
            '#value' => array(
              'contexts' => $page['contexts'],
              'submitted_context' => $page['handler']->conf['submitted_context'],
            ),
          );

          if ($page['handler']->conf['context_admin_custom_redirect']) {
            $form['context_admin_custom_redirect'] = array(
              '#type' => 'value',
              '#value' => $page['handler']->conf['context_admin_custom_redirect'],
            );
          }

          $form['actions']['submit']['#submit'][] = 'context_admin_noderef_create_node_submit';
        }
      }
      break;
  }
}

function context_admin_noderef_create_node_submit($form, &$form_state) {
  $form_state['no_redirect'] = FALSE;
  if (isset($form_state['values']['page_context'])) {
    $redirect = '';
    if (isset($form_state['values']['context_admin_custom_redirect']) && $form_state['values']['context_admin_custom_redirect']) {
      $redirect = ctools_context_keyword_substitute($form_state['values']['context_admin_custom_redirect'], array(), $form_state['values']['page_context']['contexts']);
    }
    else {
      $redirect = 'node/' . $form_state['values']['page_context']['contexts'][$form_state['values']['page_context']['submitted_context']]->data->nid;
    }

    if ($redirect) {
      $form_state['redirect'] = $redirect;
    }
  }
}
