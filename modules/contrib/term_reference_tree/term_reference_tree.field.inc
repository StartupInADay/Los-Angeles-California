<?php

/**
 * Implements hook_field_formatter_info().
 */
function term_reference_tree_field_formatter_info() {
  return array(
    'term_reference_tree' => array(
      'label' => 'Term reference tree',
      'field types' => array('taxonomy_term_reference'),
      'settings' => array(
        'token_display_selected' => '',
        'token_display_unselected' => '',
      ),
    ),
  );
}


/**
 * Implements hook_field_formatter_view().
 */
function term_reference_tree_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'term_reference_tree':
      if (!empty($items)) {
        $element[] = array(
          '#theme' => 'term_tree_list',
          '#data' => $items,
          '#display' => $display,
          '#attached' => array('css' => array(drupal_get_path('module', 'term_reference_tree') . '/term_reference_tree.css')),
        );
      }
      break;
  }

  return $element;
}


/**
 * Implements hook_field_formatter_settings_form().
 */
function term_reference_tree_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();
  if ($display['type'] == 'term_reference_tree' && module_exists('token')) {
    $element['token_display_selected'] = array(
      '#type' => 'textarea',
      '#title' => 'Custom Term Label',
      '#description' => t("Use tokens to change the term label.  Leave this field blank to use the term name linked to its taxonomy page."),
      '#default_value' => $settings['token_display_selected'],
    );

    $element['token_display_unselected'] = array(
      '#type' => 'textarea',
      '#title' => 'Custom Term Label (unselected)',
      '#description' => t("Use tokens to change the term label for unselected parent terms.  Leave this field blank to use the same tokens as above."),
      '#default_value' => $settings['token_display_unselected'],
    );

    $element['tokens_list'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('term'),
    );
  }

  return $element;
}


/**
 * Implements hook_field_formatter_settings_summary().
 */
function term_reference_tree_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = '';

  if ($display['type'] == 'term_reference_tree') {
    $summary = t('Uses tokens: ') . ($settings['token_display_selected'] != '' ? t('Yes') : t('No'));
  }

  return $summary;
}
