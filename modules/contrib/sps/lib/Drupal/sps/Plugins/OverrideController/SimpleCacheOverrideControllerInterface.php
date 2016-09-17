<?php
namespace Drupal\sps\Plugins\OverrideController;

interface SimpleCacheOverrideControllerInterface extends \Drupal\sps\Plugins\OverrideControllerInterface {
  
  /**
  * @brief 
  *
  * @param $id
  *   the int entity id
  * @param $type
  *   the type of entity
  *
  * @return 
  */
  public function getRevisionId($id, $type);
}
