<?php
/**
 * @file
 */

class LinkActionableElement extends ActionableElementBase implements IdentifiableActionableElementInterface {

  /**
   * Implements ActionableElementInterface::getActionOptions().
   */
  public static function getActionOptions() {
    return array(
      'client' => array(
        'click' => t('clicks'),
        'mouseenter' => t('hovers over')
      )
    );
  }

  /**
   * Implements IdentifiableActionableElementInterface::getIdentifierLabel().
   */
  public static function getIdentifierLabel() {
    return t('Selector');
  }

  /**
   * Implements ActionableElementInterface::getSelectable().
   */
  public static function getSelectable() {
    return 'a[href]';
  }
}