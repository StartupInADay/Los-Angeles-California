<?php
/**
 * @file
 * sbp_panels.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function sbp_panels_panelizer_defaults() {
  $export = array();

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
  $panelizer->pipeline = 'standard';
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
    $pane->pid = 'new-cd447dc9-c64b-40e6-b4f3-6e17507be835';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:a9f61511-0180-4e80-bfc8-e4f71457e431';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'bundle' => 'section_content',
        'language' => 'und',
        'is_new' => TRUE,
        'title' => 'Welcome to the Business Portal',
        'link' => 0,
        'path' => '',
        'reusable' => 1,
        'admin_title' => 'Welcome',
        'admin_description' => '',
        'category' => '',
        'vid' => '1',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '1',
        'field_background_image' => array(
          'und' => array(
            0 => array(
              'fid' => '20',
              'display' => '1',
              'description' => '',
              'upload_button' => 'Upload',
              'remove_button' => 'Remove',
              'upload' => '',
              'width' => 420,
              'height' => 698,
              'alt' => '',
              'title' => '',
              'focal_point' => '50,50',
            ),
          ),
        ),
        'field_calls_to_action' => array(
          'und' => array(
            0 => array(
              'title' => 'TRY THE STARTUP GUIDE',
              'url' => 'http://www.google.com',
              'attributes' => 'a:0:{}',
            ),
          ),
        ),
        'field_component_body' => array(
          'und' => array(
            0 => array(
              'value' => 'The Business Portal provides you with all of the information you need to plan, start, manage, and grow your business. Try our Start Up Guide to quickly map out how to register your new business and be compliant.  Explore a rich library of content and resources to help you manage and grow your business.
 
Small businesses are essential to our city’s economy. The Business Portal is here to make sure you have everything you need to succeed.',
              'format' => 'filtered_html',
            ),
          ),
        ),
        'field_pane_style' => array(
          'und' => array(
            0 => array(
              'tid' => '194',
            ),
          ),
        ),
        'log' => '',
        'created' => 1477579164,
        'timestamp' => 1477579164,
        'changed' => 1477579164,
        'uuid' => 'a9f61511-0180-4e80-bfc8-e4f71457e431',
        'vuuid' => 'e15c5828-fd9f-4e6b-b976-458871f75131',
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
    $pane->uuid = 'cd447dc9-c64b-40e6-b4f3-6e17507be835';
    $display->content['new-cd447dc9-c64b-40e6-b4f3-6e17507be835'] = $pane;
    $display->panels['middle'][1] = 'new-cd447dc9-c64b-40e6-b4f3-6e17507be835';
    $pane = new stdClass();
    $pane->pid = 'new-e948d839-cb7d-42c7-9546-424556312677';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'current:2';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'timestamp' => 1477580002,
        'uid' => '1',
        'title' => 'Resources',
        'log' => '',
        'vuuid' => 'b85f9fba-304c-4e2b-9ca5-daef82341292',
        'fpid' => '2',
        'bundle' => 'three_column_cta',
        'link' => 0,
        'path' => '',
        'reusable' => 1,
        'admin_title' => 'Resources',
        'admin_description' => '',
        'category' => '',
        'view_access' => NULL,
        'edit_access' => NULL,
        'created' => '1477579454',
        'changed' => 1477580002,
        'uuid' => '72837297-54a4-4c88-889c-8d6a05965eb8',
        'language' => '',
        'current_vid' => '2',
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
              'tid' => '195',
            ),
          ),
        ),
        'field_three_ctas' => array(
          'und' => array(
            0 => array(
              'value' => '24',
              'revision_id' => '31',
            ),
            1 => array(
              'value' => '25',
              'revision_id' => '32',
            ),
            2 => array(
              'value' => '26',
              'revision_id' => '33',
            ),
          ),
        ),
        'rdf_mapping' => array(),
        'revision' => 1,
        'is_new' => FALSE,
        'original' => (object) array(
          'vid' => '2',
          'timestamp' => '1477579454',
          'uid' => '1',
          'title' => 'Resources',
          'log' => '',
          'vuuid' => '15f7d984-5892-4d68-aba0-6a30f4cad920',
          'fpid' => '2',
          'bundle' => 'three_column_cta',
          'link' => '0',
          'path' => '',
          'reusable' => '1',
          'admin_title' => 'Resources',
          'admin_description' => '',
          'category' => '',
          'view_access' => NULL,
          'edit_access' => NULL,
          'created' => '1477579454',
          'changed' => '1477579454',
          'uuid' => '72837297-54a4-4c88-889c-8d6a05965eb8',
          'language' => '',
          'current_vid' => '2',
          'field_component_body' => array(
            'und' => array(
              0 => array(
                'value' => 'You know what running a successful business will look like, but you may need help to get there. Browse featured organizations, websites, courses, guides, incentives, and tools to help you on your way.',
                'format' => 'filtered_html',
              ),
            ),
          ),
          'field_pane_style' => array(
            'und' => array(
              0 => array(
                'tid' => '195',
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
          'rdf_mapping' => array(),
        ),
        'old_vid' => '2',
        'vid' => '6',
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
    $pane->uuid = 'e948d839-cb7d-42c7-9546-424556312677';
    $display->content['new-e948d839-cb7d-42c7-9546-424556312677'] = $pane;
    $display->panels['middle'][2] = 'new-e948d839-cb7d-42c7-9546-424556312677';
    $pane = new stdClass();
    $pane->pid = 'new-1e581f10-bf37-487c-9464-dfefa05537ae';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:eb44f779-f5dd-47cf-aef8-af87f3bded9e';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'bundle' => 'four_column_cta',
        'language' => 'und',
        'is_new' => TRUE,
        'title' => 'Build your Capacity',
        'link' => 0,
        'path' => '',
        'reusable' => 1,
        'admin_title' => 'Capacity',
        'admin_description' => '',
        'category' => '',
        'vid' => '3',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '3',
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
              'tid' => '197',
            ),
          ),
        ),
        'log' => '',
        'created' => 1477579713,
        'timestamp' => 1477579713,
        'changed' => 1477579713,
        'uuid' => 'eb44f779-f5dd-47cf-aef8-af87f3bded9e',
        'vuuid' => 'fa8c17c5-4c4c-4010-84e5-5aae99bbfe9d',
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
    $pane->uuid = '1e581f10-bf37-487c-9464-dfefa05537ae';
    $display->content['new-1e581f10-bf37-487c-9464-dfefa05537ae'] = $pane;
    $display->panels['middle'][3] = 'new-1e581f10-bf37-487c-9464-dfefa05537ae';
    $pane = new stdClass();
    $pane->pid = 'new-abff2933-b95c-4b74-a9ba-8eea79f3a93c';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:39877fb1-b036-4b83-9dbc-7f2e215a0810';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'bundle' => 'general_content',
        'language' => 'und',
        'is_new' => TRUE,
        'title' => '',
        'link' => 0,
        'path' => '',
        'reusable' => 1,
        'admin_title' => 'Manufacturing Support',
        'admin_description' => '',
        'category' => '',
        'vid' => '4',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '4',
        'field_call_to_action' => array(
          'und' => array(
            0 => array(
              'title' => 'Go',
              'url' => 'http://www.google.com',
              'attributes' => 'a:1:{s:6:"target";s:6:"_blank";}',
            ),
          ),
        ),
        'field_description' => array(
          'und' => array(
            0 => array(
              'value' => '<p>Mayor Garcetti\'s Manufacturing Support Initiative provides entrepreneurs and manufacturers across L.A. with industry support, networking opportunities, and education programs that will reach more than 1,000 entrepreneurs and manufacturers over the next 18 months.</p>
',
              'format' => 'filtered_html',
            ),
          ),
        ),
        'field_intro_title' => array(
          'und' => array(
            0 => array(
              'value' => 'Manufacturing Support Initiative',
            ),
          ),
        ),
        'field_pane_style' => array(
          'und' => array(
            0 => array(
              'tid' => '195',
            ),
          ),
        ),
        'log' => '',
        'created' => 1477579851,
        'timestamp' => 1477579851,
        'changed' => 1477579851,
        'uuid' => '39877fb1-b036-4b83-9dbc-7f2e215a0810',
        'vuuid' => '999f41db-9901-4054-9539-07b28c0482a0',
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
    $pane->uuid = 'abff2933-b95c-4b74-a9ba-8eea79f3a93c';
    $display->content['new-abff2933-b95c-4b74-a9ba-8eea79f3a93c'] = $pane;
    $display->panels['middle'][4] = 'new-abff2933-b95c-4b74-a9ba-8eea79f3a93c';
    $pane = new stdClass();
    $pane->pid = 'new-29f321b7-af08-4e20-8b75-392b5981b48e';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:05aae564-1816-4faa-8324-ca5a40a36a4e';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'bundle' => 'pull_quote',
        'language' => '',
        'is_new' => TRUE,
        'title' => '',
        'link' => 0,
        'path' => '',
        'reusable' => 1,
        'admin_title' => 'Quote',
        'admin_description' => '',
        'category' => '',
        'vid' => '5',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '5',
        'field_author' => array(
          'und' => array(
            0 => array(
              'value' => 'Terrence Trujillo',
            ),
          ),
        ),
        'field_pane_style' => array(
          'und' => array(
            0 => array(
              'tid' => '196',
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
              'fid' => '29',
              'display' => '1',
              'description' => '',
              'attach_button' => 'Attach',
              'upload' => '',
              'remove_button' => 'Remove',
              'width' => 680,
              'height' => 680,
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
        'created' => 1477579979,
        'timestamp' => 1477579979,
        'changed' => 1477579979,
        'uuid' => '05aae564-1816-4faa-8324-ca5a40a36a4e',
        'vuuid' => '1f8a6e04-63ea-4feb-aba6-eebebab3aa93',
        'uid' => '1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = '29f321b7-af08-4e20-8b75-392b5981b48e';
    $display->content['new-29f321b7-af08-4e20-8b75-392b5981b48e'] = $pane;
    $display->panels['middle'][5] = 'new-29f321b7-af08-4e20-8b75-392b5981b48e';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:sbp_landing_page:homepage'] = $panelizer;

  return $export;
}
