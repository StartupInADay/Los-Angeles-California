<?php
/**
 * @file
 * sbp_menu.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function sbp_menu_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: main-menu.
  $menus['main-menu'] = array(
    'menu_name' => 'main-menu',
    'title' => 'Main menu',
    'description' => 'The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.',
  );
  // Exported menu: menu-footer-.
  $menus['menu-footer-'] = array(
    'menu_name' => 'menu-footer-',
    'title' => 'Footer menu',
    'description' => 'Dynamic menu for bottom of business portal',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Dynamic menu for bottom of business portal');
  t('Footer menu');
  t('Main menu');
  t('The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.');

  return $menus;
}
