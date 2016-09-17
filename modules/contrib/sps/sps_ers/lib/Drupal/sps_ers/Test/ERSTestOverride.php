<?php
namespace Drupal\sps_ers\Test;

use \Drupal\sps_ers\ERSOverride;

class ERSTestOverride extends ERSOverride {
  /**
   * Override the getOverrides function to do nothing but
   * call processOverrides so we can test it.
   */
  public function getOverrides() {
    return $this->processOverrides();
  }

  /**
   * Provide an easy way to set data for processOverrides to deal with
   */
  public function setResults($data) {
    $this->results = $data;
  }
}