<?php

/**
 * @file
 * Plugin to provide access control based upon existence in a particular menu.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Node: Reference Comparison"),
  'description' => t('Control access based upon a comparison between a node context and a node reference on another node context.'),
  'callback' => 'context_access_node_reference_compare_access_check',
  'default' => array('menu' => array()),
  'settings form' => 'context_access_node_reference_compare_access_contexts',
  'settings form submit' => 'context_access_node_reference_compare_access_settings_submit',
  'summary' => 'context_access_node_reference_compare_access_summary',
  'required context' => array(
    new ctools_context_required(t('Node'), 'node'),
    new ctools_context_required(t('Node'), 'node')
  ),
);

/**
 * Settings form for the 'by node_type' access plugin
 */
function context_access_node_reference_compare_access_contexts($form, $form_state, $conf) {
  ctools_include('dependent');
  $node_entities = entity_get_info('node');
  $types = field_info_instances('node');
  $options = array();
  $fields = array();
  if ($types) {
    foreach ($types as $type => $field_instances) {
      foreach ($field_instances as $field_name => $field) {
        $field_data = field_read_field($field_name);
        if ($field_data['type'] == 'node_reference') {
          $fields[$type][$field_name] = $field['label'];
          $options[$type] = $node_entities['bundles'][$type]['label'];
        }
      }
    }
  }
  if ($options) {
    $form['settings']['ref_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Node Reference Options'),
      '#description' => t('If you select a type below, the ability to load another node by node reference field becomes available, otherwise the two context nodes will simply be compared. If they have the same node id, this access plugin will return true.'),
      '#tree' => TRUE,
    );
    $form['settings']['ref_options']['node_types'] = array(
      '#type' => 'radios',
      '#title' => t('Select a node type with a reference to compare.'),
      '#options' => $options,
      '#default_value' => $conf['ref_options']['node_types'],
    );
    foreach ($fields as $key => $field_group) {
      $form['settings']['ref_options']['type_fields'][$key] = array(
        '#type' => 'radios',
        '#title' => t('Available Reference Fields'),
        '#description' => t('Choose a reference field from the available fields'),
        '#options' => $field_group,
        '#process' => array('ctools_dependent_process', 'form_process_radios'),
        '#dependency' => array('radio:settings[ref_options][node_types]' => array($key)),
        '#prefix' => '<div id="edit-settings-ref-options-type-fields-'. str_replace('_', '-', $key) .'-wrapper"><div>',
        '#suffix' => '</div></div>',
        '#default_value' => $conf['ref_options']['type_fields'][$conf['ref_options']['node_types']],
      );
    }
  }
  return $form;
}

/**
 * Compress the node_types allowed to the minimum.
 */
function context_access_node_reference_compare_access_settings_submit(&$form, &$form_state) {

}

/**
 * Check for access.
 */
function context_access_node_reference_compare_access_check($conf, $context) {
  if (empty($context)) {
    return FALSE;
  }
  foreach ($context as $data) {
    if (empty($data->data) || empty($data->data->nid)) {
      return FALSE;
    }
  }
  $keys = array_keys($context);
  if (!isset($conf['ref_options']['node_types'])) {
    return ($context[$keys[0]]->data->nid == $context[$keys[1]]->data->nid);
  }
  else {
    if ($context[$keys[1]]->data->type != $conf['ref_options']['node_types']) {
      return FALSE;
    }
    else {
      $ref = field_get_items('node', $context[$keys[1]]->data, $conf['ref_options']['type_fields'][$conf['ref_options']['node_types']]);
      if ($ref) {
        $ref_node = node_load($ref[0]['nid']);
        return ($context[$keys[0]]->data->nid == $ref_node->nid);
      }
    }
  }
  return FALSE;
}

/**
 * Provide a summary description based upon the checked node_types.
 */
function context_access_node_reference_compare_access_summary($conf, $context) {
  $keys = array_keys($context);
  if (!isset($conf['ref_options']['node_types'])) {
    return t('@node1 is equal to @node2', array('@node1' => $context[$keys[0]]->identifier, '@node2' => $context[$keys[1]]->identifier));
  }

  return t('@node1 is equal to the @field node reference on @node2', array('@node1' => $context[$keys[0]]->identifier, '@field' => $conf['ref_options']['type_fields'][$conf['ref_options']['node_types']], '@node2' => $context[$keys[1]]->identifier));
}
