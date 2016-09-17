<?php

/**
 * @file
 * Contains SearchApiAutocompleteSuggesterPluginBase.
 */

/**
 * Provides a base class for suggester plugins.
 *
 * @see SearchApiAutocompleteSuggesterInterface
 * @see hook_search_api_autocomplete_suggester_info()
 */
abstract class SearchApiAutocompleteSuggesterPluginBase implements SearchApiAutocompleteSuggesterInterface {

  /**
   * The search this suggester is attached to.
   *
   * @var SearchApiAutocompleteSearch
   */
  protected $search;

  /**
   * The suggester plugin's ID.
   *
   * @var string
   */
  protected $pluginId;

  /**
   * The suggester plugin's definition.
   *
   * @var array
   */
  protected $pluginDefinition = array();

  /**
   * The suggester plugin's configuration.
   *
   * @var array
   */
  protected $configuration = array();

  /**
   * {@inheritdoc}
   */
  public static function create(SearchApiAutocompleteSearch $search, array $configuration, $plugin_id, array $plugin_definition) {
    // It seems there is no way to have "new static()"-like functionality in
    // PHP 5.2, so we have to use this workaround instead.
    $class = $plugin_definition['class'];
    return new $class($search, $configuration, $plugin_id, $plugin_definition);
  }

  /**
   * {@inheritdoc}
   */
  public static function supportsIndex(SearchApiIndex $index) {
    return TRUE;
  }

  /**
   * Constructs a SearchApiAutocompleteSuggesterPluginBase.
   *
   * @param SearchApiAutocompleteSearch $search
   *   The search to which this suggester is attached.
   * @param array $configuration
   *   An associative array containing the suggester's configuration, if any.
   * @param string $plugin_id
   *   The suggester's plugin ID.
   * @param array $plugin_definition
   *   The suggester plugin's definition.
   */
  public function __construct(SearchApiAutocompleteSearch $search, array $configuration, $plugin_id, array $plugin_definition) {
    $this->search = $search;
    $this->pluginId = $plugin_id;
    $this->pluginDefinition = $plugin_definition;
    $this->setConfiguration($configuration);
  }

  /**
   * {@inheritdoc}
   */
  public function getPluginId() {
    return $this->pluginId;
  }

  /**
   * {@inheritdoc}
   */
  public function getPluginDefinition() {
    return $this->pluginDefinition;
  }

  /**
   * {@inheritdoc}
   */
  public function getSearch() {
    return $this->search;
  }

  /**
   * {@inheritdoc}
   */
  public function label() {
    return $this->pluginDefinition['label'];
  }

  /**
   * {@inheritdoc}
   */
  public function getDescription() {
    return isset($this->pluginDefinition['description']) ? $this->pluginDefinition['description'] : NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function getConfiguration() {
    return $this->configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function setConfiguration(array $configuration) {
    $this->configuration = $configuration + $this->defaultConfiguration();
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, array &$form_state) {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function validateConfigurationForm(array $form, array &$form_state) {}

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array $form, array &$form_state) {
    if (!empty($form_state['values'])) {
      $this->setConfiguration($form_state['values']);
    }
  }

}
