<?php

/**
 * Implementation of hook_form_FORM_ID_alter().
 *
 * Provides customization to panel pane style form.
 */
function panels_preview_form_panels_edit_style_type_form_alter(&$form, &$form_state, $form_id) {

  if (!variable_get('panels_preview_show_panels_styles', 0)) {
    // Unset options for which we have no need
    unset($form['style']['#options']['block']);
    unset($form['style']['#options']['rounded_corners']);
    unset($form['style']['#options']['naked']);
  }

  // Better explain that "No Style" means "Default Pane Style"
  // Move the default option to the top. Also I should have learned
  // how to manage arrays in PHP.
  if (!empty($form['style']['#options']['default'])) {
    $form['style']['#options']['default'] = t('Default Pane Style');
    $default = $form['style']['#options']['default'];
    unset($form['style']['#options']['default']);
    $form['style']['#options'] = array_reverse($form['style']['#options']);
    $form['style']['#options']['default'] = $default;
    $form['style']['#options'] = array_reverse($form['style']['#options']);
  }

  // Better title for the List Style region style plugin
  if (!empty($form['style']['#options']['list'])) {
    $form['style']['#options']['list'] = t('List Style');
  }

  // Improve the UI around region style selections
  if (!empty($form['style']['#options'][0])) {
    $form['style']['#options'][0] = t('Default Region Style');
    if (empty($form['style']['#default_value']) || $form['style']['#default_value'] == '-1') {
      $form['style']['#default_value'] = '0';
    }
    unset($form['style']['#options']['default']);
  }

  // Move custom style option to bottom of list.
  if (!empty($form['style']['style']['#options']['stylizer'])) {
    unset($form['style']['style']['#options']['stylizer']);
    $form['style']['style']['#options']['stylizer'] = t('Custom Style');
  }

  // Adding a fieldset around styling
  $form['style'] = array('style' => $form['style']);
  $form['style']['#type'] = 'fieldset';
  $form['style']['#title'] = t('Style Settings');
}
