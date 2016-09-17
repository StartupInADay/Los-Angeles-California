<?php
/**
 * @file
 * Provides an extra test agent type for testing with multiple agent types.
 */

class PersonalizeTarget extends PersonalizeAgentBase implements PersonalizeExplicitTargetingInterface {

  /**
   * Implements PersonalizeAgentInterface::create().
   */
  public static function create($agent_data) {
    $status = personalize_agent_get_status($agent_data->machine_name);
    return new self($agent_data->machine_name, $agent_data->label, $agent_data->data, $status, !empty($agent_data->started) ? $agent_data->started : NULL);
  }

  /**
   * Implements PersonalizeExplicitTargetingInterface::explicitTargetingSupportMultiple().
   */
  public static function explicitTargetingSupportMultiple() {
    return PersonalizeExplicitTargetingInterface::EXPLICIT_TARGETING_MULTIPLE_OR;
  }

  /**
   * Implements PersonalizeAgentInterface::getType().
   */
  public function getType()
  {
    return 'personalize_target';
  }

  /**
   * Implements PersonalizeAgentInterface::getAssets().
   */
  public function getAssets() {
    return array(
      'js' => array(
        drupal_get_path('module', 'personalize_target') . '/js/personalize_target.js' => array('type' => 'file', 'scope' => 'footer', 'defer' => TRUE),
      )
    );
  }

  /**
   * Implements PersonalizeAgentInterface::convertContextToFeatureString().
   */
  public static function convertContextToFeatureString($name, $value, $is_mutex = FALSE) {
    $value = self::cleanFeatureValue($value);
    return $name . '::' . $value;
  }

}
