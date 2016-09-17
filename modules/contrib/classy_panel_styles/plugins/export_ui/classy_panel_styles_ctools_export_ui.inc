<?php
/**
 * @file
 * Ctools Export UI plugin file.
 *
 * Contains the plugin definition and supporting functions.
 */

// Define this Export UI plugin.
$plugin = array(
  'schema' => 'classy_panel_styles',
  'access' => 'administer classy panel styles',
  // Define the menu item.
  'menu' => array(
    'menu item' => 'classy_panel_styles',
    'menu title' => 'Classy Panel Styles',
    'menu description' => 'Administer Classy Panel Styles.',
    'menu prefix' => 'admin/config/content',
  ),
  // Define user interface texts.
  'title singular' => t('style'),
  'title plural' => t('styles'),
  'title singular proper' => t('Classy Panel Style'),
  'title plural proper' => t('Classy Panel Styles'),
  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'classy_panel_styles_ctools_export_ui_form',
    'submit'   => 'classy_panel_styles_ctools_export_ui_form_submit',
    'validate' => 'classy_panel_styles_ctools_export_ui_form_validate',
  ),
);

/**
 * Form callback for the Classy Panel Styles add/edit form.
 */
function classy_panel_styles_ctools_export_ui_form(&$form, &$form_state) {
  $style = isset($form_state['item']) ? $form_state['item'] : new stdClass();

  $form['title'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($style->title) ? $style->title : '',
    '#description' => t('A human readable name for the style.'),
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textfield',
    '#default_value' => isset($style->description) ? $style->description : '',
  );

  // Calculate the options string.
  $options_string = '';
  if (isset($style->options)) {
    foreach (unserialize($style->options) as $class => $display_name) {
      $options_string .= "$class|$display_name" . PHP_EOL;
    }
  }

  // Add the options element.
  $form['options'] = array(
    '#title' => t('Options'),
    '#type' => 'textarea',
    '#required' => TRUE,
    '#description' => t('Enter one option per line in key value pairs of class name and display name. E.g., "border-green|Green Border". Do not include the dot prefix on the class name.'),
    '#default_value' => $options_string,
  );

  // Add the "Default value" element.
  $form['default_value'] = array(
    '#type' => 'select',
    '#title' => t('Default value'),
    '#description' => t('Available options updated after save.'),
    '#options' => isset($style->options) ? unserialize($style->options) : array(),
    '#default_value' => isset($style->default_value) ? $style->default_value : NULL,
    '#empty_option' => '- None -',
  );

  $form['required'] = array(
    '#type' => 'radios',
    '#title' => t('Required?'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#default_value' => empty($style->required) ? 0 : 1,
  );

  // Calculate the current "Pane/Region/Both" setting. Default to all enabled.
  $visibility = array(
    CLASSY_PANEL_STYLES_PANE_MASK,
    CLASSY_PANEL_STYLES_REGION_MASK,
  );
  if (isset($style->visibility)) {
    foreach ($visibility as $index => $bitmask) {
      if (!($style->visibility & $bitmask)) {
        unset($visibility[$index]);
      }
    }
  }

  // Add the "Pane/Region/Both" element.
  $form['visibility'] = array(
    '#title' => t('This style can apply to...'),
    '#type' => 'checkboxes',
    '#required' => TRUE,
    '#options' => array(
      CLASSY_PANEL_STYLES_PANE_MASK => t('Panes'),
      CLASSY_PANEL_STYLES_REGION_MASK => t('Regions'),
    ),
    '#default_value' => $visibility,
  );

  // Add Layout Options to the settings form.
  $form['layouts_setting'] = array(
    '#type' => 'radios',
    '#title' => t('Layout options'),
    '#default_value' => isset($style->layouts_setting) ? $style->layouts_setting : CLASSY_PANEL_STYLES_LAYOUTS_ALL,
    '#options' => array(
      CLASSY_PANEL_STYLES_LAYOUTS_ALL     => t('Show on all layouts.'),
      CLASSY_PANEL_STYLES_LAYOUTS_EXCLUDE => t('Show on all layouts except the following...'),
      CLASSY_PANEL_STYLES_LAYOUTS_INCLUDE => t('Show ONLY on the following layouts...'),
    ),
  );

  // Load the module path.
  $path = drupal_get_path('module', 'classy_panel_styles');

  // Add custom JS to show/hide the "layout_vertical_tabs" because States API
  // does not work for vertical_tabs in Drupal 7.
  // @see https://drupal.org/node/1148950
  // @see https://drupal.org/node/1777970
  $form['#attached']['js'][] = $path . '/js/classy_panel_styles.admin.js';

  // Add CSS for styling the layouts, etc.
  $form['#attached']['css'][] = $path . '/admin_styles/css/classy_panel_styles.admin.css';

  // Add layout selection by category to the settings form.
  //
  // Vertical tabs element.
  $form['layout_vertical_tabs'] = array(
    '#type' => 'vertical_tabs',
  );
  // Fieldsets container.
  $form['layouts'] = array(
    '#tree' => TRUE,
  );
  // Loop through each layout available in the system.
  ctools_include('plugins', 'panels');
  ctools_include('cleanstring');
  $layouts = isset($style->layouts) ? unserialize($style->layouts) : array();
  foreach (panels_get_layouts() as $id => $layout) {
    $category = ctools_cleanstring($layout['category']);
    // Set up the category fieldset.
    if (!isset($form['layouts'][$category])) {
      $form['layouts'][$category] = array(
        '#type'        => 'fieldset',
        '#group'       => 'layout_vertical_tabs',
        '#title'       => $layout['category'],
      );
    }
    // Generate the icon and title output for the layout checkbox.
    $markup = panels_print_layout_icon($id, $layout, check_plain($layout['title']));

    // Add the checkbox to the form.
    $form['layouts'][$category][$id] = array(
      '#type'  => 'checkbox',
      '#title' => $markup,
    );

    // Set default value (checked or not).
    if (in_array($id, $layouts)) {
      $form['layouts'][$category][$id]['#attributes'] = array(
        'checked' => 'checked',
      );
    }
  }

  // Sort the categories and layouts.
  ksort($form['layouts']);
  foreach ($form['layouts'] as &$category) {
    if (is_array($category)) {
      ksort($category);
    }
  }
}

/**
 * Form validate callback for the Classy Panel Styles add/edit form.
 */
function classy_panel_styles_ctools_export_ui_form_validate($form, &$form_state) {
  if (CLASSY_PANEL_STYLES_CUSTOM_CLASSES == $form_state['values']['name']) {
    form_set_error('name', t('Class name %class is reserved for the system.',
      array('%class' => CLASSY_PANEL_STYLES_CUSTOM_CLASSES)));
  }
  if (!empty($form_state['values']['options'])) {
    _classy_panel_styles_parse_options($form_state['values']['options'], TRUE);
  }
}

/**
 * Form submit callback for the Classy Panel Styles add/edit form.
 *
 * Translates form values into their proper format for database storage.
 */
function classy_panel_styles_ctools_export_ui_form_submit(array $form, array &$form_state) {
  $values = &$form_state['values'];

  // Set the "visibility" value.
  $visibility = 0;
  foreach ($values['visibility'] as $bitmask => $status) {
    if ($status) {
      // Bitwise addition of the selected value.
      $visibility = $visibility | $bitmask;
    }
  }
  $values['visibility'] = $visibility;

  // Set the "options" array.
  if ($options = _classy_panel_styles_parse_options($values['options'])) {
    $values['options'] = $options;
  }
  else {
    // Form validation should prevent us from ever getting here. But just in
    // case, keep the bad value out of the database by unsetting the form value.
    unset($values['options']);
  }

  // Set the "layouts" value.
  $layouts = array();
  if ($values['layouts_setting']) {
    foreach ($values['layouts'] as $layouts_by_category) {
      foreach ($layouts_by_category as $layout => $status) {
        if ($status) {
          $layouts[] = $layout;
        }
      }
    }
  }
  $values['layouts'] = serialize($layouts);

  // Until I finish the groups/plugins functionality, assign to default group.
  if (empty($form_state['item']->groups)) {
    $values['groups'] = serialize(array('cps_default'));
  }
}

/**
 * Attempts to parse the user-provided options string into key value pairs.
 *
 * @param string $options_string
 *   The options string input on the add/edit form.
 * @param bool $set_form_errors
 *   Whether to call form_set_error() on bad input.
 *
 * @return mixed
 *   On success, the array of parsed options pairs. FALSE on failure.
 */
function _classy_panel_styles_parse_options($options_string, $set_form_errors = FALSE) {
  if (empty($options_string)) {
    // Form validation should keep us from getting here.
    return FALSE;
  }
  $options = array();
  foreach (explode(PHP_EOL, $options_string) as $option) {
    if (empty($option) || ctype_space($option)) {
      continue;
    }
    $parts         = explode('|', $option, 2);
    $class         = trim($parts[0]);
    $class_cleaned = drupal_html_class($class);
    if ($class_cleaned !== $class) {
      if ($set_form_errors) {
        form_set_error('options',
          t('Invalid class name: "@class". Consider using: "@cleaned".', array(
            '@class'   => $class,
            '@cleaned' => $class_cleaned,
          )));
      }
      return FALSE;
    }
    $name = isset($parts[1]) ? trim($parts[1]) : $class;
    $options[$class] = $name;
  }
  return serialize($options);
}
