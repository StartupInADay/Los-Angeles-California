<?php
namespace Drupal\sps\Plugins\OverrideController;

interface TableOverrideStorageControllerInterface extends \Drupal\sps\Plugins\OverrideControllerInterface {

  /**
  * @brief 
  *
  * @param $query
  *   The query to alter by adding the override table
  * @param $base_alais
  *   The base table to join on
  * @param $base_id
  *   The base id to use in the join
  * @param $overrides_alais
  *   The alais to use for the override table
  * @param $type
  *   The type of entity to be overriding
  *
  * @return 
  */
  public function addOverrideJoin(\SelectQueryInterface $query, $base_alais, $base_id, $overrides_alais, $type);

  /**
  * @brief 
  *
  * @return 
  * A dictionary of properties and the field name on the override table
  *
  */
  public function getPropertyMap();

}
