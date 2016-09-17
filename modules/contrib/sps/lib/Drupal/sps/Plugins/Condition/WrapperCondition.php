<?php
namespace Drupal\sps\Plugins\Condition;
define('SPS_CONFIG_WRAPPER_CONDITION_SUB_CONDITIONS', "wrapper_condition_sub_conditions");

use Drupal\sps\Plugins\AbstractPlugin;
use Drupal\sps\Plugins\ConditionInterface;

class WrapperCondition extends BasicCondition {
  protected $conditions = array();
  protected $manager;
  protected $active_condition;
  protected $override_set = FALSE;
  protected $override;
  protected $form_state;
  protected $conditions_config = array();

  /**
   * Implements PluginInterface::__construct().
   *
   * Create a new BasicCondition.
   *
   * @param $config
   *  An array of configuration which includes the widget to use
   *  These should be specified as the 'widget' key.
   *  The widget key may be specified as class names or instantiated
   *  classes.
   * @param $manager
   *  The current instance of the sps manager.
   */
  public function __construct(array $config, \Drupal\sps\Manager $manager) {
    $this->manager = $manager;
    $plugin_infos = array_filter(
      $this->manager->getPluginInfo('condition'),
      function($info) {
        return !(isset($info['root_condition']) && $info['root_condition']);
      });
    $configs =
    $this->manager->getConfigController()->exists(SPS_CONFIG_WRAPPER_CONDITION_SUB_CONDITIONS) ?
    $this->manager->getConfigController()->get(SPS_CONFIG_WRAPPER_CONDITION_SUB_CONDITIONS) : array();
    $this->setconditions_config($configs, $plugin_infos);
    $this->setConditions();
  }

  /**
   * @param $configs
   * @param $plugin_info
   *
   * @return WrapperCondition
   *  Self
   */
  protected function setConditions_config($configs, $plugin_info) {
    if(empty($configs)) {
      $this->conditions_config = array_map(function($info) { return array();}, $plugin_info);
    }
    else  {
      foreach($plugin_info as $name => $info) {
        if(isset($configs[$name])) {
        $this->conditions_config[$name] = $configs[$name];
        }
      }
    }
    return $this;
  }

  /**
   * Pull Conditions from the plugin system and load them all in as sub conditions
   *
   * @param null|array $configs
   *
   * @throws \Exception
   * @return \Drupal\sps\Plugins\Condition\WrapperCondition
   *  Self
   */
  protected function setConditions($configs = NULL) {
    $configs = $configs ?: $this->conditions_config;
    foreach($configs as $name => $config) {
      try {
        $this->conditions[$name] = $this->manager->getPlugin('condition', $name, $config);
      }
      catch (\Exception $e){
        //if we have  NonoperativePluginException we can move on
        //if not we need to throw it.
        if(!(method_exists($e, 'originalIs') && $e->originalIs('\Drupal\sps\Exception\NonoperativePluginException')))  {
          throw $e;
        }
      }
    }
    return $this;
  }

  /**
   * Implements ConditionInterface::getOverride().
   *
   * Retrieve the override if it is set.
   *
   * @return bool|\Drupal\sps\Plugins\OverrideInterface
   *  The override with its values set or FALSE if the form has not been
   */
  public function getOverride() {
    return $this->override;
  }

  /**
  * generate a key to use as the basis for the form items
  *
  * This is here so that when recusion is added this can be change to
  * something that varies for each instance.
  *
  * @return String
  */
  public function getActiveConditionKey() {
    return 'active-condition';
  }
  public function getContainerId() {
    return $this->getActiveConditionKey() .'-container';
  }
  public function getSelectorId() {
    return $this->getActiveConditionKey() .'-selector';
  }
  public function getResetId() {
    return $this->getActiveConditionKey() .'-wrapper-reset';
  }
  public function getContainerWrapperId() {
    return $this->getContainerId() .'-wrapper';

  }

