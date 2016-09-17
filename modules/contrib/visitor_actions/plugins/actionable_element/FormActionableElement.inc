<?php
/**
 * @file
 * Provides an actionable element for forms.

 */

class FormActionableElement extends ActionableElementBase implements IdentifiableActionableElementInterface {

  /**
   * Implements ActionableElementInterface::getActionOptions().
   */
  public static function getActionOptions() {
    return array(
      'client' => array(
        'submit_client' => t('submits (client-side)'),
      ),
      'server' => array(
        'submit_server' => t('submits (server-side)'),
      )
    );
  }

  /**
   * Implements ActionableElementInterface::actionPresave().
   */
  public static function actionPresave($action) {
    $action['identifier'] = self::getFormID($action['identifier']);
    return $action;
  }

  /**
   * Implements IdentifiableActionableElementInterface::getIdentifierLabel().
   */
  public static function getIdentifierLabel() {
    return t('Form ID');
  }

  /**
   * Implements ActionableElementInterface::getSelectable().
   */
  public static function getSelectable() {
    return 'form[action]';
  }

  /**
   * Converts a css ID into an actual form ID.
   *
   * @param $id
   * @return mixed
   */
  protected static function getFormID($id) {
    $id = str_replace('-', '_', $id);
    $id = str_replace('#', '', $id);
    return $id;
  }
}