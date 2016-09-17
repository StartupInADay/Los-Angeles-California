<?php
namespace Drupal\sps\Test;

class Condition extends \Drupal\sps\Plugins\AbstractPlugin implements \Drupal\sps\Plugins\ConditionInterface {
  public $element_form;
  public $validate_fail_message;
  public $validate_fail_name;
  public $override;
  public $override_set = FALSE;

  public function __construct(array $settings, \Drupal\sps\Manager $manager) {
    parent::__construct($settings, $manager);
    $this->element_form = isset($settings['element_form']) ? $settings['element_form'] : array();
    $this->validate_fail_message = isset($settings['validate_fail_message']) ? $settings['validate_fail_message'] : NULL;
    $this->validate_fail_name = isset($settings['validate_fail_name']) ? $settings['validate_fail_name'] : NULL;
    $this->override = isset($settings['override']) ? $settings['override'] : new \Drupal\sps\Test\Override(array(), $manager);
    $this->has_overrides = isset($settings['has_overrides']) ? $settings['has_overrides'] : TRUE;

    if(!$this->has_overrides) {
      throw new \Drupal\sps\Exception\NonoperativePluginException("condition test condition does not have any overrides avaiable");
    }
    $this->title = isset($settings['title']) ? $settings['title'] : '';
  }

  public function getTitle() {
    return $this->title;
  }

  public function hasOverrides() {
    return $this->has_overrides;
  }



  /**
   * Provide the config to allow this Condition to construct itself.
   *
   * @param $config
   *  An associative array of configuration, generally provided by the
   *  manager
   * @return
   *  Self
   */
  public function setConfig($config) {
    return $this;
  }

  /**
   * Returns the consolidated Override for this Condition
   *
   * @return
   *  An instance of a class which implements OverrideInterface
   */
  public function getOverride() {
    if ($this->override_set) {
      return $this->override;
    }
    return FALSE;
  }

  /**
   * Returns the consolidated preview form for this Condition.
   *
   * @param $element
   *  Either the full form being build, or a subform of the fullform.
   *  Must have #parent set to designate parent keys which tree.
   *
   * @param $form_state
   *  The full form_state for the form which is being built.
   *
   * @return
   *  A FAPI array containing the form for this condition.
   */
  public function getElement($element, &$form_state) {
    return $this->element_form;
  }

  /**
   * Validates this Conditions preview form.
   *
   * This function should use form_set_error() to mark any fields
   * which do not validate.
   *
   * @param $element
   *  The form portion (element) which should be validated
   * @param $form_state
   *  The full form_state for the form which is being built. Note
   *  that values my be treed as described by $elements #parent key
   *
   * @return
   *  Self
   */
  public function validateElement($element, &$form_state) {
    if ($this->validate_fail_message || $this->validate_fail_name) {
      sps_drupal()->form_set_error($this->validate_fail_name, $this->validate_fail_message);
    }
    return $this;
  }

  /**
   * Submit this Conditions preview form.
   *
   * This function should take the values from the widgets and
   * hand them off to the respective overrides.
   *
   * @param $element
   *  The element (subform) which is being submitted.
   * @param $form_state
   *  The form state containing the submitted values. Note
   *  that values my be treed as described by $elements #parent key
   *
   * @return
   *  Self
   */
  public function submitElement($element, &$form_state) {
    $this->override_set = TRUE;
    return $this;
  }
}
