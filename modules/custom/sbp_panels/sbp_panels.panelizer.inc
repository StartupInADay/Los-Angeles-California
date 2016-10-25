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
    $pane->pid = 'new-6ca28ab4-aae7-45ff-9991-0ea7a78cc81f';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'current:1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'timestamp' => 1477504709,
        'uid' => '1',
        'title' => 'Welcome to the LA Business Portal',
        'log' => '',
        'vuuid' => 'aa2f5492-c940-4b1d-9c48-c1e41970aaeb',
        'fpid' => '1',
        'bundle' => 'section_content',
        'link' => 0,
        'path' => '',
        'reusable' => 1,
        'admin_title' => 'Welcome',
        'admin_description' => '',
        'category' => '',
        'view_access' => NULL,
        'edit_access' => NULL,
        'created' => '1477503713',
        'changed' => 1477504709,
        'uuid' => '4eb91550-bda2-476f-b2a9-08fe6969bd64',
        'language' => 'und',
        'current_vid' => '1',
        'field_background_image' => array(
          'und' => array(
            0 => array(
              'alt' => 'phone',
              'title' => 'phone',
              'fid' => '10',
              'display' => '1',
              'width' => '420',
              'height' => '698',
              'focal_point' => '50,50',
              'description' => '',
              'upload_button' => 'Upload',
              'remove_button' => 'Remove',
              'upload' => '',
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
              'value' => '<p>The LA Business Portal provides you with all of the information you need to plan, start, manage, and grow your business. Try our Start Up Guide to quickly map out how to register your new business and be compliant. Explore a rich library of content and resources to help you manage and grow your business. Small businesses are essential to our city’s economy. The LA Business Portal is here to make sure you have everything you need to succeed.</p>
',
              'format' => 'filtered_html',
            ),
          ),
        ),
        'field_pane_style' => array(
          'und' => array(
            0 => array(
              'tid' => '221',
            ),
          ),
        ),
        'rdf_mapping' => array(),
        'revision' => 1,
        'is_new' => FALSE,
        'original' => (object) array(
          'vid' => '1',
          'timestamp' => '1477503713',
          'uid' => '1',
          'title' => 'Welcome to the LA Business Portal',
          'log' => '',
          'vuuid' => 'f4d7abf3-92be-41d6-9776-2310b0051e34',
          'fpid' => '1',
          'bundle' => 'section_content',
          'link' => '0',
          'path' => '',
          'reusable' => '1',
          'admin_title' => 'Welcome',
          'admin_description' => '',
          'category' => '',
          'view_access' => NULL,
          'edit_access' => NULL,
          'created' => '1477503713',
          'changed' => '1477503713',
          'uuid' => '4eb91550-bda2-476f-b2a9-08fe6969bd64',
          'language' => 'und',
          'current_vid' => '1',
          'field_background_image' => array(
            'und' => array(
              0 => array(
                'fid' => '10',
                'uid' => '1',
                'filename' => 'illustration_top_panel_6.png',
                'uri' => 'public://illustration_top_panel_6_1.png',
                'filemime' => 'image/png',
                'filesize' => '130224',
                'status' => '1',
                'timestamp' => '1477503713',
                'type' => 'image',
                'uuid' => '7d678614-7513-4041-b307-7dcbee529a37',
                'field_file_image_alt_text' => array(),
                'field_file_image_title_text' => array(),
                'rdf_mapping' => array(),
                'metadata' => array(
                  'height' => 698,
                  'width' => 420,
                ),
                'alt' => 'phone',
                'title' => 'phone',
                'height' => '698',
                'width' => '420',
                'focal_point' => '50,50',
              ),
            ),
          ),
          'field_calls_to_action' => array(
            'und' => array(
              0 => array(
                'url' => 'http://www.google.com',
                'title' => 'Go',
                'attributes' => array(),
              ),
            ),
          ),
          'field_component_body' => array(
            'und' => array(
              0 => array(
                'value' => 'The LA Business Portal provides you with all of the information you need to plan, start, manage, and grow your business. Try our Start Up Guide to quickly map out how to register your new business and be compliant.  Explore a rich library of content and resources to help you manage and grow your business.
 
Small businesses are essential to our city’s economy. The LA Business Portal is here to make sure you have everything you need to succeed.',
                'format' => 'filtered_html',
              ),
            ),
          ),
          'field_pane_style' => array(
            'und' => array(
              0 => array(
                'tid' => '221',
              ),
            ),
          ),
          'rdf_mapping' => array(),
        ),
        'old_vid' => '1',
        'vid' => '6',
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
    $pane->uuid = '6ca28ab4-aae7-45ff-9991-0ea7a78cc81f';
    $display->content['new-6ca28ab4-aae7-45ff-9991-0ea7a78cc81f'] = $pane;
    $display->panels['middle'][1] = 'new-6ca28ab4-aae7-45ff-9991-0ea7a78cc81f';
    $pane = new stdClass();
    $pane->pid = 'new-9217cd7c-5056-4d25-a5cb-5305c36f2b75';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:f9efd1d4-1eb5-4caf-b90f-4c0862901c72';
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
        'reusable' => 1,
        'admin_title' => 'Resources',
        'admin_description' => '',
        'category' => '',
        'vid' => '2',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '2',
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
              'tid' => '222',
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
        'created' => 1477504010,
        'timestamp' => 1477504010,
        'changed' => 1477504010,
        'uuid' => 'f9efd1d4-1eb5-4caf-b90f-4c0862901c72',
        'vuuid' => 'c280cbc6-651f-4df0-bedb-0198b7ad03cd',
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
    $pane->uuid = '9217cd7c-5056-4d25-a5cb-5305c36f2b75';
    $display->content['new-9217cd7c-5056-4d25-a5cb-5305c36f2b75'] = $pane;
    $display->panels['middle'][2] = 'new-9217cd7c-5056-4d25-a5cb-5305c36f2b75';
    $pane = new stdClass();
    $pane->pid = 'new-48484395-9047-42a5-84ac-1dd6087a6771';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:b74b03bd-6c6c-4b8c-a5f7-8215ef527f3e';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'bundle' => 'four_column_cta',
        'language' => 'und',
        'is_new' => TRUE,
        'title' => 'Build Your Capacity',
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
              'tid' => '223',
            ),
          ),
        ),
        'log' => '',
        'created' => 1477504327,
        'timestamp' => 1477504327,
        'changed' => 1477504327,
        'uuid' => 'b74b03bd-6c6c-4b8c-a5f7-8215ef527f3e',
        'vuuid' => 'f0a8420b-8033-4ddb-a32c-81c95620c9a6',
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
    $pane->uuid = '48484395-9047-42a5-84ac-1dd6087a6771';
    $display->content['new-48484395-9047-42a5-84ac-1dd6087a6771'] = $pane;
    $display->panels['middle'][3] = 'new-48484395-9047-42a5-84ac-1dd6087a6771';
    $pane = new stdClass();
    $pane->pid = 'new-5f84a4e1-74ec-49f2-af68-95186394b1e4';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:5c5e098e-8c3e-43c2-9653-bbe424c46c81';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'fieldable_panels_pane' => (object) array(
        'bundle' => 'general_content',
        'language' => '',
        'is_new' => TRUE,
        'title' => '',
        'link' => 0,
        'path' => '',
        'reusable' => 1,
        'admin_title' => 'General',
        'admin_description' => '',
        'category' => '',
        'vid' => '5',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '5',
        'field_call_to_action' => array(
          'und' => array(
            0 => array(
              'title' => 'Try it',
              'url' => 'http://www.google.com',
              'attributes' => 'a:1:{s:6:"target";s:6:"_blank";}',
            ),
          ),
        ),
        'field_description' => array(
          'und' => array(
            0 => array(
              'value' => 'Manufacturing Support Initiative provides entrepreneurs and manufacturers across our city with industry support, networking opportunities, and education programs that will reach more than 1,000 entrepreneurs and manufacturers over the next 18 months. ',
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
              'tid' => '221',
            ),
          ),
        ),
        'log' => '',
        'created' => 1477504645,
        'timestamp' => 1477504645,
        'changed' => 1477504645,
        'uuid' => '5c5e098e-8c3e-43c2-9653-bbe424c46c81',
        'vuuid' => 'e8e7ddd7-985f-4956-bc20-db8c946a62a9',
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
    $pane->uuid = '5f84a4e1-74ec-49f2-af68-95186394b1e4';
    $display->content['new-5f84a4e1-74ec-49f2-af68-95186394b1e4'] = $pane;
    $display->panels['middle'][4] = 'new-5f84a4e1-74ec-49f2-af68-95186394b1e4';
    $pane = new stdClass();
    $pane->pid = 'new-58695878-1e7b-408e-8048-381018569fb6';
    $pane->panel = 'middle';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:63c4278b-7de5-4d4d-a33a-c3e15ec89f00';
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
        'vid' => '4',
        'current_vid' => '',
        'revision' => 1,
        'fpid' => '4',
        'field_author' => array(
          'und' => array(
            0 => array(
              'value' => 'Peter Desrigen',
            ),
          ),
        ),
        'field_pane_style' => array(
          'und' => array(
            0 => array(
              'tid' => '221',
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
              'fid' => '18',
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
        'created' => 1477504478,
        'timestamp' => 1477504478,
        'changed' => 1477504478,
        'uuid' => '63c4278b-7de5-4d4d-a33a-c3e15ec89f00',
        'vuuid' => 'f16c5b6f-d704-482d-8f66-edc77bd05cf8',
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
    $pane->uuid = '58695878-1e7b-408e-8048-381018569fb6';
    $display->content['new-58695878-1e7b-408e-8048-381018569fb6'] = $pane;
    $display->panels['middle'][5] = 'new-58695878-1e7b-408e-8048-381018569fb6';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:sbp_landing_page:homepage'] = $panelizer;

  return $export;
}
