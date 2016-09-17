<?php
namespace Drupal\sps_ers;

use \Drupal\sps\Plugins\Override\NodeDateOverride;

class ERSOverride extends NodeDateOverride {
  protected $results = array();
  /**
   * Returns a list of vid's to override the default vids to load.
   *
   * @return
   *  An array of override vids.
   */
  public function getOverrides() {
    $select = db_select('ers_schedule', 'es')
      ->fields('es', array('schedule_id', 'entity_type', 'entity_id', 'revision_id'))
      ->condition('completed', 0)
      ->condition('publish_date', $this->timestamp, '<=')
      ->orderBy('publish_date')
      ->orderBy('revision_id');

    $this->results = $select->execute()->fetchAllAssoc('schedule_id', \PDO::FETCH_ASSOC);

    return $this->processOverrides();
  }

  protected function processOverrides() {
    $list = array();
    foreach($this->results as $key => $result) {
      if (isset($result['entity_id'])) {
        $result = array($result);
      }

      foreach($result as $sub => $row) {
        $transform = array();
        $transform['id'] = $row['entity_id'];
        $transform['type'] = $row['entity_type'];
        $transform['revision_id'] = $row['revision_id'] == 0 ? NULL : $row['revision_id'];
        $transform['status'] = $row['revision_id'] > 0 ? 1 : 0;
        $list[$row['entity_type'].'-'.$row['entity_id']] = $transform;
      }
    }

    return $list;
  }
}