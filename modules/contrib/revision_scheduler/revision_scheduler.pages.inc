<?php

function revision_scheduler_list_page($entity_type, $entity) {
  list($entity_id) = entity_extract_ids($entity_type, $entity);

  $header = array(
    t('Revision'),
    t('Action'),
    array(
      'data' => t('Scheduled Date'),
      'sort' => 'desc',
      'field' => 'time_scheduled',
    ),
    t('Operations'),
  );

  $query = db_select('revision_scheduler', 'r')
    ->extend('TableSort')
    ->extend('PagerDefault')
    ->limit(25)
    ->fields('r')
    ->orderByHeader($header)
    ->condition('entity_type', $entity_type)
    ->condition('entity_id', $entity_id);

  // For operations that have the same scheduled time, sort by executed time,
  // and then queued time, and then ID since that is the order of execution.
  // @todo This should probably be removed at some point.
  $order = $query->getOrderBy();
  if (!empty($order['time_scheduled'])) {
    $query->orderBy('time_executed', $order['time_scheduled']);
    $query->orderBy('time_queued', $order['time_scheduled']);
    $query->orderBy('id', $order['time_scheduled']);
  }

  $operations = $query->execute()->fetchAll();

  $revision_ids = array();
  foreach ($operations as $operation) {
    $revision_ids[] = $operation->revision_id;
  }
  $revisions = revision_scheduler_entity_revision_load_multiple($entity_type, $entity_id, $revision_ids);

  $rows = array();
  foreach ($operations as $id => $operation) {
    $revision = !empty($revisions[$operation->revision_id]) ? $revisions[$operation->revision_id] : FALSE;
    $operation_info = revision_scheduler_entity_revision_operation_get_info($entity_type, $operation->operation);

    $row = array(
      t('Revision @id: @label', array('@id' => $operation->revision_id, '@label' => $revision ? entity_label($entity_type, $revision) : t('(Deleted)'))),
      check_plain($operation_info['label']),
      format_date($operation->time_scheduled),
    );

    if (!empty($operation->time_executed)) {
      $row[] = t('Completed on @date', array('@date' => format_date($operation->time_executed)));
    }
    elseif (!empty($operation->time_queued)) {
      $row[] = t('Queued on @date', array('@date' => format_date($operation->time_queued)));
    }
    else {
      $operations = array();
      $value = implode('-', array('run', $operation->id, $operation->operation, $operation->time_scheduled));
      $operations['run'] = array(
        'title' => t('Run now'),
        'href' => 'revision-scheduler/' . $operation->id . '/run',
        'query' => array(
          'token' => drupal_get_token($value),
        ) + drupal_get_destination(),
      );
      $operations['edit'] = array(
        'title' => t('Edit'),
        'href' => 'revision-scheduler/' . $operation->id . '/edit',
        'query' => drupal_get_destination(),
      );
      $operations['delete'] = array(
        'title' => t('Cancel'),
        'href' => 'revision-scheduler/' . $operation->id . '/delete',
        'query' => drupal_get_destination(),
      );
      $row[] = array(
        'data' => array(
          '#theme' => 'links',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }

    $rows[$id] = $row;
  }

  $build['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No scheduled revisions available.'),
  );

  // Add the pager
  $build['pager'] = array(
    '#theme' => 'pager',
    '#weight' => 5,
  );

  return $build;
}

function revision_scheduler_add_form($form, &$form_state, $entity_type, $entity) {
  if (!revision_scheduler_entity_type_has_revisions($entity_type)) {
    drupal_access_denied();
    exit;
  }

  $revision_ids = revision_scheduler_get_all_entity_revision_ids($entity_type, $entity);
  list($entity_id) = entity_extract_ids($entity_type, $entity);
  $revisions = revision_scheduler_entity_revision_load_multiple($entity_type, $entity_id, $revision_ids);
  krsort($revisions);

  // Pass the form into the edit form to get most of the form elements.
  $operation = new stdClass();
  $operation->id = NULL;
  $operation->entity_type = $entity_type;
  $operation->entity_id = $entity_id;
  $operation->revision_id = NULL;
  $operation->operation = '';
  $operation->time_scheduled = 0;
  $operation->time_queued = 0;
  $operation->time_executed = 0;
  $operation->uid = $GLOBALS['user']->uid;
  $form = revision_scheduler_edit_form($form, $form_state, $operation);

  $form['revision_id'] = array(
    '#type' => 'select',
    '#title' => t('Revision'),
    '#options' => array(),
    '#required' => TRUE,
  );
  foreach ($revisions as $revision_id => $revision) {
    $form['revision_id']['#options'][$revision_id] = t('Revision @id: @label', array('@id' => $revision_id, '@label' => entity_label($entity_type, $revision)));
  }

  $form['#validate'][] = 'revision_scheduler_edit_form_validate';
  $form['#submit'][] = 'revision_scheduler_edit_form_submit';

  return $form;
}

function revision_scheduler_edit_form($form, &$form_state, $operation) {
  $form['id'] = array('#type' => 'value', '#value' => $operation->id);
  $form['entity_type'] = array('#type' => 'value', '#value' => $operation->entity_type);
  $form['entity_id'] = array('#type' => 'value', '#value' => $operation->entity_id);
  $form['revision_id'] = array('#type' => 'value', '#value' => $operation->revision_id);
  $form['time_queued'] = array('#type' => 'value', '#value' => $operation->time_queued);
  $form['time_executed'] = array('#type' => 'value', '#value' => $operation->time_executed);
  $form['uid'] = array('#type' => 'value', '#value' => $operation->uid);

  $entity = revision_scheduler_entity_revision_load($operation->entity_type, $operation->entity_id, $operation->revision_id);

  $form['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#options' => revision_scheduler_entity_revision_operation_get_options($operation->entity_type, $entity),
    '#default_value' => $operation->operation,
    '#required' => TRUE,
  );

  $date_format = 'Y-m-d H:i';
  $form['datetime'] = array(
    '#type' => 'textfield',
    '#title' => t('Scheduled date'),
    '#default_value' => !empty($operation->time_scheduled) ? format_date($operation->time_scheduled, 'custom', $date_format) : '',
    '#required' => TRUE,
    '#description' => t('Format: %time. The date format is YYYY-MM-DD and the timezone is %timezone.', array('%time' => format_date(REQUEST_TIME, 'custom', $date_format), '%timezone' => drupal_get_user_timezone())),
  );

  if (module_exists('date_popup')) {
    $form['datetime']['#type'] = 'date_popup';
    $form['datetime']['#date_format'] = $date_format;
    $form['datetime']['#date_year_range'] = '0:+5';
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Schedule'),
  );

  if (!isset($_GET['destination']) && $uri = entity_uri($operation->entity_type, $entity)) {
    $_GET['destination'] = $uri['path'];
  }

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => '<front>',
    '#weight' => 10,
  );

  if (isset($_GET['destination'])) {
    $url = drupal_parse_url(urldecode($_GET['destination']));
    $form['actions']['cancel']['#href'] = $url['path'];
    if (!empty($url['query'])) {
      $form['actions']['cancel']['#options']['query'] = $url['query'];
    }
  }

  return $form;
}

function revision_scheduler_edit_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['datetime']) && is_string($form_state['values']['datetime'])) {
    $date = strtotime($form_state['values']['datetime']);
    if ($date === FALSE) {
      form_set_error('datetime', t('Invalid date.'));
    }
    elseif ($date < REQUEST_TIME) {
      form_set_error('datetime', t('You cannot set a scheduled date in the past.'));
    }
    $form_state['values']['time_scheduled'] = $date;
  }
}

function revision_scheduler_edit_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $operation = (object) $form_state['values'];

  revision_scheduler_operation_save($operation);
  drupal_set_message('The scheduled revision operation has been saved.');
}

function revision_scheduler_delete_form($form, &$form_state, $operation) {
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $operation->id,
  );

  $entity = revision_scheduler_entity_revision_load($operation->entity_type, $operation->entity_id, $operation->revision_id);

  return confirm_form(
    $form,
    'Are you sure you want to cancel this scheduled revision?',
    entity_uri($operation->entity_type, $entity)
  );
}

function revision_scheduler_delete_form_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  revision_scheduler_operation_delete($id);
  drupal_set_message('The scheduled revision operation has been cancelled.');
}
