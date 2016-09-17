<?php
/**
 * @file
 * sbp_department.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function sbp_department_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:sbp_department:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'sbp_department';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = '';
  $panelizer->css = '';
  $panelizer->pipeline = 'ipe';
  $panelizer->contexts = array();
  $panelizer->relationships = array();
  $panelizer->access = array();
  $panelizer->view_mode = 'page_manager';
  $panelizer->css_class = '';
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = array();
  $display = new panels_display();
  $display->layout = 'twocol_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = '3526857c-1638-43f5-a102-4962d6000c81';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-f8bcaf72-ed1e-42d2-a54a-3e6fed27a776';
    $pane->panel = 'left';
    $pane->type = 'block';
    $pane->subtype = 'menu_block-1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'f8bcaf72-ed1e-42d2-a54a-3e6fed27a776';
    $display->content['new-f8bcaf72-ed1e-42d2-a54a-3e6fed27a776'] = $pane;
    $display->panels['left'][0] = 'new-f8bcaf72-ed1e-42d2-a54a-3e6fed27a776';
    $pane = new stdClass();
    $pane->pid = 'new-cb682fad-ff5c-46d8-b42c-9739147b5e9d';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_description';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'quickedit' => array(
          'editor' => 'plain_text',
        ),
      ),
      'context' => 'panelizer',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'cb682fad-ff5c-46d8-b42c-9739147b5e9d';
    $display->content['new-cb682fad-ff5c-46d8-b42c-9739147b5e9d'] = $pane;
    $display->panels['right'][0] = 'new-cb682fad-ff5c-46d8-b42c-9739147b5e9d';
    $pane = new stdClass();
    $pane->pid = 'new-6553d2b8-094d-4844-95fd-42b373fe62d7';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_department_type';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'hidden',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'panelizer',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '6553d2b8-094d-4844-95fd-42b373fe62d7';
    $display->content['new-6553d2b8-094d-4844-95fd-42b373fe62d7'] = $pane;
    $display->panels['right'][1] = 'new-6553d2b8-094d-4844-95fd-42b373fe62d7';
    $pane = new stdClass();
    $pane->pid = 'new-3b9dcaf0-c161-466c-9e49-0c51c7187c05';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_addresses';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'field_collection_view',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'add' => 'Add',
        'delete' => 'Delete',
        'description' => TRUE,
        'edit' => 'Edit',
        'quickedit' => array(
          'editor' => 'form',
        ),
        'view_mode' => 'full',
      ),
      'context' => 'panelizer',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '3b9dcaf0-c161-466c-9e49-0c51c7187c05';
    $display->content['new-3b9dcaf0-c161-466c-9e49-0c51c7187c05'] = $pane;
    $display->panels['right'][2] = 'new-3b9dcaf0-c161-466c-9e49-0c51c7187c05';
    $pane = new stdClass();
    $pane->pid = 'new-399a6206-9c84-4ce4-919c-cde39f9354b4';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_link';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'link_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'quickedit' => array(
          'editor' => 'form',
        ),
      ),
      'context' => 'panelizer',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '399a6206-9c84-4ce4-919c-cde39f9354b4';
    $display->content['new-399a6206-9c84-4ce4-919c-cde39f9354b4'] = $pane;
    $display->panels['right'][3] = 'new-399a6206-9c84-4ce4-919c-cde39f9354b4';
    $pane = new stdClass();
    $pane->pid = 'new-a892d50a-bf7c-4b9a-8c7a-26fed0b17a88';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_phone';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'quickedit' => array(
          'editor' => 'plain_text',
        ),
      ),
      'context' => 'panelizer',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = 'a892d50a-bf7c-4b9a-8c7a-26fed0b17a88';
    $display->content['new-a892d50a-bf7c-4b9a-8c7a-26fed0b17a88'] = $pane;
    $display->panels['right'][4] = 'new-a892d50a-bf7c-4b9a-8c7a-26fed0b17a88';
    $pane = new stdClass();
    $pane->pid = 'new-f46e8305-da76-4cde-98d1-1ba598633fc0';
    $pane->panel = 'top';
    $pane->type = 'node_links';
    $pane->subtype = 'node_links';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => FALSE,
      'override_title_text' => '',
      'build_mode' => 'page_manager',
      'identifier' => '',
      'link' => TRUE,
      'context' => 'panelizer',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_class' => 'link-wrapper',
    );
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'f46e8305-da76-4cde-98d1-1ba598633fc0';
    $display->content['new-f46e8305-da76-4cde-98d1-1ba598633fc0'] = $pane;
    $display->panels['top'][0] = 'new-f46e8305-da76-4cde-98d1-1ba598633fc0';
    $pane = new stdClass();
    $pane->pid = 'new-40b2b29d-e2ed-4485-9f21-37714b757552';
    $pane->panel = 'top';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_hero';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'field_collection_fields',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'view_mode' => 'full',
      ),
      'context' => array(),
      'override_title' => 0,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '40b2b29d-e2ed-4485-9f21-37714b757552';
    $display->content['new-40b2b29d-e2ed-4485-9f21-37714b757552'] = $pane;
    $display->panels['top'][1] = 'new-40b2b29d-e2ed-4485-9f21-37714b757552';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-cb682fad-ff5c-46d8-b42c-9739147b5e9d';
  $panelizer->display = $display;
  $export['node:sbp_department:default'] = $panelizer;

  return $export;
}
