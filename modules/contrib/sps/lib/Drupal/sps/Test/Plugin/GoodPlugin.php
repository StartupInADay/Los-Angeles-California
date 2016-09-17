<?php

namespace Drupal\sps\Test\Plugin;

use Drupal\sps\Plugins\AbstractPlugin;

class GoodPlugin extends AbstractPlugin implements TestTypeInterface {
  public $settings;
  public $manager;
  public function __construct(array $settings, \Drupal\sps\Manager $manager) {
    $this->settings = $settings;
    $this->manager = $manager;
  }
  public function testMethod() {}
}
