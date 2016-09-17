<?php
namespace Drupal\sps\Test;

class Override extends \Drupal\sps\Plugins\Override\Override {
  public $table = array();
  private $type = FALSE;

  public function __construct(array $settings, \Drupal\sps\Manager $manager) {
    parent::__construct($settings, $manager);
    if (isset($settings['type'])) {
      $this->type = $settings['type'];
    }
  }

  public function getOverrides() {
    if ($this->type) {
      return array(
        $this->type => $this->table,
      );
    }
    else {
      return $this->table;
    }
  }

  public function setData($table) {
    $this->table = $table;
  }

  public function getDataConsumerApi() {
    return 'test';
  }
}
