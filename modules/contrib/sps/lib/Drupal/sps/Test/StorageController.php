<?php
namespace Drupal\sps\Test;
class StorageController implements \Drupal\sps\StorageControllerInterface {
  protected $cache = array();
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
    return isset($this->cache[$name]);
  }

 /**
  * Retrieve a cached object
  *
  * @param $name
  *   A string name use for retrieval
  * @return the object that was cached
  */
  public function get($name) {
    if ($this->exists($name)) {
      return $this->cache[$name];
    }
    throw new \Exception("Drupal\\sps\\Test\\PersistentStorageController does not have $name cached");
  }

}
