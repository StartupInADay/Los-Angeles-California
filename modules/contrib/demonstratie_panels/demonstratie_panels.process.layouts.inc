<?php

/**
 * Helper function to combine all region attributes array down to a string.
 */
 function _demonstratie_panels_attributes_combine(&$variables) {
   // Crunch all of the region attributes arrays down to a string.
   foreach ($variables['content'] as $region => $content) {
     $variables[$region . '_attributes'] = drupal_attributes($variables[$region . '_attributes_array']);
   }
 }

/**
 * Implements hook_process_demonstratie_layout().
 */
function template_process_demonstratie_layout_single(&$variables, $hook) {
  _demonstratie_panels_attributes_combine($variables);
}

/**
 * Implements hook_process_demonstratie_layout().
 */
function template_process_demonstratie_layout_landing(&$variables, $hook) {
  _demonstratie_panels_attributes_combine($variables);
}

/**
 * Implements hook_process_demonstratie_layout().
 */
function template_process_demonstratie_layout_full_side(&$variables, $hook) {
  _demonstratie_panels_attributes_combine($variables);
}

/**
 * Implements hook_process_demonstratie_split_features().
 */
function template_process_demonstratie_layout_split_features(&$variables, $hook) {
  _demonstratie_panels_attributes_combine($variables);
}

/**
 * Implements hook_process_demonstratie_stagger().
 */
function template_process_demonstratie_layout_stagger(&$variables, $hook) {
  _demonstratie_panels_attributes_combine($variables);
}

/**
 * Implements hook_process_demonstratie_layout_three_col().
 */
function template_process_demonstratie_layout_three_col(&$variables, $hook) {
  _demonstratie_panels_attributes_combine($variables);
}
