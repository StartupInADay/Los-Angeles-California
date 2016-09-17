<?php

/**
 * @file
 * Editor CKEditor integration for the editor_ckeditor_widgets module.
 */

/**
 * Implements hook_editor_ckeditor_plugins().
 */
function editor_ckeditor_widgets_editor_ckeditor_plugins() {
  $plugins = array();

  $plugins['foundationAccordion'] = array(
    'path' => drupal_get_path('module', 'editor_ckeditor_widgets') . '/js/plugins/foundation/foundationAccordion',
    'file' => 'plugin.js',
    'css' => array(drupal_get_path('module', 'editor_ckeditor_widgets') . '/js/plugins/foundation/foundationAccordion/css/style.css'),
    'buttons' => array(
      'foundationAccordion' => array(
        'label' => t('Accordion'),
        'required_html' => array(array(
          'tags' => array('ul'),
          'attributes' => array('class' => array('accordion')),
        )),
        'image' => drupal_get_path('module', 'editor_ckeditor_widgets') . '/js/plugins/foundation/foundationAccordion/icons/foundationAccordion.png',
      ),
    ),
  );

  if (($library = libraries_detect('ckeditor-leaflet')) && !empty($library['installed'])) {
    $plugins['leaflet'] = array(
      'path' => $library['library path'],
      'file' => 'plugin.js',
      'buttons' => array(
        'leaflet' => array(
          'label' => t('Leaflet'),
          'required_html' => array(array(
            'tags' => array('div'),
            'attributes' => array('class' => array('leaflet_div')),
          )),
          'image' => $library['library path'] . '/icons/leaflet.png',
        ),
      ),
    );
  }

  if (($library = libraries_detect('placeholder')) && !empty($library['installed'])) {
    $plugins['placeholder'] = array(
      'path' => $library['library path'],
      'file' => 'plugin.js',
      'buttons' => array(
        'CreatePlaceholder' => array(
          'label' => t('Placeholder'),
          'required_html' => array(array(
            'tags' => array('span'),
            'attributes' => array('class' => array('cke_placeholder')),
          )),
          'image' => $library['library path'] . '/icons/placeholder.png',
        ),
      ),
    );
  }

  if (($library = libraries_detect('abbr')) && !empty($library['installed'])) {
    $plugins['abbr'] = array(
      'path' => $library['library path'],
      'file' => 'plugin.js',
      'buttons' => array(
        'Abbr' => array(
          'label' => t('Abbreviation'),
          'required_html' => array(array(
            'tags' => array('abbr'),
            'attributes' => array('title', 'id'),
          )),
          'image' => $library['library path'] . '/icons/abbr.png',
        ),
      ),
    );
  }

  if (($library = libraries_detect('timestamp')) && !empty($library['installed'])) {
    $plugins['timestamp'] = array(
      'path' => $library['library path'],
      'file' => 'plugin.js',
      'buttons' => array(
        'Timestamp' => array(
          'label' => t('Timestamp'),
          'required_html' => array(array(
            'tags' => array('em'),
          )),
          'image' => $library['library path'] . '/icons/timestamp.png',
        ),
      ),
    );
  }

  return $plugins;
}
