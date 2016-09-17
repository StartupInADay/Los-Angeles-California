<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('Create Node'),
  'description' => t('Allows the creation of a particular node type.  This circumvents the typical drupal access controls on node creation.'),
  'required context' => new ctools_context_optional(t('Node'), 'node'),
  'content form' => 'context_admin_node_create_menu_content_form',
  'content form submit' => 'context_admin_node_create_menu_content_form_submit',
  'render' => 'context_admin_node_create_menu_render_page',
  'node insert' => 'context_admin_node_create_menu_node_insert',
);


function context_admin_node_create_menu_content_form($form, $form_state) {
  $conf = $form_state['conf'];
  ctools_include('dependent');
  $context_options = $form['context']['#options'];
  unset($context_options['empty']);
  $context_dependencies = array();
  foreach ($context_options as $c_key => $c_option) {
    $context_dependencies[] = $c_key;
  }
  if (isset($conf['context_admin_options_items'])) {
    $default = $conf['context_admin_options_items'];
  }
  else {
    $default = '';
  }
  $options = node_type_get_names();
  $form['context_admin_options_items'] = array(
    '#type' => 'radios',
    '#title' => t('Select the node type you would like to create'),
    '#required' => TRUE,
    '#options' => $options,
    '#default_value' => $default,
  );

  if (isset($conf['context_admin_sub_page'])) {
    $sub = $conf['context_admin_sub_page'];
  }
  else {
    $sub = 0;
  }
  $form['context_admin_sub_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create as sub node'),
    '#description' => t('Creates a node as a sub node of the currently viewed node within the menu system.'),
    '#default_value' => $sub,
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-context' => array(implode(', ', $context_dependencies))),
  );

  if (module_exists('panels')) {
    if(isset($conf['context_admin_use_node_edit'])) {
      $node_edit = $conf['context_admin_use_node_edit'];
    }
    else {
      $node_edit = NULL;
    }

    $form['context_admin_use_node_edit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Render the Add form using panels and page manager'),
      '#description' => t('Enable node/%node/edit in page manager. Unchecked renders Add form with Drupal\'s node templating system.'),
      '#default_value' => $node_edit,
    );
  }

  return $form;
}

function context_admin_node_create_menu_content_form_submit($form, &$form_state) {
  $form_state['conf']['context_admin_options_items'] = $form_state['values']['context_admin_options_items'];
  if ($form_state['values']['context_admin_sub_page']) {
    $form_state['conf']['context_admin_sub_page'] = $form_state['values']['context_admin_sub_page'];
  }
  if (isset($form_state['values']['context_admin_use_node_edit'])) {
    $form_state['conf']['context_admin_use_node_edit'] = $form_state['values']['context_admin_use_node_edit'];
  }
}

function context_admin_node_create_menu_render_page($handler, $base_contexts, $args, $test = TRUE) {
  module_load_include('inc', 'node', 'node.pages');
  $use_panels = FALSE;
  if (isset($handler->conf['context_admin_use_node_edit'])) {
    $use_panels = $handler->conf['context_admin_use_node_edit'];
  }
  return context_admin_node_add_wrapper($handler->conf['context_admin_options_items'], array(), $use_panels);
}

function context_admin_node_create_menu_node_insert($node, $page) {
  if (isset($page['handler']->conf['context_admin_sub_page']) && $page['handler']->conf['context_admin_sub_page']) {
    $context = $page['contexts'][$page['handler']->conf['submitted_context']];
    $parent_id = $context->argument;
    $query = db_select('menu_links', 'ml')
      ->fields('ml', array('menu_name', 'mlid'))
      ->condition('link_path', 'node/' . $parent_id, '=')
      ->condition('module', 'menu', '=')
      ->range(0, 1);
    $results = $query->execute();
    foreach ($results as $result) {
      if ($result) {
        $item = array(
          'link_title' => $node->title,
          'menu_name' => $result->menu_name,
          'plid' => $result->mlid,
          'link_path' => 'node/'. $node->nid,
        );
        menu_link_save($item);
      }
    }
  }
}
