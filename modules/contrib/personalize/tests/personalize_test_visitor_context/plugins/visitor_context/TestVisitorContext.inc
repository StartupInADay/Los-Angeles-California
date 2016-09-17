<?php
/**
 * @file
 * Provides a visitor context plugin for querystring params

 */

class TestVisitorContext extends PersonalizeContextBase {

  /**
   * Implements PersonalizeContextInterface::create().
   */
  public static function create(PersonalizeAgentInterface $agent = NULL, $selected_context = array()) {
    return new self($agent, $selected_context);
  }

  /**
   * Implements PersonalizeContextInterface::getOptions().
   */
  public static function getOptions() {
    $options = array();
    $options['test_session'] = array(
      'name' => 'Test Session Option',
      'group' => 'Test Visitor Context',
      'cache_type' => 'session',
    );
    $options['test_local_30'] = array(
      'name' => 'Test Local Option 30 min',
      'group' => 'Test Visitor Context',
      'cache_type' => 'local',
      'cache_expiration' => 30,
    );
    $options['test_local_none'] = array(
      'name' => 'Test Local Option No Expire',
      'group' => 'Test Visitor Context',
      'cache_type' => 'local',
      'cache_expiration' => 'none',
    );
    $options['test_nocache'] = array(
      'name' => 'Test No Cache',
      'group' => 'Test Visitor Context',
    );
    return $options;
  }

  /**
   * Implements PersonalizeAgentInterface::getPossibleValues().
   */
  public function getPossibleValues($limit = FALSE) {
    $options = $this->getOptions();
    $possible_values = array();
    foreach ($options as $option_name => $option) {
      $possible_values[$option_name] = array(
        'value type' => 'string',
      );
    }
    if ($limit) {
      $possible_values = array_intersect_key($possible_values, array_flip($this->selectedContext));
    }
    return $possible_values;
  }
}
