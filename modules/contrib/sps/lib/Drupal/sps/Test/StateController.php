<?php
namespace Drupal\sps\Test;
class StateController implements \Drupal\sps\StateControllerInterface {
  public $cache;
  /**
  * Cache away a object
  *
  * @param $cache
  *   an object to be cached
  * @return NULL
  */
  public function set($cache) {
    $this->cache = $cache;
  }

/**
  * Test if we have an object cached
  * This should be less expensive then using get
  *
  * @return bool
  */
  public function exists() {
    return isset($this->cache);
  }

 /**
  * Retrieve a cached object
  *
  * @return the object that was cached
  */
  public function get(){
    if ($this->exists()) {
      return $this->cache;
    }
    throw new \Exception("Drupal\\sps\\Test\\PersistentStorageController does not have state cached");
  }

  public function clear() {
    unset($this->cache);
  }
}
