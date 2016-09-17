<?php

namespace Drupal\sps;

class SiteState {
  protected $condition;
  protected $override_controllers;

  /**
   * Constructor for SiteState
   *
   * @param $condition
   *   The current condition in use
   * @param $controller_map
   *   A map of controller api's to OVerride Controller Plugin obj
   */
  public function __construct(Plugins\ConditionInterface $condition, $controller_map) {

    $this->setOverrideControllers($controller_map);
    $this->setCondition($condition);
    $this->setOverrides();
  }

  /**
   * Set the Controller for storing overrrides.
   *
   * @param $controller_map
   *   The Map of overrides controllers
   *
   * @return \Drupal\sps\SiteState
   *   Self
   */
  protected function setOverrideControllers($controller_map) {
    $this->override_controllers = $controller_map;

    return $this;
  }

  /**
   * Get the condtion stored in the state.
   *
   * @return \Drupal\sps\Plugins\ConditionInterface
   *   the stored condition
   */
  public function getCondition() {
    return $this->condition;
  }

  /**
   * Store the Override to use for generating overrides.
   *
   * @param Plugins\ConditionInterface $condition
   *
   * @return \Drupal\sps\SiteState
   *   Self
   */
  protected function setCondition(Plugins\ConditionInterface $condition) {
    $this->condition = $condition;

    return $this;
  }

  /**
   * Retrieve Stored Overrides.
   *
   * @return array
   *   Array of overrides
   *   @TODO make this a iterator?
   */
  public function getOverrideController($api) {
    if (isset($this->override_controllers[$api])) {
      return $this->override_controllers[$api];
    }
  }

  /**
   * Get overrides from the controller and pass it on to the override
   * controllers;
   *
   * @TODO as some of the $controllers might be the same obj there should
   * be something we can do to not call set on it twice
   *
   * @return \Drupal\sps\SiteState
   *   Self
   */
  protected function setOverrides() {
    foreach ($this->override_controllers as $api => $controller) {
      $controller->set($this->condition->getOverride()->getOverrides());
    }
    return $this;
  }
}


