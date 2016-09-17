<?php

/**
 * @file
 * Filter hook implementations and callbacks.
 */

/**
 * Implements hook_filter_format_insert().
 */
function editor_filter_format_insert($format) {
  _editor_format_save($format);
}

/**
 * Implements hook_filter_format_update().
 */
function editor_filter_format_update($format) {
  _editor_format_save($format);
}

/**
 * Helper function to save editor information on text format insert/update.
 *
 * @param object $format
 *   An object representing the text format.
 */
function _editor_format_save($format) {
  if (!isset($format->editor)) {
    $format->editor = '';
  }

  if (!isset($format->editor_settings)) {
    $format->editor_settings = array();
  }

  // Add 'support' for features.module which will store editor settings as a
  // serialized array since it gets its information from filter_format_load()
  // and filter_formats() which are hard-coded to ensure that the standard
  // filter settings are serialized but cannot be altered to do the same for
  // editor settings.
  // @todo Remove this when we can ensure that editor setting are unserialized.
  $editor_settings = $format->editor_settings;

  // Unserialize the editor settings when necessary. This prevents the editor
  // settings from being serialized twice.
  if ($editor_settings == serialize(false) || @unserialize($editor_settings) !== false) {
    $format->editor_settings = unserialize($editor_settings);
  }

  // Insert or update the text format.
  db_merge('filter_format')
    ->key(array('format' => $format->format))
    ->fields(array(
      'editor' => $format->editor,
      'editor_settings' => serialize($format->editor_settings),
    ))
    ->execute();
}

/**
 * Implements hook_filter_info().
 */
function editor_filter_info() {
  $filters['editor_caption'] = array(
    'title' => t('Convert image captions to figure and figcaption elements'),
    'process callback' => '_editor_caption',
    'tips callback' => '_editor_caption_tips',
    'weight' => 4,
  );
  $filters['editor_align'] = array(
    'title' => t('Align images'),
    'process callback' => '_editor_align',
    'tips callback' => '_editor_align_tips',
    'weight' => 5,
  );

  // Assigns filter types for the Quick Edit module.
  if (module_exists('quickedit')) {
    $filters['editor_caption']['type'] = FILTER_TYPE_TRANSFORM_REVERSIBLE;
    $filters['editor_align']['type'] = FILTER_TYPE_TRANSFORM_REVERSIBLE;
  }

  return $filters;
}

/**
 * Implements callback_filter_process().
 *
 * Replace img data-caption attributes with figure and figcaption elements.
 */
function _editor_caption($text) {
  $result = $text;

  if (stristr($text, 'data-caption') !== FALSE) {
    // Load the text as a DOM object for manipulation.
    $dom = filter_dom_load($text);
    $xpath = new DOMXPath($dom);
    foreach ($xpath->query('//*[@data-caption]') as $node) {
      // Read the data-caption attribute's value, then delete it.
      $caption = $node->getAttribute('data-caption');
      $node->removeAttribute('data-caption');

      // Sanitize caption: decode HTML encoding, limit allowed HTML tags; only
      // allow inline tags that are allowed by default, plus <br>.
      $caption = filter_xss($caption, array('a', 'em', 'strong', 'cite', 'code', 'br'));

      // The caption must be non-empty.
      if (drupal_strlen($caption) === 0) {
        continue;
      }

      // Given the updated node and caption: re-render it with a caption, but
      // bubble up the value of the class attribute of the captioned element,
      // this allows it to collaborate with e.g. the filter_align filter.
      $attributes = array();
      $tag = $node->tagName;
      $classes = $node->getAttribute('class');

      if ($classes) {
        $attributes['class'] = explode(' ', $classes);
      }

      $attributes['class'][] = 'caption';
      $attributes['class'][] = 'caption-' . $node->tagName;
      $node->removeAttribute('class');
      $node = ($node->parentNode->tagName === 'a') ? $node->parentNode : $node;

      $theme_parameters = array(
        'item' => $node->ownerDocument->saveHTML($node),
        'tag' => $tag,
        'caption' => $caption,
        'attributes' => $attributes,
      );

      $editor_caption = theme('editor_caption', $theme_parameters);

      // Load the altered HTML into a new DOMDocument and retrieve the element.
      $updated_node = filter_dom_load($editor_caption)->getElementsByTagName('body')
        ->item(0)
        ->firstChild;

      // Import the updated node from the new DOMDocument into the original
      // one, importing also the child nodes of the updated node.
      $updated_node = $dom->importNode($updated_node, TRUE);
      // Finally, replace the original node with the new node.
      $node->parentNode->replaceChild($updated_node, $node);
    }

    $result = filter_dom_serialize($dom);
  }

  return $result;
}

/**
 * Implements callback_filter_tips().
 *
 * Provides help for the caption filter.
 */
