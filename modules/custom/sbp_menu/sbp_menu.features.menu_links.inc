<?php
/**
 * @file
 * sbp_menu.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function sbp_menu_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-footer-_disclaimer:<front>.
  $menu_links['menu-footer-_disclaimer:<front>'] = array(
    'menu_name' => 'menu-footer-',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Disclaimer',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-footer-_disclaimer:<front>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-footer-_privacy-policy:<front>.
  $menu_links['menu-footer-_privacy-policy:<front>'] = array(
    'menu_name' => 'menu-footer-',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Privacy Policy',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-footer-_privacy-policy:<front>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Disclaimer');
  t('Privacy Policy');

  return $menu_links;
}
