<?php

namespace Drupal\sps\Plugins;

interface PluginInterface {
  /**
   * The construct that is expect by the plugin system
   *
   * @param $settings array
   * @param $manager \Drupal\sps\Manager
   */
  public function __construct(array $settings, \Drupal\sps\Manager $manager);

  /**
   * Get a Settings
   *
   * @param $key string
   *  The key of the settings array to retrieve
   */
  //public function getSetting($key);
}