function _editor_caption_tips($filter, $format, $long = FALSE) {
  if ($long) {
    return t('
        <p>You can caption images, videos, blockquotes, and so on. Examples:</p>
        <ul>
            <li><code>&lt;img src="" data-caption="This is a caption" /&gt;</code></li>
            <li><code>&lt;video src="" data-caption="The Drupal Dance" /&gt;</code></li>
            <li><code>&lt;blockquote data-caption="Dries Buytaert"&gt;Drupal is awesome!&lt;/blockquote&gt;</code></li>
            <li><code>&lt;code data-caption="Hello world in JavaScript."&gt;alert("Hello world!");&lt;/code&gt;</code></li>
        </ul>');
  }
  else {
    return t('You can caption images (<code>data-caption="Text"</code>), but also videos, blockquotes, and so on.');
  }
}

/**
 * Implements callback_filter_process().
 *
 * Uses a data-align attribute on <img> tags to align images.
 */
function _editor_align($text) {
  $result = $text;

  if (stristr($text, 'data-align') !== FALSE) {
    $dom = filter_dom_load($text);
    $xpath = new DOMXPath($dom);
    foreach ($xpath->query('//*[@data-align]') as $node) {
      // Read the data-align attribute's value, then delete it.
      $align = $node->getAttribute('data-align');
      $node->removeAttribute('data-align');

      // If one of the allowed alignments, add the corresponding class.
      if (in_array($align, array('left', 'center', 'right'))) {
        $classes = $node->getAttribute('class');
        $classes = (strlen($classes) > 0) ? explode(' ', $classes) : array();
        $classes[] = 'align-' . $align;
        $node->setAttribute('class', implode(' ', $classes));
      }
    }

    $result = filter_dom_serialize($dom);
  }

  return $result;
}

/**
 * Implements callback_filter_tips().
 *
 * Provides help for the align filter.
 */
function _editor_align_tips($filter, $format, $long = FALSE) {
  if ($long) {
    return t('
        <p>You can align images, videos, blockquotes and so on to the left, right or center. Examples:</p>
        <ul>
          <li>Align an image to the left: <code>&lt;img src="" data-align="left" /&gt;</code></li>
          <li>Align an image to the center: <code>&lt;img src="" data-align="center" /&gt;</code></li>
          <li>Align an image to the right: <code>&lt;img src="" data-align="right" /&gt;</code></li>
          <li>… and you can apply this to other elements as well: <code>&lt;video src="" data-align="center" /&gt;</code></li>
        </ul>');
  }
  else {
    return t('You can align images (<code>data-align="center"</code>), but also videos, blockquotes, and so on.');
  }
}

/**
 * Implements hook_filter_info_alter().
 */
function editor_filter_info_alter(&$info) {
  // Add an allowed HTML callback to the core 'filter_html' filter which enables
  // it to specify a whitelist of approved HTML tags/attributes.
  if (isset($info['filter_html'])) {
    $info['filter_html']['allowed html callback'] = '_editor_html_allowed_html';
    $info['filter_html']['settings callback'] = '_editor_html_settings';
  }

  // Add a JS settings callback to the core 'filter_url' filter which makes its
  // URL length restrictions available as JS settings.
  if (isset($info['filter_url'])) {
    $info['filter_url']['js settings callback'] = '_editor_url_js_settings';
  }
}

/**
 * Implements callback_filter_allowed_html().
 */
function _editor_html_allowed_html($filter, $format) {
  // This example is pulled from "filter_html" filter provided by core.
  $restrictions = array('allowed' => array());
  $tags = preg_split('/\s+|<|>/', $filter->settings['allowed_html'], -1, PREG_SPLIT_NO_EMPTY);

  // List the allowed HTML tags.
  foreach ($tags as $tag) {
    $restrictions['allowed'][$tag] = TRUE;
  }

  // The 'style' and 'on*' ('onClick' etc.) attributes are always forbidden.
  $restrictions['allowed']['*'] = array('style' => FALSE, 'on*' => FALSE);

  return $restrictions;
}

/**
 * Implements callback_filter_settings().
 *
 * Filter settings callback for the HTML content filter.
 */
function _editor_html_settings($form, $form_state, $filter, $format, $defaults = array(), $filters = array()) {
  $filter->settings += $defaults;
  $settings['#attached']['library'][] = array('filter', 'filter.filtered_html.admin');

  $settings['allowed_html'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed HTML tags'),
    '#default_value' => $filter->settings['allowed_html'],
    '#maxlength' => 1024,
    '#description' => t('A list of HTML tags that can be used. JavaScript event attributes, JavaScript URLs, and CSS are always stripped.'),
  );
  $settings['filter_html_help'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display basic HTML help in long filter tips'),
    '#default_value' => $filter->settings['filter_html_help'],
  );
  $settings['filter_html_nofollow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add rel="nofollow" to all links'),
    '#default_value' => $filter->settings['filter_html_nofollow'],
  );

  return $settings;
}

/**
 * Implements callback_filter_js_settings().
 *
 * Filter URL JS settings callback: return settings for JavaScript.
 */
function _editor_url_js_settings($filter, $format) {
  return array(
    'filterUrlLength' => $filter->settings['filter_url_length'],
  );
}
