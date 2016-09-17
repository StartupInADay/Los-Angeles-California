<?php
/**
 * @file
 * sbp_wizard.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function sbp_wizard_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'sbp_wizard';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Business Portal Wizard';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Business Portal Wizard';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Section (field_section) */
  $handler->display->display_options['relationships']['field_section_tid']['id'] = 'field_section_tid';
  $handler->display->display_options['relationships']['field_section_tid']['table'] = 'field_data_field_section';
  $handler->display->display_options['relationships']['field_section_tid']['field'] = 'field_section_tid';
  $handler->display->display_options['relationships']['field_section_tid']['required'] = TRUE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Content: Section */
  $handler->display->display_options['fields']['field_section']['id'] = 'field_section';
  $handler->display->display_options['fields']['field_section']['table'] = 'field_data_field_section';
  $handler->display->display_options['fields']['field_section']['field'] = 'field_section';
  $handler->display->display_options['fields']['field_section']['label'] = 'section';
  $handler->display->display_options['fields']['field_section']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_section']['type'] = 'services';
  $handler->display->display_options['fields']['field_section']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
    'term_name' => 1,
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Field: Description */
  $handler->display->display_options['fields']['field_description']['id'] = 'field_description';
  $handler->display->display_options['fields']['field_description']['table'] = 'field_data_field_description';
  $handler->display->display_options['fields']['field_description']['field'] = 'field_description';
  /* Field: Content: Button(s) */
  $handler->display->display_options['fields']['field_button']['id'] = 'field_button';
  $handler->display->display_options['fields']['field_button']['table'] = 'field_data_field_button';
  $handler->display->display_options['fields']['field_button']['field'] = 'field_button';
  $handler->display->display_options['fields']['field_button']['label'] = 'buttons';
  $handler->display->display_options['fields']['field_button']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_button']['settings'] = array(
    'edit' => '',
    'delete' => '',
    'add' => '',
    'description' => 0,
    'view_mode' => 'services',
  );
  $handler->display->display_options['fields']['field_button']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_button']['multi_type'] = 'ul';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'sbp_wizard_screen' => 'sbp_wizard_screen',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['enabled'] = FALSE;
  $handler->display->display_options['path'] = 'business-portal-wizard';

  /* Display: Services */
  $handler = $view->new_display('services', 'Services', 'wizard_services');
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Section (field_section) */
  $handler->display->display_options['relationships']['field_section_tid']['id'] = 'field_section_tid';
  $handler->display->display_options['relationships']['field_section_tid']['table'] = 'field_data_field_section';
  $handler->display->display_options['relationships']['field_section_tid']['field'] = 'field_section_tid';
  $handler->display->display_options['relationships']['field_section_tid']['required'] = TRUE;
  /* Relationship: Content: Section (field_section) */
  $handler->display->display_options['relationships']['field_section_tid_1']['id'] = 'field_section_tid_1';
  $handler->display->display_options['relationships']['field_section_tid_1']['table'] = 'field_data_field_section';
  $handler->display->display_options['relationships']['field_section_tid_1']['field'] = 'field_section_tid';
  /* Relationship: Taxonomy term: Content using Section */
  $handler->display->display_options['relationships']['reverse_field_section_node']['id'] = 'reverse_field_section_node';
  $handler->display->display_options['relationships']['reverse_field_section_node']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['relationships']['reverse_field_section_node']['field'] = 'reverse_field_section_node';
  $handler->display->display_options['relationships']['reverse_field_section_node']['relationship'] = 'field_section_tid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Content: Section */
  $handler->display->display_options['fields']['field_section']['id'] = 'field_section';
  $handler->display->display_options['fields']['field_section']['table'] = 'field_data_field_section';
  $handler->display->display_options['fields']['field_section']['field'] = 'field_section';
  $handler->display->display_options['fields']['field_section']['label'] = 'section';
  $handler->display->display_options['fields']['field_section']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_section']['type'] = 'services';
  $handler->display->display_options['fields']['field_section']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
    'term_name' => 1,
  );
  /* Field: Content: Screen Type */
  $handler->display->display_options['fields']['field_screen_type']['id'] = 'field_screen_type';
  $handler->display->display_options['fields']['field_screen_type']['table'] = 'field_data_field_screen_type';
  $handler->display->display_options['fields']['field_screen_type']['field'] = 'field_screen_type';
  $handler->display->display_options['fields']['field_screen_type']['label'] = 'screen-type';
  $handler->display->display_options['fields']['field_screen_type']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Field: Description */
  $handler->display->display_options['fields']['field_description']['id'] = 'field_description';
  $handler->display->display_options['fields']['field_description']['table'] = 'field_data_field_description';
  $handler->display->display_options['fields']['field_description']['field'] = 'field_description';
  /* Field: Content: Tip */
  $handler->display->display_options['fields']['field_tip']['id'] = 'field_tip';
  $handler->display->display_options['fields']['field_tip']['table'] = 'field_data_field_tip';
  $handler->display->display_options['fields']['field_tip']['field'] = 'field_tip';
  $handler->display->display_options['fields']['field_tip']['label'] = 'tip';
  $handler->display->display_options['fields']['field_tip']['element_label_colon'] = FALSE;
  /* Field: Content: Button(s) */
  $handler->display->display_options['fields']['field_button']['id'] = 'field_button';
  $handler->display->display_options['fields']['field_button']['table'] = 'field_data_field_button';
  $handler->display->display_options['fields']['field_button']['field'] = 'field_button';
  $handler->display->display_options['fields']['field_button']['label'] = 'buttons';
  $handler->display->display_options['fields']['field_button']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_button']['settings'] = array(
    'edit' => '',
    'delete' => '',
    'add' => '',
    'description' => 0,
    'view_mode' => 'services',
  );
  $handler->display->display_options['fields']['field_button']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_button']['multi_type'] = 'ul';
  /* Field: Taxonomy term: Primary Color */
  $handler->display->display_options['fields']['field_color']['id'] = 'field_color';
  $handler->display->display_options['fields']['field_color']['table'] = 'field_data_field_color';
  $handler->display->display_options['fields']['field_color']['field'] = 'field_color';
  $handler->display->display_options['fields']['field_color']['relationship'] = 'field_section_tid';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'field_section_tid';
  /* Field: Taxonomy term: Contextual Color */
  $handler->display->display_options['fields']['field_contextual_color']['id'] = 'field_contextual_color';
  $handler->display->display_options['fields']['field_contextual_color']['table'] = 'field_data_field_contextual_color';
  $handler->display->display_options['fields']['field_contextual_color']['field'] = 'field_contextual_color';
  $handler->display->display_options['fields']['field_contextual_color']['relationship'] = 'field_section_tid';
  /* Field: Taxonomy term: Drawer Color */
  $handler->display->display_options['fields']['field_drawer_color']['id'] = 'field_drawer_color';
  $handler->display->display_options['fields']['field_drawer_color']['table'] = 'field_data_field_drawer_color';
  $handler->display->display_options['fields']['field_drawer_color']['field'] = 'field_drawer_color';
  $handler->display->display_options['fields']['field_drawer_color']['relationship'] = 'field_section_tid';
  $handler->display->display_options['fields']['field_drawer_color']['label'] = 'drawer-color';
  $handler->display->display_options['fields']['field_drawer_color']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Illustration */
  $handler->display->display_options['fields']['field_illustration']['id'] = 'field_illustration';
  $handler->display->display_options['fields']['field_illustration']['table'] = 'field_data_field_illustration';
  $handler->display->display_options['fields']['field_illustration']['field'] = 'field_illustration';
  $handler->display->display_options['fields']['field_illustration']['relationship'] = 'field_section_tid';
  $handler->display->display_options['fields']['field_illustration']['label'] = 'illustration';
  $handler->display->display_options['fields']['field_illustration']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_illustration']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_illustration']['type'] = 'file_image_file_url_plain';
  $handler->display->display_options['fields']['field_illustration']['settings'] = array(
    'image_style' => '',
    'image_link' => '',
  );
  /* Field: Field: Icon */
  $handler->display->display_options['fields']['field_image']['id'] = 'field_image';
  $handler->display->display_options['fields']['field_image']['table'] = 'field_data_field_image';
  $handler->display->display_options['fields']['field_image']['field'] = 'field_image';
  $handler->display->display_options['fields']['field_image']['relationship'] = 'field_section_tid';
  $handler->display->display_options['fields']['field_image']['label'] = 'icon';
  $handler->display->display_options['fields']['field_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_image']['type'] = 'file_image_file_url_plain';
  $handler->display->display_options['fields']['field_image']['settings'] = array(
    'image_style' => '',
    'image_link' => '',
  );
  /* Field: Taxonomy term: Jurisdiction */
  $handler->display->display_options['fields']['field_jurisdiction']['id'] = 'field_jurisdiction';
  $handler->display->display_options['fields']['field_jurisdiction']['table'] = 'field_data_field_jurisdiction';
  $handler->display->display_options['fields']['field_jurisdiction']['field'] = 'field_jurisdiction';
  $handler->display->display_options['fields']['field_jurisdiction']['relationship'] = 'field_section_tid';
  $handler->display->display_options['fields']['field_jurisdiction']['label'] = 'jurisdiction';
  $handler->display->display_options['fields']['field_jurisdiction']['element_label_colon'] = FALSE;
  $handler->display->display_options['path'] = 'business-portal-wizard';
  $export['sbp_wizard'] = $view;

  return $export;
}
