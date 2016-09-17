<?php

/**
 * @file
 * Handle rendering entity fields as panes.
 */

$plugin = array(
  'title' => t('Context Admin Plugins'),
  'content type' => 'context_admin_context_admin_content_type_content_type',
);

/**
 * Just one subtype.
 */
function context_admin_context_admin_content_type_content_type($subtype) {
  $types = context_admin_context_admin_content_type_content_types();
  if (isset($types[$subtype])) {
    return $types[$subtype];
  }
}

/**
 * Return all context_admin plugins.
 */
function context_admin_context_admin_content_type_content_types() {
  $types = &drupal_static(__FUNCTION__, array());
  if (!empty($types)) {
    return $types;
  }

  $plugins = ctools_get_plugins('context_admin', 'context_admin');

  foreach ($plugins as $plugin_name => $plugin) {
    $types['context_admin:' . $plugin_name] = array(
      'category' => t('Context Admin: Experimental'),
      'edit form' => 'context_admin_context_admin_content_type_form',
    );
    if (isset($plugin['title'])) {
      $types['context_admin:' . $plugin_name]['title'] = $plugin['title'];
    }
    if (isset($plugin['description'])) {
      $types['context_admin:' . $plugin_name]['description'] = $plugin['description'];
    }
    //if (isset($plugin['edit form'])) {
      //$types['context_admin:' . $plugin_name]['edit form'] = $plugin['content form'];
    //}
    if (isset($plugin['required context'])) {
      $types['context_admin:' . $plugin_name]['required context'] = $plugin['required context'];
    }
  }

  return $types;
}

/**
* Render the custom content type.
*/
function context_admin_context_admin_content_type_render($subtype, $conf, $panel_args, $context) {
  list( , $plugin_name) = explode(':', $subtype, 2);
  $plugin = ctools_get_plugins('context_admin', 'context_admin', $plugin_name);
  // This logic should be extend to work with multiple contexts.
  if (isset($plugin['required context'])) {
    $count = count($plugin['required context']);
    if ($count === 1 && $plugin['required context']->required) {
      if (empty($context) || empty($context->data)) {
        return;
      }
    }
  }

  $contexts = array();
  if (isset($conf['context'])) {
    $conf['submitted_context'] = $conf['context'];
    $contexts[$conf['context']] = $context;
  }
  $conf['context_admin_options'] = $plugin['name'];
  $handler = new stdClass();
  $handler->conf = $conf;

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'context_admin';
  $block->title = $plugin['title'];
  if ($conf['override_title']) {
    $block->title = $conf['override_title_text'];
  }
  if ($function = ctools_plugin_get_function($plugin, 'render')) {
    // We have to get creative and swap the current page, so we're going to
    // save a copy to reset back to later.
    $current_page = page_manager_get_current_page();
    $page = array(
      'handler' => $handler,
      'contexts' => $contexts,
    );
    page_manager_get_current_page($page);
    $output = $function($handler, $contexts, array());
    // Reset the page back to its proper settings in case it was changed.
    page_manager_get_current_page($current_page);
    $block->content = $output;
  }
  else {
    $block->content = '';
  }

  return $block;
}

function context_admin_context_admin_content_type_form($form, &$form_state) {
  list( , $plugin_name) = explode(':', $form_state['pane']->subtype, 2);
  $form_state['conf']['context_admin_options'] = $plugin_name;
  $plugin = ctools_get_plugins('context_admin', 'context_admin', $plugin_name);
  if ($function = ctools_plugin_get_function($plugin, 'content form')) {
    $contexts = isset($form_state['contexts']) ? $form_state['contexts'] : array();
    $form += $function($form, $form_state, $contexts);
  }
  return $form;
}

function context_admin_context_admin_content_type_form_submit($form, &$form_state) {
  list( , $plugin_name) = explode(':', $form_state['pane']->subtype, 2);
  $form_state['conf']['context_admin_options'] = $plugin_name;
  $plugin = ctools_get_plugins('context_admin', 'context_admin', $plugin_name);
  if ($function = ctools_plugin_get_function($plugin, 'content form submit')) {
    $function($form, $form_state);
  }
}

/**
* Returns the administrative title for a type.
*/
function context_admin_context_admin_content_type_admin_title($subtype, $conf, $context) {
  list( , $plugin_name) = explode(':', $subtype, 2);
  $plugin = ctools_get_plugins('context_admin', 'context_admin', $plugin_name);
  return t('Context Admin: @plugin_title', array('@plugin_title' => $plugin['title']));
}
