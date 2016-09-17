<?php

/**
 * @file
 * Definition of Demonstratie panels styles.
 */

// Plugin definition
$plugin = array(
  'title' => t('Demo styles'),
  'description' => t('Region and panel styles to apply styles from the Demonstratie theme.'),
  'render region' => 'demonstratie_panels_style_render_region',
  'settings form' => 'demonstratie_panels_style_settings_form',
  'render pane' => 'demonstratie_panels_style_render_pane',
  'pane settings form' => 'demonstratie_panels_style_pane_settings_form',
);

/**
 * Region settings form callback.
 */
function demonstratie_panels_style_settings_form($style_settings) {
  // Add demonstratie (or custom theme) specific styles for layouts.
  $base_theme = variable_get('theme_default');
  $theme_check = variable_get('demo_theme', 'demonstratie');
  if (substr($base_theme, 0, strlen($theme_check)) === $theme_check) {
    // Provide multiple layout options.
    $form['layouts'] = array(
      '#type' => 'select',
      '#title' => t('Layout options'),
      '#default_value' => isset($style_settings['layouts']) ? $style_settings['layouts'] : 'default',
      '#options' => array(
        'default' => 'Default',
        'horizontal' => 'Horizontal row',
      ),
    );

    // Provide alternate color schemes.
    $form['color_schemes'] = _demonstratie_panels_color_schemes($style_settings);

    // Provide alternate button styles.
    $form['button_styles'] = _demonstratie_panels_button_styles($style_settings);
  }

  // Provide custom CSS classes.
  $form['css_class'] = _demonstratie_panels_css_class($style_settings);

  // Override default 'row' wrapper.
  $form['region_wrapper'] = _demonstratie_panels_region_wrapper($style_settings);

  return $form;
}

/**
 * Pane settings form callback.
 */
function demonstratie_panels_style_pane_settings_form($style_settings) {
  // Give the option to hide the title, but leave the text on the page for
  // screen readers.
  $form['title'] = array(
    '#type' => 'select',
    '#title' => t('Title display options'),
    '#default_value' => isset($style_settings['title']) ? $style_settings['title'] : 'default',
    '#options' => array(
      'default' => t('Default'),
      'hidden' => t('Hidden'),
      'banner' => t('Banner'),
      'inline' => t('Inline'),
    ),
  );

  // Add demonstratie (or custom theme) specific styles for Panes.
  $base_theme = variable_get('theme_default');
  $theme_check = variable_get('demo_theme', 'demonstratie');
  if (substr($base_theme, 0, strlen($theme_check)) === $theme_check) {
    // Allow the hero image to be extracted from a pane.
    $form['hero'] = array(
      '#type' => 'select',
      '#title' => t('Hero image options'),
      '#default_value' => isset($style_settings['hero']) ? $style_settings['hero'] : 'default',
      '#options' => array(
        'default' => t('Do not manipulate the first image from this pane.'),
        'hero' => t('Use the first image from this pane as a full-page hero.'),
        'background' => t('Use the first image from this pane as a background image.'),
      ),
    );

    // Provide multiple layout options.
    $form['layouts'] = array(
      '#type' => 'select',
      '#title' => t('Layout options'),
      '#default_value' => isset($style_settings['layouts']) ? $style_settings['layouts'] : 'default',
      '#options' => array(
        'default' => 'Default',
        'horizontal' => 'Horizontal content items',
      ),
    );

    // Provide multiple style options.
    $form['styles'] = array(
      '#type' => 'select',
      '#title' => t('Content styles'),
      '#default_value' => isset($style_settings['styles']) ? $style_settings['styles'] : 'default',
      '#options' => array(
        'default' => 'Default',
        'feature_small' => 'Small feature',
        'feature_medium' => 'Medium feature',
        'feature_large' => 'Large feature',
      ),
    );

    // Provide alternate color schemes.
    $form['color_schemes'] = _demonstratie_panels_color_schemes($style_settings);

    // Provide alternate button styles.
    $form['button_styles'] = _demonstratie_panels_button_styles($style_settings);

  }

  // Provide custom CSS classes.
  $form['css_class'] = _demonstratie_panels_css_class($style_settings);

  return $form;
}

/**
 * Output markup for the the Demonstratie region style.
 */
function theme_demonstratie_panels_style_render_region($variables) {
  return implode(PHP_EOL, $variables['panes']);
}

/**
 * Output the markup for the Demonstratie panel style.
 */
function theme_demonstratie_panels_style_render_pane($variables) {
  // Add the javascript for finding the hero image if needed.
  if (isset($variables['settings']['hero']) && $variables['settings']['hero'] != 'default') {
    drupal_add_js(drupal_get_path('module', 'demonstratie_panels') . '/scripts/demonstratie-panels.js');
  }

  return theme('panels_pane__demonstratie', $variables);
}

/**
 * Provides color style settings that are common between regions and panels.
 */
function _demonstratie_panels_color_schemes($style_settings) {
  return array(
    '#type' => 'select',
    '#title' => t('Color options'),
    '#default_value' => isset($style_settings['color_schemes']) ? $style_settings['color_schemes'] : 'default',
    '#options' => array(
      'default' => 'Default',
      'alternate' => 'Alternate',
      'reverse' => 'Reverse',
    ),
  );
}

/**
 * Provides button style settings that are common between regions and panels.
 */
function _demonstratie_panels_button_styles($style_settings) {
  return array(
    '#type' => 'select',
    '#title' => t('Button style'),
    '#default_value' => isset($style_settings['button_styles']) ? $style_settings['button_styles'] : 'default',
    '#options' => array(
      'default' => 'Default',
      'call-to-action' => 'Call-to-Action',
    ),
  );
}

/**
 * Provides the ability to add custom CSS classes.
 */
function _demonstratie_panels_css_class($style_settings) {
  return array(
    '#type' => 'textfield',
    '#title' => t('CSS Class'),
    '#default_value' => isset($style_settings['css_class']) ? $style_settings['css_class'] : '',
  );
}

/**
 * Provides 'full-width' override for the default 'row' wrapper.
 */
function _demonstratie_panels_region_wrapper($style_settings) {
  return array(
    '#type' => 'select',
    '#title' => t('Region wrapper'),
    '#description' => t('The region wrapper only applies to Demonstratie\'s single-column layout by default.'),
    '#default_value' => isset($style_settings['region_wrapper']) ? $style_settings['region_wrapper'] : 'row',
    '#options' => array(
      'row' => 'Default row',
      'full-width' => 'Full width',
    ),
  );
}


