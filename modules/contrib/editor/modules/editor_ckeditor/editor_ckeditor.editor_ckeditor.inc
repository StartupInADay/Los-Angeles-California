<?php

/**
 * @file
 * Editor CKEditor hooks implemented by the Editor CKEditor module.
 */

/**
 * Implements hook_editor_ckeditor_plugins().
 */
function editor_ckeditor_editor_ckeditor_plugins() {
  $image_prefix = drupal_get_path('module', 'editor_ckeditor') . '/images/buttons/';

  $buttons = array(
    'Bold' => array(
      'label' => t('Bold'),
      'required_html' => array(array('tags' => array('strong'))),
    ),
    'Italic' => array(
      'label' => t('Italic'),
      'required_html' => array(array('tags' => array('em'))),
    ),
    'Underline' => array(
      'label' => t('Underline'),
      // A class is used on spans for underline.
      'required_html' => array(array('tags' => array('span'))),
    ),
    'Strike' => array(
      'label' => t('Strike-through'),
      'required_html' => array(array('tags' => array('del'))),
    ),
    'JustifyLeft' => array(
      'label' => t('Align left'),
      'required_html' => array(array('tags' => array('p'), 'classes' => array('text-align-left'))),
    ),
    'JustifyCenter' => array(
      'label' => t('Align center'),
      'required_html' => array(array('tags' => array('p'), 'classes' => array('text-align-center'))),
    ),
    'JustifyRight' => array(
      'label' => t('Align right'),
      'required_html' => array(array('tags' => array('p'), 'classes' => array('text-align-right'))),
    ),
    'JustifyBlock' => array(
      'label' => t('Justify'),
      'required_html' => array(array('tags' => array('p'), 'classes' => array('text-align-justify'))),
    ),
    'BulletedList' => array(
      'label' => t('Bullet list'),
      'image_rtl' => $image_prefix . '/bulletedlist-rtl.png',
      'required_html' => array(array('tags' => array('ul', 'li'))),
    ),
    'NumberedList' => array(
      'label' => t('Numbered list'),
      'image_rtl' => $image_prefix . '/numberedlist-rtl.png',
      'required_html' => array(array('tags' => array('ol', 'li'))),
    ),
    'Undo' => array(
      'label' => t('Undo'),
      'image_rtl' => $image_prefix . '/undo-rtl.png',
    ),
    'Redo' => array(
      'label' => t('Redo'),
      'image_rtl' => $image_prefix . '/redo-rtl.png',
    ),
    'Anchor' => array(
      'image_rtl' => $image_prefix . '/anchor-rtl.png',
      'label' => t('Anchor'),
      'required_html' => array(array(
        'tags' => array('a'),
        'attributes' => array('href', 'title', 'name', 'id'),
      )),
    ),
    'Superscript' => array(
      'label' => t('Superscript'),
      'required_html' => array(array('tags' => array('sup'))),
    ),
    'Subscript' => array(
      'label' => t('Subscript'),
      'required_html' => array(array('tags' => array('sub'))),
    ),
    'Blockquote' => array(
      'label' => t('Blockquote'),
      'required_html' => array(array('tags' => array('blockquote'))),
    ),
    'Source' => array(
      'label' => t('Source code'),
    ),
    'HorizontalRule' => array(
      'label' => t('Horizontal rule'),
      'required_html' => array(array('tags' => array('hr'))),
    ),
    'Cut' => array(
      'label' => t('Cut'),
    ),
    'Copy' => array(
      'label' => t('Copy'),
    ),
    'Paste' => array(
      'label' => t('Paste'),
    ),
    'PasteText' => array(
      'label' => t('Paste Text'),
      'image_rtl' => $image_prefix . '/pastetext-rtl.png',
    ),
    'PasteFromWord' => array(
      'label' => t('Paste from Word'),
      'image_rtl' => $image_prefix . '/pastefromword-rtl.png',
    ),
    'ShowBlocks' => array(
      'label' => t('Show blocks'),
      'image_rtl' => $image_prefix . '/showblocks-rtl.png',
    ),
    'RemoveFormat' => array(
      'label' => t('Remove format'),
    ),
    'SpecialChar' => array(
      'label' => t('Character map'),
    ),
    'Format' => array(
      'label' => t('HTML block format'),
      'image_alternative' => '<span class="ckeditor-button-dropdown">' . t('Format') . '<span class="ckeditor-button-arrow"></span></span>',
    ),
    'Styles' => array(
      'label' => t('Font style'),
      'image_alternative' => '<span class="ckeditor-button-dropdown">' . t('Styles') . '<span class="ckeditor-button-arrow"></span></span>',
    ),
    'Table' => array(
      'label' => t('Table'),
      'required_html' => array(array('tags' => array('table', 'thead', 'tbody', 'tr', 'td', 'th'))),
    ),
    'Maximize' => array(
      'label' => t('Maximize'),
    ),
    '-' => array(
      'label' => t('Separator'),
      'image_alternative' => '<span class="ckeditor-separator" title="' . t('Button separator') . '" aria-label="' . t('Button separator') . '">&nbsp;</span>',
      'attributes' => array('class' => array('ckeditor-button-separator')),
      'multiple' => TRUE,
    ),
  );

  // Populate image locations that match button names.
  foreach ($buttons as $button_name => &$button) {
    if (!isset($button['image_alternative']) && !isset($button['image'])) {
      // Because button names are ASCII text, drupal_strtolower() is not needed.
      $button['image'] = $image_prefix . strtolower($button_name) . '.png';
    }
  }

  // List all the basic plugin buttons as an "internal" plugin.
  $plugins['default'] = array(
    'buttons' => $buttons,
    'internal' => TRUE,
  );

  // The drupalimage plugin replaces normal image functionality.
  $plugins['drupalimage'] = array(
    'path' => drupal_get_path('module', 'editor_ckeditor') . '/js/plugins/drupalimage',
    'file' => 'plugin.js',
    'buttons' => array(
      'DrupalImage' => array(
        'label' => t('Image'),
        'required_html' => array(
          array(
            'tags' => array('img'),
            'attributes' => array('src', 'alt', 'data-entity-type', 'data-entity-id', 'data-align'),
          ),
        ),
        'image' => $image_prefix . '/image.png',
      ),
    ),
  );

  // The drupalcaption plugin provides consistent behaviors for image captions.
  $plugins['drupalimagecaption'] = array(
    'path' => drupal_get_path('module', 'editor_ckeditor') . '/js/plugins/drupalimagecaption',
    'file' => 'plugin.js',
    'enabled callback' => 'editor_ckeditor_image_plugin_check',
    'required_html' => array(
      array(
        'tags' => array('img'),
        'attributes' => array('data-caption'),
      ),
      array(
        'tags' => array('figure', 'figcaption'),
        'attributes' => array('src', 'alt', 'class', 'data-placeholder'),
      ),
    ),
  );

  // The drupallink plugin replaces normal link functionality.
  $plugins['drupallink'] = array(
    'path' => drupal_get_path('module', 'editor_ckeditor') . '/js/plugins/drupallink',
    'file' => 'plugin.js',
    'buttons' => array(
      'DrupalLink' => array(
        'label' => t('Link'),
        'required_html' => array(array(
          'tags' => array('a'),
          'attributes' => array('href'),
        )),
        'image' => $image_prefix . '/link.png',
      ),
      'DrupalUnlink' => array(
        'label' => t('Link'),
        'required_html' => array(array(
          'tags' => array('a'),
          'attributes' => array('href'),
        )),
        'image' => $image_prefix . '/unlink.png',
      ),
    ),
  );

  return $plugins;
}

