<?php

/**
 * @file
 * Contains SearchApiAutocompleteServerSuggester.
 */

/**
 * Provides a suggester plugin that retrieves suggestions from the server.
 *
 * The server needs to support the "search_api_autocomplete" feature for this to
 * work.
 */
class SearchApiAutocompleteServerSuggester extends SearchApiAutocompleteSuggesterPluginBase {

  /**
   * {@inheritdoc}
   */
  public static function supportsIndex(SearchApiIndex $index) {
    try {
      return $index->server() && $index->server()->supportsFeature('search_api_autocomplete');
    }
    catch (SearchApiException $e) {
      return FALSE;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return array(
      'fields' => array(),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, array &$form_state) {
    // Add a list of fields to include for autocomplete searches.
    $search = $this->getSearch();
    $fields = $search->index()->getFields();
    $fulltext_fields = $search->index()->getFulltextFields();
    $options = array();
    foreach ($fulltext_fields as $field) {
      $options[$field] = $fields[$field]['name'];
    }
    $form['fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Override used fields'),
      '#description' => t('Select the fields which should be searched for matches when looking for autocompletion suggestions. Leave blank to use the same fields as the underlying search.'),
      '#options' => $options,
      '#default_value' => drupal_map_assoc($this->configuration['fields']),
      '#attributes' => array('class' => array('search-api-checkboxes-list')),
    );
    $form['#attached']['css'][] = drupal_get_path('module', 'search_api') . '/search_api.admin.css';

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array $form, array &$form_state) {
    $values = $form_state['values'];
    $values['fields'] = array_keys(array_filter($values['fields']));
    $this->setConfiguration($values);
  }

  /**
   * {@inheritdoc}
   */
  public function getAutocompleteSuggestions(SearchApiQueryInterface $query, $incomplete_key, $user_input) {
    if ($this->configuration['fields']) {
      $query->fields($this->configuration['fields']);
    }
    return $query->getIndex()->server()->getAutocompleteSuggestions($query, $this->getSearch(), $incomplete_key, $user_input);
  }

}
