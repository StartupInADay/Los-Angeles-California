<?php

/**
 * Implements hook_theme().
 */
function panels_preview_theme($existing, $type, $theme, $path) {
  return array(
    'panels_preview_stylizer_preview_form' => array(
      'render element' => 'form',
    ),
  );
}

/**
 * Implementation of hook_form_FORM_ID_alter().
 *
 * Provide customizations for the ctools stylizer edit form.
 */
function panels_preview_form_ctools_stylizer_edit_style_form_default_alter(&$form, &$form_state, $form_id) {
  // If Auto-submit is enabled.
  if (variable_get('panels_preview_live_preview', 1)) {

    // Customize the settings preview button.
    $form['top box']['preview']['submit']['#attributes'] = array(
      'class' => array('widget-preview-button', 'ctools-use-ajax', 'ctools-auto-submit-click'),
    );

    // Autosubmit the form.
    ctools_add_js('auto-submit');
    $form['#attributes']['class'][] = 'ctools-auto-submit-full-form';
    $form['#attributes']['class'][] = 'panels-preview-ctools-form';
    $form['top box']['preview']['#theme'] = 'panels_preview_stylizer_preview_form';

  }

  // Change the Weight Around
  $form['top box']['preview']['#weight'] = -50;
}

/**
 * Theme the stylizer preview form.
 */
function theme_panels_preview_stylizer_preview_form($vars) {
  $form = &$vars['form'];
  $plugin = $form['#form_state']['base_style_plugin'];
  $settings = $form['#form_state']['settings'];

  if (!empty($form['#form_state']['settings']['old_settings'])) {
    ctools_stylizer_cleanup_style($plugin, $form['#form_state']['settings']['old_settings']);
  }
  $preview = '';
  if (!empty($plugin['preview'])) {
    $preview = $plugin['preview'];
  }
  else {
    $base_types = ctools_get_style_base_types();

    if (!empty($base_types[$plugin['module']][$plugin['type']]['preview'])) {
      $preview = $base_types[$plugin['module']][$plugin['type']]['preview'];
    }
  }

  if (!empty($preview) && function_exists($preview)) {
    $output = '<fieldset id="preview" class="widget-preview-single"><legend>' . t('Preview') . '</legend><div class="fieldset-wrapper">';
    $output .= $preview($plugin, $settings);
    $output .= drupal_render_children($form);
    $output .= '</div></fieldset>';
    return $output;
  }
}

/**
 * Implementation of hook_form_FORM_ID_alter().
 *
 * Provide customizations for the ctools stylizer edit choose form.
 */
function panels_preview_form_ctools_stylizer_edit_style_form_choose_alter(&$form, &$form_state, $form_id) {

  // Remove the rounded corners options
  if (!empty($form['style_base']['Basic-styles']['pane_rounded_shadow'])) {
    unset($form['style_base']['Basic-styles']['pane_rounded_shadow']);
  }
  if (!empty($form['style_base']['Basic-styles']['region_rounded_shadow'])) {
    unset($form['style_base']['Basic-styles']['region_rounded_shadow']);
  }
}
