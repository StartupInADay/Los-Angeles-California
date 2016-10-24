<?php
/**
 * @file
 * sbp_global_blocks.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function sbp_global_blocks_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['sbp_global_blocks-footer_col_1'] = array(
    'cache' => 1,
    'css_class' => '',
    'custom' => 0,
    'delta' => 'footer_col_1',
    'module' => 'sbp_global_blocks',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'business_portal_theme' => array(
        'region' => 'footer_content1',
        'status' => 1,
        'theme' => 'business_portal_theme',
        'weight' => 0,
      ),
      'ember' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'ember',
        'weight' => 0,
      ),
      'zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'zen',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  $export['sbp_global_blocks-footer_col_2'] = array(
    'cache' => 1,
    'css_class' => '',
    'custom' => 0,
    'delta' => 'footer_col_2',
    'module' => 'sbp_global_blocks',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'business_portal_theme' => array(
        'region' => 'footer_content2',
        'status' => 1,
        'theme' => 'business_portal_theme',
        'weight' => 0,
      ),
      'ember' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'ember',
        'weight' => 0,
      ),
      'zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'zen',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  $export['sbp_global_blocks-footer_col_3'] = array(
    'cache' => 1,
    'css_class' => '',
    'custom' => 0,
    'delta' => 'footer_col_3',
    'module' => 'sbp_global_blocks',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'business_portal_theme' => array(
        'region' => 'footer_content3',
        'status' => 1,
        'theme' => 'business_portal_theme',
        'weight' => 0,
      ),
      'ember' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'ember',
        'weight' => 0,
      ),
      'zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'zen',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  $export['sbp_search-sbp_search_form'] = array(
    'cache' => 1,
    'css_class' => '',
    'custom' => 0,
    'delta' => 'sbp_search_form',
    'module' => 'sbp_search',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'business_portal_theme' => array(
        'region' => 'header',
        'status' => 1,
        'theme' => 'business_portal_theme',
        'weight' => 0,
      ),
      'ember' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'ember',
        'weight' => 0,
      ),
      'zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'zen',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  $export['sbp_search-sbp_search_icon'] = array(
    'cache' => 1,
    'css_class' => '',
    'custom' => 0,
    'delta' => 'sbp_search_icon',
    'module' => 'sbp_search',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'business_portal_theme' => array(
        'region' => 'header',
        'status' => 1,
        'theme' => 'business_portal_theme',
        'weight' => 0,
      ),
      'ember' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'ember',
        'weight' => 0,
      ),
      'zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'zen',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  $export['sbp_translation-sbp_translation_form'] = array(
    'cache' => 1,
    'css_class' => '',
    'custom' => 0,
    'delta' => 'sbp_translation_form',
    'module' => 'sbp_translation',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'business_portal_theme' => array(
        'region' => 'header',
        'status' => 1,
        'theme' => 'business_portal_theme',
        'weight' => 0,
      ),
      'ember' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'ember',
        'weight' => 0,
      ),
      'zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'zen',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  $export['sbp_translation-sbp_translation_icon'] = array(
    'cache' => 1,
    'css_class' => '',
    'custom' => 0,
    'delta' => 'sbp_translation_icon',
    'module' => 'sbp_translation',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'business_portal_theme' => array(
        'region' => 'header',
        'status' => 1,
        'theme' => 'business_portal_theme',
        'weight' => 0,
      ),
      'ember' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'ember',
        'weight' => 0,
      ),
      'zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'zen',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  $export['system-main-menu'] = array(
    'cache' => -1,
    'css_class' => '',
    'custom' => 0,
    'delta' => 'main-menu',
    'module' => 'system',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'business_portal_theme' => array(
        'region' => 'header',
        'status' => 1,
        'theme' => 'business_portal_theme',
        'weight' => 0,
      ),
      'ember' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'ember',
        'weight' => 0,
      ),
      'zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'zen',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  $export['twitter_block-1'] = array(
    'cache' => -1,
    'css_class' => '',
    'custom' => 0,
    'delta' => 1,
    'module' => 'twitter_block',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'business_portal_theme' => array(
        'region' => 'footer_content4',
        'status' => 1,
        'theme' => 'business_portal_theme',
        'weight' => 0,
      ),
      'ember' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'ember',
        'weight' => 0,
      ),
      'zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'zen',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  $export['views-bp_sponsors-block'] = array(
    'cache' => -1,
    'css_class' => '',
    'custom' => 0,
    'delta' => 'bp_sponsors-block',
    'module' => 'views',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'business_portal_theme' => array(
        'region' => 'footer_sponsors',
        'status' => 1,
        'theme' => 'business_portal_theme',
        'weight' => 0,
      ),
      'ember' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'ember',
        'weight' => 0,
      ),
      'zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'zen',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  return $export;
}
