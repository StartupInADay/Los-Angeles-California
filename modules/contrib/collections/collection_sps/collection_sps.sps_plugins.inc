<?php

/**
 * implements hook_sps_condition_plugins
 */
function collection_sps_sps_condition_plugins() {
  return array(
    'collection_condition'=> array(
      'instance_settings' => array(
        'name' => 'collection_condition',
        'widget' => 'collection_widget',
        'title' => t('Collection'),
      ),
    ),
  );
}

/**
 * Implements hook_sps_widget_plugins().
 *
 * Provide some default widgets.
 */
function collection_sps_sps_widget_plugins() {
  return array(
    'collection_widget'=> array(
      'class' => 'Drupal\collection_sps\SPSCollectionWidget',
      'instance_settings' => array(),
    ),
  );
}

/**
 * Implements hook_sps_override_plugins().
 *
 * Provide some basic overrides for workflow modules.
 */
function collection_sps_sps_override_plugins() {
  return array(
    'collection_override'=> array(
      'class' => 'Drupal\collection_sps\SPSCollectionOverride',
      'condition' => 'collection_condition',
    ),
  );
}
