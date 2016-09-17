<?php
/**
 * @file
 * sbp_panels.fieldable_panels_pane_type.inc
 */

/**
 * Implements hook_default_fieldable_panels_pane_type().
 */
function sbp_panels_default_fieldable_panels_pane_type() {
  $export = array();

  $fieldable_panels_pane_type = new stdClass();
  $fieldable_panels_pane_type->disabled = FALSE; /* Edit this to true to make a default fieldable_panels_pane_type disabled initially */
  $fieldable_panels_pane_type->api_version = 1;
  $fieldable_panels_pane_type->name = 'featured_resources';
  $fieldable_panels_pane_type->title = 'Featured Resources';
  $fieldable_panels_pane_type->description = '';
  $export['featured_resources'] = $fieldable_panels_pane_type;

  $fieldable_panels_pane_type = new stdClass();
  $fieldable_panels_pane_type->disabled = FALSE; /* Edit this to true to make a default fieldable_panels_pane_type disabled initially */
  $fieldable_panels_pane_type->api_version = 1;
  $fieldable_panels_pane_type->name = 'four_column_cta';
  $fieldable_panels_pane_type->title = 'Four Column CTA';
  $fieldable_panels_pane_type->description = '';
  $export['four_column_cta'] = $fieldable_panels_pane_type;

  $fieldable_panels_pane_type = new stdClass();
  $fieldable_panels_pane_type->disabled = FALSE; /* Edit this to true to make a default fieldable_panels_pane_type disabled initially */
  $fieldable_panels_pane_type->api_version = 1;
  $fieldable_panels_pane_type->name = 'general_content';
  $fieldable_panels_pane_type->title = 'General Content';
  $fieldable_panels_pane_type->description = 'General content widget for landing pages';
  $export['general_content'] = $fieldable_panels_pane_type;

  $fieldable_panels_pane_type = new stdClass();
  $fieldable_panels_pane_type->disabled = FALSE; /* Edit this to true to make a default fieldable_panels_pane_type disabled initially */
  $fieldable_panels_pane_type->api_version = 1;
  $fieldable_panels_pane_type->name = 'pull_quote';
  $fieldable_panels_pane_type->title = 'Pull Quote';
  $fieldable_panels_pane_type->description = '';
  $export['pull_quote'] = $fieldable_panels_pane_type;

  $fieldable_panels_pane_type = new stdClass();
  $fieldable_panels_pane_type->disabled = FALSE; /* Edit this to true to make a default fieldable_panels_pane_type disabled initially */
  $fieldable_panels_pane_type->api_version = 1;
  $fieldable_panels_pane_type->name = 'section_content';
  $fieldable_panels_pane_type->title = 'Section Content';
  $fieldable_panels_pane_type->description = '';
  $export['section_content'] = $fieldable_panels_pane_type;

  $fieldable_panels_pane_type = new stdClass();
  $fieldable_panels_pane_type->disabled = FALSE; /* Edit this to true to make a default fieldable_panels_pane_type disabled initially */
  $fieldable_panels_pane_type->api_version = 1;
  $fieldable_panels_pane_type->name = 'three_column_cta';
  $fieldable_panels_pane_type->title = 'Three Column CTA';
  $fieldable_panels_pane_type->description = '';
  $export['three_column_cta'] = $fieldable_panels_pane_type;

  $fieldable_panels_pane_type = new stdClass();
  $fieldable_panels_pane_type->disabled = FALSE; /* Edit this to true to make a default fieldable_panels_pane_type disabled initially */
  $fieldable_panels_pane_type->api_version = 1;
  $fieldable_panels_pane_type->name = 'timeline';
  $fieldable_panels_pane_type->title = 'Timeline';
  $fieldable_panels_pane_type->description = '';
  $export['timeline'] = $fieldable_panels_pane_type;

  return $export;
}
