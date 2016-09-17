<?php
namespace Drupal\sps\Plugins\Reaction;

class EntityLoadReaction implements \Drupal\sps\Plugins\ReactionInterface {
  protected $type;
  /**
   * The EntitySelectQueryAlterReaction can work with any number of Entities.
   * Each entity must be shown in the $config['entities'] array.  It will
   * then alter any query (it can touch) and use the override revision id
   * instead of the one in base
   *
   * @param $config
   *   has a value saying which type it is for
   * @param $manager \Drupal\sps\Manager
   *                 the Current Manager Object
   *
   * @return \Drupal\sps\Plugins\Reaction\EntityLoadReaction
   */
  public function __construct(array $config, \Drupal\sps\Manager $manager) {
  }

  /**
   * Return the revision id for a passed in id for the type stored from the constructor
   *
   * @param \StdClass $data
   *   and object with the following properties
   *   revision_id_key
   *   base_id_key
   *   base_table
   *   ids : the ids
   * @param \Drupal\sps\Plugins\OverrideControllerInterface $override_controller
   *   This is an override controller to use to find
   *   override data
   *
   * @return array|mixed
   *  array of revision ids;
   */
  public function react($data, \Drupal\sps\Plugins\OverrideControllerInterface $override_controller) {
    $had_overrides = FALSE;
    if (!empty($data->ids)) {
      $revision_id_query = db_select($data->base_table, 'b');
      $revision_id_query->fields('b', array($data->revision_id_key, $data->base_id_key));
      $revision_id_query->condition('b.'. $data->base_id_key, $data->ids);
      $revision_id_query->addTag(SPS_NO_ALTER_QUERY_TAG);
      $result = $revision_id_query->execute()->fetchAllAssoc($data->base_id_key);

      foreach($result as $id => $row) {
        $override = $override_controller->getRevisionId($id, $data->type);
        $vids[] = $override['revision_id'] ?: $row->{$data->revision_id_key};
        $had_overrides = $had_overrides || $override;
      }
    }

    return $had_overrides ? $vids : array();
  }
}
