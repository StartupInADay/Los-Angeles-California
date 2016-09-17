<?php
/**
 * @file
 * Classy Panel Styles API include file.
 */

/**
 * Implements hook_classy_panel_styles().
 *
 * Provides a set of example Classy Panel Styles definitions.
 */
function cps_example_classy_panel_styles() {
  $export = array();
  $groups = serialize(array('cps_example'));

  $style = new stdClass();
  $style->disabled = FALSE; /* Edit this to true to make a default style disabled initially */
  $style->api_version = 1;
  $style->name = 'cps_example_region_layout';
  $style->visibility = 2;
  $style->groups = $groups;
  $style->title = 'CPS Region Layout';
  $style->required = FALSE;
  $style->description = '';
  $style->options = serialize(array(
    'region-1col' => '1 Column',
    'region-2col' => '2 Columns',
    'region-2col-1x3' => '2 Columns: 1 item in the 1st column, 3 items in the 2nd',
    'region-3col' => '3 Columns',
    'region-4col' => '4 Columns',
    'region-1col-2col' => 'Multi: 1st row is 1 column, additional rows are in 2 columns',
    'region-1col-3col' => 'Multi: 1st row is 1 column, additional rows are in 3 columns',
  ));
  $style->default_value = '';
  $style->layouts_setting = 0;
  $style->layouts = 'a:0:{}';
  $export['cps_example_region_layout'] = $style;

  $style = new stdClass();
  $style->disabled = FALSE; /* Edit this to true to make a default style disabled initially */
  $style->api_version = 1;
  $style->name = 'cps_spacing';
  $style->visibility = 3;
  $style->groups = $groups;
  $style->title = 'CPS Spacing';
  $style->required = FALSE;
  $style->description = '';
  $style->options = serialize(array(
    'no-padding' => 'No Padding',
    'double-padding' => 'Double Padding',
    'no-margin' => 'No Margin',
    'no-margin-top' => 'No Top Margin',
    'no-padding-no-margin' => 'No Padding, No Margin',
  ));
  $style->default_value = '';
  $style->layouts_setting = 0;
  $style->layouts = 'a:0:{}';
  $export['region_layout'] = $style;

  $style = new stdClass();
  $style->disabled = FALSE; /* Edit this to true to make a default style disabled initially */
  $style->api_version = 1;
  $style->name = 'cps_example_image_layout';
  $style->visibility = 1;
  $style->groups = $groups;
  $style->title = 'CPS Image Layout';
  $style->required = FALSE;
  $style->description = '';
  $style->options = serialize(array(
    'image-full' => 'Image is 100% width, no stretching',
    'image-float-right' => 'Image floats right, text flows around on left',
    'image-float-left' => 'Image floats left, text flows around on right',
  ));
  $style->default_value = '';
  $style->layouts_setting = 0;
  $style->layouts = serialize(array());
  $export['cps_example_image_layout'] = $style;

  $style = new stdClass();
  $style->disabled = FALSE; /* Edit this to true to make a default style disabled initially */
  $style->api_version = 1;
  $style->name = 'cps_example_text_columns';
  $style->visibility = 1;
  $style->groups = $groups;
  $style->title = 'CPS Text Columns';
  $style->required = FALSE;
  $style->description = '';
  $style->options = serialize(array(
    'text-columns-4' => 'Text flows into 4 columns',
    'text-columns-3' => 'Text flows into 3 columns',
    'text-columns-2' => 'Text flows into 2 columns',
  ));
  $style->default_value = '';
  $style->layouts_setting = 0;
  $style->layouts = serialize(array());
  $export['cps_example_text_columns'] = $style;

  $style = new stdClass();
  $style->disabled = FALSE; /* Edit this to true to make a default style disabled initially */
  $style->api_version = 1;
  $style->name = 'cps_example_background';
  $style->visibility = 1;
  $style->groups = $groups;
  $style->title = 'CPS Background';
  $style->required = FALSE;
  $style->description = 'Add some color to your panel pane';
  $style->options = serialize(array(
    'bg-one' => 'Color one',
    'bg-two' => 'Color two',
    'bg-three' => 'Color three',
    'bg-four' => 'Color four',
    'bg-five' => 'Color five',
    'bg-white' => 'White',
    'bg-gray' => 'Light Gray',
    'bg-black' => 'Black',

  ));
  $style->default_value = '';
  $style->layouts_setting = 0;
  $style->layouts = serialize(array());
  $export['cps_example_background'] = $style;

  return $export;
}
