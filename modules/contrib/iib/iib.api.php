<?php
/**
 * @file Provides the basic developer documentation for working with iib.
 *
 * IIB provides a centralized way of adding addition administrative information and
 * tasks, including full forms, to pages in drupal.  It provides a hook, a function
 * similar to drupal_set_message and an alter hook to allow modules to control what
 * is rendered in the bar.
 *
 * @see iib.module
 */

/**
 * Allows modules to add items into the render array for the page level IIB.
 * This will be displayed at the top of the page.
 *
 * @return array
 *  A renderable array, each return should have a top level key.  Modules with the
 */
function hook_iib_page_item() {
  $items['left'] = array(
    '#weight' => -10,
    '#prefix' => '<div>',
    '#markup' => t('Hi this is the left side.'),
    '#suffix' => '</div>',
  );
  return $items;
}

/**
 * Allows modules to add items into the render array for the entity level IIB.
 * This will be displayed above the entity itself.
 *
 * @param $entity
 *  The entity we're currently getting items to display for.
 * @param $entity_type
 *  The type of entity being viewed
 * @param $view_mode
 *  The view mode which the entity is currently being viewed in.
 *
 * @return
 *  A renderable array, each return should have a top level key.  Modules with the
 */
function hook_iib_entity_item($entity, $entity_type, $view_mode) {
  $items['info'] = array(
    '#weight' => -10,
    '#prefix' => '<div>',
    '#markup' => t('Hi this is some entity info.'),
    '#suffix' => '</div>',
  );
  return $items;
}

/**
 * Allows modules to alter the result of all the iib_page_item hook invocations.
 *
 * @param $items
 *  A render array as returned from module_invoke_all for the iib_page_item hook.
 */
function hook_iib_page_item_alter(&$items) {
  $items['left']['#weight'] = 0;
}

/**
 * Allows modules to alter the result of all the iib_entity_item hook invocations.
 *
 * @param $items
 *  A render array as returned from module_invoke_all for the iib_entity_item hook.
 * @param $entity
 *  The entity currently being rendered
 * @param $extra
 *  An associative array with keys:
 *    entity_type : The type of the entity param (i.e. node)
 *    view_mode : The view mode which the entity is being rendered in.
 */
function hook_iib_entity_item_alter(&$items, $entity, $extra) {
  $items['info']['#weight'] = 0;
}

/**
 * iib_set_item allows modules to add items to the bar from any hook that runs before
 * hook_footer.
 *
 * @param $vars
 *  A render array to be placed into the items array.  Modules may use weight
 *  to control the placement of the items.  If more control is needed, a module should
 *  alter the results.
 *
 * @return NULL
 */
function example_module_node_view($vars) {
  iib_set_item($vars['node']->body);
}

/**
 * Allow modules to alter if the iib should be rendered but hidden
 *
 * @param bool
 */
function hook_iib_hidden_alter(&$hidden) {
  $hidden = TRUE;
}
