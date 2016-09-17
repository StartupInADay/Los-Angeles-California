<?php

/**
 * @file
 * Contains code for integrating with the "Search views" module.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Adds autocompletion to input fields for fulltext keywords on views with
 * exposed filters.
 */
function search_api_autocomplete_form_views_exposed_form_alter(array &$form, array &$form_state) {
  $view = $form_state['view'];
  if (substr($view->base_table, 0, 17) != 'search_api_index_') {
    return;
  }
  $search_id = 'search_api_views_' . $view->name;
  $search = search_api_autocomplete_search_load($search_id);
  if (!$search || !search_api_autocomplete_access($search)) {
    return;
  }
  $index_id = substr($view->base_table, 17);
  $index = search_api_index_load($index_id);
  if (empty($index->options['fields'])) {
    return;
  }
  $fields = $index->getFulltextFields(TRUE);
  // Add the "Search: Fulltext search" filter as another text field.
  $fields[] = 'search_api_views_fulltext';
  drupal_alter('search_api_autocomplete_views_fulltext_fields', $fields, $search, $view);
  // We need the _entity_views_field_identifier() function to translate Search
  // API field names into Views identifiers.
  module_load_include('views.inc', 'entity', 'views/entity');
  foreach ($fields as $search_field) {
    $field = _entity_views_field_identifier($search_field, array());
    if (!empty($view->filter[$field]->options['expose']['identifier'])) {
      $key = $view->filter[$field]->options['expose']['identifier'];
      if (isset($form[$key]) && $form[$key]['#type'] == 'textfield') {
        if ($field == 'search_api_views_fulltext') {
          $fields = $view->filter[$field]->options['fields'];
        }
        else {
          $fields = array($search_field);
        }
        $search->alterElement($form[$key], $fields);
      }
    }
  }
}

/**
 * Returns a list of search views for the given index.
 *
 * @param SearchApiIndex $index
 *   The index whose searches should be returned.
 *
 * @return array
 *   An array of searches, keyed by their machine name. The values are arrays
 *   with the following keys:
 *   - name: A human-readable name for this search.
 *   - options: (optional) An array of options to use for this search.
 *     Type-specific options should go into the "custom" nested key in these
 *     options.
 */
function search_api_autocomplete_views_searches(SearchApiIndex $index) {
  $ret = array();
  $base_table = 'search_api_index_' . $index->machine_name;
  foreach (views_get_all_views() as $name => $view) {
    if ($view->base_table === $base_table) {
      // @todo Check whether there is an exposed fulltext filter
      $ret['search_api_views_' . $name] = array(
        'name' => !empty($view->human_name) ? $view->human_name : $name,
      );
    }
  }
  return $ret;
}

/**
 * Create the query that would be issued for the given search for the complete keys.
 *
 * @param SearchApiAutocompleteSearch $search
 *   The search for which to create the query.
 * @param $complete
 *   A string containing the complete search keys.
 * @param $incomplete
 *   A string containing the incomplete last search key.
 *
 * @return SearchApiQueryInterface
 *   The query that would normally be executed when only $complete was entered
 *   as the search keys for the given search.
 *
 * @throws SearchApiException
 *   If the query couldn't be created.
 */
function search_api_autocomplete_views_query(SearchApiAutocompleteSearch $search, $complete, $incomplete) {
  $views_id = substr($search->machine_name, 17);
  $view = views_get_view($views_id);
  if (!$view) {
    $vars['@view'] = $views_id;
    throw new SearchApiException(t('Could not load view @view.', $vars));
  }
  $view->set_display(isset($search->options['custom']['display']) ? $search->options['custom']['display'] : NULL);
  $view->pre_execute();
  $view->build();
  $query = $view->query->getSearchApiQuery();
  if (!$query) {
    $vars['@view'] = !empty($view->human_name) ? $view->human_name : $views_id;
    throw new SearchApiException(t('Could not create query for view @view.', $vars));
  }
  $query->keys($complete);
  return $query;
}

/**
 * Form callback for a Views-specific autocomplete configuration form.
 *
 * @param SearchApiAutocompleteSearch $search
 *   The search being configured.
 *
 * @see example_autocomplete_config_form()
 * @see search_api_autocomplete_views_config_form_submit()
 */
function search_api_autocomplete_views_config_form(array $form, array &$form_state, SearchApiAutocompleteSearch $search) {
  $views_id = substr($search->machine_name, 17);
  $view = views_get_view($views_id);
  $options = array();
  foreach ($view->display as $id => $display) {
    $options[$id] = $display->display_title;
  }
  $form['display'] = array(
    '#type' => 'select',
    '#title' => t('Views display'),
    '#description' => t('Please select the Views display whose settings should be used for autocomplete queries.<br />' .
        "<strong>Note:</strong> Autocompletion doesn't work well with contextual filters. Please see the <a href='@readme_url'>README.txt</a> file for details.",
        array('@readme_url' => url(drupal_get_path('module', 'search_api_autocomplete') . '/README.txt'))),
    '#options' => $options,
    '#default_value' => isset($search->options['custom']['display']) ? $search->options['custom']['display'] : 'default',
  );

  return $form;
}

/**
 * Submit callback for search_api_autocomplete_views_config_form().
 *
 * @see example_autocomplete_config_form_submit()
 * @see search_api_autocomplete_views_config_form()
 */
function search_api_autocomplete_views_config_form_submit(array $form, array &$form_state, array &$values) {
  $views_id = substr($form_state['search']->machine_name, 17);
  $view = views_get_view($views_id);
  $view->set_display($values['display']);
  $view->pre_execute();
  if ($view->argument) {
    drupal_set_message(t('You have selected a display with contextual filters. This can lead to various problems. Please see the <a href="@readme_url">README.txt</a> file for details.',
        array('@readme_url' => url(drupal_get_path('module', 'search_api_autocomplete') . '/README.txt'))), 'warning');
  }
}
