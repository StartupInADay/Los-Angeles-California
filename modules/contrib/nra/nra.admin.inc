<?php

/**
 * @file
 * Revision Content administration.
 */



/**
 * Return form for node administration filters.
 */
function nra_filter_form() {
  $session = isset($_SESSION['node_revision_filter']) ? $_SESSION['node_revision_filter'] : array();
  $filters = nra_revision_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
  );
  foreach ($session as $filter_key => $filter) {
    $filters[$filter_key]['form']['#default_value'] = $filter;
  }

  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status']['filters'][$key] = $filter['form'];
  }

  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
  );
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['status']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_js('misc/form.js');

  return $form;
}

/**
 * Process result from node administration filter form.
 */
function nra_filter_form_submit($form, &$form_state) {
  $filters = nra_revision_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
    case t('Refine'):
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter_key => $filter) {
        if (isset($form_state['values'][$filter_key]) && $form_state['values'][$filter_key] != '[any]') {
          $_SESSION['node_revision_filter'][$filter_key] = $form_state['values'][$filter_key];
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['node_revision_filter']);
      break;
    case t('Reset'):
      $_SESSION['node_revision_filter'] = array();
      break;
  }
}
/**
 * Menu callback: content administration.
 */
function nra_content_page($form, $form_state) {
  $form['filter'] = nra_filter_form();
  $form['#submit'][] = 'nra_filter_form_submit';
  $form['admin'] = nra_admin_nodes();

  return $form;
}

/**
 * Form builder: Builds the node administration overview.
 */
function nra_admin_nodes() {
  $admin_access = user_access('administer nodes');

  // Build the 'Update options' form.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
    '#access' => $admin_access,
  );
  $options = array();

  $operations = nra_get_node_revision_operations();
  foreach ($operations as $operation => $array) {
    $options[$operation] = $array['label'];
  }

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'approve',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('nra_admin_nodes_validate'),
    '#submit' => array('nra_admin_nodes_submit'),
  );

  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'nr.title'),
    'type' => array('data' => t('Type'), 'field' => 'n.type'),
    'author' => t('Author'),
    'status' => t('Status'),
  );

  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort');
  $query->join('node_revision', 'nr', 'n.nid = nr.nid');

  $filters = isset($_SESSION['node_revision_filter']) ? $_SESSION['node_revision_filter'] : array();
  $rows = $query
    ->fields('nr',array('nid', 'vid'))
    ->limit(50)
    ->orderByHeader($header)
    ->addTag('node_revision')
    ->addMetaData('filters', $filters)
    ->execute();

  $options = array();
  if ($rows) {
    // Prepare the list of nodes.
    $destination = drupal_get_destination();

    foreach ($rows as $row) {
      $node = node_load($row->nid, $row->vid);
      $key = "{$node->nid}:::{$node->vid}";
      $state = nra_get_revision_status($node);

      $options[$key] = array(
        'title' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => "$node->title (nid: $node->nid vid: $node->vid)",
            '#href' => "node/$node->nid/revisions/$node->vid/view",
          ),
        ),
        'type' => check_plain(node_type_get_name($node)),
        'author' => theme('username', array('account' => $node)),
        'status' => $state,
      );

      // Build a list of all the accessible operations for the current node.
      $operations = array();
      if (node_access('update', $node)) {
        $operations['edit'] = array(
          'title' => t('edit'),
          'href' => 'node/' . $node->nid . '/edit',
          'query' => $destination,
        );
      }
      if (node_access('delete', $node)) {
        $operations['delete'] = array(
          'title' => t('delete'),
          'href' => 'node/' . $node->nid . '/delete',
          'query' => $destination,
        );
      }

      $options[$key]['operations'] = array();
      if (count($operations) > 1) {
        // Render an unordered list of operations links.
        $options[$key]['operations'] = array(
          'data' => array(
            '#theme' => 'links__node_operations',
            '#links' => $operations,
            '#attributes' => array('class' => array('links', 'inline')),
          ),
        );
      }
      elseif (!empty($operations)) {
        // Render the first and only operation as a link.
        $link = reset($operations);
        $options[$key]['operations'] = array(
          'data' => array(
            '#type' => 'link',
            '#title' => $link['title'],
            '#href' => $link['href'],
            '#options' => array('query' => $link['query']),
          ),
        );
      }
    }
  }

  // Only use a tableselect when the current user is able to perform any
  // operations.
  if ($admin_access) {
    $form['nodes'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#empty' => t('No content available.'),
    );
  }
  // Otherwise, use a simple table.
  else {
    $form['nodes'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
      '#empty' => t('No content available.'),
    );
  }

  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}

/**
 * Validate node_admin_nodes form submissions.
 *
 * Check if any nodes have been selected to perform the chosen
 * 'Update option' on.
 */
function nra_admin_nodes_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['nodes']) || !count(array_filter($form_state['values']['nodes']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Process node_admin_nodes form submissions.
 *
 * Execute the chosen 'Update option' on the selected nodes.
 */
function nra_admin_nodes_submit($form, &$form_state) {
  $operations = nra_get_node_revision_operations();

  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked nodes
  $keys = array_filter($form_state['values']['nodes']);
  $keys = array_map('_nra_split_keys', $keys);

  if ($function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($keys), $operation['callback arguments']);
    }
    else {
      $args = array($keys);
    }
    call_user_func_array($function, $args);

    cache_clear_all();
  }
  else {
    // We need to rebuild the form to go to a second step. For example, to
    // show the confirmation form for the deletion of nodes.
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Helper function to to split the keys
 */
function _nra_split_keys($key) {
  list($nid, $vid) = explode(':::', $key);
  return array(
    'vid' => $vid,
    'nid' => $nid,
  );
}

