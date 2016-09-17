<?php

namespace Drupal\sps\Plugins\Override;

use Drupal\sps\Plugins\OverrideInterface;

class AggregatorOverride implements OverrideInterface {
  protected $override_table = array();
  protected $overrides_set = FALSE;

  /**
   * Implementation of OverrideInterface::setData().
   *
   * Take in other overrides and put their overrides together.
   *
   * @param array $data
   *  An array of overrides.
   *
   * @return \Drupal\sps\Plugins\Override\AggregatorOverride
   * @throws \Drupal\sps\Exception\InvalidOverrideException
   *  If 2 overrides passed as data contain overrides for the same type.
   */
  public function setData($data) {
    $this->override_table = array();
    foreach ($data as $override) {
      $overrides = $override->getOverrides();
      foreach ($overrides as $type => $items) {
        if (!empty($this->override_table[$type]) && !empty($items)) {
          throw new \Drupal\sps\Exception\InvalidOverrideException(
            'AggregatorOverride may not be passed two overrides that handle the same type.');
        }
        $this->override_table[$type] = $items;
      }
    }
    $this->overrides_set = TRUE;

    return $this;
  }

  /**
   * Implementation of OverrideInterface::getOverrides().
   *
   * @return array|bool
   *  An array of overrides keyed by type with subarrays keyed by id and values
   *  representing the revision ids.
   *  Example:
   *  array(
   *    'node' => array(
   *      11 => 23,
   *    ),
   *  );
   */
  public function getOverrides() {
    if ($this->overrides_set) {
      return $this->override_table;
    }
    return FALSE;
  }

  /**
   * Declares what type of data this override takes.
   *
   * OverridesArray means an array of things implementing
   * \Drupal\sps\Plugins\OverrideInterface.
   * These overrides should already have their data set.
   *
   * @return string
   *  A string.
   */
  public function getDataConsumerApi() {
    return 'OverridesArray';
  }
}
