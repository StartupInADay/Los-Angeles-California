<?php

/**
 * @file
 * All Views hooks except hook_views_api.
 *
 * * Adds a Fieldset field to the Global 'category'.
 * * Registers a Display Extender (plugin) for misc options.
 *
 * @see views_fieldsets_display_extender
 */

/**
 * Implements hook_views_data().
 */
function views_fieldsets_views_data() {
  $data['views']['fieldset'] = array(
    'title' => t('Fieldset'),
    'help' => t('Adds a fieldset to put fields and other fieldsets in.'),
    'field' => array(
      'handler' => 'views_fieldsets_fieldset_field_handler',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function views_fieldsets_views_plugins() {
  return array(
    'display_extender' => array(
      'views_fieldsets' => array(
        'title' => t('Views fieldsets options'),
        'help' => t('A way to export Views Fieldsets.'),
        'handler' => 'views_fieldsets_display_extender',
        'enabled' => TRUE,
        'no ui' => TRUE,
      ),
    ),
  );
}
