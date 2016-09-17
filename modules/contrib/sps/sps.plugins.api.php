<?php
/**
 * @file
 * SPS Plugins API Documentation.
 *
 * The sps module allows a set of plugins to expand what entities can be
 * overridden in preview mode, and how those overrides should be chosen.
 *
 *
 * Conditions are use for selecting which entity should be overridden, That info
 * is then passed on to override controllers which store the data in meaningful
 * ways.  Finally the Reaction Use that data to change things in Drupal so the
 * user may preview how overrides apply.
 *
 *  .-----------------------------.
 *  |      Condition Plugins      |
 *  |-----------------------------|
 *  | Find what overrides         |
 *  | should be in place           |
 *  | when previewing a site      |
 *  '-----------------------------'
 *                 |
 *                 |
 *                 v
 *  .-----------------------------.
 *  | Override Controller Plugins |
 *  |-----------------------------|
 *  | Store Overrides In          |
 *  | Ways for reactions          |
 *  | to use them                 |
 *  '-----------------------------'
 *                 |
 *                 |
 *                 v
 *  .-----------------------------.
 *  |      Reaction Plugins       |
 *  |-----------------------------|
 *  | Change what is displayed    |
 *  | based on what is in the     |
 *  | Override Controllers        |
 *  '-----------------------------'
 *
 * There are two other Plugins Widgets and Overrides, This are used by
 * Condition that use the BasicCondition class.  Condition of this class
 * search the plugin system for the correct widget and overrides to use @see
 * hook_sps_condition_plugins()
 */

/**
 * Used to define Condition for the SPS System.
 *
 * The condition plugin is used to collect, store condition information, and
 * then use that information to query what overrides should be in effect.
 * There is a default class for conditions
 * (Drupal\sps\Plugins\Condition\BasicCondition) which can be use in most
 * case.
 *
 * The hook should return an array of plugin arrays keyed by the name of the
 * condition. There are no required fields for condition plugins, if one is
 * going to use the default value for class (which is
 * Drupal\sps\plugins\Condition\BasicCondition) then the they should provide
 * instance_settings with items for name, widget and title.
 * if the class is stated it must be a class that implements
 * Drupal\sps\plugins\ConditionInterface
 *
 * @see Drupal\sps\plugins\ConditionInterface
 * @see Drupal\sps\plugins\Condition\BasicCondition
 *
 * There is one other key root_condition by default this is set to false.  if
 * it is set to TRUE then this condition will be excluded from use by the root
 * condition sps implements (default_root_condition).  This should be use if
 * one is planing on replacing the default_root_condition.
 *
 * BasicCondition
 *
 *   This base condition use widget and override plugins.  The widget to use
 *   is passed in as a instance setting and the name is use to find all
 *   override plugins that should be used by the condition.
 *  .----------------------------.
 *  |       date_condition       |
 *  |----------------------------|
 *  | class : BasicCondition     |
 *  | instance_settings :        |        .-----------------------------------.
 *  |   widget : date_widget ------------------------> date_widget            |
 *  |   name : date_condition  <------.   |-----------------------------------|
 *  '----------------------------'   |    | class : Drupal\sps\...\DateWidget |
 *                                   |    '-----------------------------------'
 *                                   |
 *                                   |    .------------------------------------.
 *                                   |    |            ers_override            |
 *                                   |    |------------------------------------|
 *                                   |    | class : Drupal\sps_ers\ERSOverride |
 *                                   '------condition : date_condition         |
 *                                        '------------------------------------'
 *
 * @see hook_sps_widget_plugins
 * @see Drupal\sps\Plugins\WidgetInterface
 * @see hook_sps_override_plugins
 * @see Drupal\sps\Plugins\OverrideInterface
 */
function hook_sps_condition_plugins() {
  return array(
    'date_condition' => array(
      'instance_settings' => array(
        'name' => 'date_condition',
        'widget' => 'date_widget',
        'title' => 'Site as of ...',
      ),
    ),
  );
}

