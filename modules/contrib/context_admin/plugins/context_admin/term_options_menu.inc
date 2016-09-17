<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('Taxonomy Term Options'),
  'description' => t('Provides a number of different term related functions to allow for contextualized taxonomy term maintenance.'),
  'required context' => new ctools_context_required(t('Term'), 'taxonomy_term'),
  'content form' => 'context_admin_term_options_menu_content_form',
  'content form submit' => 'context_admin_term_options_menu_content_form_submit',
  'render' => 'context_admin_term_options_menu_render_page',
  'form alter' => 'context_admin_term_options_menu_form_alter',
);

function context_admin_term_options_menu_content_form($form, &$form_state) {
  if (isset($form_state['conf']['context_admin_term_options'])) {
    $default_options = $form_state['conf']['context_admin_term_options'];
  }
  else {
    $default_options = NULL;
  }
  $form['help'] = array(
    '#type' => 'markup',
    '#value' => t('Taxonomy edit tabs can be limited via the access controls/selection criteria.  If they are not limited, they will apply to ALL terms within your site.'),
  );
  $form['context_admin_term_options'] = array(
    '#type' => 'radios',
    '#title' => t('Term Options'),
    '#required' => TRUE,
    '#options' => array(
      'add' => t('Add Sub Term'),
      'term' => t('Edit Term'),
    ),
    '#default_value' => $default_options,
  );
  return $form;
}

function context_admin_term_options_menu_content_form_submit($form, &$form_state) {
  $form_state['conf']['context_admin_term_options'] = $form_state['values']['context_admin_term_options'];
}

function context_admin_term_options_menu_render_page($handler, $base_contexts, $args, $test = TRUE) {
  module_load_include('inc', 'taxonomy', 'taxonomy.admin');
  switch($handler->conf['context_admin_term_options']) {
    case 'add':
      return context_admin_term_options_menu_admin_term_edit($base_contexts[$handler->conf['submitted_context']]->data, TRUE);
    case 'term':
      return context_admin_term_options_menu_admin_term_edit($base_contexts[$handler->conf['submitted_context']]->data);
  }
}

function context_admin_term_options_menu_admin_term_edit($term, $add = NULL) {
  if (isset($add)) {
    return drupal_get_form('taxonomy_form_term', array(), taxonomy_vocabulary_load($term->vid));
  }
  else {
    return drupal_get_form('taxonomy_form_term', $term, taxonomy_vocabulary_load($term->vid));
  }
}

function context_admin_term_options_menu_form_alter(&$form, $form_state, $form_id, $page) {
  if ($page && $form_id == 'taxonomy_form_term') {
    $context = $page['contexts'][$page['handler']->conf['submitted_context']];
    if ($page['handler']->conf['context_admin_term_options'] == 'add' && $context->data->tid) {
      $form['relations']['parent']['#default_value'] = array($context->data->tid);
    }
  }
}
