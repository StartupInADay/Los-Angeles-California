<?php

/**
 * @file
 * Contains SearchApiAutocompleteSearch.
 */

/**
 * Describes the autocomplete settings for a certain search.
 */
class SearchApiAutocompleteSearch extends Entity {

  // Entity properties, loaded from the database:

  /**
   * @var integer
   */
  public $id;

  /**
   * @var string
   */
  public $machine_name;

  /**
   * @var string
   */
  public $name;

  /**
   * @var integer
   */
  public $index_id;

  /**
   * @var string
   */
  public $suggester_id;

  /**
   * @var string
   */
  public $type;

  /**
   * @var boolean
   */
  public $enabled;

  /**
   * An array of options for this search, containing any of the following:
   * - results: Boolean indicating whether to also list the estimated number of
   *   results for each suggestion (if possible).
   * - fields: Array containing the fulltext fields to use for autocompletion.
   * - custom: An array of type-specific settings.
   *
   * @var array
   */
  public $options = array();

  // Inferred properties, for caching:

  /**
   * @var SearchApiIndex
   */
  protected $index;

  /**
   * @var SearchApiServer
   */
  protected $server;

  /**
   * The suggester plugin this search uses.
   *
   * @var SearchApiAutocompleteSuggesterInterface
   */
  protected $suggester;

  /**
   * Constructs a SearchApiAutocompleteSearch.
   *
   * @param array $values
   *   The entity properties.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'search_api_autocomplete_search');
  }

  /**
   * @return SearchApiIndex
   *   The index this search belongs to.
   */
  public function index() {
    if (!isset($this->index)) {
      $this->index = search_api_index_load($this->index_id);
      if (!$this->index) {
        $this->index = FALSE;
      }
    }
    return $this->index;
  }

  /**
   * Retrieves the server this search would at the moment be executed on.
   *
   * @return SearchApiServer
   *   The server this search would at the moment be executed on.
   *
   * @throws SearchApiException
   *   If a server is set for the index but it doesn't exist.
   */
  public function server() {
    if (!isset($this->server)) {
      if (!$this->index() || !$this->index()->server) {
        $this->server = FALSE;
      }
      else {
        $this->server = $this->index()->server();
        if (!$this->server) {
          $this->server = FALSE;
        }
      }
    }
    return $this->server;
  }

  /**
   * Retrieves the ID of the suggester plugin for this search.
   *
   * @return string
   *   This search's suggester plugin's ID.
   */
  public function getSuggesterId() {
    return $this->suggester_id;
  }

  /**
   * Retrieves the suggester plugin for this search.
   *
   * @param bool $reset
   *   (optional) If TRUE, clear the internal static cache and reload the
   *   suggester.
   *
   * @return SearchApiAutocompleteSuggesterInterface|null
   *   This search's suggester plugin, or NULL if it could not be loaded.
   */
  public function getSuggester($reset = FALSE) {
    if (!isset($this->suggester) || $reset) {
      $configuration = !empty($this->options['suggester_configuration']) ? $this->options['suggester_configuration'] : array();
      $this->suggester = search_api_autocomplete_suggester_load($this->suggester_id, $this, $configuration);
      if (!$this->suggester) {
        $variables['@search'] = $this->machine_name;
        $variables['@index'] = $this->index() ? $this->index()->label() : $this->index_id;
        $variables['@suggester_id'] = $this->suggester_id;
        watchdog('search_api_autocomplete', 'Autocomplete search @search on index @index specifies an invalid suggester plugin @suggester_id.', $variables, WATCHDOG_ERROR);
        $this->suggester = FALSE;
      }
    }
    return $this->suggester ? $this->suggester : NULL;
  }

  /**
   * Determines whether autocompletion is currently supported for this search.
   *
   * @return bool
   *   TRUE if autocompletion is possible for this search with the current
   *   settings; FALSE otherwise.
   */
  public function supportsAutocompletion() {
    return $this->index() && $this->getSuggester() && $this->getSuggester()->supportsIndex($this->index());
  }

