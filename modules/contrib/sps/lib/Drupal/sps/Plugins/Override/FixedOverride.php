<?php
namespace Drupal\sps\Plugins\Override;

/*
 * FixedOverride is a simple case of an Override, to be used
 * when on knows what the override table is.  It is used as
 * part of the sps_add_override function
 */
class FixedOverride extends \Drupal\sps\Plugins\Override\Override {
  public $table = array();

  public function __construct(array $settings, \Drupal\sps\Manager $manager) {
    parent::__construct($settings, $manager);
    if($settings['overrides']) {
      $this->setData($settings['overrides']);
    }
  }
  /*
   * implements OverrideInterface::getOverrides()
   */
  public function getOverrides() {
    return $this->table;
  }

  /*
   * implements OverrideInterface::setData()
   */
  public function setData($table) {
    $this->table = $table;
  }

  /*
   * implements OverrideInterface::getDataConsumerApi()
   */
  public function getDataConsumerApi() {
    return 'fixed';
  }
}
