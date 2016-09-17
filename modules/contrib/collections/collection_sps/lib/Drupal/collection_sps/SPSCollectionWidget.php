<?php

namespace Drupal\collection_sps;

use Drupal\sps\Plugins\Widget\Widget;

class SPSCollectionWidget extends Widget {

  /**
   * Implements WidgetInterface::getPreviewForm().
   *
   * Return a form to collect the date information from the user.
   */
  public function getPreviewForm($element, &$form_state) {
    $options = collection_get_collection_list();

    $element['collection'] = array(
      '#title' => t('Select a Collection'),
      '#type' => 'select',
      '#required' => TRUE,
      '#options' => $options,
      '#default_value' => isset($form_state['values']['collection']) ? $form_state['values']['collection'] : NULL,
    );

    return $element;
  }

  /**
   * Validate the form section for this widget.  Use form_set_error()
   * to designate an error on the form.
   *
   * @param $form
   *   The form section for this widget
   * @param $form_state
   *   The form_state from the general form, with only this widget's values
   *
   * @return
   *   null
   */
  public function validatePreviewForm($form, &$form_state) {

  }

  /**
   * Extract values from the form_state and format them as needed.
   *
   * @param $form
   *   This widget's subsection of the form
   * @param $form_state
   *   The subsection of the form_state related to this widget
   *
   * @return
   *   A value corresponding to the data api type this widget implements
   */
  public function extractValues($form, $form_state) {
    return $form_state['values']['collection'];
  }
}
