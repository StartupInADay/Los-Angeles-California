<?php

/**
 * Implements hook_preprocess_demonstratie_layout().
 */
function template_preprocess_demonstratie_layout_single(&$variables, $hook) {
  // Load the css here instead of defining it in the layout configuration in
  // order to keep the sass organization cleaner.
  drupal_add_css(drupal_get_path('module', 'demonstratie_panels') . '/styles/layouts.css');

  // If there is a css_id, roll it into the attributes array.
  if (!empty($variables['css_id'])) {
    $variables['content_attributes_array']['id'] = $variables['css_id'];
  }

  // Add an appropriate class based on the layout.
  $variables['classes_array'][] = drupal_html_class('demonstratie-layout-' . $variables['display']->layout);

  // Add a generic class to the content.
  $variables['content_attributes_array']['class'][] = 'demonstratie-layout-content';

  // Create an attributes array for each region and add a related a class.
  foreach ($variables['content'] as $region => $content) {
    $variables[$region . '_attributes_array'] = array(
      'class' => array(drupal_html_class($region . '-region'), 'clearfix'),
    );

    // Ensure that the 'row' wrapper is added without demonstratie styles.
    $variables[$region . '_wrapper'] = drupal_html_class("row");

    if (isset($variables['display']->panel_settings[$region]) && $variables['display']->panel_settings[$region]['style'] == 'demonstratie_panels_styles') {

      $style_settings = $variables['display']->panel_settings['style_settings'][$region];

      if (isset($style_settings['region_wrapper'])) {
        $variables[$region . '_wrapper'] = drupal_html_class($style_settings['region_wrapper']);
      }

      if (isset($style_settings['layouts'])) {
        $variables[$region . '_attributes_array']['class'][] = drupal_html_class($style_settings['layouts'] . '-layout');
      }

      if (isset($style_settings['color_schemes'])) {
        $variables[$region . '_attributes_array']['class'][] = drupal_html_class($style_settings['color_schemes'] . '-scheme');
      }
      if (isset($style_settings['button_styles'])) {
        $variables[$region . '_attributes_array']['class'][] = drupal_html_class($style_settings['button_styles'] . '-button');
      }
      if (isset($style_settings['css_class'])) {
        $classes = explode(' ', $style_settings['css_class']);
        foreach ($classes as $class) {
          $variables[$region . '_attributes_array']['class'][] = drupal_html_class($class);
        }
      }
    }

  }
}

function template_preprocess_demonstratie_layout_three_col(&$variables, $hook) {
  // Load the css here instead of defining it in the layout configuration in
  // order to keep the sass organization cleaner.
  drupal_add_css(drupal_get_path('module', 'demonstratie_panels') . '/styles/layouts.css');

  // If there is a css_id, roll it into the attributes array.
  if (!empty($variables['css_id'])) {
    $variables['content_attributes_array']['id'] = $variables['css_id'];
  }

  // Add an appropriate class based on the layout.
  $variables['classes_array'][] = drupal_html_class('demonstratie-layout-' . $variables['display']->layout);

  // Add a generic class to the content.
  $variables['content_attributes_array']['class'][] = 'demonstratie-layout-content';

  // Create an attributes array for each region and add a related a class.
  foreach ($variables['content'] as $region => $content) {
    $variables[$region . '_attributes_array'] = array(
      'class' => array(drupal_html_class($region . '-region'), 'clearfix'),
    );

    // Ensure that the 'row' wrapper is added without demonstratie styles.
    $variables[$region . '_wrapper'] = drupal_html_class("row");

    if (isset($variables['display']->panel_settings[$region]) && $variables['display']->panel_settings[$region]['style'] == 'demonstratie_panels_styles') {

      $style_settings = $variables['display']->panel_settings['style_settings'][$region];

      if (isset($style_settings['region_wrapper'])) {
        $variables[$region . '_wrapper'] = drupal_html_class($style_settings['region_wrapper']);
      }

      if (isset($style_settings['layouts'])) {
        $variables[$region . '_attributes_array']['class'][] = drupal_html_class($style_settings['layouts'] . '-layout');
      }

      if (isset($style_settings['color_schemes'])) {
        $variables[$region . '_attributes_array']['class'][] = drupal_html_class($style_settings['color_schemes'] . '-scheme');
      }
      if (isset($style_settings['button_styles'])) {
        $variables[$region . '_attributes_array']['class'][] = drupal_html_class($style_settings['button_styles'] . '-button');
      }
      if (isset($style_settings['css_class'])) {
        $classes = explode(' ', $style_settings['css_class']);
        foreach ($classes as $class) {
          $variables[$region . '_attributes_array']['class'][] = drupal_html_class($class);
        }
      }
    }

  }
}

