<?php
/**
 * @file
 * Provides an actionable element for base forms like 'node_form', 'comment_form'.
 */

class BaseFormActionableElement extends FormActionableElement {
  /**
   * Implements ActionableElementInterface::getActionOptions().
   */
  public static function getActionOptions() {
    return array(
      'server' => array(
        'submit_server' => t('submits (server-side)'),
      )
    );
  }

  /**
   * Implements IdentifiableActionableElementInterface::getIdentifierLabel().
   */
  public static function getIdentifierLabel() {
    return t('Base form callback name.');
  }

  /**
   * Implements ActionableElementInterface::actionPresave().
   */
  public static function actionPresave($action) {
    return $action;
  }
}