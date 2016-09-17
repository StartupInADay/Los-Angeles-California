<?php
/**
 * @file
 * Provides an agent type for testing purposes.
 */

class PersonalizeTestAgent extends PersonalizeAgentBase implements PersonalizeAgentGoalInterface {

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
    return 'test_agent';
  }

  /**
   * Implements PersonalizeAgentGoalInterface::useClientSideGoalDelivery().
   */
  public function useClientSideGoalDelivery() {
    return isset($this->data['use_client_side_goal_processing']) ? $this->data['use_client_side_goal_processing'] : NULL;
  }

  /**
   * Implements PersonalizeAgentGoalInterface::sendGoal().
   */
  public function sendGoal($goal_name, $value = NULL) {
    // Just print info to the screen.
    print sprintf("The %s goal with value %d was received by the %s agent", $goal_name, $value, $this->getMachineName());
  }

  /**
   * Implements PersonalizeAgentInterface::getAssets().
   */
  public function getAssets() {
    return array(
      'js' => array(
        drupal_get_path('module', 'personalize_test') . '/personalize_test.js' => array('type' => 'file', 'scope' => 'footer', 'defer' => TRUE),
      )
    );
  }

}
