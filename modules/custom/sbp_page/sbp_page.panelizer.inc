<?php
/**
 * @file
 * sbp_page.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function sbp_page_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:sbp_basic_page:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'sbp_basic_page';
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
  $display->uuid = 'f4493eff-20a9-4e7c-8b04-7c91cbd962d3';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-9739c21a-7908-4931-9b38-9a8c9b9cdfd9';
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
    $pane->uuid = '9739c21a-7908-4931-9b38-9a8c9b9cdfd9';
    $display->content['new-9739c21a-7908-4931-9b38-9a8c9b9cdfd9'] = $pane;
    $display->panels['left'][0] = 'new-9739c21a-7908-4931-9b38-9a8c9b9cdfd9';
    $pane = new stdClass();
    $pane->pid = 'new-4be2b67a-d95c-4925-8d43-df76801ab721';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:body';
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
    $pane->uuid = '4be2b67a-d95c-4925-8d43-df76801ab721';
    $display->content['new-4be2b67a-d95c-4925-8d43-df76801ab721'] = $pane;
    $display->panels['right'][0] = 'new-4be2b67a-d95c-4925-8d43-df76801ab721';
    $pane = new stdClass();
    $pane->pid = 'new-71e5bf08-a7a1-4619-ab4c-ab8cfad02578';
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
    $pane->uuid = '71e5bf08-a7a1-4619-ab4c-ab8cfad02578';
    $display->content['new-71e5bf08-a7a1-4619-ab4c-ab8cfad02578'] = $pane;
    $display->panels['top'][0] = 'new-71e5bf08-a7a1-4619-ab4c-ab8cfad02578';
    $pane = new stdClass();
    $pane->pid = 'new-d9740886-1e0d-4387-b89b-67f7e0f71baf';
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
    $pane->uuid = 'd9740886-1e0d-4387-b89b-67f7e0f71baf';
    $display->content['new-d9740886-1e0d-4387-b89b-67f7e0f71baf'] = $pane;
    $display->panels['top'][1] = 'new-d9740886-1e0d-4387-b89b-67f7e0f71baf';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-4be2b67a-d95c-4925-8d43-df76801ab721';
  $panelizer->display = $display;
  $export['node:sbp_basic_page:default'] = $panelizer;

  return $export;
}