/**
 * Implements hook_editor_ckeditor_css_alter().
 */
function editor_ckeditor_editor_ckeditor_css_alter(array &$css, $format) {
  // Add the editor caption CSS if the text format associated with this text
  // editor uses the editor_caption filter. This is used by the included
  // CKEditor DrupalImageCaption plugin.
  if (isset($format->filters['editor_caption']) && $format->filters['editor_caption']->status) {
    $css[] = drupal_get_path('module', 'editor') . '/css/filter/filter.caption.css';
  }

  // Add the editor align CSS if the text format associated with this text
  // editor uses the editor_align filter. This is used by the included
  // CKEditor DrupalImage plugin.
  if (isset($format->filters['editor_align']) && $format->filters['editor_align']->status) {
    $css[] = drupal_get_path('module', 'editor_ckeditor') . '/css/plugins/drupalimagecaption/editor_ckeditor.drupalimagecaption.css';
  }
}

/**
 * Subform constructor to configure the text editor's image upload settings.
 *
 * Each text editor plugin that is configured to offer the ability to insert
 * images and uses EditorImageDialog for that, should use this form to update
 * the text editor's configuration so that EditorImageDialog knows whether it
 * should allow the user to upload images.
 *
 * @param object $format
 *   The text format that is being edited.
 *
 * @return array
 *   The image upload settings form.
 *
 * @ingroup forms
 */
