<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('Vocabulary Options'),
  'description' => t('Provides a number of different vocabulary related functions to allow for contextualized vocabulary maintenance.'),
  'content form' => 'context_admin_taxonomy_list_menu_content_form',
  'content form submit' => 'context_admin_taxonomy_list_menu_content_form_submit',
  'render' => 'context_admin_taxonomy_list_menu_render_page',
  'form alter' => 'context_admin_taxonomy_list_menu_form_alter',
);

function context_admin_taxonomy_list_menu_content_form($form, &$form_state) {
  if (isset($form_state['conf']['context_admin_vocabulary'])) {
    $default = $form_state['conf']['context_admin_vocabulary'];
  }
  else {
    $default = NULL;
  }

  if (isset($form_state['conf']['context_admin_vocabulary_options'])) {
    $default_option = $form_state['conf']['context_admin_vocabulary_options'];
  }
  else {
    $default_option = NULL;
  }
  $types = taxonomy_get_vocabularies();
  foreach ($types as $type) {
    $options[$type->machine_name] = $type->name;
  }
  $form['context_admin_vocabulary'] = array(
    '#type' => 'radios',
    '#title' => t('Vocabulary'),
    '#description' => t('Select the vocabulary list you would like.'),
    '#required' => TRUE,
    '#options' => $options,
    '#default_value' => $default,
  );
  $form['context_admin_vocabulary_options'] = array(
    '#type' => 'radios',
    '#title' => t('Vocabulary Option'),
    '#required' => TRUE,
    '#options' => array(
      'edit' => 'Edit Vocabulary',
      'list' => 'List Terms',
      'add' => 'Add Terms',
    ),
    '#default_value' => $default_option,
  );
  return $form;
}

function context_admin_taxonomy_list_menu_content_form_submit($form, &$form_state) {
  $form_state['conf']['context_admin_vocabulary'] = $form_state['values']['context_admin_vocabulary'];
  $form_state['conf']['context_admin_vocabulary_options'] = $form_state['values']['context_admin_vocabulary_options'];
}

function context_admin_taxonomy_list_menu_render_page($handler, $base_contexts, $args, $test = TRUE) {
  module_load_include('inc', 'taxonomy', 'taxonomy.admin');
  if (isset($handler->conf['context_admin_vocabulary'])) {
    $vocabularies = taxonomy_vocabulary_get_names();
    $vocabulary = taxonomy_vocabulary_load($vocabularies[$handler->conf['context_admin_vocabulary']]->vid);
  }
  switch($handler->conf['context_admin_vocabulary_options']) {
    case 'edit':
      $form = drupal_get_form('context_admin_taxonomy_list_menu_taxonomy_form_vocabulary', (array)$vocabulary);
      return $form;
    case 'list':
      return drupal_get_form('taxonomy_overview_terms', $vocabulary);
    case 'add':
      return drupal_get_form('taxonomy_form_term', array(), $vocabulary);
  }
}

function context_admin_taxonomy_list_menu_taxonomy_form_vocabulary($form, $form_state, $vocabulary) {
  $form = taxonomy_form_vocabulary($form, $form_state, $vocabulary);
  return $form;
}

function context_admin_taxonomy_list_menu_taxonomy_form_vocabulary_submit($form, &$form_state) {
  taxonomy_form_vocabulary_submit($form, $form_state);
  $form_state['redirect'] = $_GET['q'];
}

function context_admin_taxonomy_list_menu_form_alter(&$form, &$form_state, $form_id, $page) {
  if ($form_id == 'context_admin_taxonomy_list_menu_taxonomy_form_vocabulary') {
    unset($form['actions']['delete']);
  }
}
