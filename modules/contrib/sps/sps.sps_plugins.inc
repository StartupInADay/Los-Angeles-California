<?php

/**
 * Implements hook_sps_reaction_plugins().
 */
function sps_sps_reaction_plugins() {
  $reactions = array(
    'entity_load' => array(
      'class' => 'Drupal\sps\Plugins\Reaction\EntityLoadReaction',
      'use_controller_api' => 'simple_cache',
      'instance_settings' => array(),
    ),
    'add_class' => array(
      'class' => 'Drupal\sps\Plugins\Reaction\AddClassReaction',
      'use_controller_api' => 'simple_cache',
      'instance_settings' => array(),
    ),
    'entity_select_query_alter' => array(
      'class' => 'Drupal\sps\Plugins\Reaction\EntitySelectQueryAlterReaction',
      'use_controller_api' => 'mysql_table',
      'instance_settings' => array(
        'entities' => array(),
      ),
    ),
  );

  // Lets add all of the entities that have revisions to the query_alter
  // and then lets add in the node revision fields.
  foreach (module_invoke_all('entity_info') as $type => $info) {
    if (isset($info['revision table'])) {
      $reactions['entity_select_query_alter']['instance_settings']['entities'][$type] = array(
        'base_table' => $info['base table'],
        'revision_table' => $info['revision table'],
        'base_id' => $info['entity keys']['id'],
        'revision_id' => $info['entity keys']['revision'],
      );
      $reactions['entity_select_query_alter']['instance_settings']['entities'][$type]['revision_fields'] = array_values(
        array_diff(
          array_intersect(
            drupal_schema_fields_sql($info['base table']),
            drupal_schema_fields_sql($info['revision table'])
          ),
          array(
            $info['entity keys']['id'],
            $info['entity keys']['revision'],
          )
        )
      );
    }
  }
  return $reactions;
}

/**
 * Implements hook_sps_override_controller_plugins().
 */
function sps_sps_override_controller_plugins() {
  return array(
    'simple_cache' => array(
      'class' => '\Drupal\sps\Plugins\OverrideController\SimpleCacheOverrideController',
      'implements_controller_api' => 'simple_cache',
    ),
    'temp_table' => array(
      'class' => '\Drupal\sps\Plugins\OverrideController\TempTableOverrideController',
      'implements_controller_api' => 'mysql_table',
    ),
  );
}

/**
 * Implements hook_sps_widget_plugins().
 *
 * Provide some default widgets.
 */
function sps_sps_widget_plugins() {
  return array(
    'date_widget' => array(
      'class' => 'Drupal\sps\Plugins\Widget\DateWidget',
      'instance_settings' => array(
        'title' => 'Preview Date',
      ),
    ),
  );
}

/**
 * Implements hook_sps_condition_plugins().
 *
 * Provide a basic condition to show.
 */
function sps_sps_condition_plugins() {
  return array(
    'date_condition' => array(
      'instance_settings' => array(
        'name' => 'date_condition',
        'widget' => 'date_widget',
        'title' => 'Site as of ...',
      ),
    ),
    'default_root_condition' => array(
      'class' => '\Drupal\sps\Plugins\Condition\WrapperCondition',
      'instance_settings' => array(),
      'root_condition' => 'True',
    ),
  );
}

