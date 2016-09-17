<?php

/**
 * @file
 * Provides a visitor context plugin for taxonomy.

 */

class TaxonomyContext extends PersonalizeContextBase {
  const TERMS_MAX_COUNT = 100;

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
    $vocabularies = taxonomy_get_vocabularies();
    $options = array();
    foreach ($vocabularies as $vocabulary) {
      $machine_name = $vocabulary->machine_name;
      $options[$machine_name] = array(
        'name' => $vocabulary->name,
        'machine_name' => $vocabulary->machine_name,
        'group' => 'Taxonomy',
        'id' => $vocabulary->vid,
      );
    }
    return $options;
  }

  /**
   * Implements PersonalizeContextInterface::getPossibleValues().
   */
  public function getPossibleValues($limit = FALSE) {
    $possible_values = array();
    $options = $this->getOptions();
    foreach ($options as $machine_name => $info) {
      $values = array();
      $terms = taxonomy_get_tree($info['id']);
      $count = 0;
      foreach($terms as $term) {
        // Collect terms up to the max count.
        if ($count >= self::TERMS_MAX_COUNT) {
          break;
        }
        $values[$term->name] = $term->name;
        $count++;
      }

      $machine_name = $info['machine_name'];
      $name = $info['name'];
      $possible_values[$machine_name] = array(
        'value type' => 'predefined',
        'friendly name' => $name,
        'values' => $values,
      );
    }
    return $possible_values;
  }
}
