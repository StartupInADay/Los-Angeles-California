<?php

/**
 * @file
 * Personalize controller classes.
 */

class PersonalizeAgentController extends EntityAPIController {

  /**
   * @overridden
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    // Default to caching decisions if not specified.
    if (!isset($entity->data['cache_decisions'])) {
      $entity->data['cache_decisions'] = 1;
    }

    if (!isset($entity->started)) {
      $entity->started = 0;
    }

    $entity->original = entity_load_unchanged('personalize_agent', $entity->machine_name);
    if ($entity->original === FALSE) {
      $entity->is_new = TRUE;
    }

    return parent::save($entity, $transaction);
  }
}

class PersonalizeCampaignGoalController extends EntityAPIController {

  /**
   * @overridden
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'personalize_campaign_goal')
      ->propertyCondition('agent', $entity->agent)
      ->propertyCondition('action', $entity->action);
    $result = $query->execute();

    // Updates existing goal if there's one with the same agent and action.
    if (!empty($result['personalize_campaign_goal'])) {
      $id = current($result['personalize_campaign_goal'])->id;
      $entity->id = $id;
    }

    return parent::save($entity, $transaction);
  }
}

class PersonalizeOptionSetController extends EntityAPIController {

  /**
   * @overridden
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    // Ensure default values for properties that aren't set.

    if (!isset($entity->label)) {
      $entity->label = 'Untitled Option Set';
    }

    if (!isset($entity->stateful)) {
      $entity->stateful = 0;
    }

    if (!isset($entity->executor)) {
      $entity->executor = 'show';
    }

    return parent::save($entity, $transaction);
  }
}
