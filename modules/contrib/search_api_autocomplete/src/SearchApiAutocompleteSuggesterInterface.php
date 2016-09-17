<?php

/**
 * @file
 * Contains SearchApiAutocompleteSuggesterInterface.
 */

/**
 * Represents a plugin for creating autocomplete suggestions.
 *
 * @see SearchApiAutocompleteSuggesterPluginBase
 * @see hook_search_api_autocomplete_suggester_info()
 */
interface SearchApiAutocompleteSuggesterInterface {

  /**
   * Creates a new instance of this class.
   *
   * @param SearchApiAutocompleteSearch $search
   *   The search to which this suggester is attached.
   * @param array $configuration
   *   An associative array containing the suggester's configuration, if any.
   * @param string $plugin_id
   *   The suggester's plugin ID.
   * @param array $plugin_definition
   *   The suggester plugin's definition.
   *
   * @return static
   *   A new instance of this class.
   */
  public static function create(SearchApiAutocompleteSearch $search, array $configuration, $plugin_id, array $plugin_definition);

  /**
   * Determines whether this plugin class supports the given index.
   *
   * @param SearchApiIndex $index
   *   The search index in question.
   *
   * @return bool
   *   TRUE if this plugin supports the given search index, FALSE otherwise.
   */
  public static function supportsIndex(SearchApiIndex $index);

  /**
   * Retrieves the plugin's ID.
   *
   * @return string
   *   The plugin's ID.
   */
  public function getPluginId();

  /**
   * Retrieves the plugin's definition.
   *
   * @return array
   *   The plugin's definition.
   */
  public function getPluginDefinition();

  /**
   * Retrieves the search this plugin is configured for.
   *
   * @return SearchApiAutocompleteSearch
   *   The search this plugin is configured for.
   */
  public function getSearch();

  /**
   * Retrieves the plugin's label.
   *
   * @return string
   *   The plugin's human-readable and translated label.
   */
  public function label();

  /**
   * Retrieves the plugin's description.
   *
   * @return string|null
   *   The plugin's translated description; or NULL if it has none.
   */
  public function getDescription();

  /**
   * Retrieves the plugin's configuration.
   *
   * @return array
   *   An associative array containing the plugin's configuration.
   */
  public function getConfiguration();

  /**
   * Sets the plugin's configuration.
   *
   * @param array $configuration
   *   An associative array containing the plugin's configuration.
   *
   * @return $this
   */
  public function setConfiguration(array $configuration);

  /**
   * Retrieves the default configuration for this plugin.
   *
   * @return array
   *   An associative array containing the plugin's default configuration.
   */
  public function defaultConfiguration();

  /**
   * Constructs the plugin's configuration form.
   *
   * @param array $form
   *   An associative array containing the structure of the form.
   * @param array $form_state
   *   The current state of the form.
   *
   * @return array
   *   An associative array containing the structure of the form. An empty array
   *   if the plugin has no configuration form.
   */
  public function buildConfigurationForm(array $form, array &$form_state);

  /**
   * Validates the plugin's configuration form.
   *
   * @param array $form
   *   An associative array containing the structure of the form.
   * @param array $form_state
   *   The current state of the form.
   */
  public function validateConfigurationForm(array $form, array &$form_state);

  /**
   * Submits the plugin's configuration form.
   *
   * Should take care of calling setConfiguration() with the new configuration
   * values as appropriate.
   *
   * @param array $form
   *   An associative array containing the structure of the form.
   * @param array $form_state
   *   The current state of the form.
   */
  public function submitConfigurationForm(array $form, array &$form_state);

  /**
   * Retrieves autocompletion suggestions for some user input.
   *
   * For example, when given the user input "teach us", with "us" being
   * considered incomplete, the following might be returned:
   *
   * @code
   *   array(
   *     array(
   *       'prefix' => t('Did you mean:'),
   *       'user_input' => 'reach us',
   *     ),
   *     array(
   *       'user_input' => 'teach us',
   *       'suggestion_suffix' => 'ers',
   *     ),
   *     array(
   *       'user_input' => 'teach us',
   *       'suggestion_suffix' => ' swimming',
   *     ),
   *     'teach users swimming',
   *   );
   * @endcode
   *
   * @param SearchApiQueryInterface $query
   *   A query representing the completed user input so far.
   * @param string $incomplete_key
   *   The start of another fulltext keyword for the search, which should be
   *   completed. Might be empty, in which case all user input up to now was
   *   considered completed. Then, additional keywords for the search could be
   *   suggested.
   * @param string $user_input
   *   The complete user input for the fulltext search keywords so far.
   *
   * @return array
   *   An array of suggestions. Each suggestion is either a simple string
   *   containing the whole suggested keywords, or an array containing the
   *   following keys:
   *   - keys: The keyword (or keywords) this suggestion will autocomplete to.
   *     If it is not present, a direct concatenation (no spaces in between) of
   *     "suggestion_prefix", "user_input" and "suggestion_suffix" will be used
   *     instead.
   *   - url: A URL to which the suggestion should redirect instead of
   *     completing the user input in the text field. This overrides the normal
   *     behavior and thus makes "keys" obsolete.
   *   - prefix: For special suggestions, some kind of HTML prefix describing
   *     them.
   *   - suggestion_prefix: A suggested prefix for the entered input.
   *   - user_input: The input entered by the user. Defaults to $user_input.
   *   - suggestion_suffix: A suggested suffix for the entered input.
   *   - results: If available, the estimated number of results for these keys.
   *   - render: If given, an HTML string or render array which should be
   *     displayed to the user for this suggestion. If missing, the suggestion
   *     is instead passed to theme_search_api_autocomplete_suggestion().
   *   All the keys are optional, with the exception that at least one of
   *   "keys", "url", "suggestion_prefix", "user_input" or "suggestion_suffix"
   *   has to be present.
   */
  public function getAutocompleteSuggestions(SearchApiQueryInterface $query, $incomplete_key, $user_input);

}
