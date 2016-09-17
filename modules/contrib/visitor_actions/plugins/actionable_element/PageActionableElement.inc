<?php
/**
 * @file
 * Provides an actionable element for pages.
 */

class PageActionableElement extends ActionableElementBase {

  /**
   * Implements ActionableElementInterface::getActionOptions().
   */
  public static function getActionOptions() {
    return array(
      'client' => array(
        'view' => t('views'),
        'scrollToBottom' => t('scrolls to the bottom of'),
        'stay' => t('stays for longer than the specified time on')
      )
    );
  }

  /**
   * Implements ActionableElementInterface::optionsForm().
   */
  public static function optionsForm($action = array()) {
    $form = array();
    $form['remains_for'] = array(
      '#type' => 'textfield',
      '#title' => t('Trigger action after'),
      '#field_suffix' => t('seconds'),
      '#size' => 4,
      '#default_value' => isset($action['data']['remains_for']) ? $action['data']['remains_for'] : 5,
      '#states' => array(
        'visible' => array(
          ':input[name="event[page]"]' => array('value' => 'client::stay'),
        ),
      ),
      '#element_validate' => array('element_validate_integer_positive'),
    );
    $form['bottom_offset'] = array(
      '#type' => 'textfield',
      '#title' => t('Trigger action when within'),
      '#field_suffix' => t('pixels of the bottom'),
      '#size' => 4,
      '#default_value' => isset($action['data']['bottom_offset']) ? $action['data']['bottom_offset'] : 100,
      '#states' => array(
        'visible' => array(
          ':input[name="event[page]"]' => array('value' => 'client::scrollToBottom'),
        ),
      ),
      '#element_validate' => array('element_validate_integer_positive'),
    );
    return $form;
  }

  /**
   * Implements ActionableElementInterface::validate().
   */
  public static function validate($values) {
    if (empty($values['pages'])) {
      form_set_error('pages', t('You must specify one or more pages'));
    }
  }
}
