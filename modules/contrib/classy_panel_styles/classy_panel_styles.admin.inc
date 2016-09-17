<?php

/**
 * @file
 * Contains admin forms for the Classy Panel Styles project.
 */

/**
 * Form callback for Classy Panel Styles admin settings.
 */
function classy_panel_styles_admin_settings_form($form, &$form_state) {
  // Add admin page CSS.
  $form['#attached']['css'][] = drupal_get_path('module', 'classy_panel_styles')
    . '/admin_styles/css/classy_panel_styles.admin.css';

  // The location of the CPS styles.
  $form[CLASSY_PANEL_STYLES_CSS_PATH] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('CSS file path'),
    '#description' => t('Path to the CSS file with your @name classes, relative to <code>base_path()</code>.', array('@name' => 'Classy Panel Styles')) . '<br />' . t('Available token: <code>%t</code> (path to theme, eg: <code>themes/garland</code>)') . '<br />' . t('Example:') . ' themes/garland/classy_panel_styles.css, %t/classy_panel_styles.css',
    '#default_value' => variable_get(CLASSY_PANEL_STYLES_CSS_PATH),
    '#size' => 140,
  );

  // @TODO Allow disabling of Kendall's "Panels admin styling improvements"?
  // @TODO Change to bitmask to support distinct on/off states for backend +IPE.
  //
  // Whether to apply CPS styles in the editor.
  $form[CLASSY_PANEL_STYLES_EDITOR_STYLING] = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#title' => t('Panels editor styling'),
    '#description' => t('Whether to apply your @name to the panes and regions in the Panels editor. In most cases, you want this. Disable if you experience styling issues in the Panels editor.', array('@name' => 'Classy Panel Styles')),
    '#options' => array(
      1 => t('Enabled'),
      0 => t('Disabled'),
    ),
    '#default_value' => variable_get(CLASSY_PANEL_STYLES_EDITOR_STYLING),
  );

  // Let the user choose which region style to use as the base.
  $options = array();
  ctools_form_include($form_state, 'plugins', 'panels');
  foreach (panels_get_styles() as $style) {
    // Only allow simple region styles that have no settings form.
    if (empty($style['settings form']) && !empty($style['render region'])) {
      $theme_hook = $style['render region'];
      $options[$theme_hook] = $style['title'];
    }
  }
  $form[CLASSY_PANEL_STYLES_REGION_STYLE] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Base region style'),
    '#description' => t('The base region style to use when rendering regions. Your @name classes get wrapped around this style. "No style" is the Panels default and includes pane separators (div). "No markup at all" is the same but without pane separators.', array('@name' => 'CPS')),
    '#options' => $options,
    '#default_value' => variable_get(CLASSY_PANEL_STYLES_REGION_STYLE),
  );

  // Whether to set CPS as the default style on new Display objects.
  $form[CLASSY_PANEL_STYLES_DISPLAY_DEFAULT] = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#title' => t('Make @cps the default style on new Display objects?', array('@cps' => 'CPS')),
    '#description' => t('By default, regions inherit their style from the Display object. So this means all regions will use the @cps region markup unless you set their style to something else. This setting does not affect existing displays.', array('@cps' => 'CPS')),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
    '#default_value' => variable_get(CLASSY_PANEL_STYLES_DISPLAY_DEFAULT),
  );

  // Send to system_settings_form() to add submit buttons, etc.
  return system_settings_form($form);
}
