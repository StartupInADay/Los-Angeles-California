<?php
namespace Drupal\collection_sps;

use Drupal\sps\Plugins\AbstractPlugin;
use Drupal\sps\Plugins\OverrideControllerInterface;
use Drupal\sps\Plugins\Override\Override;

class SPSCollectionOverride extends Override  {
  protected $collection;

  public function getOverrides() {
    $return = array();
    $results = collection_field_get_revisions($this->collection);
    foreach ($results as $result) {
      foreach($result as $record) {
        $return[] = array(
          'id' => $record->entity_id,
          'type' => $record->entity_type,
          'revision_id' => $record->revision_id,
          'status' => 1,
        );
      }
    }

    return $return;
  }

  /**
   * Set the data for this override.
   *
   * This method should be called before get overrides and provides the
   * data which the override will use to find the available overrides.
   *
   * @param $data
   *    The data in the format specified by this overrides implementation
   *    of getDataConsumerApi().
   */
  public function setData($collection_name) {
    $this->collection = collection_load($collection_name);
  }

  /**
   * Report which data api this Override can consume.
   *
   * This allows overrides and widgets to be matched based on the
   * type of data which they consume and provide (respectively).
   *
   * @return
   *    A string designating the data api this override accepts
   */
  public function getDataConsumerApi() {
    return 'collection';
  }
}
