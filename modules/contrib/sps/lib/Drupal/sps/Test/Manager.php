<?php
namespace Drupal\sps\Test;
class Manager extends \Drupal\sps\Manager{
  public $state_controller_site_state_key = 'sps_site_state_key';
  public $state_controller;
  public $config_controller;
  public $override_controller;
  public $root_condition;
  public $plugin_controller;
  public $hook_controller;
  public function __construct(
    \Drupal\sps\StorageControllerInterface $config_controller = NULL) {


    $config_controller = $config_controller ?: new \Drupal\sps\Test\ConfigController();
    parent::__construct($config_controller);

  }
}
