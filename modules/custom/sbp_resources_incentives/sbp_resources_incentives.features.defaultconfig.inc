<?php
/**
 * @file
 * sbp_resources_incentives.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function sbp_resources_incentives_defaultconfig_features() {
  return array(
    'sbp_resources_incentives' => array(
      'facetapi_default_facet_settings' => 'facetapi_default_facet_settings',
      'strongarm' => 'strongarm',
      'views_default_views' => 'views_default_views',
    ),
  );
}

/**
 * Implements hook_defaultconfig_facetapi_default_facet_settings().
 */
function sbp_resources_incentives_defaultconfig_facetapi_default_facet_settings() {
  $export = array();

  return $export;
}

/**
 * Implements hook_defaultconfig_strongarm().
 */
function sbp_resources_incentives_defaultconfig_strongarm() {
  $export = array();

  return $export;
}

/**
 * Implements hook_defaultconfig_views_default_views().
 */
function sbp_resources_incentives_defaultconfig_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'sbp_resource_incentives';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_index_resources_incentives';
  $view->human_name = 'Business Portal Resource & Incentives';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'entity';
  /* Field: Indexed Node: Node ID */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'search_api_index_resources_incentives';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Filter criterion: Resource &amp; Incentives Full Text Search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_resources_incentives';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['ui_name'] = 'Resource & Incentives Full Text Search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['operator'] = 'OR';
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    30037204 => 0,
    134764228 => 0,
    103270763 => 0,
    175031666 => 0,
    197817655 => 0,
  );
  $handler->display->display_options['filters']['search_api_views_fulltext']['fields'] = array(
    'field_description' => 'field_description',
    'field_heading' => 'field_heading',
    'search_api_viewed' => 'search_api_viewed',
    'title' => 'title',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['exposed_block'] = TRUE;
  $export['sbp_resource_incentives'] = $view;

  return $export;
}
