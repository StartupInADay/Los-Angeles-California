<?php
namespace Drupal\sps_test;

class SingleCheckWidget extends \Drupal\sps\Plugins\Widget\Widget {
  private $title;
  private $desc;

  /**
   * Implements WidgetInterface::getPreviewForm().
   *
   * Return a simple text box form
   */
  public function getPreviewForm($form, &$form_state) {
    $form['check'] = array(
      '#type' => 'checkbox',
      '#title' => t($this->title),
      '#description' => t($this->desc),
      '#default_value' => empty($form_state['values']['check']) ? '' : $form_state['values']['check'],
    );
    return $form;
  }

  /**
   * Implments WidgetInterface::validatePreviewForm().
   *
   * Check that text_input is not empty.
   */
  public function validatePreviewForm($form, &$form_state) {
  }

  /**
   * Implements WidgetInterface::extractValues().
   *
   * Get the value from text_input and return it.
   */
  public function extractValues($form, $form_state) {
    $values = $form_state['values']['check'];
    return $values;
  }

  public function __construct(array $config, \Drupal\sps\Manager $manager) {
    $this->title = isset($config['title']) ? $config['title'] : '';
    $this->desc = isset($config['desc']) ? $config['desc'] : '';
  }
}
