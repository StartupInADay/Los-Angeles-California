<?php
namespace Drupal\sps\Plugins\Condition;

/**
* the FixedOverrideCondition is use when there are no interactive elements (widget) and
* the override to use is fixed.  This is mostly used in the sps_add_override function.
* It can be used any time a condition needs to be set programmatically.
*
* It has a message that it used to display in the sps preview and the override that it pass on
*/
class FixedOverrideCondition extends BasicCondition {
  /**
  * expect to get message and overrides in the $config
  */
  public function __construct(array $config, \Drupal\sps\Manager $manager) {
    $this->manager = $manager;
    $this->message = $config['message'];
    if(isset($config['override'])) {
      $this->override = $config['override'];
    }
    else {
      throw new \Drupal\sps\Exception\NonoperativePluginException("condition does not have an override");
    }
  }

  /**
   * implements ConditionInterface::getElement()
   *
   * @param $element
   * @param $form_state
   *
   * @return array a render array with a container wrapped around the message from the constructor
   */
  public function getElement($element, &$form_state) {
    $element['message'] = array(
      '#type' => 'container',
      'message_body' => $this->message,
      '#attributes' => array('class' => array('form-item'))
    );
    $form_state['values']['message'] = TRUE;
    return $element;
  }

  /**
  * implements ConditionInterface::validateElement()
  */
  public function validateElement($element, &$form_state) { }

  /**
  * implements ConditionInterface::submitElement()
  */
  public function submitElement($element, &$form_state) { }


  /**
  * implements ConditionInterface::getOverrides()
  */
  public function getOverride() {
    return $this->override;
  }
}
