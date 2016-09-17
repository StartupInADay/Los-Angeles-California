<?php
namespace Drupal\sps\Test;
class PluginController extends \Drupal\sps\PluginFactory{

  public function setInfo($plugin_info) {
    $this->plugin_info = $plugin_info;
    foreach($plugin_info as $type => $stuff) {
      $this->plugin_type_info[$type] = array( 'name' => $type);
    }
  }

  public function __construct($plugin_info, $manager) {
    $this->setInfo($plugin_info);
  }
  protected function loadPluginInfo($plugin_type) {
    if(!isset($this->plugin_info[$plugin_type])) {
      $this->plugin_info[$plugin_type] = array();
    }
    return $this;
  }
  protected function loadPluginTypeInfo() {
    //insure we have reaction info type (as the test manager ask for this)
    $this->plugin_type_info['reaction'] = array('name' => 'reaction');
    $this->plugin_type_info['override_controller'] = array('name' => 'override_controller');
    return $this;
  }
  
}
