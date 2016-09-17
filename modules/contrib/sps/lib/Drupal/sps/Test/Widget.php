<?php
namespace Drupal\sps\Test;

class Widget extends \Drupal\sps\Plugins\Widget\Widget {
  private $type;

  /**
   * Implements WidgetInterface::getPreviewForm().
   *
   * Return a simple text box form
   */
  public function getPreviewForm($form, &$form_state) {
    $form['text_input'] = array(
      '#type' => 'textfield',
      '#title' => t('Text Input'),
      '#description' => t('This is simply text input for testing.'),
      '#default_value' => empty($form_state['values']['text_input']) ? '' : $form_state['values']['text_input'],
    );

    return $form;
  }

  /**
   * Implments WidgetInterface::validatePreviewForm().
   *
   * Check that text_input is not empty.
   */
  public function validatePreviewForm($form, &$form_state) {
    if (empty($form_state['values']['text_input'])) {
      form_set_error('text_input', t('Text Input may not be empty.'));
    }
  }

  /**
   * Implements WidgetInterface::extractValues().
   *
   * Get the value from text_input and return it.
   */
  public function extractValues($form, $form_state) {
    $values = $form_state['values']['text_input'];
    return $values;
  }
}
