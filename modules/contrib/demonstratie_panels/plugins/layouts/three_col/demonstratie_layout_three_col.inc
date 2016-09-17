<?php
/**
 * @file
 * General processing for Demonstratie layouts.
 */

/**
 * Define a base, three_col column layout.
 */
$plugin = array(
  'title' => t('Three column'),
  'icon' => 'demonstratie-three_col-layout.png',
  'category' => t('Demo Layouts'),
  'theme' => 'demonstratie_layout_three_col',
  'regions' => array(
    'featured' => t('Featured'),
    'first' => t('First'),
    'second' => t('Second'),
    'third' => t('Third'),
    'footer_area' => t('Bottom'),
  ),
);
