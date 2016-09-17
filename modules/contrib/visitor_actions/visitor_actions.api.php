<?php

/**
 * CTools hook for defining and ActionableElement type.
 *
 * ActionableElement type plugins allow for the definition of
 * various actions that can be taken on a particular type of element,
 * e.g. a form, a link, or a page. In fact, there is a plugin provided
 * by visitor_actions module itself for each of the above three types
 * of elements. The reason for defining the elements and the types of
 * actions that can be taken on them, is to be able to provide a
 * helpful UI so that site owners can easily set up custom actions on
 * these elements.
 *
 * Each ActionableElement plugin must define a class that implements the
 * ActionableElementInterface. In many cases, the element type will require
 * some sort of identifier (e.g. a form id for forms, a selector for links)
 * and so the class will implement the IdentifiableActionableElementInterface.
 *
 * @see plugins/actionable_element/FormActionableElement.inc
 * @see plugins/actionable_element/LinkActionableElement.inc
 * @see plugins/actionable_element/PageActionableElement.inc
 */
function hook_visitor_actions_actionable_element() {
  $path = drupal_get_path('module', 'my_module') . '/plugins/actionable_element';
  // A module could define a NodeActionableElement plugin. The class for this
  // would implement the getActionOptions() method to return a list of all the
  // things people could do to a node, e.g. "comments on", "views", "edits", etc.
  $info['node'] = array(
    'path' => $path,
    'handler' => array(
      'file' => 'NodeActionableElement.inc',
      'class' => 'NodeActionableElement',
    ),
  );
}

/**
 * Hook for defining custom actions in code.
 *
 * While many custom actions will be created via the UI either using
 * visitor_actions_ui module or using the administrative UI in visitor_actions
 * module, custom actions can also be defined in code using this hook.
 *
 * A custom action can either specify a plugin that is responsible for
 * triggering the action, or it can correspond to an action that will be
 * triggered at some point (e.g. in a particular hook implementation like
 * hook_user_login or hook_node_view) by the module itself. In the latter
 * case, the only information that is needed is the action machine name and
 * a human-readable label, allowing other modules to subscribe to it.
 *
 * @return array
 *   An associative array with custom action names as keys and whose
 *   values are associative arrays with the following keys:
 *   - label The administrative label for this action
 *   - (optional) client_side Whether this is a client-side action
 *   - (optional) plugin The plugin that will trigger the action (required
 *     if client_side)
 *   - (optional) event The specific event, e.g. 'click' or 'hover' for a
 *     link (required if client_side)
 *
 */
function hook_visitor_actions_info() {
  $actions = array(
    // The module defining this action would be responsible for implementing
    // hook_node_view() and triggering the 'node_view' action.
    'node_view' => array(
      'label' => t('views any node'),
    ),
  );
  return $actions;
}

/**
 * Subscribe to a particular action.
 *
 * Returns an array of callbacks to be called when the passed in action
 * is triggered. Each callback function will be passed the action name and
 * an array of context for the action.
 */
function hook_visitor_action_subscribe($action) {
  // Assuming my_visitor_action_subscriber is a function described thus:
  //   function my_visitor_action_subscriber($name, $context) {
  //     // do something.
  //   }
  return array('my_visitor_action_subscriber');
}

/**
 * Respond to an action being saved.
 *
 * Invoked after a custom action is saved to the database.
 */
function hook_visitor_actions_save_action() {

}

/**
 * Respond to deletion of an action.
 *
 * Invoked after a custom action is deleted from the database.
 */
function hook_visitor_actions_delete_action() {

}

/**
 * React to client-side actions being loaded on the page.
 *
 * @param $page
 *   The page array after visitor_actions module's hook_page_build()
 *   has run.
 * @param $actions
 *   The list of client-side actions that have been added to the js
 *   settings, allowing another module to add its own client-side
 *   subscriber.
 */
function visitor_actions_page_build(&$page, $actions) {

}
