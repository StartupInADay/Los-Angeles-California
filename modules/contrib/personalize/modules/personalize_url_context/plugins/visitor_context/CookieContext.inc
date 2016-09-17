<?php
/**
 * @file
 * Provides a visitor context plugin for cookies.

 */

class CookieContext extends PersonalizeContextBase {

  /**
   * Implements PersonalizeContextInterface::create().
   */
  public static function create(PersonalizeAgentInterface $agent = NULL, $selected_context = array()) {
    return new self($agent, $selected_context);
  }

  /**
   * Implements PersonalizeContextInterface::getOptions().
   */
  public static function getOptions() {
    $params = variable_get('personalize_cookie_contexts', array());
    $options = array();
    foreach ($params as $name) {
      $options[$name] = array(
        'name' => $name,
        'group' => 'Cookies',
        'cache_type' => 'session',
      );
    }

    return $options;
  }

  /**
   * Implements PersonalizeAgentInterface::getAssets().
   */
  public function getAssets() {
    return array(
      'js' => array(
        drupal_get_path('module', 'personalize_url_context') . '/js/personalize_cookie_context.js' => array(),
        array('type' => 'setting', 'data' => array('personalize_url_context' => array(
          'cookie_context_exists_prefix' => PERSONALIZE_COOKIE_EXISTS_PREFIX
        )))
      ),
      'library' => array(
        array('system', 'jquery.cookie'),
      )
    );
  }

  /**
   * Implements PersonalizeContextInterface::getPossibleValues().
   */
  public function getPossibleValues($limit = FALSE) {
    $possible_values = array();
    $options = $this->getOptions();
    foreach ($options as $name => $info) {
      $possible_values[$name] = array(
        'value type' => 'string',
      );
      $possible_values[PERSONALIZE_COOKIE_EXISTS_PREFIX . $name] = array(
        'friendly name' => t('Cookie exists: @cookie', array('@cookie' => $name)),
        'value type' => 'boolean',
      );
    }
    if ($limit) {
      $possible_values = array_intersect_key($possible_values, array_flip($this->selectedContext));
    }
    return $possible_values;
  }
}
