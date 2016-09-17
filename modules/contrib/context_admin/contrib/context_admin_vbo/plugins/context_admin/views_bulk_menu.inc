<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('Views Administration'),
  'description' => t('Provides a generic Views Bulk Operations based view that should work for most node type needs.'),
  'content form' => 'context_admin_vbo_views_bulk_menu_content_form',
  'content form submit' => 'context_admin_vbo_views_bulk_menu_content_form_submit',
  'render' => 'context_admin_vbo_views_bulk_menu_render_page',
  'save' => 'context_admin_vbo_views_bulk_menu_save',
  'delete' => 'context_admin_vbo_views_bulk_menu_delete',
);

function context_admin_vbo_views_bulk_menu_content_form($form, &$form_state, $cache = NULL) {
  $machine_name = isset($form_state['conf']['context_admin_vbo_machine_name']) ? $form_state['conf']['context_admin_vbo_machine_name'] : NULL;
  $default_type = isset($form_state['conf']['context_admin_vbo_views_bulk_node_type']) ? $form_state['conf']['context_admin_vbo_views_bulk_node_type'] : NULL;
  $default_pub = isset($form_state['conf']['context_admin_vbo_views_bulk_published']) ? $form_state['conf']['context_admin_vbo_views_bulk_published'] : NULL;

  $node_entities = entity_get_info('node');
  $types = field_info_instances('node');
  foreach ($types as $type => $field_instances) {
    $options[$type] = $node_entities['bundles'][$type]['label'];
  }
  $form['context_admin_vbo_machine_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Views: Machine Name'),
    '#description' => t('A unique machine-readable name for this View. It must only contain lowercase letters, numbers, and underscores.'),
    '#required' => TRUE,
    '#default_value' => $machine_name,
  );
  $form['context_admin_vbo_views_bulk_node_type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node Type'),
    '#required' => TRUE,
    '#options' => $options,
  );
  if ($default_type) {
    $form['context_admin_vbo_views_bulk_node_type']['#default_value'] = $default_type;
  }
  $form['context_admin_vbo_views_bulk_published'] = array(
    '#type' => 'radios',
    '#title' => t('Published Options'),
    '#required' => TRUE,
    '#options' => array(
      'published' => t('Published'),
      'unpublished' => t('Unpublished'),
      'both' => t('both'),
    ),
    '#default_value' => $default_pub,
  );
  $form['#validate'][] = 'context_admin_vbo_views_bulk_menu_content_form_validate';
  return $form;
}

function context_admin_vbo_views_bulk_menu_content_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['context_admin_vbo_machine_name']) && preg_match('/[^a-zA-Z0-9_]/', $form_state['values']['context_admin_vbo_machine_name'])) {
    form_error($form['context_admin_vbo_machine_name'], t('View name must be alphanumeric or underscores only.'));
  }
}

function context_admin_vbo_views_bulk_menu_content_form_submit($form, &$form_state) {
  $form_state['conf']['context_admin_vbo_machine_name'] = $form_state['values']['context_admin_vbo_machine_name'];
  $form_state['conf']['context_admin_vbo_views_bulk_node_type'] = $form_state['values']['context_admin_vbo_views_bulk_node_type'];
  $form_state['conf']['context_admin_vbo_views_bulk_published'] = $form_state['values']['context_admin_vbo_views_bulk_published'];
}

function context_admin_vbo_views_bulk_menu_render_page($handler, $base_contexts, $args, $test = TRUE) {
  return views_embed_view($handler->conf['context_admin_vbo_machine_name']);
}

function context_admin_vbo_views_bulk_menu_save($handler, $update) {
  views_invalidate_cache();
}

function context_admin_vbo_views_bulk_menu_delete($handler) {
  views_invalidate_cache();
}

