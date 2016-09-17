<?php

/**
 * @file
 * Picture formatter with flexslider support.
 */


/**
 * Preprocess the items and prepare the item slides to be rendered.
 *
 * @param $vars
 */
function template_preprocess_flexslider_picture_list(&$vars) {
  // Call default preprocessor first.
  template_preprocess_flexslider_list($vars);

  $optionset = &$vars['settings']['optionset'];

  // Check if this is a picture optionset.
  $vars['picture_formatter_enabled'] = (isset($optionset->imagestyle_type) && !empty($optionset->imagestyle_type) && $optionset->imagestyle_type == 'picture_mapping');

  if ($vars['picture_formatter_enabled']) {
    $items = &$vars['items'];

    // Get the breakpoints based on the mapping.
    $fallback_image_style = '';
    $mappings = picture_mapping_load($optionset->mapping);
    $breakpoint_styles = picture_get_mapping_breakpoints($mappings, $fallback_image_style);

    // If colorbox is enabled build additional configuration.
    if (!empty($optionset->options['colorboxEnabled'])) {
      // Add additional necessary scripts and styles.
      drupal_add_js(drupal_get_path('module', 'picture') . '/picture_colorbox.js');
      drupal_add_js(drupal_get_path('module', 'picture') . '/picture_colorbox.css');
      if (!variable_get('colorbox_inline', 0)) {
        drupal_add_js(drupal_get_path('module', 'colorbox') . '/js/colorbox_inline.js');
      }

      $colorbox_fallback_image_style = '';
      $mappings = picture_mapping_load($optionset->options['colorboxImageStyle']);
      $colorbox_breakpoints = picture_get_mapping_breakpoints($mappings, $colorbox_fallback_image_style);
      // Grouping ID for the colorbox gallery. Use more_entropy to ensure the
      // php function works on every environment (cygwin).
      $colorbox_group_id = uniqid('flexgroup', TRUE);
    }

    // Prepare the item slides to be passed to render().
    foreach ($items as $i => &$item) {
      // If the slide hasn't been set, build the slide using the image
      // attributes given (assumes we're using a multi-image field)
      // @todo need to allow for different types of field and collection fields.
      if (!isset($item['slide'])) {
        $item['slide'] = array(
          '#theme' => 'picture',
          '#style_name' => $optionset->imagestyle_normal,
          '#uri' => $item['uri'],
          '#height' => $item['height'],
          '#width' => $item['width'],
          '#alt' => $item['alt'],
          '#title' => $item['title'],
          '#breakpoints' => $breakpoint_styles,
        );

        // If colorbox is enabled change the theming function and add settings.
        if (!empty($optionset->options['colorboxEnabled'])) {
          $item['slide'] = array(
            '#theme' => 'picture_formatter_colorbox',
            '#item' => $item,
            '#image_style' => $optionset->imagestyle_normal,
            '#path' => $item['uri'],
            '#colorbox_image_style' => $colorbox_fallback_image_style,
            '#colorbox' => $colorbox_breakpoints,
            '#colorbox_group_id' => $colorbox_group_id,
          ) + $item['slide'];
        }
      }
      if (!isset($item['thumb'])) {
        $item['thumb'] = image_style_url($optionset->imagestyle_thumbnail, $item['uri']);
      }
    }
  }
}

/**
 * Theme callback.
 */
function theme_flexslider_picture_list(&$vars) {
  if (!empty($vars['picture_formatter_enabled'])) {
    // Reference configuration variables.
    $attributes = &$vars['settings']['attributes'];
    $type = &$vars['settings']['type'];
    $output = '';

    // Build the list.
    if (!empty($vars['items'])) {
      $output .= "<$type" . drupal_attributes($attributes) . '>';
      foreach ($vars['items'] as $i => $item) {
        $item = render($item['slide']);
        $output .= theme('flexslider_list_item', array(
          'item' => $item,
          'thumb' => $item['thumb'],
          'optionset' => $vars['settings']['optionset'],
        ));
      }
      $output .= "</$type>";
    }

    return $output;
  }
  // If this isn't a picture optionset use the default theming.
  return theme_flexslider_list($vars);
}