/**
 * Define Override Controller (override storage objects) for the SPS System
 *
 * The override controllers provide an interface for reaction plugins to access
 * override information. Each plugin must say which api it implements and what
 * class is used.  There are two api and two plugins provided by sps.
 *
 * They system will only use one override controller per api key.  The system
 * first looks to a map table in the sps config, under the key
 * SPS_CONFIG_OVERRIDE_CONTROLLER, then it finds the first plugin that
 * implements a particular api.
 *
 * The two override controller provide by sps are the simple_cache and
 * temp_table plugins they implement the controller_api simple_cache and
 * mysql_table respectively.  For more detail about then one can see there
 * class or the interface they implement, but in general the simple_cache is a
 * simple storing of the overrides in its internals, with methods for retrieving
 * that data.  The temp_table has methods for pushing the override data into a
 * temp table, that can then be use in queries.  for example of how this are
 * used one can inspect the entity_load and entity_select_query_alter reactions
 * respectively.
 *
 * @see Drupal\sps\Manager::getOverrideControllerMap()
 * @see Drupal\sps\plugins\OverrideControllerInterface
 * @see Drupal\sps\plugins\OverrideController\SimpleCacheOverrideControllerInterface
 * @see Drupal\sps\plugins\OverrideController\TableOverrideStorageControllerInterface
 */
function hook_sps_override_controller_plugins() {
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
 * Define Reactions for the SPS System.
 *
 * The reaction plugins use current override data to react to the fact that
 * some entity values are overridden.  Each reaction must specify what
 * controller api in uses (see Override Controllers).  Defining a reaction by
 * it self will have no effect, one must also call the reaction using the
 * managers react method. as an example the entity_select_query_alter reaction
 * is call in sps_query_alter
 *
 *   function sps_query_alter($query) {
 *     $data = new stdClass();
 *     $data->query=$query;
 *     sps_get_manager()->react('entity_select_query_alter', $data);
 *   }

 * One should put all logic in to the react and not in to the hook use to call
 * it.  This why the manager can issue the the reaction should occur before
 * calling any of that logic.
 *
 * @see Drupal\sps\Manager::react
 *
 * The class is a mandatory value in the plugin and should implement
 * Drupal\sps\Plugins\ReactionInterface.  Also the use_controller_api is
 * mandatory, not if there are no override controllers that implement the api
 * stated then the reaction will not be run.
 *
 * @see hook_sps_override_controller_plugins
 * @see Drupal\sps\Plugins\ReactionInterface
 */
function hook_sps_reaction_plugins() {
  $reactions = array(
    'entity_load' => array(
      'class' => 'Drupal\sps\Plugins\Reaction\EntityLoadReaction',
      'use_controller_api' => 'simple_cache',
      'instance_settings' => array(),
    ),
  );
  return $reactions;
}

/**
 * Define Wddget plugins for use by sps conditions.
 *
 * Widget plugins are used by the BasicCondition class, they provide a widget
 * to be used in the sps preview form, this includes validation and a
 * transformation of the form data in to a array of data to be used by the
 * condition.
 *
 * The class used should implement the Drupal\sps\Plugins\WidgetInterface
 *
 * @see Drupal\sps\Plugins\WidgetInterface
 * @see hook_sps_condition_plugins()
 */
function hook_sps_widget_plugins() {
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
 * Define Override Object for use by sps Conditions.
 *
 * Override Plugins do the heavy lifting for Conditions that use the
 * BasicCondition Class. Each Override Plugin should say which condition should
 * use it.
 *
 * @see Drupal\sps\Plugins\WidgetInterface
 * @see hook_sps_condition_plugins()
 */
function hook_sps_override_plugins() {
  return array(
    'ers_override' => array(
      'class' => 'Drupal\sps_ers\ERSOverride',
      'condition' => 'date_condition',
    ),
  );
}

