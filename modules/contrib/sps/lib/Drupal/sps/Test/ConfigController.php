<?php
namespace Drupal\sps\Test;
class ConfigController implements \Drupal\sps\StorageControllerInterface {
  protected $cache = array();
  public $default = array(
    SPS_CONFIG_PLUGIN_CONTROLLER => array(
      'class' => '\Drupal\sps\Test\PluginController',
      'instance_settings' => array(),
    ),
    SPS_CONFIG_DRUPAL_CONTROLLER => array(
      'class' => '\Drupal\sps\Drupal',
      'instance_settings' => array(),
    ),
    SPS_CONFIG_STATE_CONTROLLER => array(
      'class' => '\Drupal\sps\Test\StateController',
      'instance_settings' => array(),
    ),
    SPS_CONFIG_SITESTATE => '\Drupal\sps\Test\SiteState',
  );

  /**
  * Cache away a object
  *
  * @param $name
  *   A string name use for retrieval
  * @param $cache
  *   an object to be cached
  * @return NULL
  */
  public function set($name, $cache) {
    $this->cache[$name] = $cache;
  }

/**
  * Test if we have an object cached
  * This should be less expensive then using get
  *
  * @param $name
  *   A string name use for retrieval
  * @return bool
  */
  public function exists($name) {
    return isset($this->cache[$name]) ||isset($this->default[$name]);
  }

 /**
  * Retrieve a cached object
  *
  * @param $name
  *   A string name use for retrieval
  * @return the object that was cached
  */
  public function get($name) {
    if (isset($this->cache[$name])) {
      return $this->cache[$name];
    }
    if (isset($this->default[$name])) {
      return $this->default[$name];
    }
    throw new \Exception("Drupal\\sps\\Test\\PersistentStorageController does not have $name cached");
  }

}
