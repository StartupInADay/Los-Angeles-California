<?php
/**
 * @file
 * Provides an extra test agent type for testing with multiple agent types.
 */

class PersonalizeTestExtraAgent extends PersonalizeAgentBase implements PersonalizeExplicitTargetingInterface {

  public $goals_received = array();

  /**
   * Implements PersonalizeAgentInterface::create().
   */
  public static function create($agent_data) {
    $status = personalize_agent_get_status($agent_data->machine_name);
    return new self($agent_data->machine_name, $agent_data->label, $agent_data->data, $status, !empty($agent_data->started) ? $agent_data->started : NULL);
  }

  /**
   * Implements PersonalizeAgentInterface::getType().
   */
  public function getType()
  {
    return 'test_extra_agent';
  }

  /**
   * Implements PersonalizeExplicitTargetingInterface::explicitTargetingSupportMultiple().
   */
  public static function explicitTargetingSupportMultiple() {
    return PersonalizeExplicitTargetingInterface::EXPLICIT_TARGETING_MULTIPLE_BOTH;
  }

  /**
   * Implements PersonalizeAgentInterface:errors().
   */
  public function errors() {
    return array();
  }

  public function stopNow() {
    return TRUE;
  }
}
