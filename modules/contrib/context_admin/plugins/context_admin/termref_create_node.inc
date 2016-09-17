<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Create Node with auto term reference'),
  'description' => t('Creates a node with an automatic reference back to its parent term.'),
  'content form' => 'context_admin_termref_create_node_content_form',
  'content form submit' => 'context_admin_termref_create_node_content_form_submit',
  'render' => 'context_admin_termref_create_node_render_page',
  'form alter' => 'context_admin_termref_create_node_form_alter',
  'get child' => 'context_admin_termref_create_node_get_child',
  'get children' => 'context_admin_termref_create_node_get_children',
);

function context_admin_termref_create_node_get_child($plugin, $parent, $child) {
  $plugins = context_admin_termref_create_node_get_children($plugin, $parent);
  return $plugins[$parent . ':' . $child];
}

function context_admin_termref_create_node_get_children($plugin, $parent) {
  $entities = entity_get_info();
  $plugins = array();
  foreach ($entities as $entity_type => $entity) {
    foreach ($entity['bundles'] as $bundle_type => $bundle) {
      foreach (field_info_instances($entity_type, $bundle_type) as $field_name => $field) {
        $field_info = field_info_field($field_name);
        if ($field_info['type'] == 'taxonomy_term_reference' && $field['entity_type'] == 'node') {
          $target_node_type = $entities[$field['entity_type']]['bundles'][$field['bundle']];
          if (empty($field_info['settings']['allowed_values'][0]['vocabulary'])) {
            continue;
          }
          $target_bundle = $field_info['settings']['allowed_values'][0]['vocabulary'];
          $plugin['title'] = t('New @entity node from @source_bundle term on @field field', array('@entity' => $bundle['label'], '@source_bundle' => $entities['taxonomy_term']['bundles'][$target_bundle]['label'], '@field' => $field_name));
          $plugin['keyword'] = 'taxonomy_term';
          $plugin['required context'] = new ctools_context_required('Taxonomy Term', 'taxonomy_term', array('type' => $target_bundle));
          $plugin['name'] = $parent . ':' . $field['bundle'] . ':' . $field_name;
          $plugins[$parent . ':' . $field['bundle'] . ':' . $field_name] = $plugin;
        }
      }
    }
  }

  return $plugins;
}

function context_admin_termref_create_node_content_form($form, &$form_state, $contexts = array()) {
  $conf = $form_state['conf'];
  list($parent, $bundle, $field_name) = explode(':', $conf['context_admin_options']);
  ctools_include('dependent');
  if (isset($conf['context_admin_custom_redirect'])) {
    $custom_redirect = $conf['context_admin_custom_redirect'];
  }
  else {
    $custom_redirect = NULL;
  }

  $form['context_admin_options_items'] = array(
    '#type' => 'value',
    '#value' => $bundle,
  );

  $form['context_admin_custom_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom redirect path'),
    '#default_value' => $custom_redirect,
    '#description' => t('Define a custom path to redirect to after the node creation. This path will be translated with the node tokens of the parent node. Note: This overrides auto forwarding back to the original node.'),
  );
  $rows = array();
  foreach ($contexts as $context) {
    foreach (ctools_context_get_converters('%' . check_plain($context->keyword) . ':', $context) as $keyword => $title) {
      $rows[] = array(
        check_plain($keyword),
        t('@identifier: @title', array('@title' => $title, '@identifier' => $context->identifier)),
      );
    }
  }

  $header = array(t('Keyword'), t('Value'));
  $form['display_title']['contexts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Substitutions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#value' => theme('table', array('header' => $header, 'rows' => $rows)),
  );

  if (module_exists('panels')) {
    if (isset($conf['context_admin_use_node_edit'])) {
      $node_edit = $conf['context_admin_use_node_edit'];
    }
    else {
      $node_edit = NULL;
    }

    $form['context_admin_use_node_edit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Render the Add form using panels and page manager'),
      '#description' => t('Enable node/%node/edit in page manager. Unchecked renders Add form with Drupal\'s node templating system. This will prevent redirects from working.'),
      '#default_value' => $node_edit,
    );
  }

  return $form;
}

function context_admin_termref_create_node_content_form_submit($form, &$form_state) {
  list($parent, $bundle, $field_name) = explode(':', $form_state['conf']['context_admin_options']);
  $form_state['conf']['context_admin_options_items'] = $form_state['values']['context_admin_options_items'];
  $form_state['conf']['context_admin_field'] = $field_name;
  if (isset($form_state['values']['context_admin_custom_redirect'])) {
    $form_state['conf']['context_admin_custom_redirect'] = $form_state['values']['context_admin_custom_redirect'];
  }
  if (isset($form_state['values']['context_admin_use_node_edit'])) {
    $form_state['conf']['context_admin_use_node_edit'] = $form_state['values']['context_admin_use_node_edit'];
  }
}

function context_admin_termref_create_node_render_page($handler, $base_contexts, $args, $test = TRUE) {
  module_load_include('inc', 'node', 'node.pages');
  $type = $handler->conf['context_admin_options_items'];
  $fields = array(
    $handler->conf['context_admin_field'] => array(
      'language' => LANGUAGE_NONE,
      'key' => 'tid',
      'values' => array(
        $base_contexts[$handler->conf['submitted_context']]->data->tid
      ),
    ),
  );
  $use_panels = FALSE;
  if (isset($handler->conf['context_admin_use_node_edit'])) {
    $use_panels = $handler->conf['context_admin_use_node_edit'];
  }

  return context_admin_node_add_wrapper($type, $fields, $use_panels);
}

function context_admin_termref_create_node_form_alter(&$form, &$form_state, $form_id, $page) {
  switch($form_id) {
    case 'context_admin_node_form_wrapper':
      if ($form['#node']->type == $page['handler']->conf['context_admin_options_items']) {
        $form[$page['handler']->conf['context_admin_field']]['#access'] = FALSE;
        $form[$page['handler']->conf['context_admin_field']]['und']['#required'] = FALSE;

        if (!empty($page['handler']->conf['context_admin_custom_redirect'])) {
          $form['page_context'] = array(
            '#type' => 'value',
            '#value' => array(
              'contexts' => $page['contexts'],
              'submitted_context' => $page['handler']->conf['submitted_context'],
            ),
          );

          if ($page['handler']->conf['context_admin_custom_redirect']) {
            $form['context_admin_custom_redirect'] = array(
              '#type' => 'value',
              '#value' => $page['handler']->conf['context_admin_custom_redirect'],
            );
          }

          $form['actions']['submit']['#submit'][] = 'context_admin_termref_create_node_submit';
        }
      }
      break;
  }
}

function context_admin_termref_create_node_submit($form, &$form_state) {
  $form_state['no_redirect'] = FALSE;
  if (isset($form_state['values']['page_context'])) {
    $redirect = '';
    if (isset($form_state['values']['context_admin_custom_redirect']) && $form_state['values']['context_admin_custom_redirect']) {
      $redirect = ctools_context_keyword_substitute($form_state['values']['context_admin_custom_redirect'], array(), $form_state['values']['page_context']['contexts']);
    }
    else {
      $redirect = 'node/' . $form_state['values']['page_context']['contexts'][$form_state['values']['page_context']['submitted_context']]->data->nid;
    }

    if ($redirect) {
      $form_state['redirect'] = $redirect;
    }
  }
}
