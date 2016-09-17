<?php

namespace Drupal\sps\Plugins;

class AbstractPlugin implements PluginInterface {
  protected $settings = array();
  protected $manager;

  /**
   * The construct that is expect by the plugin system
   *
   * @param $settings array
   * @param $manager  \Drupal\sps\Manager
   */
  public function __construct(array $settings, \Drupal\sps\Manager $manager) {
    $this->settings = $settings;
    $this->manager = $manager;
  }

  /**
   * Get a Settings
   *
   * @param $key string
   *  The key of the settings array to retrieve
   *
   * @return mixed|NULL
   *  Value of the settings
   */
  public function getSetting($key) {
    if (isset($this->settings[$key])) {
      return $this->settings[$key];
    }

    return NULL;
  }
}
