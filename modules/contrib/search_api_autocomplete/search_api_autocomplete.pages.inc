<?php

/**
 * @file
 * Contains page callbacks and theme functions for the frontend UI.
 */

/**
 * Page callback for getting autocomplete suggestions.
 *
 * @param SearchApiAutocompleteSearch $search
 *   The search for which to retrieve autocomplete suggestions.
 * @param string $fields
 *   A comma-separated list of fields on which to do autocompletion. Or "-" to
 *   use all fulltext fields.
 * @param string $keys
 *   The user input so far.
 */
function search_api_autocomplete_autocomplete(SearchApiAutocompleteSearch $search, $fields, $keys = '') {
  $ret = array();
  try {
    if ($search->supportsAutocompletion()) {
      list($complete, $incomplete) = $search->splitKeys($keys);
      $query = $search->getQuery($complete, $incomplete);
      if ($query) {
        // @todo Maybe make range configurable?
        $query->range(0, 10);
        $query->setOption('search id', 'search_api_autocomplete:' . $search->machine_name);
        if (!empty($search->options['fields'])) {
          $query->fields($search->options['fields']);
        }
        elseif ($fields != '-') {
          $fields = explode(' ', $fields);
          $query->fields($fields);
        }
        $query->preExecute();
        $suggestions = $search->getSuggester()->getAutocompleteSuggestions($query, $incomplete, $keys);
        if ($suggestions) {
          foreach ($suggestions as $suggestion) {
            // Convert suggestion strings into an array.
            if (is_string($suggestion)) {
              $pos = strpos($suggestion, $keys);
              if ($pos === FALSE) {
                $suggestion = array(
                  'user_input' => '',
                  'suggestion_suffix' => $suggestion,
                );
              }
              else {
                $suggestion = array(
                  'suggestion_prefix' => substr($suggestion, 0, $pos),
                  'user_input' => $keys,
                  'suggestion_suffix' => substr($suggestion, $pos + strlen($keys)),
                );
              }
            }
            // Add defaults.
            $suggestion += array(
              'url' => NULL,
              'keys' => NULL,
              'prefix' => NULL,
              'suggestion_prefix' => '',
              'user_input' => $keys,
              'suggestion_suffix' => '',
              'results' => NULL,
            );
            if (empty($search->options['results'])) {
              unset($suggestion['results']);
            }

            // Decide what the action of the suggestion is – entering specific
            // search terms or redirecting to a URL.
            if (isset($suggestion['url'])) {
              $key = ' ' . $suggestion['url'];
            }
            else {
              // Also set the "keys" key so it will always be available in alter
              // hooks and the theme function.
              if (!isset($suggestion['keys'])) {
                $suggestion['keys'] = $suggestion['suggestion_prefix'] . $suggestion['user_input'] . $suggestion['suggestion_suffix'];
              }
              $key = trim($suggestion['keys']);
            }

            if (!isset($ret[$key])) {
              $ret[$key] = $suggestion;
            }
          }

          $alter_params = array(
            'query' => $query,
            'search' => $search,
            'incomplete_key' => $incomplete,
            'user_input' => $keys,
          );
          drupal_alter('search_api_autocomplete_suggestions', $ret, $alter_params);

          foreach ($ret as $key => $suggestion) {
            if (isset($suggestion['render'])) {
              $ret[$key] = render($suggestion['render']);
            }
            else {
              $escaped_variables = array('keys', 'suggestion_prefix', 'user_input', 'suggestion_suffix');
              foreach ($escaped_variables as $variable) {
                if ($suggestion[$variable]) {
                  $suggestion[$variable] = check_plain($suggestion[$variable]);
                }
              }
              $ret[$key] = theme('search_api_autocomplete_suggestion', $suggestion);
            }
          }
        }
      }
    }
  }
  catch (SearchApiException $e) {
    watchdog_exception('search_api_autocomplete', $e, '%type while retrieving autocomplete suggestions: !message in %function (line %line of %file).');
  }

  drupal_json_output($ret);
}

/**
 * Returns HTML for an autocomplete suggestion.
 *
 * @param array $variables
 *   An associative array containing:
 *   - keys: The keyword (or keywords) this suggestion will autocomplete to.
 *   - prefix: For special suggestions, some kind of HTML prefix describing
 *     them.
 *   - suggestion_prefix: A suggested prefix for the entered input.
 *   - user_input: The input entered by the user.
 *   - suggestion_suffix: A suggested suffix for the entered input.
 *   - results: If available, the estimated number of results for these keys.
 */
function theme_search_api_autocomplete_suggestion(array $variables) {
  $prefix = $variables['prefix'];
  $suggestion_prefix = $variables['suggestion_prefix'];
  $user_input = $variables['user_input'];
  $suggestion_suffix = $variables['suggestion_suffix'];
  $results = $variables['results'];

  $output = '';
  if ($prefix) {
    $output .= "<span class=\"autocomplete-suggestion-note\">$prefix</span> ";
  }
  if ($suggestion_prefix) {
    $output .= "<span class=\"autocomplete-suggestion-prefix\">$suggestion_prefix</span>";
  }
  if ($user_input) {
    $output .= "<span class=\"autocomplete-user-input\">$user_input</span>";
  }
  if ($suggestion_suffix) {
    $output .= "<span class=\"autocomplete-suggestion-suffix\">$suggestion_suffix</span>";
  }
  if ($results) {
    $output .= " <span class=\"autocomplete-suggestion-results\">$results</span>";
  }

  return "<div class=\"search-api-autocomplete-suggestion\">$output</div>";
}
