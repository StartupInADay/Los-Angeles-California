<?php
/**
 * @file
 * sbp_landing_page.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function sbp_landing_page_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:sbp_landing_page:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'sbp_landing_page';
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
      'center' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = 'f9e5ccf9-2f91-4ddf-b884-568bae4826f0';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-38f813ea-5dfd-4bf8-9028-1608479626ed';
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
    $pane->uuid = '38f813ea-5dfd-4bf8-9028-1608479626ed';
    $display->content['new-38f813ea-5dfd-4bf8-9028-1608479626ed'] = $pane;
    $display->panels['left'][0] = 'new-38f813ea-5dfd-4bf8-9028-1608479626ed';
    $pane = new stdClass();
    $pane->pid = 'new-35afaf58-0e9b-4a8d-9437-939eaaee9fc8';
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
    $pane->uuid = '35afaf58-0e9b-4a8d-9437-939eaaee9fc8';
    $display->content['new-35afaf58-0e9b-4a8d-9437-939eaaee9fc8'] = $pane;
    $display->panels['right'][0] = 'new-35afaf58-0e9b-4a8d-9437-939eaaee9fc8';
    $pane = new stdClass();
    $pane->pid = 'new-8c9065bf-8190-4995-b293-c02bbc06ba5c';
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
    $pane->uuid = '8c9065bf-8190-4995-b293-c02bbc06ba5c';
    $display->content['new-8c9065bf-8190-4995-b293-c02bbc06ba5c'] = $pane;
    $display->panels['top'][0] = 'new-8c9065bf-8190-4995-b293-c02bbc06ba5c';
    $pane = new stdClass();
    $pane->pid = 'new-dedae91e-18bc-41ab-a698-172af343dcad';
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
    $pane->uuid = 'dedae91e-18bc-41ab-a698-172af343dcad';
    $display->content['new-dedae91e-18bc-41ab-a698-172af343dcad'] = $pane;
    $display->panels['top'][1] = 'new-dedae91e-18bc-41ab-a698-172af343dcad';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-35afaf58-0e9b-4a8d-9437-939eaaee9fc8';
  $panelizer->display = $display;
  $export['node:sbp_landing_page:default'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:sbp_landing_page:departments';
  $panelizer->title = 'Departments';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'sbp_landing_page';
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
  $display->uuid = 'c84999ff-3a55-4624-8cc5-5d0f547e7f34';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-3a9cbd11-7bfd-4a25-abee-00e7758e34e9';
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
    $pane->uuid = '3a9cbd11-7bfd-4a25-abee-00e7758e34e9';
    $display->content['new-3a9cbd11-7bfd-4a25-abee-00e7758e34e9'] = $pane;
    $display->panels['left'][0] = 'new-3a9cbd11-7bfd-4a25-abee-00e7758e34e9';
    $pane = new stdClass();
    $pane->pid = 'new-7409f623-3645-416a-b277-008a696745c5';
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
    $pane->uuid = '7409f623-3645-416a-b277-008a696745c5';
    $display->content['new-7409f623-3645-416a-b277-008a696745c5'] = $pane;
    $display->panels['right'][0] = 'new-7409f623-3645-416a-b277-008a696745c5';
    $pane = new stdClass();
    $pane->pid = 'new-9be7cde8-0b99-4df4-8b80-c366851dde04';
    $pane->panel = 'right';
    $pane->type = 'views';
    $pane->subtype = 'departments';
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
      'display' => 'block',
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
    $pane->uuid = '9be7cde8-0b99-4df4-8b80-c366851dde04';
    $display->content['new-9be7cde8-0b99-4df4-8b80-c366851dde04'] = $pane;
    $display->panels['right'][1] = 'new-9be7cde8-0b99-4df4-8b80-c366851dde04';
    $pane = new stdClass();
    $pane->pid = 'new-db08b58d-162e-43af-8daf-5159f690382c';
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
    $pane->uuid = 'db08b58d-162e-43af-8daf-5159f690382c';
    $display->content['new-db08b58d-162e-43af-8daf-5159f690382c'] = $pane;
    $display->panels['top'][0] = 'new-db08b58d-162e-43af-8daf-5159f690382c';
    $pane = new stdClass();
    $pane->pid = 'new-577fe1a6-4ee9-49cd-982b-2336c8981c4b';
    $pane->panel = 'top';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_hero';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '577fe1a6-4ee9-49cd-982b-2336c8981c4b';
    $display->content['new-577fe1a6-4ee9-49cd-982b-2336c8981c4b'] = $pane;
    $display->panels['top'][1] = 'new-577fe1a6-4ee9-49cd-982b-2336c8981c4b';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-7409f623-3645-416a-b277-008a696745c5';
  $panelizer->display = $display;
  $export['node:sbp_landing_page:departments'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:sbp_landing_page:homepage';
  $panelizer->title = 'Homepage';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'sbp_landing_page';
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
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = '9651d298-1204-4968-8c95-80567812310d';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-a5e85646-3b64-4a6d-b1b0-dfd80875d29f';
    $pane->panel = 'middle';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_hero';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'a5e85646-3b64-4a6d-b1b0-dfd80875d29f';
    $display->content['new-a5e85646-3b64-4a6d-b1b0-dfd80875d29f'] = $pane;
    $display->panels['middle'][0] = 'new-a5e85646-3b64-4a6d-b1b0-dfd80875d29f';
    $pane = new stdClass();
    $pane->pid = 'new-90ee3a06-6f59-4ad7-9147-07d2120cabd7';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:81dde205-336c-4f62-a34d-1c30349f04b2';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'bundle' => 'general_content',
        'language' => '',
        'is_new' => TRUE,
        'title' => 'Welcome to the Business Portal',
        'link' => 0,
        'path' => '',
        'reusable' => 0,
        'admin_title' => '',
        'admin_description' => '',
        'category' => '',
        'vid' => '5',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '3',
        'field_call_to_action' => array(
          'und' => array(
            0 => array(
              'title' => 'Try the startup guide',
              'url' => 'start',
              'attributes' => 'a:1:{s:6:"target";i:0;}',
            ),
          ),
        ),
        'field_description' => array(
          'und' => array(
            0 => array(
              'value' => '<p>The Business Portal has many features available to businesses served in your area.</p>
',
              'format' => 'full_html',
            ),
          ),
        ),
        'field_intro_title' => array(
          'und' => array(
            0 => array(
              'value' => 'Welcome to the Business Portal',
            ),
          ),
        ),
        'field_pane_style' => array(
          'und' => array(
            0 => array(
              'tid' => '184',
            ),
          ),
        ),
        'log' => '',
        'created' => 1477938036,
        'timestamp' => 1477938036,
        'changed' => 1477938036,
        'uuid' => '81dde205-336c-4f62-a34d-1c30349f04b2',
        'vuuid' => '4f183426-4a16-4b7d-b894-422ab148651b',
        'uid' => '1',
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
    $pane->uuid = '90ee3a06-6f59-4ad7-9147-07d2120cabd7';
    $display->content['new-90ee3a06-6f59-4ad7-9147-07d2120cabd7'] = $pane;
    $display->panels['middle'][1] = 'new-90ee3a06-6f59-4ad7-9147-07d2120cabd7';
    $pane = new stdClass();
    $pane->pid = 'new-ec46f2a0-2352-41c0-8673-1b1ad7fb40c9';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:5a864689-f4ed-4da5-ab68-e0d77287577e';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'bundle' => 'pull_quote',
        'language' => '',
        'is_new' => TRUE,
        'title' => 'From the Mayor',
        'link' => 0,
        'path' => '',
        'reusable' => 0,
        'admin_title' => '',
        'admin_description' => '',
        'category' => '',
        'vid' => '6',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '4',
        'field_author' => array(
          'und' => array(
            0 => array(
              'value' => 'Peter Garret',
            ),
          ),
        ),
        'field_pane_style' => array(
          'und' => array(
            0 => array(
              'tid' => '188',
            ),
          ),
        ),
        'field_quote' => array(
          'und' => array(
            0 => array(
              'value' => 'Whether you’re starting a bakery or run an auto shop — we designed the Business Portal to help ALL businesses.',
            ),
          ),
        ),
        'field_quote_image' => array(
          'und' => array(
            0 => array(
              'fid' => '20',
              'display' => '1',
              'description' => '',
              'attach_button' => 'Attach',
              'upload' => '',
              'remove_button' => 'Remove',
              'width' => 881,
              'height' => 1150,
            ),
          ),
        ),
        'field_quote_subtitle' => array(
          'und' => array(
            0 => array(
              'value' => 'Mayor of Cityville',
            ),
          ),
        ),
        'log' => '',
        'created' => 1477938303,
        'timestamp' => 1477938303,
        'changed' => 1477938303,
        'uuid' => '5a864689-f4ed-4da5-ab68-e0d77287577e',
        'vuuid' => 'bc02d46d-3d02-4b56-a90c-c4a992bfe022',
        'uid' => '1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'ec46f2a0-2352-41c0-8673-1b1ad7fb40c9';
    $display->content['new-ec46f2a0-2352-41c0-8673-1b1ad7fb40c9'] = $pane;
    $display->panels['middle'][2] = 'new-ec46f2a0-2352-41c0-8673-1b1ad7fb40c9';
    $pane = new stdClass();
    $pane->pid = 'new-020d573a-f5fc-45c9-aca7-40e8dcdf87d2';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:a0c321f6-ea1e-4dcb-bc7e-45e344d373df';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'bundle' => 'three_column_cta',
        'language' => 'und',
        'is_new' => TRUE,
        'title' => 'Resources',
        'link' => 0,
        'path' => '',
        'reusable' => 0,
        'admin_title' => '',
        'admin_description' => '',
        'category' => '',
        'vid' => '7',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '5',
        'field_component_body' => array(
          'und' => array(
            0 => array(
              'value' => '<p>You know what running a successful business will look like, but you may need help to get there. Browse featured organizations, websites, courses, guides, incentives, and tools to help you on your way.</p>
',
              'format' => 'filtered_html',
            ),
          ),
        ),
        'field_pane_style' => array(
          'und' => array(
            0 => array(
              'tid' => '192',
            ),
          ),
        ),
        'field_three_ctas' => array(
          'und' => array(
            0 => array(
              'value' => '24',
              'revision_id' => '24',
            ),
            1 => array(
              'value' => '25',
              'revision_id' => '25',
            ),
            2 => array(
              'value' => '26',
              'revision_id' => '26',
            ),
          ),
        ),
        'log' => '',
        'created' => 1477938693,
        'timestamp' => 1477938693,
        'changed' => 1477938693,
        'uuid' => 'a0c321f6-ea1e-4dcb-bc7e-45e344d373df',
        'vuuid' => '9edc5486-5082-4fa1-8f1a-9ff8345cda67',
        'uid' => '1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '020d573a-f5fc-45c9-aca7-40e8dcdf87d2';
    $display->content['new-020d573a-f5fc-45c9-aca7-40e8dcdf87d2'] = $pane;
    $display->panels['middle'][3] = 'new-020d573a-f5fc-45c9-aca7-40e8dcdf87d2';
    $pane = new stdClass();
    $pane->pid = 'new-b37d6645-dca3-47dc-a531-bb7deac2bce9';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:1c0cd9fa-b871-46cb-b569-0f6effe32f3c';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'bundle' => 'four_column_cta',
        'language' => '',
        'is_new' => TRUE,
        'title' => 'Build Your Capacity',
        'link' => 0,
        'path' => '',
        'reusable' => 0,
        'admin_title' => '',
        'admin_description' => '',
        'category' => '',
        'vid' => '8',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '6',
        'field_calls_to_actions' => array(
          'und' => array(
            0 => array(
              'value' => '27',
              'revision_id' => '27',
            ),
            1 => array(
              'value' => '28',
              'revision_id' => '28',
            ),
            2 => array(
              'value' => '29',
              'revision_id' => '29',
            ),
            3 => array(
              'value' => '30',
              'revision_id' => '30',
            ),
          ),
        ),
        'field_pane_style' => array(
          'und' => array(
            0 => array(
              'tid' => '191',
            ),
          ),
        ),
        'log' => '',
        'created' => 1477939113,
        'timestamp' => 1477939113,
        'changed' => 1477939113,
        'uuid' => '1c0cd9fa-b871-46cb-b569-0f6effe32f3c',
        'vuuid' => '79a1060e-f544-4540-9ddf-4a1992a8e88b',
        'uid' => '1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = 'b37d6645-dca3-47dc-a531-bb7deac2bce9';
    $display->content['new-b37d6645-dca3-47dc-a531-bb7deac2bce9'] = $pane;
    $display->panels['middle'][4] = 'new-b37d6645-dca3-47dc-a531-bb7deac2bce9';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:sbp_landing_page:homepage'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:sbp_landing_page:resources';
  $panelizer->title = 'Resources';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'sbp_landing_page';
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
  $display->uuid = '97c75553-3435-4e3b-8674-c28f535066cb';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-fafe38c0-db50-48be-b707-337d7a132439';
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
    $pane->uuid = 'fafe38c0-db50-48be-b707-337d7a132439';
    $display->content['new-fafe38c0-db50-48be-b707-337d7a132439'] = $pane;
    $display->panels['left'][0] = 'new-fafe38c0-db50-48be-b707-337d7a132439';
    $pane = new stdClass();
    $pane->pid = 'new-5b07c23e-5757-4866-a6b1-16a83e633f4c';
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
    $pane->uuid = '5b07c23e-5757-4866-a6b1-16a83e633f4c';
    $display->content['new-5b07c23e-5757-4866-a6b1-16a83e633f4c'] = $pane;
    $display->panels['right'][0] = 'new-5b07c23e-5757-4866-a6b1-16a83e633f4c';
    $pane = new stdClass();
    $pane->pid = 'new-52e6e3a8-b934-415a-85b5-895f7c5e8190';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'entityreference_label',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'link' => FALSE,
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
    $pane->uuid = '52e6e3a8-b934-415a-85b5-895f7c5e8190';
    $display->content['new-52e6e3a8-b934-415a-85b5-895f7c5e8190'] = $pane;
    $display->panels['right'][1] = 'new-52e6e3a8-b934-415a-85b5-895f7c5e8190';
    $pane = new stdClass();
    $pane->pid = 'new-bfe9c11c-3362-474c-8af8-e8d1212b2af4';
    $pane->panel = 'right';
    $pane->type = 'views';
    $pane->subtype = 'bp_featured_resource_page';
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
      'override_title' => 1,
      'override_title_text' => 'Featured Resources & Incentives',
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
    $pane->uuid = 'bfe9c11c-3362-474c-8af8-e8d1212b2af4';
    $display->content['new-bfe9c11c-3362-474c-8af8-e8d1212b2af4'] = $pane;
    $display->panels['right'][2] = 'new-bfe9c11c-3362-474c-8af8-e8d1212b2af4';
    $pane = new stdClass();
    $pane->pid = 'new-549dbcc1-f064-40f1-93e6-27359157d9e2';
    $pane->panel = 'right';
    $pane->type = 'views';
    $pane->subtype = 'bp_featured_resource_page';
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
      'display' => 'block_main_featured_depts',
      'override_title' => 1,
      'override_title_text' => 'Featured Departments',
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
    $pane->uuid = '549dbcc1-f064-40f1-93e6-27359157d9e2';
    $display->content['new-549dbcc1-f064-40f1-93e6-27359157d9e2'] = $pane;
    $display->panels['right'][3] = 'new-549dbcc1-f064-40f1-93e6-27359157d9e2';
    $pane = new stdClass();
    $pane->pid = 'new-8a4379cd-3d64-40db-aa44-b306aea7ba9e';
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
    $pane->uuid = '8a4379cd-3d64-40db-aa44-b306aea7ba9e';
    $display->content['new-8a4379cd-3d64-40db-aa44-b306aea7ba9e'] = $pane;
    $display->panels['top'][0] = 'new-8a4379cd-3d64-40db-aa44-b306aea7ba9e';
    $pane = new stdClass();
    $pane->pid = 'new-cb74c767-7f78-48e1-ab6a-eb32bf209ccc';
    $pane->panel = 'top';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_hero';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'cb74c767-7f78-48e1-ab6a-eb32bf209ccc';
    $display->content['new-cb74c767-7f78-48e1-ab6a-eb32bf209ccc'] = $pane;
    $display->panels['top'][1] = 'new-cb74c767-7f78-48e1-ab6a-eb32bf209ccc';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-5b07c23e-5757-4866-a6b1-16a83e633f4c';
  $panelizer->display = $display;
  $export['node:sbp_landing_page:resources'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:sbp_landing_page:resources_incentives';
  $panelizer->title = 'Resources Incentives';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'sbp_landing_page';
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
  $display->uuid = 'fff555ab-e094-459f-8b90-64c4fa79d0a1';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-c09e4c99-c8bc-465f-9e6b-482dada43c99';
    $pane->panel = 'bottom';
    $pane->type = 'views_panes';
    $pane->subtype = 'sbp_resource_incentives-panel_pane_2';
    $pane->shown = TRUE;
    $pane->access = array(
      'plugins' => array(
        0 => array(
          'name' => 'php',
          'settings' => array(
            'description' => 'Search Filters',
            'php' => '// Check that there is no facet query.
return (empty($_GET[\'f\']) && empty($_GET[\'search_api_views_fulltext\']));',
          ),
          'not' => FALSE,
        ),
      ),
    );
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'c09e4c99-c8bc-465f-9e6b-482dada43c99';
    $display->content['new-c09e4c99-c8bc-465f-9e6b-482dada43c99'] = $pane;
    $display->panels['bottom'][0] = 'new-c09e4c99-c8bc-465f-9e6b-482dada43c99';
    $pane = new stdClass();
    $pane->pid = 'new-c95504f2-9ac7-4556-bde4-77cbd142e562';
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
    $pane->uuid = 'c95504f2-9ac7-4556-bde4-77cbd142e562';
    $display->content['new-c95504f2-9ac7-4556-bde4-77cbd142e562'] = $pane;
    $display->panels['left'][0] = 'new-c95504f2-9ac7-4556-bde4-77cbd142e562';
    $pane = new stdClass();
    $pane->pid = 'new-77cb1f46-c146-4d52-a678-0a72de6d51d0';
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
    $pane->uuid = '77cb1f46-c146-4d52-a678-0a72de6d51d0';
    $display->content['new-77cb1f46-c146-4d52-a678-0a72de6d51d0'] = $pane;
    $display->panels['left'][1] = 'new-77cb1f46-c146-4d52-a678-0a72de6d51d0';
    $pane = new stdClass();
    $pane->pid = 'new-167e8488-64eb-4a82-bb6c-0b8333745f22';
    $pane->panel = 'left';
    $pane->type = 'block';
    $pane->subtype = 'facetapi-eZL7XTwV0MlFlPwlwCAzjdRIBpwHUu1W';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '167e8488-64eb-4a82-bb6c-0b8333745f22';
    $display->content['new-167e8488-64eb-4a82-bb6c-0b8333745f22'] = $pane;
    $display->panels['left'][2] = 'new-167e8488-64eb-4a82-bb6c-0b8333745f22';
    $pane = new stdClass();
    $pane->pid = 'new-a4894486-4170-42d7-927d-fff3689bb576';
    $pane->panel = 'left';
    $pane->type = 'block';
    $pane->subtype = 'facetapi-i0ABb7Ee3JVdTmlkrtpUfFAY42jkcuji';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'a4894486-4170-42d7-927d-fff3689bb576';
    $display->content['new-a4894486-4170-42d7-927d-fff3689bb576'] = $pane;
    $display->panels['left'][3] = 'new-a4894486-4170-42d7-927d-fff3689bb576';
    $pane = new stdClass();
    $pane->pid = 'new-bafe6dc0-e405-48f2-9608-68ec3e1e34e1';
    $pane->panel = 'left';
    $pane->type = 'block';
    $pane->subtype = 'facetapi-aOSrQ4ZSfXMgOqIbMghaNuDFpQu7cM4j';
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
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = 'bafe6dc0-e405-48f2-9608-68ec3e1e34e1';
    $display->content['new-bafe6dc0-e405-48f2-9608-68ec3e1e34e1'] = $pane;
    $display->panels['left'][4] = 'new-bafe6dc0-e405-48f2-9608-68ec3e1e34e1';
    $pane = new stdClass();
    $pane->pid = 'new-abd299d7-4e24-4f69-8900-e582b1c235c7';
    $pane->panel = 'left';
    $pane->type = 'block';
    $pane->subtype = 'facetapi-5oVYr0th2DwK1loKsfoeapzfOLl45AXf';
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
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = 'abd299d7-4e24-4f69-8900-e582b1c235c7';
    $display->content['new-abd299d7-4e24-4f69-8900-e582b1c235c7'] = $pane;
    $display->panels['left'][5] = 'new-abd299d7-4e24-4f69-8900-e582b1c235c7';
    $pane = new stdClass();
    $pane->pid = 'new-6ac7de10-0b02-4bb9-b68f-577331a3da51';
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
    $pane->uuid = '6ac7de10-0b02-4bb9-b68f-577331a3da51';
    $display->content['new-6ac7de10-0b02-4bb9-b68f-577331a3da51'] = $pane;
    $display->panels['right'][0] = 'new-6ac7de10-0b02-4bb9-b68f-577331a3da51';
    $pane = new stdClass();
    $pane->pid = 'new-042d420f-6bb5-4218-90da-bd74cd51d060';
    $pane->panel = 'right';
    $pane->type = 'views_panes';
    $pane->subtype = 'sbp_resource_incentives-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array(
      'plugins' => array(
        0 => array(
          'name' => 'php',
          'settings' => array(
            'description' => 'Search Filters',
            'php' => '// Check if there is a facet query.
return (!empty($_GET[\'f\']) || !empty($_GET[\'search_api_views_fulltext\']));',
          ),
          'not' => FALSE,
        ),
      ),
    );
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
    $pane->uuid = '042d420f-6bb5-4218-90da-bd74cd51d060';
    $display->content['new-042d420f-6bb5-4218-90da-bd74cd51d060'] = $pane;
    $display->panels['right'][1] = 'new-042d420f-6bb5-4218-90da-bd74cd51d060';
    $pane = new stdClass();
    $pane->pid = 'new-4cc8b5c1-27c0-4fa7-8a5d-64e0ae35b0d9';
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
      'override_title' => 1,
      'override_title_text' => 'Featured Resources and Incentives',
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
    $pane->uuid = '4cc8b5c1-27c0-4fa7-8a5d-64e0ae35b0d9';
    $display->content['new-4cc8b5c1-27c0-4fa7-8a5d-64e0ae35b0d9'] = $pane;
    $display->panels['right'][2] = 'new-4cc8b5c1-27c0-4fa7-8a5d-64e0ae35b0d9';
    $pane = new stdClass();
    $pane->pid = 'new-66f03279-bce1-4da7-a3d4-daabad4cd970';
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
    $pane->uuid = '66f03279-bce1-4da7-a3d4-daabad4cd970';
    $display->content['new-66f03279-bce1-4da7-a3d4-daabad4cd970'] = $pane;
    $display->panels['top'][0] = 'new-66f03279-bce1-4da7-a3d4-daabad4cd970';
    $pane = new stdClass();
    $pane->pid = 'new-4fc341ba-63a1-4234-818d-3acb4a7202d2';
    $pane->panel = 'top';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_hero';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '4fc341ba-63a1-4234-818d-3acb4a7202d2';
    $display->content['new-4fc341ba-63a1-4234-818d-3acb4a7202d2'] = $pane;
    $display->panels['top'][1] = 'new-4fc341ba-63a1-4234-818d-3acb4a7202d2';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-6ac7de10-0b02-4bb9-b68f-577331a3da51';
  $panelizer->display = $display;
  $export['node:sbp_landing_page:resources_incentives'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:sbp_landing_page:service_centers';
  $panelizer->title = 'Service Centers';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'sbp_landing_page';
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
  $display->uuid = '34a06dc2-55c0-47be-9107-e76d9c923f73';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-e976024b-c7b5-45a8-a2ee-ab12d9db6c0b';
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
    $pane->uuid = 'e976024b-c7b5-45a8-a2ee-ab12d9db6c0b';
    $display->content['new-e976024b-c7b5-45a8-a2ee-ab12d9db6c0b'] = $pane;
    $display->panels['left'][0] = 'new-e976024b-c7b5-45a8-a2ee-ab12d9db6c0b';
    $pane = new stdClass();
    $pane->pid = 'new-e53feb22-e742-443f-a4fb-6743716b91d8';
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
    $pane->uuid = 'e53feb22-e742-443f-a4fb-6743716b91d8';
    $display->content['new-e53feb22-e742-443f-a4fb-6743716b91d8'] = $pane;
    $display->panels['right'][0] = 'new-e53feb22-e742-443f-a4fb-6743716b91d8';
    $pane = new stdClass();
    $pane->pid = 'new-12a42bac-bec0-4cf0-8497-b073ed1018cf';
    $pane->panel = 'right';
    $pane->type = 'views';
    $pane->subtype = 'labp_service_center';
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
      'panel_args' => 0,
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '12a42bac-bec0-4cf0-8497-b073ed1018cf';
    $display->content['new-12a42bac-bec0-4cf0-8497-b073ed1018cf'] = $pane;
    $display->panels['right'][1] = 'new-12a42bac-bec0-4cf0-8497-b073ed1018cf';
    $pane = new stdClass();
    $pane->pid = 'new-8dbf6db5-eba1-4b28-9583-e762db7c375d';
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
    $pane->uuid = '8dbf6db5-eba1-4b28-9583-e762db7c375d';
    $display->content['new-8dbf6db5-eba1-4b28-9583-e762db7c375d'] = $pane;
    $display->panels['top'][0] = 'new-8dbf6db5-eba1-4b28-9583-e762db7c375d';
    $pane = new stdClass();
    $pane->pid = 'new-2b92824d-c3aa-4651-aa06-50554e6c9780';
    $pane->panel = 'top';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_hero';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '2b92824d-c3aa-4651-aa06-50554e6c9780';
    $display->content['new-2b92824d-c3aa-4651-aa06-50554e6c9780'] = $pane;
    $display->panels['top'][1] = 'new-2b92824d-c3aa-4651-aa06-50554e6c9780';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-e53feb22-e742-443f-a4fb-6743716b91d8';
  $panelizer->display = $display;
  $export['node:sbp_landing_page:service_centers'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:sbp_landing_page:terms_and_definitions';
  $panelizer->title = 'Terms and Definitions';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'sbp_landing_page';
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
      'center' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = 'f9e5ccf9-2f91-4ddf-b884-568bae4826f0';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-38f813ea-5dfd-4bf8-9028-1608479626ed';
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
    $pane->uuid = '38f813ea-5dfd-4bf8-9028-1608479626ed';
    $display->content['new-38f813ea-5dfd-4bf8-9028-1608479626ed'] = $pane;
    $display->panels['left'][0] = 'new-38f813ea-5dfd-4bf8-9028-1608479626ed';
    $pane = new stdClass();
    $pane->pid = 'new-ed07128b-aa73-4d83-849f-a604ac40d995';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'ed07128b-aa73-4d83-849f-a604ac40d995';
    $display->content['new-ed07128b-aa73-4d83-849f-a604ac40d995'] = $pane;
    $display->panels['left'][1] = 'new-ed07128b-aa73-4d83-849f-a604ac40d995';
    $pane = new stdClass();
    $pane->pid = 'new-35afaf58-0e9b-4a8d-9437-939eaaee9fc8';
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
    $pane->uuid = '35afaf58-0e9b-4a8d-9437-939eaaee9fc8';
    $display->content['new-35afaf58-0e9b-4a8d-9437-939eaaee9fc8'] = $pane;
    $display->panels['right'][0] = 'new-35afaf58-0e9b-4a8d-9437-939eaaee9fc8';
    $pane = new stdClass();
    $pane->pid = 'new-56584cbc-188b-46d0-b56e-c02ba57e900f';
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
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'default',
      'context' => array(
        0 => '',
      ),
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
    $pane->uuid = '56584cbc-188b-46d0-b56e-c02ba57e900f';
    $display->content['new-56584cbc-188b-46d0-b56e-c02ba57e900f'] = $pane;
    $display->panels['right'][1] = 'new-56584cbc-188b-46d0-b56e-c02ba57e900f';
    $pane = new stdClass();
    $pane->pid = 'new-8c9065bf-8190-4995-b293-c02bbc06ba5c';
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
    $pane->uuid = '8c9065bf-8190-4995-b293-c02bbc06ba5c';
    $display->content['new-8c9065bf-8190-4995-b293-c02bbc06ba5c'] = $pane;
    $display->panels['top'][0] = 'new-8c9065bf-8190-4995-b293-c02bbc06ba5c';
    $pane = new stdClass();
    $pane->pid = 'new-dedae91e-18bc-41ab-a698-172af343dcad';
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
    $pane->uuid = 'dedae91e-18bc-41ab-a698-172af343dcad';
    $display->content['new-dedae91e-18bc-41ab-a698-172af343dcad'] = $pane;
    $display->panels['top'][1] = 'new-dedae91e-18bc-41ab-a698-172af343dcad';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-35afaf58-0e9b-4a8d-9437-939eaaee9fc8';
  $panelizer->display = $display;
  $export['node:sbp_landing_page:terms_and_definitions'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:sbp_landing_page:wizard';
  $panelizer->title = 'Wizard';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'sbp_landing_page';
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
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = '580cb959-8e5a-433e-88eb-c1e4a7e70c79';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-65f26957-6e55-4041-8af2-04e125b92039';
    $pane->panel = 'middle';
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
    $pane->uuid = '65f26957-6e55-4041-8af2-04e125b92039';
    $display->content['new-65f26957-6e55-4041-8af2-04e125b92039'] = $pane;
    $display->panels['middle'][0] = 'new-65f26957-6e55-4041-8af2-04e125b92039';
    $pane = new stdClass();
    $pane->pid = 'new-b799fbbf-217f-4c2a-ab8e-df6361bb2077';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'sbp_wizard-wizard';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'b799fbbf-217f-4c2a-ab8e-df6361bb2077';
    $display->content['new-b799fbbf-217f-4c2a-ab8e-df6361bb2077'] = $pane;
    $display->panels['middle'][1] = 'new-b799fbbf-217f-4c2a-ab8e-df6361bb2077';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:sbp_landing_page:wizard'] = $panelizer;

  return $export;
}
