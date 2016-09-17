<?php

namespace Drupal\sps\Test;

class SiteState extends \Drupal\sps\SiteState{
  public $condition;
  public $override_controllers;
  public function __construct($condition, $map) {
    $this->condition = $condition;
    $this->override_controllers = $map;
  }
}

