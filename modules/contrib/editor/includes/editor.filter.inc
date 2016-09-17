<?php

/**
 * @file
 * Declares hooks on behalf of filter.module.
 */

/**
 * Implements hook_library() on behalf of filter.module.
 *
 * Filter does not register its own assets as libraries, so we do so here.
 */
function filter_library() {
  $path = drupal_get_path('module', 'editor');

  $libraries['filter'] = array(
    'version' => VERSION,
    'js' => array(
      $path . '/js/filter/filter.js' => array(),
    ),
    'css' => array(
      $path . '/css/filter/filter.css' => array(),
    ),
    'dependencies' => array(
      array('system', 'drupal.ajax'),
    ),
  );

  $libraries['filter.filtered_html.admin'] = array(
    'version' => VERSION,
    'js' => array(
      $path . '/js/filter/filter.filtered_html.admin.js' => array('group' => JS_THEME, 'aggregate' => FALSE),
    ),
    'dependencies' => array(
      array('filter', 'filter.admin'),
    ),
  );

  // Filter admin is put in the JS_THEME group to force it after tabledrag.js.
  $libraries['filter.admin'] = array(
    'version' => VERSION,
    'js' => array(
      $path . '/js/filter/filter.admin.js' => array('group' => JS_THEME, 'aggregate' => FALSE),
    ),
    'css' => array(
      $path . '/css/filter/filter.css' => array(),
    ),
  );

  return $libraries;
}

/**
 * Implements hook_js_alter() on behalf of filter.module.
 *
 * Replace Filter's JS files with more fully-featured alternatives.
 */
function filter_js_alter(&$javascript) {
  // Replace filter.js.
  $path = drupal_get_path('module', 'filter') . '/filter.js';

  if (isset($javascript[$path])) {
    $javascript[$path]['data'] = drupal_get_path('module', 'editor') . '/js/filter/filter.js';
    $javascript[$path]['type'] = 'file';
  }

  // Replace filter.admin.js.
  $path = drupal_get_path('module', 'filter') . '/filter.admin.js';

  if (isset($javascript[$path])) {
    $javascript[$path]['data'] = drupal_get_path('module', 'editor') . '/js/filter/filter.admin.js';
    $javascript[$path]['type'] = 'file';
  }
}

/**
 * Implements hook_css_alter() on behalf of filter.module.
 *
 * Replace Filter's CSS files with more fully-featured alternatives.
 */
function filter_css_alter(&$css) {
  // Replace filter.css.
  $path = drupal_get_path('module', 'filter') . '/filter.css';

  if (isset($javascript[$path])) {
    $css[$path]['data'] = drupal_get_path('module', 'editor') . '/css/filter.css';
    $css[$path]['type'] = 'file';
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Adds editor support to the text format overview form.
 */
function editor_form_filter_admin_overview_alter(&$form, &$form_state, $form_id) {
  // Overview of all formats.
  $formats = filter_formats();
  $fallback_format = filter_fallback_format();
  $editors = editor_get_editors();

  $form['#tree'] = TRUE;
  foreach ($formats as $id => $format) {
    // Check whether this is the fallback text format. This format is available
    // to all roles and cannot be disabled via the admin interface.
    $form['formats'][$id]['#is_fallback'] = ($id == $fallback_format);
    if ($form['formats'][$id]['#is_fallback']) {
      $form['formats'][$id]['name'] = array('#markup' => drupal_placeholder($format->name));
      $roles_markup = drupal_placeholder(t('All roles may use this format'));
    }
    else {
      $form['formats'][$id]['name'] = array('#markup' => check_plain($format->name));
      $roles = array_map('check_plain', filter_get_roles_by_format($format));
      $roles_markup = $roles ? implode(', ', $roles) : t('No roles may use this format');
    }
    $form['formats'][$id]['editor'] = array('#markup' => isset($editors[$format->editor]) ? check_plain($editors[$format->editor]['label']) : t('None'));
    $form['formats'][$id]['roles'] = array('#markup' => $roles_markup);
    $form['formats'][$id]['configure'] = array('#type' => 'link', '#title' => t('configure'), '#href' => 'admin/config/content/formats/' . $id);
    $form['formats'][$id]['disable'] = array('#type' => 'link', '#title' => t('disable'), '#href' => 'admin/config/content/formats/' . $id . '/disable', '#access' => !$form['formats'][$id]['#is_fallback']);
    $form['formats'][$id]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $format->name)),
      '#title_display' => 'invisible',
      '#default_value' => $format->weight,
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));
}
