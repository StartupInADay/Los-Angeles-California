<?php
namespace Drupal\sps\Plugins\Condition;

use Drupal\sps\Plugins\AbstractPlugin;
use Drupal\sps\Plugins\ConditionInterface;

class BasicCondition extends AbstractPlugin implements ConditionInterface {
  protected $overrides;
  protected $overrides_info;
  protected $widget;
  protected $manager;
  protected $override_set = FALSE;

  /**
   * Implements PluginInterface::__construct().
   *
   * Create a new BasicCondition.
   *
   * @param $config array
   *  An array of configuration which includes the widget to use
   *  These should be specified as the 'widget' key.
   *  The widget key may be specified as class names or instantiated classes.
   * @param $manager \Drupal\sps\Manager
   *                 The current instance of the sps manager.
   *
   * @throws \Drupal\sps\Exception\NonoperativePluginException
   */
  public function __construct(array $config, \Drupal\sps\Manager $manager) {
    $this->overrides_info = $manager->getPluginByMeta('override', 'condition', $config['name']);

    if(empty($this->overrides_info)) {
      throw new \Drupal\sps\Exception\NonoperativePluginException(
        "condition {$config['name']} does not have any overrides avaiable");
    }

    if (!empty($config['widget']) && is_string($config['widget'])) {
      $config['widget'] = $manager->getPlugin('widget', $config['widget']);
    }
    $this->title = isset($config['title']) ? $config['title'] : $config['name'];
    $this->widget = $config['widget'];
    $this->manager = $manager;
  }


  public function getTitle() {
    return $this->title;
  }

  public function hasOverrides() {
    return !empty($this->overrides_info);
  }

  /**
   * Implements ConditionInterface::getOverride().
   *
   * Retrieve the override if it is set.
   *
   * @return bool|\Drupal\sps\Plugins\Override\AggregatorOverride
   *  The override with its values set or FALSE if the form has not been
   */
  public function getOverride() {
    if ($this->override_set) {
      $override = new \Drupal\sps\Plugins\Override\AggregatorOverride($this->settings, $this->manager);

      $override->setData($this->overrides);
      return $override;
    }
    return FALSE;
  }

  /**
   * Implements ConditionInterface::getElement().
   *
   * Gets the form for the condition.
   *
   * @see sps_condition_form_validate_callback
   * @see sps_condition_form_submit_callback
   */
  public function getElement($element, &$form_state) {
    if (empty($this->widget)) {
      throw new \Drupal\sps\Exception\ClassLoadException(
        'Element requested but no valid Widget found for the Condition.');
    }

    $sub_state = $form_state;
    $sub_state['values'] = isset($form_state['values']['widget']) ? $form_state['values']['widget'] : array();
    $element['widget'] = $this->widget->getPreviewForm(array(), $sub_state);
    $element['widget']['#tree'] = TRUE;

    //$element['#sps_validate'] = array($this, 'validateElement');
    //$element['#sps_submit'] = array($this, 'submitElement');

    return $element;
  }

  /**
   * Implements ConditionInterface::validateElement().
   *
   * Validates the form for the condition by calling the widget's validate function.
   * The widget will be passed only its portion of the form and the values section of
   * $form_state.
   */
  public function validateElement($element, &$form_state) {
    $this->handleWidgetForm($element, $form_state, 'validatePreviewForm');
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
    $values = $this->handleWidgetForm($element, $form_state, 'extractValues');
    foreach($this->overrides_info as $key=>$override_info) {
      $override = $this->manager->getPlugin('override', $override_info['name']);
      if (!empty($override)) {
        $override->setData($values);
        $this->overrides[$key] = $override;
      }
    }

    $this->override_set = TRUE;

    return $this;
  }

  /**
   * Utility function to help with widget form methods.
   *
   * Creates a sub form array and subsection of the $form_state['values']
   * and calls the given method of the Condition with these sub items.
   *
   * @param $element
   *  The full form as passed to the element method.
   * @param $form_state
   *  The full form_state as poassed to the element method
   * @param $method
   *  A string which is the name of the method to call on the widget
   *
   * @return mixed
   */
  protected function handleWidgetForm($element, &$form_state, $method) {
    $widget_el = $element['widget'];
    $widget_state = $form_state;

    if (isset($form_state['values']['widget'])) {
        $widget_state['values'] = $form_state['values']['widget'];
    }
    else {
      $widget_state['values'] = array();
    }

    $return = $this->widget->{$method}($widget_el, $widget_state);

    $form_state['values']['widget'] = $widget_state['values'];

    return $return;
  }
}
