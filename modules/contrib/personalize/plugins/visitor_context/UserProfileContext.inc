<?php
/**
 * @file
 * Provides a visitor context plugin for user profiles

 */

class UserProfileContext extends PersonalizeContextBase {

  /**
   * A fully-loaded user object representing the current user.
   *
   * @var stcClass
   */
  protected $account;

  /**
   * Implements PersonalizeContextInterface::create().
   */
  public static function create(PersonalizeAgentInterface $agent = NULL, $selected_context = array()) {
    global $user;
    $account = $user->uid ? user_load($user->uid) : NULL;
    return new self($agent, $selected_context, $account);
  }

  /**
   * Override the parent constructor to add the user account.
   */
  public function __construct(PersonalizeAgentInterface $agent = NULL, $selected_context, $account) {
    parent::__construct($agent, $selected_context);
    $this->account = $account;
  }

  /**
   * Implements PersonalizeContextInterface::getOptions().
   */
  public static function getOptions() {
    $options = array();
    // Go through all user profile fields.
    $fields = field_info_instances('user', 'user');
    foreach ($fields as $field_name => $field) {
      $info = field_info_field($field_name);
      // We currently only support fields with a 'value' column.
      // @todo Figure out a sane UI for supporting fields with columns
      //   other than 'value'.
      // Only single-value fields make sense for targeting.
      if ($info['cardinality'] == 1 && isset($info['columns']['value'])) {
        // Strip the 'field_' prefix off as it is redundant and we need
        // to keep feature strings short.
        $option_name = preg_replace('/^field_/', '', $field_name);
        $options[$option_name] = array(
          'name' => $field['label'],
          'group' => 'User Profile'
        );
      }
    }

    return $options;
  }

  /**
   * Implements PersonalizeAgentInterface::getAssets().
   */
  public function getAssets() {
    if ($this->account === NULL) {
      // Nothing to do for anonymous users.
      return array();
    }

    $user_profile_settings = array();
    foreach ($this->selectedContext as $name) {

      $field_name = 'field_' . $name;
      // Given that we have restricted the usable profile fields to those
      // that have a 'value' column and that take a single value, it is ok to
      // specify the exact array structure in retrieving the value.
      // @todo Revisit this when we add support for fields with columns other
      //   than 'value'.
      if (!empty($this->account->{$field_name}) && isset($this->account->{$field_name}[LANGUAGE_NONE][0]['value'])) {
        $value = $this->account->{$field_name}[LANGUAGE_NONE][0]['value'];
        $user_profile_settings[$name] = $value;
      }
    }

    return array(
      'js' => array(
        array(
          'type' => 'setting',
          'data' => array(
            'personalize_user_profile_context' => $user_profile_settings
          )
        ),
      ),
      // @todo Once the js for the user profile context has been split off into its
      //   own file where it belongs, we'll only need to load that here.
      'library' => array(
        array('personalize', 'personalize')
      )
    );
  }

  /**
   * Implements PersonalizeAgentInterface::getPossibleValues().
   */
  public function getPossibleValues($limit = FALSE) {
    $possible_values = array();
    $entity = new stdClass();
    $form = $form_state = array();
    $options = $this->getOptions();
    foreach ($options as $name => $info) {

      $field_name = 'field_' . $name;
      $field = field_info_field($field_name);
      $instance = field_info_instance('user', $field_name, 'user');
      $possible_values[$name] = array(
        'friendly name' => $instance['label']
      );
      if ($instance['widget']['module'] == 'options') {
        if ($instance['widget']['type'] === 'options_onoff') {
          $possible_values[$name]['value type'] = 'boolean';
          $possible_values[$name]['on_label'] = 'On';
          $possible_values[$name]['off_label'] = 'Off';
        }
        else {
          $possible_values[$name]['value type'] = 'predefined';
          // Build the form element that would be used to display the field
          // for editing and pull the options list out of it. (There is no API
          // function for getting the list of possible values from a field.)
          $element = options_field_widget_form($form, $form_state, $field, $instance, LANGUAGE_NONE, array(), 0, array('#required' => FALSE, '#entity_type' => 'user', '#entity' => $entity));
          foreach ($element['#options'] as $key => $label) {
            if ($key != '_none') {
              $possible_values[$name]['values'][$key] = $label;
            }
          }
        }
      }
      else {
        if (strpos($field['type'], 'number_') === 0) {
          $possible_values[$name]['value type'] = 'number';
        }
        else {
          $possible_values[$name]['value type'] = 'string';
        }
      }
    }
    if ($limit) {
      $possible_values = array_intersect_key($possible_values, array_flip($this->selectedContext));
    }
    return $possible_values;
  }
}
