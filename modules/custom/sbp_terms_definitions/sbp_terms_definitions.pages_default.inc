<?php
/**
 * @file
 * sbp_terms_definitions.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function sbp_terms_definitions_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view__panel_context_94b246b0-cdce-4218-87c7-1d9ec3c32dc3';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -22;
  $handler->conf = array(
    'title' => 'Terms and Definitions',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(
      0 => array(
        'identifier' => 'Multiple terms from node',
        'keyword' => 'terms',
        'name' => 'terms_from_node',
        'vocabulary' => array(
          'terms_definitions' => 'terms_definitions',
          'tags' => 0,
          'identifiers' => 0,
          'business_needs' => 0,
          'business_ownership' => 0,
          'business_portal_wizard_sections' => 0,
          'carousel' => 0,
          'department_type' => 0,
          'document_business_need' => 0,
          'document_business_stage' => 0,
          'document_type' => 0,
          'geographical_area' => 0,
          'industry_tags' => 0,
          'permit_government_level' => 0,
          'permit_issuing_department' => 0,
          'permit_tags' => 0,
          'question_category' => 0,
          'required_featured_items' => 0,
          'resource_type' => 0,
          'sbp_tags' => 0,
          'term_alphabet' => 0,
        ),
        'concatenator' => ',',
        'context' => 'argument_entity_id:node_1',
        'id' => 1,
      ),
    ),
    'name' => '',
    'access' => array(
      'logic' => 'and',
      'plugins' => array(
        0 => array(
          'name' => 'path_visibility',
          'settings' => array(
            'visibility_setting' => '1',
            'paths' => 'resources/terms-definitions',
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
  $display->uuid = '374e1c6e-3f91-41e0-ae73-04ff57cc6959';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-a18d527e-70ac-4b0e-8d89-90019ea58301';
    $pane->panel = 'left';
    $pane->type = 'block';
    $pane->subtype = 'sbp_terms_definitions-terms_definitions_grouping';
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
    $pane->uuid = 'a18d527e-70ac-4b0e-8d89-90019ea58301';
    $display->content['new-a18d527e-70ac-4b0e-8d89-90019ea58301'] = $pane;
    $display->panels['left'][0] = 'new-a18d527e-70ac-4b0e-8d89-90019ea58301';
    $pane = new stdClass();
    $pane->pid = 'new-1dacde95-d47b-4916-9f66-f5c78ed52ce8';
    $pane->panel = 'right';
    $pane->type = 'views_panes';
    $pane->subtype = 'terms_definitions-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_settings' => 'fields',
      'header_type' => 'none',
      'view_mode' => 'full',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '1dacde95-d47b-4916-9f66-f5c78ed52ce8';
    $display->content['new-1dacde95-d47b-4916-9f66-f5c78ed52ce8'] = $pane;
    $display->panels['right'][0] = 'new-1dacde95-d47b-4916-9f66-f5c78ed52ce8';
    $pane = new stdClass();
    $pane->pid = 'new-e301e670-4735-426d-a68b-f813df237c91';
    $pane->panel = 'top';
    $pane->type = 'node_content';
    $pane->subtype = 'node_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'links' => 0,
      'no_extras' => 1,
      'override_title' => 0,
      'override_title_text' => '',
      'identifier' => '',
      'link' => 0,
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
    $pane->uuid = 'e301e670-4735-426d-a68b-f813df237c91';
    $display->content['new-e301e670-4735-426d-a68b-f813df237c91'] = $pane;
    $display->panels['top'][0] = 'new-e301e670-4735-426d-a68b-f813df237c91';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['node_view__panel_context_94b246b0-cdce-4218-87c7-1d9ec3c32dc3'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function sbp_terms_definitions_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'terms_definitions';
  $page->task = 'page';
  $page->admin_title = 'Terms & Definitions';
  $page->admin_description = '';
  $page->path = 'resources/terms-definitions/grouping/!name';
  $page->access = array();
  $page->menu = array();
  $page->arguments = array(
    'name' => array(
      'id' => '',
      'identifier' => '',
      'argument' => '',
      'settings' => array(),
    ),
  );
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_terms_definitions__panel';
  $handler->task = 'page';
  $handler->subtask = 'terms_definitions';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'name' => 'panel',
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
  $display->uuid = '4f2c0087-fdec-4473-a67f-221a011f23a5';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-67ebf9fd-4e28-423a-a335-883e9f41ccaa';
    $pane->panel = 'right';
    $pane->type = 'views';
    $pane->subtype = 'terms_definitions';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '0',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 1,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'default',
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
    $pane->uuid = '67ebf9fd-4e28-423a-a335-883e9f41ccaa';
    $display->content['new-67ebf9fd-4e28-423a-a335-883e9f41ccaa'] = $pane;
    $display->panels['right'][0] = 'new-67ebf9fd-4e28-423a-a335-883e9f41ccaa';
    $pane = new stdClass();
    $pane->pid = 'new-b167c279-e120-4f4f-9645-2c2843fda4bf';
    $pane->panel = 'top';
    $pane->type = 'node';
    $pane->subtype = 'node';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'nid' => '7',
      'links' => 1,
      'leave_node_title' => 0,
      'identifier' => '',
      'build_mode' => 'teaser',
      'link_node_title' => 0,
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
    $pane->uuid = 'b167c279-e120-4f4f-9645-2c2843fda4bf';
    $display->content['new-b167c279-e120-4f4f-9645-2c2843fda4bf'] = $pane;
    $display->panels['top'][0] = 'new-b167c279-e120-4f4f-9645-2c2843fda4bf';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-b167c279-e120-4f4f-9645-2c2843fda4bf';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages[''] = $page;

  return $pages;

}
