<?php
/**
 * @file
 * sbp_starter_kit.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function sbp_starter_kit_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'sbp_starter_kit';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Business Starter Kit';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
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
    'sbp_starter_kit' => 'sbp_starter_kit',
  );

  /* Display: Starter Kit Documents */
  $handler = $view->new_display('panel_pane', 'Starter Kit Documents', 'sbp_starter_kit_docs');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'field_document_type',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Field: Document (field_document:fid) */
  $handler->display->display_options['relationships']['field_document_fid']['id'] = 'field_document_fid';
  $handler->display->display_options['relationships']['field_document_fid']['table'] = 'field_data_field_document';
  $handler->display->display_options['relationships']['field_document_fid']['field'] = 'field_document_fid';
  $handler->display->display_options['relationships']['field_document_fid']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: File: Document Type */
  $handler->display->display_options['fields']['field_document_type']['id'] = 'field_document_type';
  $handler->display->display_options['fields']['field_document_type']['table'] = 'field_data_field_document_type';
  $handler->display->display_options['fields']['field_document_type']['field'] = 'field_document_type';
  $handler->display->display_options['fields']['field_document_type']['relationship'] = 'field_document_fid';
  $handler->display->display_options['fields']['field_document_type']['label'] = '';
  $handler->display->display_options['fields']['field_document_type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_document_type']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_document_type']['alter']['text'] = '[field_document_type]s';
  $handler->display->display_options['fields']['field_document_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_document_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['relationship'] = 'field_document_fid';
  $handler->display->display_options['fields']['filename']['label'] = '';
  $handler->display->display_options['fields']['filename']['element_class'] = 'icon-download button--link';
  $handler->display->display_options['fields']['filename']['element_label_colon'] = FALSE;
  /* Field: Field: Description */
  $handler->display->display_options['fields']['field_description']['id'] = 'field_description';
  $handler->display->display_options['fields']['field_description']['table'] = 'field_data_field_description';
  $handler->display->display_options['fields']['field_description']['field'] = 'field_description';
  $handler->display->display_options['fields']['field_description']['relationship'] = 'field_document_fid';
  $handler->display->display_options['fields']['field_description']['label'] = '';
  $handler->display->display_options['fields']['field_description']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;

  /* Display: Starter Kits */
  $handler = $view->new_display('panel_pane', 'Starter Kits', 'sbp_starter_kits');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Starter Kits';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;

  /* Display: Permits */
  $handler = $view->new_display('panel_pane', 'Permits', 'sbp_starter_kit_permits');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Permits';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Broken/missing handler */
  $handler->display->display_options['relationships']['field_related_permits_target_id']['id'] = 'field_related_permits_target_id';
  $handler->display->display_options['relationships']['field_related_permits_target_id']['table'] = 'field_data_field_related_permits';
  $handler->display->display_options['relationships']['field_related_permits_target_id']['field'] = 'field_related_permits_target_id';
  $handler->display->display_options['relationships']['field_related_permits_target_id']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_related_permits_target_id';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Field: Description */
  $handler->display->display_options['fields']['field_description']['id'] = 'field_description';
  $handler->display->display_options['fields']['field_description']['table'] = 'field_data_field_description';
  $handler->display->display_options['fields']['field_description']['field'] = 'field_description';
  $handler->display->display_options['fields']['field_description']['relationship'] = 'field_related_permits_target_id';
  $handler->display->display_options['fields']['field_description']['label'] = '';
  $handler->display->display_options['fields']['field_description']['element_label_colon'] = FALSE;

  /* Display: Permits and Guides Download */
  $handler = $view->new_display('panel_pane', 'Permits and Guides Download', 'panel_pane_1');
  $handler->display->display_options['display_description'] = 'Images & Download links for the permits and guides';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class'] = 'permit-guides-wrapper';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Checklist Image */
  $handler->display->display_options['fields']['field_guide_image']['id'] = 'field_guide_image';
  $handler->display->display_options['fields']['field_guide_image']['table'] = 'field_data_field_guide_image';
  $handler->display->display_options['fields']['field_guide_image']['field'] = 'field_guide_image';
  $handler->display->display_options['fields']['field_guide_image']['label'] = '';
  $handler->display->display_options['fields']['field_guide_image']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_guide_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_guide_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_guide_image']['settings'] = array(
    'image_style' => '',
    'image_link' => '',
  );
  /* Field: Content: Checklist PDF */
  $handler->display->display_options['fields']['field_guide_pdf']['id'] = 'field_guide_pdf';
  $handler->display->display_options['fields']['field_guide_pdf']['table'] = 'field_data_field_guide_pdf';
  $handler->display->display_options['fields']['field_guide_pdf']['field'] = 'field_guide_pdf';
  $handler->display->display_options['fields']['field_guide_pdf']['label'] = '';
  $handler->display->display_options['fields']['field_guide_pdf']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_guide_pdf']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_guide_pdf']['alter']['text'] = 'Download Checklist';
  $handler->display->display_options['fields']['field_guide_pdf']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_guide_pdf']['alter']['path'] = '[field_guide_pdf]';
  $handler->display->display_options['fields']['field_guide_pdf']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['field_guide_pdf']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_guide_pdf']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_guide_pdf']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_guide_pdf']['type'] = 'file_url_plain';
  $handler->display->display_options['fields']['field_guide_pdf']['settings'] = array(
    'text' => 'Download Checklist',
  );
  /* Field: Content: Permit Image */
  $handler->display->display_options['fields']['field_permit_image']['id'] = 'field_permit_image';
  $handler->display->display_options['fields']['field_permit_image']['table'] = 'field_data_field_permit_image';
  $handler->display->display_options['fields']['field_permit_image']['field'] = 'field_permit_image';
  $handler->display->display_options['fields']['field_permit_image']['label'] = '';
  $handler->display->display_options['fields']['field_permit_image']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_permit_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_permit_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_permit_image']['settings'] = array(
    'image_style' => '',
    'image_link' => '',
  );
  /* Field: Content: Permit PDF */
  $handler->display->display_options['fields']['field_permit_pdf']['id'] = 'field_permit_pdf';
  $handler->display->display_options['fields']['field_permit_pdf']['table'] = 'field_data_field_permit_pdf';
  $handler->display->display_options['fields']['field_permit_pdf']['field'] = 'field_permit_pdf';
  $handler->display->display_options['fields']['field_permit_pdf']['label'] = '';
  $handler->display->display_options['fields']['field_permit_pdf']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_permit_pdf']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_permit_pdf']['alter']['text'] = 'Download Permit List';
  $handler->display->display_options['fields']['field_permit_pdf']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_permit_pdf']['alter']['path'] = '[field_permit_pdf]';
  $handler->display->display_options['fields']['field_permit_pdf']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['field_permit_pdf']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_permit_pdf']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_permit_pdf']['type'] = 'file_url_plain';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class = "page__starter-kit-download--block>
  <div class = "page__starter-kit-download">
    <div class = "page__starter-kit--icon">[field_guide_image]</div>
    <div class = "page__button">[field_guide_pdf]</div>
  </div>
    <div class = "page__starter-kit-download">
    <div class = "page__starter-kit--icon">[field_permit_image]</div>
    <div class = "page__button">[field_permit_pdf]</div>
  </div>
</div>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'sbp_starter_kit' => 'sbp_starter_kit',
  );
  $export['sbp_starter_kit'] = $view;

  return $export;
}
