<?php

namespace Drupal\sps;

interface StateControllerInterface {

  /**
   * Cache away a object.
   *
   * @param $cache
   *   an object to be cached
   * @return
   *   NULL
   */
  public function set($cache);

  /**
   * Test if we have an object cached.
   *
   * This should be less expensive then using get
   *
   * @return
   *   bool
   */
  public function exists();

  /**
   * Retrieve a cached object.
   *
   * @param $name
   *   A string name use for retrieval
   * @return
   *   the object that was cached
   */
  public function get();

  /*
   * Clear out the current site state
   */
  public function clear();
}
