<?php
/**
 * @file
 * Provides interfaces and base classes for Visitor Actions plugins.
 */

/**
 * Interface that all actionable element plugins must implement.
 */
interface ActionableElementInterface {

  /**
   * Returns the available actions for this type of element.
   *
   * Actions are divided into server-side and client-side actions.
   *
   * @return mixed
   *   An array with possible keys 'server' and 'client' whose values
   *   are associative arrays of action options with names as keys
   *   and labels as values. E.g.:
   *   array(
   *     'client' => array(
   *       'click' => t('Clicks on')
   *     )
   *   )
   */
  public static function getActionOptions();

  /**
   * Returns the form elements for advanced configuration.
   *
   * @param $action
   *   The action being edited, if it's an edit form.
   *
   * @return array
   *   A FAPI array.
   */
  public static function optionsForm($action = array());

  /**
   * Validation check for form values.
   *
   * This gets called by the action add/edit form's validation callback
   * to allow the plugin responsible for the action being saved to
   * check the values and set an error if there's anything amiss.
   *
   * @param $values
   *   The form values being submitted on an action add/edit form.
   */
  public static function validate($values);

  /**
   * Perform necessary modifications of the action before it's saved.
   *
   * @param array $action
   *   The action that's about to be saved.
   * @return array
   *   THe modified action.
   */
  public static function actionPresave($action);

  /**
   * Returns which elements on a page are selectable for this element type.
   *
   * @return string
   *   A string representing selectable html elements.
   */
  public static function getSelectable();

}

interface IdentifiableActionableElementInterface extends ActionableElementInterface {
  /**
   * Returns a label for the identifier field on the action form.
   *
   * Because the identifier is used differently by different plugins, it
   * corresponds to different things, depending on the plugin, e.g. it could
   * be a node ID, a form ID, a CSS selector, etc.
   *
   * @return string
   *   The label to use as the title of the identifier form field.
   */
  public static function getIdentifierLabel();
}

/**
 * Base class for actionable element plugins.
 *
 * Class ActionableElementBase
 */
abstract class ActionableElementBase implements ActionableElementInterface {

  /**
   * Implements ActionableElementInterface::getActionOptions().
   */
  public static function getActionOptions() {
    return array();
  }

  /**
   * Implements ActionableElementInterface::optionsForm().
   */
  public static function optionsForm($action = array()) {
    return array();
  }

  /**
   * Implements ActionableElementInterface::validate().
   */
  public static function validate($values) {

  }

  /**
   * Implements ActionableElementInterface::actionPresave().
   */
  public static function actionPresave($action) {
    return $action;
  }

  /**
   * Implements ActionableElementInterface::getSelectable().
   */
  public static function getSelectable() {
    return NULL;
  }
}