/**
 * Implements hook_preprocess_demonstratie_layout().
 */
function template_preprocess_demonstratie_layout_landing(&$variables, $hook) {
  template_preprocess_demonstratie_layout_single($variables, $hook);

  if (!empty($variables['content']['secondary_features']) && !empty($variables['content']['tertiary_features'])) {
    $variables['secondary_features_attributes_array']['class'][] = 'region-featured-medium';
    $variables['tertiary_features_attributes_array']['class'][] = 'region-featured-medium';
  }
  else {
    $variables['secondary_features_attributes_array']['class'][] = 'region-featured-large';
    $variables['tertiary_features_attributes_array']['class'][] = 'region-featured-large';
  }
}

function template_preprocess_demonstratie_layout_full_side(&$variables, $hook) {
  template_preprocess_demonstratie_layout_single($variables, $hook);
  template_preprocess_demonstratie_layout_landing($variables, $hook);
}
/**
 * Implements hook_preprocess_demonstratie_split_features().
 */
function template_preprocess_demonstratie_layout_split_features(&$variables, $hook) {
  template_preprocess_demonstratie_layout_single($variables, $hook);
  template_preprocess_demonstratie_layout_landing($variables, $hook);
}

/**
 * Implements hook_preprocess_demonstratie_layout().
 */
function template_preprocess_demonstratie_layout_stagger(&$variables, $hook) {
  template_preprocess_demonstratie_layout_single($variables, $hook);

  $variables['header_attributes_array']['class'][] = 'content-container';
  $variables['footer_attributes_array']['class'][] = 'content-container';

  if (isset($variables['content']['primary_features']) && isset($variables['content']['secondary_features'])) {
    $variables['primary_features_attributes_array']['class'][] = 'medium-third-x-2';
    $variables['primary_features_attributes_array']['class'][] = 'row-item-first';
    $variables['secondary_features_attributes_array']['class'][] = 'medium-third-x-1';
    $variables['secondary_features_attributes_array']['class'][] = 'row-item-last';
  }

  if (isset($variables['content']['tertiary_features']) && isset($variables['content']['quaternary_features'])) {
    $variables['tertiary_features_attributes_array']['class'][] = 'medium-third-x-2';
    $variables['tertiary_features_attributes_array']['class'][] = 'medium-push-third-x-1';
    $variables['tertiary_features_attributes_array']['class'][] = 'row-item-last';
    $variables['quaternary_features_attributes_array']['class'][] = 'medium-third-x-1';
    $variables['quaternary_features_attributes_array']['class'][] = 'medium-pull-third-x-2';
    $variables['quaternary_features_attributes_array']['class'][] = 'row-item-first';
  }

  $medium_third_regions = array(
    'primary_supplements',
    'secondary_supplements',
    'tertiary_supplements',
  );

  foreach ($medium_third_regions as $key => $region) {
    if (!isset($variables['content'][$region])) {
      unset($medium_third_regions[$key]);
    }
  }

  $supplement_count = count($medium_third_regions);
  $counter = 1;

  foreach ($medium_third_regions as $region) {
    if ($supplement_count == 3) {
      $variables[$region . '_attributes_array']['class'][] = 'medium-third-x-1';
    }
    elseif ($supplement_count == 2) {
      $variables[$region . '_attributes_array']['class'][] = 'medium-half';
    }

    if ($counter == 1) {
      $variables[$region . '_attributes_array']['class'][] = 'row-item-first';
    }
    if ($counter == $supplement_count) {
      $variables[$region . '_attributes_array']['class'][] = 'row-item-last';
    }

    $counter++;
  }
}
