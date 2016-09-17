<?php

namespace Drupal\sps\EntityController;


class EntityControllerWrapper implements  \DrupalEntityControllerInterface {

  protected $controller;
  protected $info;
  protected $type;
  /**
   * Constructor.
   *
   * @param $entityType
   *   The entity type for which the instance is created.
   */
  public function __construct($entityType) {
    $this->info = sps_drupal()->entity_get_info($entityType);
    $class = $this->info['controller class base'];
    $this->controller = new $class($entityType);
    $this->type = $entityType;
  }

  /**
   * Resets the internal, static entity cache.
   *
   * @param $ids
   *   (optional) If specified, the cache is reset for the entities with the
   *   given ids only.
   */
  public function resetCache(array $ids = NULL) {
    return $this->controller->resetCache($ids);
  }

  /**
   * Loads one or more entities.
   *
   * @param $ids
   *   An array of entity IDs, or FALSE to load all entities.
   * @param $conditions
   *   An array of conditions in the form 'field' => $value.
   *
   * @return
   *   An array of entity objects indexed by their ids. When no results are
   *   found, an empty array is returned.
   */
  public function load($ids = array(), $conditions = array()) {
    // If not loading a specific revision, look for and load a revision matching
    // the currently active revision tag.
    if (($revision_id_key = $this->info['entity keys']['revision']) &&
        empty($conditions[$revision_id_key])) {
      $data = new \StdClass();
      $data->base_id_key = $this->info['entity keys']['id'];
      $data->revision_id_key = $revision_id_key;
      $data->base_table = $this->info['base table'];
      $data->type = $this->type;
      $data->ids = $ids;
      if($revision_ids  = sps_drupal()->sps_get_manager()->react('entity_load', $data)) {
        $conditions[$revision_id_key] = $revision_ids;
      }
    }
    return $this->controller->load($ids, $conditions);
  }

  public function __call($name, $args) {
    return call_user_func_array(array($this->controller, $name), $args);
  }

  /**
   * Create a new entity.
   *
   * We need a creation callback because the entity API module requires
   * the EntityAPIControllerInterface. That way we ensure the controller wrapper
   * works with controllers that don't implement EntityAPIControllerInterface.
   *
   * @see EntityAPIControllerInterface::create()
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   *
   * @return object
   *   A new instance of the entity type.
   */
  public function create($values) {
    if (method_exists($this->controller, 'create')) {
      return $this->controller->create($values);
    }
    // If there's no create method just convert the values to an object.
    return (object) $values;
  }
}
