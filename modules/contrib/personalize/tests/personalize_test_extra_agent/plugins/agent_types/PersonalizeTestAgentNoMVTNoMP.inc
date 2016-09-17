<?php
/**
 * @file
 * Provides an invalid test agent type for testing with multiple agent types
 * and scenarios.
 */

class PersonalizeTestAgentNoMVTNoMP extends PersonalizeAgentBase {

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
    return 'test_agent_no_mvt_no_mp';
  }

  public function supportsMVTs() {
    return FALSE;
  }

  public function supportsMultipleDecisionPoints() {
    return FALSE;
  }

}
