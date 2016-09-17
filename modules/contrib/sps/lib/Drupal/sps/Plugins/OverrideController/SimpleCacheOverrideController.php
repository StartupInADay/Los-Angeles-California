<?php
namespace Drupal\sps\Plugins\OverrideController;

class SimpleCacheOverrideController extends \Drupal\sps\Plugins\OverrideController\AbstractOverrideController implements SimpleCacheOverrideControllerInterface {

  protected $overrides = array();

  public function __construct(array $settings, \Drupal\sps\Manager $manager) {}
  /**
   * Set what data should be cached way for retrival.
   *
   * @param $overrides
   *   a array of arrays with the following keys 
   *     id, revision_id, type
   */
  public function set($overrides) {
    foreach($overrides as $row) {
      $this->validateRow($row);
      $this->overrides[$this->getKey($row)] = $row;
    }

  }

  /**
  * generate a lookup key for a override row
  *
  * @param $row
  * an array with keys id, revision_id, type
  * @return 
  * a key built out of the id and type
  */
  protected function getKey($row) {
    return $row['id'] . '::' . $row['type'];
  }

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
  public function getRevisionId($id, $type) {
    if(isset($this->overrides[$this->getKey(array('id' => $id, 'type' => $type))])) {
      return $this->overrides[$this->getKey(array('id' => $id, 'type' => $type))];
    }
  }
}