  /**
   * Implements ConditionInterface::getElement().
   *
   * Gets the form for the condition.
   * This uses ajax to allow the user to select from the other conditions
   * and then submit the settings of that sub condition
   *
   * @see sps_condition_form_validate_callback
   * @see sps_condition_form_submit_callback
   */
  public function getElement($element, &$form_state) {

    //check and see if we have a form_state from previous runs
    if(!isset($form_state['values']) &&
       isset($this->form_state['values'])) {
      $form_state['values'] = $this->form_state['values'];
    }

    $container_id = $this->getContainerId();
    $selector_id = $this->getSelectorId();

    $element[$container_id] = array(
      '#type' => 'container',
      '#tree' => TRUE,
      '#prefix' => "<div id = '".$this->getContainerwrapperId()."'>",
      '#suffix' => "</div>",
    );

    $element[$container_id][$selector_id] = array(
      '#type' => 'select',
      '#title' => $this->getTitle(),
      '#options' => array(),
      '#required' => TRUE,
      '#ajax' => array(
        'callback' => 'sps_wrapper_condition_ajax_callback',
        'wrapper' => $this->getContainerWrapperId(),
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#tree' => TRUE,
    );
    foreach($this->conditions as $name => $condition) {
      if ($condition->hasOverrides()) {
        $element[$container_id][$selector_id]['#options'][$name] = $condition->getTitle();
      }
    }

    // this should be set after an ajax call to select a condition
    if (isset($form_state['values'][$container_id][$selector_id])) {
      $this->active_condition = $form_state['values'][$container_id][$selector_id];
      $condition = $this->conditions[$this->active_condition];
      $sub_state = $form_state;
      $sub_state['values'] = isset($form_state['values'][$container_id][$this->active_condition]) ? $form_state['values'][$container_id][$this->active_condition] : array();
      $element[$container_id][$this->active_condition] = $condition->getElement(array(), $sub_state);
      $element[$container_id][$this->active_condition]['#tree'] = TRUE;

      $element[$container_id][$selector_id]['#default_value'] = $this->active_condition;
    }

    return $element;
  }

  public function getTitle() {
    return t('Select a Condition');
  }

  /**
   * @param $element
   * @param $form_state
   * @param $container_id
   *
   * @return array
   */
  protected function extractSubState($element, $form_state, $container_id= NULL) {

    $container_id= $container_id ?: $this->getContainerId();


    $sub_state = $form_state;
    $sub_state['values'] = isset($form_state['values'][$container_id][$this->active_condition]) ?
      $form_state['values'][$container_id][$this->active_condition] : array();

    $sub_element = isset($element[$container_id][$this->active_condition]) ? $element[$container_id][$this->active_condition] : array();

    return array($sub_element, $sub_state);
  }

  /**
   * Implements ConditionInterface::validateElement().
   *
   * Validates the form for the condition by calling the widget's validate function.
   * The widget will be passed only its portion of the form and the values section of
   * $form_state.
   */
  public function validateElement($element, &$form_state) {
    list($sub_element, $sub_state) = $this->extractSubState($element, $form_state);
    if ($this->active_condition) {
      $this->conditions[$this->active_condition]->validateElement($sub_element, $sub_state);
    }

    return $this;
  }

  /**
   * Implements ConditionInterface::submitElement().
   *
   * Submits the form for the condition by calling the widget's submit function.
   * The widget will be passed only its portion of the form and the values section of
   * $form_state.
   */
  public function submitElement($element, &$form_state) {

    list($sub_element, $sub_state) = $this->extractSubState($element, $form_state);
    $this->conditions[$this->active_condition]->submitElement($sub_element, $sub_state);
    $this->override = $this->conditions[$this->active_condition]->getOverride();

    $this->override_set = TRUE;

    $this->form_state = $form_state;
    $this->form_state['values'][$this->getContainerId()][$this->getSelectorId()] = $this->active_condition;
    return $this;
  }

}
