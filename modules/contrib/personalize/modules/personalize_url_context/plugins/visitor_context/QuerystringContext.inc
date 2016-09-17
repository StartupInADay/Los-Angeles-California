<?php
/**
 * @file
 * Provides a visitor context plugin for querystring params

 */

class QuerystringContext extends PersonalizeContextBase {

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
    $params = variable_get('personalize_url_querystring_contexts', _personalize_url_context_get_utm_parameter_names());
    $options = array();
    foreach ($params as $name) {
      $options[$name] = array(
        'name' => $name,
        'group' => 'Querystring Params',
        'cache_type' => 'session',
      );
    }
    // Add the referrer URL context. This will get lumped into the
    // "Miscellaneous" group.
    $options['referrer_url'] = array(
      'name' => t('Most recent referrer'),
      'cache_type' => 'session'
    );

    // Add the original URL referrer to the misc. group.
    $options['original_referrer_url'] = array(
      'name' => t('First referrer'),
      'cache_type' => 'local',
      'cache_expiration' => 'none',
    );

    return $options;
  }

  /**
   * Implements PersonalizeAgentInterface::getAssets().
   */
  public function getAssets() {
    return array(
      'js' => array(
        drupal_get_path('module', 'personalize_url_context') . '/js/personalize_url_context.js' => array(),
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
    }
    // Also add the possible values for the Referrer URL context.
    $possible_values['referrer_url'] = array(
      'value type' => 'string',
      'friendly name' => t('Most recent referrer')
    );
    $possible_values['original_referrer_url'] = array(
      'value type' => 'string',
      'friendly name' => t('First referrer'),
    );
    if ($limit) {
      $possible_values = array_intersect_key($possible_values, array_flip($this->selectedContext));
    }
    return $possible_values;
  }
}
