<?php
namespace Drupal\sps\Plugins\OverrideController;

abstract class AbstractOverrideController implements \Drupal\sps\Plugins\OverrideControllerInterface {

  /**
  * @brief 
  *
  * @param $row
  *   an override dictionary 
  *
  * @return
  * NULL
  * @exception \Drupal\sps\Exception\InvalidOverrideRowException
  */
  public function validateRow($row) {
    if (!isset($row['id'])) {
      throw new \Drupal\sps\Exception\InvalidOverrideRowException("Override row must have id field");
    }
    if (!isset($row['type'])) {
      throw new \Drupal\sps\Exception\InvalidOverrideRowException("Override row must have type field");
    }
  
  }

}

