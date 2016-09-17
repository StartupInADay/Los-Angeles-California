<?php
/**
 * @file
 * sbp_starter_kit.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function sbp_starter_kit_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:sbp_starter_kit:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'sbp_starter_kit';
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
  $display->uuid = 'ca5adb18-62d1-41f0-985d-d9aa8bfa74d0';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-66172876-ea54-4494-a827-9993bdc86518';
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
    $pane->uuid = '66172876-ea54-4494-a827-9993bdc86518';
    $display->content['new-66172876-ea54-4494-a827-9993bdc86518'] = $pane;
    $display->panels['left'][0] = 'new-66172876-ea54-4494-a827-9993bdc86518';
    $pane = new stdClass();
    $pane->pid = 'new-57964b36-4e02-4b7d-bb58-fc7d932f91f1';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_guide_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'panelizer',
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
    $pane->uuid = '57964b36-4e02-4b7d-bb58-fc7d932f91f1';
    $display->content['new-57964b36-4e02-4b7d-bb58-fc7d932f91f1'] = $pane;
    $display->panels['right'][0] = 'new-57964b36-4e02-4b7d-bb58-fc7d932f91f1';
    $pane = new stdClass();
    $pane->pid = 'new-df2bfeff-2fa3-47e5-8cb3-39aec329beca';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_guide_description';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'panelizer',
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
    $pane->uuid = 'df2bfeff-2fa3-47e5-8cb3-39aec329beca';
    $display->content['new-df2bfeff-2fa3-47e5-8cb3-39aec329beca'] = $pane;
    $display->panels['right'][1] = 'new-df2bfeff-2fa3-47e5-8cb3-39aec329beca';
    $pane = new stdClass();
    $pane->pid = 'new-10d20a04-427d-46a5-ac31-1ee81867036d';
    $pane->panel = 'right';
    $pane->type = 'views_panes';
    $pane->subtype = 'sbp_starter_kit-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '10d20a04-427d-46a5-ac31-1ee81867036d';
    $display->content['new-10d20a04-427d-46a5-ac31-1ee81867036d'] = $pane;
    $display->panels['right'][2] = 'new-10d20a04-427d-46a5-ac31-1ee81867036d';
    $pane = new stdClass();
    $pane->pid = 'new-14499bc3-b8d9-4ced-9ad4-62147b05c91b';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_checklist_description';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'panelizer',
      'override_title' => 1,
      'override_title_text' => 'Checklist',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '14499bc3-b8d9-4ced-9ad4-62147b05c91b';
    $display->content['new-14499bc3-b8d9-4ced-9ad4-62147b05c91b'] = $pane;
    $display->panels['right'][3] = 'new-14499bc3-b8d9-4ced-9ad4-62147b05c91b';
    $pane = new stdClass();
    $pane->pid = 'new-651c3c4e-ecbd-42ff-9533-01c0981197ed';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_checklist_items';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'field_collection_fields',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'view_mode' => 'full',
      ),
      'context' => 'panelizer',
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
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '651c3c4e-ecbd-42ff-9533-01c0981197ed';
    $display->content['new-651c3c4e-ecbd-42ff-9533-01c0981197ed'] = $pane;
    $display->panels['right'][4] = 'new-651c3c4e-ecbd-42ff-9533-01c0981197ed';
    $pane = new stdClass();
    $pane->pid = 'new-b34c4e26-d452-466f-89b4-c4f79b46e66e';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_permit_description';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'panelizer',
      'override_title' => 1,
      'override_title_text' => 'Permits',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = 'b34c4e26-d452-466f-89b4-c4f79b46e66e';
    $display->content['new-b34c4e26-d452-466f-89b4-c4f79b46e66e'] = $pane;
    $display->panels['right'][5] = 'new-b34c4e26-d452-466f-89b4-c4f79b46e66e';
    $pane = new stdClass();
    $pane->pid = 'new-be16bda6-825c-490e-9083-6aff037ea6f9';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_permit_section';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'field_collection_fields',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'view_mode' => 'full',
      ),
      'context' => 'panelizer',
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
    $pane->position = 6;
    $pane->locks = array();
    $pane->uuid = 'be16bda6-825c-490e-9083-6aff037ea6f9';
    $display->content['new-be16bda6-825c-490e-9083-6aff037ea6f9'] = $pane;
    $display->panels['right'][6] = 'new-be16bda6-825c-490e-9083-6aff037ea6f9';
    $pane = new stdClass();
    $pane->pid = 'new-c476ae30-69e5-466c-a102-f6fb98e76163';
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
    $pane->uuid = 'c476ae30-69e5-466c-a102-f6fb98e76163';
    $display->content['new-c476ae30-69e5-466c-a102-f6fb98e76163'] = $pane;
    $display->panels['top'][0] = 'new-c476ae30-69e5-466c-a102-f6fb98e76163';
    $pane = new stdClass();
    $pane->pid = 'new-802d0a3d-bab2-4842-8635-997141abe67d';
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
    $pane->uuid = '802d0a3d-bab2-4842-8635-997141abe67d';
    $display->content['new-802d0a3d-bab2-4842-8635-997141abe67d'] = $pane;
    $display->panels['top'][1] = 'new-802d0a3d-bab2-4842-8635-997141abe67d';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:sbp_starter_kit:default'] = $panelizer;

  return $export;
}
