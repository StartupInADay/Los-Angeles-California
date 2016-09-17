<?php
namespace Drupal\sps\Plugins\Widget;


class DateWidget extends Widget {

  /**
   * Implements WidgetInterface::getPreviewForm().
   *
   * Return a form to collect the date information from the user.
   */
  public function getPreviewForm($element, &$form_state) {
    $element['#type'] = 'fieldset';
    $element['#title'] = empty($this->settings['title']) ? t('Date/Time:') : $this->settings['title'];
    $element['#description']= t('Preview nodes published on or after this date.');
    $element['#attributes']['class'] = array('sps-date-widget');

    $element['preview_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Date to Preview'),
      '#default_value' => isset($form_state['values']['preview_date']) ? $form_state['values']['preview_date'] : NULL,
    );

    //have a fallback if they don't have date_popup installed.
    if (!module_exists('date_popup')) {
      $element['preview_date']['#type'] = 'date';
      $element['preview_time'] = array(
        '#type' => 'textfield',
        '#title' => t('Time'),
        '#size' => 9,
        '#default_value' => isset($form_state['values']['preview_time']) ? $form_state['values']['preview_time'] : '00:00:00',
      );
    }

    return $element;
  }

  /**
   * Implements WidgetInterface::validatePreviewForm().
   *
   * Right now no validation is necessary.
   */
  public function validatePreviewForm($form, &$form_state) {
    $date = self::getTimeStamp($form_state);
    if (!$date) {
      form_set_error('preview_date_wrapper', t('Invalid Date/Time given.'));
    }
  }

  /**
   * Implements WidgetInterface::extractValues().
   *
   * @param $form
   * @param $form_state
   *
   * @return int|Bool
   *  A unix timestamp, 0 for an empty value or FALSE for
   */
  public function extractValues($form, $form_state) {
    return self::getTimeStamp($form_state);
  }

  /**
   * Helper function to retrieve the values from the form_state and create a
   * timestamp from them.
   *
   * @param $form_state
   *  The form state with a value set for preview_date
   *
   * @return int|Bool
   *  A unix timestamp, 0 for an empty value or FALSE for
   */
  protected static function getTimeStamp($form_state) {
    $date_arr = $form_state['values']['preview_date'];
    if (!module_exists('date_popup')) {
      $date = $date_arr['month'] . '/' . $date_arr['day'] . '/' . $date_arr['year'];
      if (!empty($form_state['values']['preview_time'])) {
        $date .= ' ' . $form_state['values']['preview_time'];
      }
      return strtotime($date);
    }
    else {
      if (is_array($date_arr)) {
        return strtotime(implode(' ', $date_arr));
      }
      else {
        return strtotime($date_arr);
      }
    }
  }
}
