<?php
namespace Drupal\sps\Plugins\Reaction;

class AddClassReaction implements \Drupal\sps\Plugins\ReactionInterface {
  /**
   * @param $config  array
   *                 has a value saying which type it is for
   * @param $manager \Drupal\sps\Manager
   *                 the Current Manager Object
   *
   * @return \Drupal\sps\Plugins\Reaction\AddClassReaction
   */
  public function __construct(array $config, \Drupal\sps\Manager $manager) {
  }

  /**
   *
   * @param int $data
   *   the entity id
   * @param $override_controller
   *   This is an override controller to use to find
   *   override data
   *
   * @return int | NULL
   *  the revision id;
   */
  public function react($data, \Drupal\sps\Plugins\OverrideControllerInterface $override_controller) {
    if($override_controller->getRevisionId($data['id'], $data['type'])) {
      return 'sps-overridden';
    }
  }
}
