<?php
namespace Drupal\sps\StorageController;

define('SPS_SITE_STATE_STORAGE_KEY', "sps_site_state_storage_key");
/**
 * Defines a PersistentStorage Controller that uses ctools_object_cache
 *
 *
 * TODO This class is not functional, and more work is need to be done
 * here. the current code is just for helping to describe direction.
 *
 */
class CToolsObjectCache implements \Drupal\sps\StateControllerInterface {
 protected $obj = 'sps-ctools-object-cache';
 protected $key = 'sps_site_state_storage_key';


 public function __construct($key = NULL) {
   if($key) {
     $this->key = $key;
   }
 }
 /**
  * Cache away a object
  *
  * @param $name
  *   A string name use for retrieval
  * @param $cache
  *   an object to be cached
  * @return NULL
  */
public function set($cache) {
   $_SESSION[$this->obj][$this->key] = TRUE;
   ctools_include('object-cache');
   ctools_object_cache_set($this->obj, $this->key, $cache);
 }
 /**
  * Test if we have an object cached
  * This should be less expensive then using get
  *
  * @param $name
  *   A string name use for retrieval
  * @return bool
  */
 public function exists() {
   return isset($_SESSION[$this->obj][$this->key]);
 }
 /**
  * Retrieve a cached object
  *
  * @param $name
  *   A string name use for retrieval
  * @return the object that was cached
  */
 public function get() {
   ctools_include('object-cache');
   return ctools_object_cache_get($this->obj, $this->key);
 }

  /*
   * Clear out the current site state
   */
  public function clear() {
   ctools_include('object-cache');
   unset($_SESSION[$this->obj][$this->key]);
   return ctools_object_cache_clear($this->obj, $this->key);
  }
}
