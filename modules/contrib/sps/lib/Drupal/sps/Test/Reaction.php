<?php
namespace Drupal\sps\Test;

class Reaction extends \Drupal\sps\Plugins\AbstractPlugin implements \Drupal\sps\Plugins\ReactionInterface {

  protected $react_callback;
  /**
   * the construct that is expect by the plugin system
   * @Param setting
   * @param $manager an object of class Drupal\sps\Manager
   */
  public function __construct(array $settings, \Drupal\sps\Manager $manager) {
    parent::__construct($settings, $manager);
    $this->react_callback = $settings['callback'];
  }

  /**
   * React in some way
   * This could be to alter the $data, or return some data, or even a sideeffect of some kind
   *
   * @param $data Vary
   * @return Vary
   */
  public function react($data, \Drupal\sps\Plugins\OverrideControllerInterface $override_controller) {
    return $this->react_callback->__invoke($data, $override_controller);
  }
}