  /**
   * Helper method for altering a textfield form element to use autocompletion.
   */
  public function alterElement(array &$element, array $fields = array()) {
    if (search_api_autocomplete_access($this)) {
      // Add option defaults (in case of updates from earlier versions).
      $options = $this->options + array(
        'submit_button_selector' => ':submit',
        'autosubmit' => TRUE,
        'min_length' => 1,
      );

      $fields_string = $fields ? implode(' ', $fields) : '-';

      $module_path = drupal_get_path('module', 'search_api_autocomplete');
      $autocomplete_path = 'search_api_autocomplete/' . $this->machine_name . '/' . $fields_string;
      $element['#attached']['css'][] = $module_path . '/search_api_autocomplete.css';
      $element['#attached']['js'][] = $module_path . '/search_api_autocomplete.js';

      $js_settings = array();
      if ($options['submit_button_selector'] != ':submit') {
        $js_settings['selector'] = $options['submit_button_selector'];
      }
      if (($delay = variable_get('search_api_autocomplete_delay')) !== NULL) {
        $js_settings['delay'] = $delay;
      }

      // Allow overriding of the default handler with a custom script.
      $path_overrides = variable_get('search_api_autocomplete_scripts', array());
      if (!empty($path_overrides[$this->machine_name])) {
        $autocomplete_path = NULL;
        $override = $path_overrides[$this->machine_name];
        if (is_scalar($override)) {
          $autocomplete_path = url($override, array('absolute' => TRUE, 'query' => array('machine_name' => $this->machine_name)));
        }
        elseif (!empty($override['#callback']) && is_callable($override['#callback'])) {
          $autocomplete_path = call_user_func($override['#callback'], $this, $element, $override);
        }
        if (!$autocomplete_path) {
          return;
        }
        $js_settings['custom_path'] = TRUE;
      }

      if ($js_settings) {
        $element['#attached']['js'][] = array(
          'type' => 'setting',
          'data' => array(
            'search_api_autocomplete' => array(
              $this->machine_name => $js_settings,
            ),
          ),
        );
      }

      $element['#autocomplete_path'] = $autocomplete_path;
      $element += array('#attributes' => array());
      $element['#attributes'] += array('class'=> array());
      if ($options['autosubmit']) {
        $element['#attributes']['class'][] = 'auto_submit';
      }
      $element['#attributes']['data-search-api-autocomplete-search'] = $this->machine_name;
      if ($options['min_length'] > 1) {
        $element['#attributes']['data-min-autocomplete-length'] = $options['min_length'];
      }
    }
  }

  /**
   * Split a string with search keywords into two parts.
   *
   * The first part consists of all words the user has typed completely, the
   * second one contains the beginning of the last, possibly incomplete word.
   *
   * @return array
   *   An array with $keys split into exactly two parts, both of which may be
   *   empty.
   */
  public function splitKeys($keys) {
    $keys = ltrim($keys);
    // If there is whitespace or a quote on the right, all words have been
    // completed.
    if (rtrim($keys, " \"") != $keys) {
      return array(rtrim($keys, ' '), '');
    }
    if (preg_match('/^(.*?)\s*"?([\S]*)$/', $keys, $m)) {
      return array($m[1], $m[2]);
    }
    return array('', $keys);
  }

  /**
   * Create the query that would be issued for this search for the complete keys.
   *
   * @param $complete
   *   A string containing the complete search keys.
   * @param $incomplete
   *   A string containing the incomplete last search key.
   *
   * @return SearchApiQueryInterface
   *   The query that would normally be executed when only $complete was entered
   *   as the search keys for this search.
   *
   * @throws SearchApiException
   *   If the query couldn't be created.
   */
  public function getQuery($complete, $incomplete) {
    $info = search_api_autocomplete_get_types($this->type);
    if (empty($info['create query'])) {
      return NULL;
    }
    $query = $info['create query']($this, $complete, $incomplete);
    if ($complete && !$query->getKeys()) {
      $query->keys($complete);
    }
    return $query;
  }

}
