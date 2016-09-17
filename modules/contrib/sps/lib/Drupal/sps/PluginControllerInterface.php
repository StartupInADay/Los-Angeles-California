<?php

namespace Drupal\sps;

interface PluginControllerInterface {

  /**
   * Factory for building a plugin object.
   *
   * The loading of the plugin is not cached.
   *
   * @param $type
   *   the type of plugin as defined in hook_sps_plugin_types_info
   * @param $name
   *   the name of the plugin as defined in hook_sps_PLUGIN_TYPE_plugin_info;
   * @param $manager \Drupal\sps\Manager
   *
   * @return
   *   The plugin object
   */
  public function getPlugin($type, $name, \Drupal\sps\Manager $manager, $settings = NULL);

  /**
   * get meta info on a plugin
   *
   * @param $type
   *   the type of plugin as defined in hook_sps_plugin_types_info
   * @param $name
   *   the name of the plugin as defined in hook_sps_PLUGIN_TYPE_plugin_info;
   *
   * @return array
   *   an array of meta data for the plugin or an array of plugin arrays
   */
  public function getPluginInfo($type, $name=NULL);

  /**
   * get meta info on plugins for a given criteria
   *
   * @param $type
   *   the type of plugin as defined in hook_sps_plugin_types_info
   * @param $property
   *   the
   * @param $value
   *   the value to compare to the meta property
   *
   * @return
   *   an array of meta data for the plugins that match the criteria
   */
  public function getPluginByMeta($type, $property, $value);

}
