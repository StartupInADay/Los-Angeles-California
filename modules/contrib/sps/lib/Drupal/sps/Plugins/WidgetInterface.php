<?php
namespace Drupal\sps\Plugins;

interface WidgetInterface {
  /**
   * Provide the widget's preview form to be aggregated into the
   * full preview form.
   *
   * @param $form
   *   The form array to add the widget's preview form into.  Generally
   *   this an empty array
   * @param $form_state
   *   The current $form_state for the form being built
   *
   * @return
   *   A FAPI array.
   */
  public function getPreviewForm($form, &$form_state);

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
  public function validatePreviewForm($form, &$form_state);

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
  public function extractValues($form, $form_state);
}
