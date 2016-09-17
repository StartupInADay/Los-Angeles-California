<?php
/**
 * @file
 * sbp_starter_kit.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function sbp_starter_kit_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view__panel_context_f43af71f-fd46-44b0-9735-6cb69c73ddbd';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -96;
  $handler->conf = array(
    'title' => 'Starter Kit',
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
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'sbp_starter_kit' => 'sbp_starter_kit',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
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
  $display->uuid = 'f97e9b26-0249-4a59-a99d-506c85cb048b';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-9c782649-0db8-4b06-976a-ee0b84731598';
    $pane->panel = 'left';
    $pane->type = 'views_panes';
    $pane->subtype = 'sbp_starter_kit-sbp_starter_kits';
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
    $pane->uuid = '9c782649-0db8-4b06-976a-ee0b84731598';
    $display->content['new-9c782649-0db8-4b06-976a-ee0b84731598'] = $pane;
    $display->panels['left'][0] = 'new-9c782649-0db8-4b06-976a-ee0b84731598';
    $pane = new stdClass();
    $pane->pid = 'new-d235d76d-e28c-4678-b791-a9a76786b984';
    $pane->panel = 'right';
    $pane->type = 'views_panes';
    $pane->subtype = 'sbp_starter_kit-sbp_starter_kit_docs';
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
    $pane->uuid = 'd235d76d-e28c-4678-b791-a9a76786b984';
    $display->content['new-d235d76d-e28c-4678-b791-a9a76786b984'] = $pane;
    $display->panels['right'][0] = 'new-d235d76d-e28c-4678-b791-a9a76786b984';
    $pane = new stdClass();
    $pane->pid = 'new-0657f4e2-3cdf-4f1d-a5ee-9803d5cfd4da';
    $pane->panel = 'right';
    $pane->type = 'views_panes';
    $pane->subtype = 'sbp_starter_kit-sbp_starter_kit_permits';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '0657f4e2-3cdf-4f1d-a5ee-9803d5cfd4da';
    $display->content['new-0657f4e2-3cdf-4f1d-a5ee-9803d5cfd4da'] = $pane;
    $display->panels['right'][1] = 'new-0657f4e2-3cdf-4f1d-a5ee-9803d5cfd4da';
    $pane = new stdClass();
    $pane->pid = 'new-71d545f2-039c-46a3-b2b9-9a0de908708a';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_starter_kit_key_points';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'field_collection_view',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'edit' => 'Edit',
        'delete' => 'Delete',
        'add' => 'Add',
        'description' => 1,
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '71d545f2-039c-46a3-b2b9-9a0de908708a';
    $display->content['new-71d545f2-039c-46a3-b2b9-9a0de908708a'] = $pane;
    $display->panels['right'][2] = 'new-71d545f2-039c-46a3-b2b9-9a0de908708a';
    $pane = new stdClass();
    $pane->pid = 'new-846d008d-31e5-4b93-93c3-b19aa77072c6';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_featured_resources';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '846d008d-31e5-4b93-93c3-b19aa77072c6';
    $display->content['new-846d008d-31e5-4b93-93c3-b19aa77072c6'] = $pane;
    $display->panels['right'][3] = 'new-846d008d-31e5-4b93-93c3-b19aa77072c6';
    $pane = new stdClass();
    $pane->pid = 'new-da509671-78bc-494a-bfe9-62d917bd5f6d';
    $pane->panel = 'top';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:body';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'da509671-78bc-494a-bfe9-62d917bd5f6d';
    $display->content['new-da509671-78bc-494a-bfe9-62d917bd5f6d'] = $pane;
    $display->panels['top'][0] = 'new-da509671-78bc-494a-bfe9-62d917bd5f6d';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-da509671-78bc-494a-bfe9-62d917bd5f6d';
  $handler->conf['display'] = $display;
  $export['node_view__panel_context_f43af71f-fd46-44b0-9735-6cb69c73ddbd'] = $handler;

  return $export;
}
