<?php
/**
 * @file
 * sbp_service_center.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function sbp_service_center_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:labp_service_center:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'labp_service_center';
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
  $display->uuid = '8aa2a86c-5b21-4d5b-9db4-57a877c87688';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-a4888d5c-ad01-4289-b92e-220a0059100f';
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
    $pane->uuid = 'a4888d5c-ad01-4289-b92e-220a0059100f';
    $display->content['new-a4888d5c-ad01-4289-b92e-220a0059100f'] = $pane;
    $display->panels['left'][0] = 'new-a4888d5c-ad01-4289-b92e-220a0059100f';
    $pane = new stdClass();
    $pane->pid = 'new-f4384dc6-83dc-439b-a179-06dc36147356';
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
    $pane->uuid = 'f4384dc6-83dc-439b-a179-06dc36147356';
    $display->content['new-f4384dc6-83dc-439b-a179-06dc36147356'] = $pane;
    $display->panels['right'][0] = 'new-f4384dc6-83dc-439b-a179-06dc36147356';
    $pane = new stdClass();
    $pane->pid = 'new-d8853f52-4fba-4817-9825-348501a0401f';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_geographical_area';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'taxonomy_term_reference_link',
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'd8853f52-4fba-4817-9825-348501a0401f';
    $display->content['new-d8853f52-4fba-4817-9825-348501a0401f'] = $pane;
    $display->panels['right'][1] = 'new-d8853f52-4fba-4817-9825-348501a0401f';
    $pane = new stdClass();
    $pane->pid = 'new-7f8a2e08-f268-4cd1-bc53-65fcfe6624b9';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_address';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'addressfield_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'format_handlers' => array(
          0 => 'address',
        ),
        'quickedit' => array(
          'editor' => 'form',
        ),
        'use_widget_handlers' => 1,
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
    $pane->uuid = '7f8a2e08-f268-4cd1-bc53-65fcfe6624b9';
    $display->content['new-7f8a2e08-f268-4cd1-bc53-65fcfe6624b9'] = $pane;
    $display->panels['right'][2] = 'new-7f8a2e08-f268-4cd1-bc53-65fcfe6624b9';
    $pane = new stdClass();
    $pane->pid = 'new-4ea14bc1-e71e-452a-b8e9-dfad346cff00';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_map';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'image',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'image_link' => '',
        'image_style' => '',
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
    $pane->uuid = '4ea14bc1-e71e-452a-b8e9-dfad346cff00';
    $display->content['new-4ea14bc1-e71e-452a-b8e9-dfad346cff00'] = $pane;
    $display->panels['right'][3] = 'new-4ea14bc1-e71e-452a-b8e9-dfad346cff00';
    $pane = new stdClass();
    $pane->pid = 'new-cbf4e7e2-f566-4ee6-bc07-7f80ffe42f7c';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_phone';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
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
    $pane->uuid = 'cbf4e7e2-f566-4ee6-bc07-7f80ffe42f7c';
    $display->content['new-cbf4e7e2-f566-4ee6-bc07-7f80ffe42f7c'] = $pane;
    $display->panels['right'][4] = 'new-cbf4e7e2-f566-4ee6-bc07-7f80ffe42f7c';
    $pane = new stdClass();
    $pane->pid = 'new-84976920-0d4b-4272-b80c-bdb1bcc90e41';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_link';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
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
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = '84976920-0d4b-4272-b80c-bdb1bcc90e41';
    $display->content['new-84976920-0d4b-4272-b80c-bdb1bcc90e41'] = $pane;
    $display->panels['right'][5] = 'new-84976920-0d4b-4272-b80c-bdb1bcc90e41';
    $pane = new stdClass();
    $pane->pid = 'new-0f2ef351-1593-4637-8c2a-3a170467ba1b';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_services';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
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
    $pane->position = 6;
    $pane->locks = array();
    $pane->uuid = '0f2ef351-1593-4637-8c2a-3a170467ba1b';
    $display->content['new-0f2ef351-1593-4637-8c2a-3a170467ba1b'] = $pane;
    $display->panels['right'][6] = 'new-0f2ef351-1593-4637-8c2a-3a170467ba1b';
    $pane = new stdClass();
    $pane->pid = 'new-9ba12106-4cda-4b47-b7f4-508392e4188a';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_languages';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
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
    $pane->position = 7;
    $pane->locks = array();
    $pane->uuid = '9ba12106-4cda-4b47-b7f4-508392e4188a';
    $display->content['new-9ba12106-4cda-4b47-b7f4-508392e4188a'] = $pane;
    $display->panels['right'][7] = 'new-9ba12106-4cda-4b47-b7f4-508392e4188a';
    $pane = new stdClass();
    $pane->pid = 'new-8670f56d-4976-44a7-bad6-17068b2d39e0';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_email';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'email_default',
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
    $pane->position = 8;
    $pane->locks = array();
    $pane->uuid = '8670f56d-4976-44a7-bad6-17068b2d39e0';
    $display->content['new-8670f56d-4976-44a7-bad6-17068b2d39e0'] = $pane;
    $display->panels['right'][8] = 'new-8670f56d-4976-44a7-bad6-17068b2d39e0';
    $pane = new stdClass();
    $pane->pid = 'new-9d1269aa-d1d2-4029-a5a9-aa5d9a6b0873';
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
    $pane->uuid = '9d1269aa-d1d2-4029-a5a9-aa5d9a6b0873';
    $display->content['new-9d1269aa-d1d2-4029-a5a9-aa5d9a6b0873'] = $pane;
    $display->panels['top'][0] = 'new-9d1269aa-d1d2-4029-a5a9-aa5d9a6b0873';
    $pane = new stdClass();
    $pane->pid = 'new-26d13d0b-a444-414f-906b-8aada6f2894d';
    $pane->panel = 'top';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'vid:14';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'bundle' => 'hero',
        'language' => '',
        'is_new' => TRUE,
        'title' => 'Hero',
        'link' => 0,
        'path' => '',
        'reusable' => 0,
        'admin_title' => '',
        'admin_description' => '',
        'category' => 'Reusable panes',
        'vid' => '14',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '12',
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
              'value' => 'ffffff',
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
        'created' => 1460515851,
        'timestamp' => 1460515851,
        'changed' => 1460515851,
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '26d13d0b-a444-414f-906b-8aada6f2894d';
    $display->content['new-26d13d0b-a444-414f-906b-8aada6f2894d'] = $pane;
    $display->panels['top'][1] = 'new-26d13d0b-a444-414f-906b-8aada6f2894d';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-f4384dc6-83dc-439b-a179-06dc36147356';
  $panelizer->display = $display;
  $export['node:labp_service_center:default'] = $panelizer;

  return $export;
}
