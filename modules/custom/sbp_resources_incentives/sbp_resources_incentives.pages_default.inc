<?php
/**
 * @file
 * sbp_resources_incentives.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function sbp_resources_incentives_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view__panel_context_bc081167-0c22-474e-9a5e-e1e70dbd9e1a';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -27;
  $handler->conf = array(
    'title' => 'Resource and Incentives',
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
      'plugins' => array(
        0 => array(
          'name' => 'path_visibility',
          'settings' => array(
            'visibility_setting' => '1',
            'paths' => 'resources/resources-incentives',
          ),
          'context' => 'empty',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
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
  $display->uuid = '4223f4f3-6be6-4516-825b-2d2cdc392cca';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-f1d5eedb-4ea4-4205-83a4-271f9b14522e';
    $pane->panel = 'left';
    $pane->type = 'block';
    $pane->subtype = 'menu_block-1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => '<none>',
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
    $pane->uuid = 'f1d5eedb-4ea4-4205-83a4-271f9b14522e';
    $display->content['new-f1d5eedb-4ea4-4205-83a4-271f9b14522e'] = $pane;
    $display->panels['left'][0] = 'new-f1d5eedb-4ea4-4205-83a4-271f9b14522e';
    $pane = new stdClass();
    $pane->pid = 'new-fca7479d-bad9-481e-b6f7-ad4c02e59248';
    $pane->panel = 'left';
    $pane->type = 'block';
    $pane->subtype = 'views-7ec551585f34d5faf92f3452b90809d4';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'inherit_path' => 1,
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
    $pane->uuid = 'fca7479d-bad9-481e-b6f7-ad4c02e59248';
    $display->content['new-fca7479d-bad9-481e-b6f7-ad4c02e59248'] = $pane;
    $display->panels['left'][1] = 'new-fca7479d-bad9-481e-b6f7-ad4c02e59248';
    $pane = new stdClass();
    $pane->pid = 'new-65e195fe-ce69-420e-87c0-f15fa673b0ca';
    $pane->panel = 'left';
    $pane->type = 'block';
    $pane->subtype = 'facetapi-eZL7XTwV0MlFlPwlwCAzjdRIBpwHUu1W';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => 'Business Need',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '65e195fe-ce69-420e-87c0-f15fa673b0ca';
    $display->content['new-65e195fe-ce69-420e-87c0-f15fa673b0ca'] = $pane;
    $display->panels['left'][2] = 'new-65e195fe-ce69-420e-87c0-f15fa673b0ca';
    $pane = new stdClass();
    $pane->pid = 'new-c924b80f-f67b-439c-9e38-e31854c69e8f';
    $pane->panel = 'left';
    $pane->type = 'block';
    $pane->subtype = 'facetapi-i0ABb7Ee3JVdTmlkrtpUfFAY42jkcuji';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => 'Business Ownership',
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
    $pane->uuid = 'c924b80f-f67b-439c-9e38-e31854c69e8f';
    $display->content['new-c924b80f-f67b-439c-9e38-e31854c69e8f'] = $pane;
    $display->panels['left'][3] = 'new-c924b80f-f67b-439c-9e38-e31854c69e8f';
    $pane = new stdClass();
    $pane->pid = 'new-bab172bc-bee5-46e4-8404-3369269dc62d';
    $pane->panel = 'left';
    $pane->type = 'block';
    $pane->subtype = 'facetapi-aOSrQ4ZSfXMgOqIbMghaNuDFpQu7cM4j';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => 'Industry',
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
    $pane->uuid = 'bab172bc-bee5-46e4-8404-3369269dc62d';
    $display->content['new-bab172bc-bee5-46e4-8404-3369269dc62d'] = $pane;
    $display->panels['left'][4] = 'new-bab172bc-bee5-46e4-8404-3369269dc62d';
    $pane = new stdClass();
    $pane->pid = 'new-1762375d-4ca1-4277-be2d-cf7e7e22496b';
    $pane->panel = 'left';
    $pane->type = 'block';
    $pane->subtype = 'facetapi-5oVYr0th2DwK1loKsfoeapzfOLl45AXf';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => 'Resource Type',
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
    $pane->uuid = '1762375d-4ca1-4277-be2d-cf7e7e22496b';
    $display->content['new-1762375d-4ca1-4277-be2d-cf7e7e22496b'] = $pane;
    $display->panels['left'][5] = 'new-1762375d-4ca1-4277-be2d-cf7e7e22496b';
    $pane = new stdClass();
    $pane->pid = 'new-5cd07851-64b7-4e20-b41b-393aa4f8554b';
    $pane->panel = 'right';
    $pane->type = 'views_panes';
    $pane->subtype = 'sbp_resource_incentives-panel_pane_1';
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
    $pane->uuid = '5cd07851-64b7-4e20-b41b-393aa4f8554b';
    $display->content['new-5cd07851-64b7-4e20-b41b-393aa4f8554b'] = $pane;
    $display->panels['right'][0] = 'new-5cd07851-64b7-4e20-b41b-393aa4f8554b';
    $pane = new stdClass();
    $pane->pid = 'new-8c1e5dfc-dc23-4acc-a5c4-bb68d9a8f608';
    $pane->panel = 'right';
    $pane->type = 'views';
    $pane->subtype = 'bp_featured_sub_page';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '0',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block_main_featured_r_and_i',
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
    $pane->uuid = '8c1e5dfc-dc23-4acc-a5c4-bb68d9a8f608';
    $display->content['new-8c1e5dfc-dc23-4acc-a5c4-bb68d9a8f608'] = $pane;
    $display->panels['right'][1] = 'new-8c1e5dfc-dc23-4acc-a5c4-bb68d9a8f608';
    $pane = new stdClass();
    $pane->pid = 'new-13925a74-eff5-4fc3-b5f7-83cbcc1f17bb';
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
      'build_mode' => 'teaser',
      'context' => array(),
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
    $pane->uuid = '13925a74-eff5-4fc3-b5f7-83cbcc1f17bb';
    $display->content['new-13925a74-eff5-4fc3-b5f7-83cbcc1f17bb'] = $pane;
    $display->panels['top'][0] = 'new-13925a74-eff5-4fc3-b5f7-83cbcc1f17bb';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-13925a74-eff5-4fc3-b5f7-83cbcc1f17bb';
  $handler->conf['display'] = $display;
  $export['node_view__panel_context_bc081167-0c22-474e-9a5e-e1e70dbd9e1a'] = $handler;

  return $export;
}
