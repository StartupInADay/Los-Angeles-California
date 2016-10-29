<?php
/**
 * @file
 * sbp_landing_page.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function sbp_landing_page_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view__panel_context_a5dcff65-418b-4dcd-9eaa-b4d3a5fdd737';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -97;
  $handler->conf = array(
    'title' => 'Sample Default Page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'name' => '',
    'access' => array(
      'logic' => 'and',
      'plugins' => array(
        0 => array(
          'name' => 'front',
          'settings' => NULL,
          'not' => TRUE,
        ),
      ),
    ),
  );
  $display = new panels_display();
  $display->layout = 'flexible';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '76f25bfc-0727-4705-ac69-cd177c971da9';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-0208eca6-43d0-4fb8-a647-dab6bec4ebc4';
    $pane->panel = 'center';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_image';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'image',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'image_style' => '',
        'image_link' => '',
      ),
      'context' => array(),
      'override_title' => 1,
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
    $pane->uuid = '0208eca6-43d0-4fb8-a647-dab6bec4ebc4';
    $display->content['new-0208eca6-43d0-4fb8-a647-dab6bec4ebc4'] = $pane;
    $display->panels['center'][0] = 'new-0208eca6-43d0-4fb8-a647-dab6bec4ebc4';
    $pane = new stdClass();
    $pane->pid = 'new-9c4537a3-c3d7-47ff-80ab-55bfa5c6803a';
    $pane->panel = 'center';
    $pane->type = 'node_title';
    $pane->subtype = 'node_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'link' => 0,
      'markup' => 'h1',
      'id' => '',
      'class' => '',
      'context' => array(),
      'override_title' => 1,
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
    $pane->uuid = '9c4537a3-c3d7-47ff-80ab-55bfa5c6803a';
    $display->content['new-9c4537a3-c3d7-47ff-80ab-55bfa5c6803a'] = $pane;
    $display->panels['center'][1] = 'new-9c4537a3-c3d7-47ff-80ab-55bfa5c6803a';
    $pane = new stdClass();
    $pane->pid = 'new-2046703c-731e-4f2e-805a-d385647556cd';
    $pane->panel = 'center';
    $pane->type = 'node_body';
    $pane->subtype = 'node_body';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(),
      'override_title' => 1,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => array(
        'title' => 'default',
        'css_class' => 'intro-text',
      ),
      'style' => 'demonstratie_panels_styles',
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '2046703c-731e-4f2e-805a-d385647556cd';
    $display->content['new-2046703c-731e-4f2e-805a-d385647556cd'] = $pane;
    $display->panels['center'][2] = 'new-2046703c-731e-4f2e-805a-d385647556cd';
    $pane = new stdClass();
    $pane->pid = 'new-db807968-3ad8-4abd-934e-dd4ccd131b97';
    $pane->panel = 'center';
    $pane->type = 'views_panes';
    $pane->subtype = 'landing_page-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_settings' => 'fields',
      'header_type' => 'none',
      'view_mode' => 'teaser',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'db807968-3ad8-4abd-934e-dd4ccd131b97';
    $display->content['new-db807968-3ad8-4abd-934e-dd4ccd131b97'] = $pane;
    $display->panels['center'][3] = 'new-db807968-3ad8-4abd-934e-dd4ccd131b97';
    $pane = new stdClass();
    $pane->pid = 'new-3be4a0f3-8811-4d78-b32a-c3d1ecf1dcd6';
    $pane->panel = 'center';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'entityreference_entity_view',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'link' => 0,
        'view_mode' => 'teaser',
        'links' => 1,
      ),
      'context' => array(),
      'override_title' => 1,
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
    $pane->uuid = '3be4a0f3-8811-4d78-b32a-c3d1ecf1dcd6';
    $display->content['new-3be4a0f3-8811-4d78-b32a-c3d1ecf1dcd6'] = $pane;
    $display->panels['center'][4] = 'new-3be4a0f3-8811-4d78-b32a-c3d1ecf1dcd6';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['node_view__panel_context_a5dcff65-418b-4dcd-9eaa-b4d3a5fdd737'] = $handler;

  return $export;
}