function editor_ckeditor_image_upload_settings_form($format) {
  // Defaults.
  $settings = isset($format->editor_settings['image_upload']) ? $format->editor_settings['image_upload'] : array();

  $settings += array(
    'status' => FALSE,
    'scheme' => file_default_scheme(),
    'directory' => 'inline-images',
    'max_size' => '',
    'max_dimensions' => array('width' => '', 'height' => ''),
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable image uploads'),
    '#default_value' => $settings['status'],
    '#attributes' => array(
      'data-editor-image-upload' => 'status',
    ),
  );

  $show_if_image_uploads_enabled = array(
    'visible' => array(
      ':input[data-editor-image-upload="status"]' => array('checked' => TRUE),
    ),
  );

  // Any visible, writable wrapper can potentially be used for uploads,
  // including a remote file system that integrates with a CDN.
  $stream_wrappers = file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE);

  foreach ($stream_wrappers as $scheme => $info) {
    $options[$scheme] = $info['description'];
  }

  if (!empty($options)) {
    $form['scheme'] = array(
      '#type' => 'radios',
      '#title' => t('File storage'),
      '#default_value' => $settings['scheme'],
      '#options' => $options,
      '#states' => $show_if_image_uploads_enabled,
      '#access' => count($options) > 1,
    );
  }

  // Set data- attributes with human-readable names for all possible stream
  // wrappers, so that drupal.ckeditor.drupalimage.admin's summary rendering
  // can use that.
  foreach ($stream_wrappers as $scheme => $info) {
    $form['scheme'][$scheme]['#attributes']['data-label'] = t('Storage: @name', array('@name' => $info['name']));
  }

  $form['directory'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['directory'],
    '#title' => t('Upload directory'),
    '#description' => t("A directory relative to Drupal's files directory where uploaded images will be stored."),
    '#states' => $show_if_image_uploads_enabled,
  );

  $default_max_size = format_size(file_upload_max_size());

  $form['max_size'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['max_size'],
    '#title' => t('Maximum file size'),
    '#description' => t('If this is left empty, then the file size will be limited by the PHP maximum upload size of @size.', array('@size' => $default_max_size)),
    '#maxlength' => 20,
    '#size' => 10,
    '#placeholder' => $default_max_size,
    '#states' => $show_if_image_uploads_enabled,
  );

  $form['max_dimensions'] = array(
    '#type' => 'item',
    '#title' => t('Maximum dimensions'),
    '#field_prefix' => '<div class="container-inline clearfix">',
    '#field_suffix' => '</div>',
    '#description' => t('Images larger than these dimensions will be scaled down.'),
    '#states' => $show_if_image_uploads_enabled,
  );
  $form['max_dimensions']['width'] = array(
    '#title' => t('Width'),
    '#title_display' => 'invisible',
    '#type' => module_exists('elements') ? 'numberfield' : 'textfield',
    '#default_value' => $settings['max_dimensions']['width'],
    '#size' => 8,
    '#maxlength' => 8,
    '#min' => 1,
    '#max' => 99999,
    '#placeholder' => 'width',
    '#field_suffix' => ' x ',
    '#states' => $show_if_image_uploads_enabled,
  );
  $form['max_dimensions']['height'] = array(
    '#title' => t('Height'),
    '#title_display' => 'invisible',
    '#type' => module_exists('elements') ? 'numberfield' : 'textfield',
    '#default_value' => $settings['max_dimensions']['height'],
    '#size' => 8,
    '#maxlength' => 8,
    '#min' => 1,
    '#max' => 99999,
    '#placeholder' => 'height',
    '#field_suffix' => 'pixels',
    '#states' => $show_if_image_uploads_enabled,
  );

  return $form;
}
