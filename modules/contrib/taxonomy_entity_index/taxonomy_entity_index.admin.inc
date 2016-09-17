<?php

/**
 * Form builder for the batch reindexing form.
 */
function taxonomy_entity_index_admin_form() {
  // Retrieve a list of all entity types.
  $entities = module_invoke_all('entity_info');
  foreach ($entities as $key => $entity) {
    $options[$key] = $entity['label'];
  }
  $form['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Entity types'),
    '#options' => $options,
    '#description' => t('Re-index all terms for the selected entity types.'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Rebuild Index',
  );
  return $form;
}

/**
 * Form submit handler for the batch reindexing form.  Processes the batch.
 *
 * @see taxonomy_entity_index_admin_form()
 * @see taxonomy_entity_index_reindex()
 * @see taxonomy_entity_index_reindex_finished()
 */
function taxonomy_entity_index_admin_form_submit($form, &$form_state) {
  $types = array_filter($form_state['values']['types']);

  // Add an operation for each entity type.
  foreach ($types as $type) {
    $operations[] = array('taxonomy_entity_index_reindex_entity_type', array($type));
  }
  // Set a batch operation for each selected entity type.
  $batch = array(
    'operations' => $operations,
    'finished' => 'taxonomy_entity_index_reindex_finished',
    'file' => drupal_get_path('module', 'taxonomy_entity_index') . '/taxonomy_entity_index.admin.inc',
  );

  // Execute the batch.
  batch_set($batch);
}
