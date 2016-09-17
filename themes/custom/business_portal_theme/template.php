<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728096
 */


/**
 * Override or insert variables into the maintenance page template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("maintenance_page" in this case.)
 */
/* -- Delete this line if you want to use this function
function labusinessportaltheme_preprocess_maintenance_page(&$variables, $hook) {
  // When a variable is manipulated or added in preprocess_html or
  // preprocess_page, that same work is probably needed for the maintenance page
  // as well, so we can just re-use those functions to do that work here.
  labusinessportaltheme_preprocess_html($variables, $hook);
  labusinessportaltheme_preprocess_page($variables, $hook);
}
// */

/**
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
/* -- Delete this line if you want to use this function
function labusinessportaltheme_preprocess_html(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // The body tag's classes are controlled by the $classes_array variable. To
  // remove a class from $classes_array, use array_diff().
  //$variables['classes_array'] = array_diff($variables['classes_array'], array('class-to-remove'));
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */

function labusinessportaltheme_preprocess_page(&$variables, $hook) {
  global $user;
  if (!$variables['logged_in']) {
    drupal_add_js('https://globalnav.lacity.org/global_nav_2.0.js', 'external');
  } else {
    drupal_add_css('#main { margin-top: 60px; }', 'inline');
  }
  drupal_add_library('system', 'ui.accordion');

}


/**
 * Implements preprocess_panels_pane().
 */
function labusinessportaltheme_preprocess_panels_pane(&$vars) {
  if ($vars['title'] == 'Filter by business need:') {
    $vars['title'] = 'Business Need';
  }
  if ($vars['title'] == 'Filter by business ownership:') {
  $vars['title'] = 'Business Ownership';
  }
  if ($vars['title'] == 'Filter by industry tag:') {
    $vars['title'] = 'Industry';
  }
  if ($vars['title'] == 'Filter by resource type:') {
    $vars['title'] = 'Resource Type';
  }
  if ($vars['pane']->subtype == 'sbp_wizard-wizard') {
    $theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
    $profile_path = drupal_get_path('profile', 'labp');
    drupal_add_js($profile_path . '/libraries/underscore/underscore-min.js');
    drupal_add_js($profile_path . '/libraries/backbone/backbone-min.js');
    drupal_add_js($theme_path . '/js/wizard/models.js');
    drupal_add_js($theme_path . '/js/wizard/collections.js');
    drupal_add_js($theme_path . '/js/wizard/views.js');
    drupal_add_js($theme_path . '/js/wizard/app.js');
    drupal_add_js($theme_path . '/js/wizard/theming.js');
  }
}

/**
 * Implements hook_preprocess_html().
 *
 * @param array &$variables
 *   Template variables.
 */
function labusinessportaltheme_preprocess_html(&$vars) {
  /* LA-313 Add google verification code */
  $google_site_verification = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
          'name' => 'google-site-verification',
          'content' =>  'w3jz11h8zojzXkOEgAegvWidD7Z1TCXAbmZbs32ZyNs',
      )
  );
  // Add header meta tag for Google code
  drupal_add_html_head($google_site_verification, 'meta_google_site_verification');
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function labusinessportaltheme_preprocess_node(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // labusinessportaltheme_preprocess_node_page() or labusinessportaltheme_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $variables['node']->type;
  if (function_exists($function)) {
    $function($variables, $hook);
  }
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function labusinessportaltheme_preprocess_comment(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the region templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("region" in this case.)
 */
/* -- Delete this line if you want to use this function
function labusinessportaltheme_preprocess_region(&$variables, $hook) {
  // Don't use Zen's region--sidebar.tpl.php template for sidebars.
  //if (strpos($variables['region'], 'sidebar_') === 0) {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('region__sidebar'));
  //}
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function labusinessportaltheme_preprocess_block(&$variables, $hook) {
  // Add a count to all the blocks in the region.
  // $variables['classes_array'][] = 'count-' . $variables['block_id'];

  // By default, Zen will use the block--no-wrapper.tpl.php for the main
  // content. This optional bit of code undoes that:
  //if ($variables['block_html_id'] == 'block-system-main') {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('block__no_wrapper'));
  //}
}
// */
