<?php
/**
 * @file
 * General processing for Demonstratie layouts.
 */

/**
 * Define a base, single column layout.
 */
$plugin = array(
  'title' => t('Single column'),
  'icon' => 'demonstratie-single-layout.png',
  'category' => t('Demo Layouts'),
  'theme' => 'demonstratie_layout_single',
  'regions' => array(
    'primary_features' => t('Primary features'),
    'secondary_features' => t('Secondary features'),
    'tertiary_features' => t('Tertiary features'),
    'quaternary_features' => t('Quaternary features'),
    'primary_supplements' => t('Primary supplements'),
    'secondary_supplements' => t('Secondary supplements'),
    'tertiary_supplements' => t('Tertiary supplements'),
  ),
);
