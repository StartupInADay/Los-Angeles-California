<?php
namespace Drupal\sps\Plugins\OverrideController;

abstract class AbstractTableOverrideStorageController implements TableOverrideStorageControllerInterface {
  /**
  * @brief 
  *
  * @return 
  * A dictionary of properties and the field name on the override table
  *
  */
  public function getPropertyMap() {
    $properties = array();
    if(!empty($this->table)) {
      $properties = array_keys(call_user_func_array('array_merge', $this->table));
      $values = array_map(function($v) { return "override_$v"; }, $properties);
      $properties = array_combine($properties, $values);
      unset($properties['type']);
      unset($properties['id']);
    }
    $properties['revision_id'] = isset($properties['revision_id']) ? $properties['revision_id'] : 'override_revision_id';
    
    return $properties;
  }

}
