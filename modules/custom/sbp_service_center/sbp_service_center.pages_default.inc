<?php
/**
 * @file
 * sbp_service_center.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function sbp_service_center_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view__service-centers';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -26;
  $handler->conf = array(
    'title' => 'Service Centers',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'name' => 'service-centers',
    'access' => array(
      'logic' => 'and',
      'plugins' => array(
        0 => array(
          'name' => 'path_visibility',
          'settings' => array(
            'visibility_setting' => '1',
            'paths' => 'resources/service-centers',
          ),
          'context' => 'empty',
          'not' => FALSE,
        ),
      ),
    ),
  );
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
  $display->title = '';
  $display->uuid = '9f303ccb-71ca-4c6c-a1b0-67ac15a0c43a';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-bc1d7e89-bc71-4c2f-ab04-97cbaed62bdb';
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
    $pane->uuid = 'bc1d7e89-bc71-4c2f-ab04-97cbaed62bdb';
    $display->content['new-bc1d7e89-bc71-4c2f-ab04-97cbaed62bdb'] = $pane;
    $display->panels['left'][0] = 'new-bc1d7e89-bc71-4c2f-ab04-97cbaed62bdb';
    $pane = new stdClass();
    $pane->pid = 'new-bb102333-b1e5-402d-9a21-9bc4986295d1';
    $pane->panel = 'right';
    $pane->type = 'views_panes';
    $pane->subtype = 'labp_service_center-panel_pane_1';
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'bb102333-b1e5-402d-9a21-9bc4986295d1';
    $display->content['new-bb102333-b1e5-402d-9a21-9bc4986295d1'] = $pane;
    $display->panels['right'][0] = 'new-bb102333-b1e5-402d-9a21-9bc4986295d1';
    $pane = new stdClass();
    $pane->pid = 'new-27576cec-c834-416a-aef3-92974627f0a8';
    $pane->panel = 'top';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'vid:1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'bundle' => 'hero',
        'language' => 'und',
        'is_new' => TRUE,
        'title' => '',
        'link' => 0,
        'path' => '',
        'reusable' => 0,
        'admin_title' => '',
        'admin_description' => '',
        'category' => 'Reusable panes',
        'vid' => '1',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '1',
        'field_background' => array(
          'und' => array(
            0 => array(
              'value' => 0,
            ),
          ),
        ),
        'field_background_color' => array(
          'und' => array(
            0 => array(
              'value' => 'ffffff',
            ),
          ),
        ),
        'field_background_image' => array(
          'und' => array(),
        ),
        'field_calls_to_action' => array(
          'und' => array(),
        ),
        'field_icon' => array(
          'und' => array(),
        ),
        'field_line_and_subtitle_color' => array(
          'und' => array(
            0 => array(
              'value' => '000',
            ),
          ),
        ),
        'field_subtitle_1' => array(
          'und' => array(),
        ),
        'field_subtitle_2' => array(
          'und' => array(),
        ),
        'field_width' => array(
          'und' => array(),
        ),
        'log' => '',
        'created' => 1460048311,
        'timestamp' => 1460048311,
        'changed' => 1460048311,
        'uuid' => '',
        'uid' => '1',
        'vuuid' => '',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '27576cec-c834-416a-aef3-92974627f0a8';
    $display->content['new-27576cec-c834-416a-aef3-92974627f0a8'] = $pane;
    $display->panels['top'][0] = 'new-27576cec-c834-416a-aef3-92974627f0a8';
    $pane = new stdClass();
    $pane->pid = 'new-1977a855-1541-49e5-8610-cf5dd88a5dcc';
    $pane->panel = 'top';
    $pane->type = 'node_content';
    $pane->subtype = 'node_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'links' => 1,
      'no_extras' => 1,
      'override_title' => 0,
      'override_title_text' => '',
      'identifier' => '',
      'link' => 1,
      'leave_node_title' => 0,
      'build_mode' => 'full',
      'context' => array(),
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
    $pane->uuid = '1977a855-1541-49e5-8610-cf5dd88a5dcc';
    $display->content['new-1977a855-1541-49e5-8610-cf5dd88a5dcc'] = $pane;
    $display->panels['top'][1] = 'new-1977a855-1541-49e5-8610-cf5dd88a5dcc';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['node_view__service-centers'] = $handler;

  return $export;
}
