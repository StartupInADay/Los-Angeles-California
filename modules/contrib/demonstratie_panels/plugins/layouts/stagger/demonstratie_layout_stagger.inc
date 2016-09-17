<?php
// Plugin definition
$plugin = array(
  'title' => t('Stagger'),
  'icon' => 'demonstratie-stagger-layout.png',
  'category' => t('Demo Layouts'),
  'theme' => 'demonstratie_layout_stagger',
  'regions' => array(
    'header' => t('Header'),
    'primary_features' => t('Primary features'),
    'secondary_features' => t('Secondary features'),
    'tertiary_features' => t('Tertiary features'),
    'quaternary_features' => t('Quaternary features'),
    'primary_supplements' => t('Primary supplements'),
    'secondary_supplements' => t('Secondary supplements'),
    'tertiary_supplements' => t('Tertiary supplements'),
    'footer' => t('Footer'),
  ),
);
