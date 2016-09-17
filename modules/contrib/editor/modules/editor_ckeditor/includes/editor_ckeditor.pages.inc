<?php

/**
 * @file
 * Page callbacks for the Editor CKEditor module.
 */

/**
 * Form callback: Display a form for inserting/editing a link.
 */
function editor_ckeditor_link_dialog_form($form, &$form_state, $format) {
  editor_format_ensure_additional_properties($format);

  $values = isset($form_state['values']) ? $form_state['values'] : array();
  $input = $_POST;

  $link_element = empty($values['attributes']) ? array() : $values['attributes'];

  // The default values are set directly from \Drupal::request()->request,
  // provided by the editor plugin opening the dialog.
  if (empty($form_state['link_element'])) {
    $form_state['link_element'] = isset($input['editor_object']) ? $input['editor_object'] : array();
  }

  $link_element += $form_state['link_element'];
  $link_element += array(
    'data-entity-type' => '',
    'data-entity-id' => '',
    'data-entity-label' => '',
    'href' => '',
  );

  // Set the dialog title.
  if (!empty($link_element['href'])) {
    drupal_set_title(t('Edit link'));
  }
  else {
    drupal_set_title(t('Insert link'));
  }

  $form['#tree'] = TRUE;
  $form['#prefix'] = '<div id="editor-ckeditor-link-dialog-form">';
  $form['#suffix'] = '</div>';

  global $user;

  $options = array();
  $entity_types = entity_get_info();

  foreach ($entity_types as $entity_type => $info) {
    if (!empty($info['uri callback']) && entity_access('view', $entity_type, NULL, $user)) {
      $options[$entity_type] = $info['label'];
    }
  }

  $form['attributes']['data-entity-type'] = array(
    '#type' => 'select',
    '#title' => t('Entity type'),
    '#default_value' => $link_element['data-entity-type'],
    '#access' => count($options) > 1,
    '#options' => $options,
    '#empty_option' => t('- Select an entity type -'),
    '#description' => t('Optionally create links to existing entities by selecting an entity type.'),
    '#ajax' => array(
      'callback' => 'editor_ckeditor_link_dialog_form_href_update',
    ),
  );

  $default = '';
  $entity = FALSE;
  if (!empty($link_element['data-entity-type']) && !empty($link_element['data-entity-uuid']) && module_exists('uuid')) {
    $entity = entity_uuid_load($link_element['data-entity-type'], array($link_element['data-entity-uuid']));
    $entity = reset($entity);
  }
  elseif(!empty($link_element['data-entity-type']) && !empty($link_element['data-entity-id'])) {
    $entity = entity_load_single($link_element['data-entity-type'], $link_element['data-entity-id']);
  }

  if ($entity) {
    $default = entity_label($link_element['data-entity-type'], $entity);
  }

  $form['attributes']['data-entity-id'] = array(
    '#title' => t('Entity'),
    '#type' => !empty($link_element['data-entity-type']) ? 'textfield' : 'value',
    '#default_value' => $default,
    '#autocomplete_path' => 'editor-ckeditor/autocomplete-entity/' . $link_element['data-entity-type'] . '/' . NULL,
    '#prefix' => '<div id="editor-ckeditor-link-entity-id">',
    '#suffix' => '</div>',
  );

  // Everything under the "attributes" key is merged directly into the
  // generated link tag's attributes.
  $form['attributes']['href'] = array(
    '#title' => t('URL'),
    '#type' => empty($link_element['data-entity-type']) ? 'textfield' : 'value',
    '#default_value' => isset($link_element['href']) ? $link_element['href'] : '',
    '#maxlength' => 2048,
    '#prefix' => '<div id="editor-ckeditor-link-href">',
    '#suffix' => '</div>',
  );

  $form['attributes']['data-entity-label'] = array(
    '#type' => 'value',
    '#value' => $link_element['data-entity-label'],
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    // No regular submit-handler. This form only works via JavaScript.
    '#submit' => array(),
    '#ajax' => array(
      'callback' => 'editor_ckeditor_link_dialog_save',
      'event' => 'click',
    ),
    '#attributes' => array(
      'class' => array('button--primary'),
    ),
  );

  return $form;
}

/**
 * Re-renders the href form element when the entity type selection changes.
 *
 * This function is called via Ajax when the selected entity type is changed on
 * the insert/edit link form.
 *
 * @return
 *   The rendered parent page select element.
 */
function editor_ckeditor_link_dialog_form_href_update($form, $form_state) {
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#editor-ckeditor-link-entity-id", render($form['attributes']['data-entity-id'])),
      ajax_command_replace("#editor-ckeditor-link-href", render($form['attributes']['href']))
    ),
  );
}

/**
 * Form AJAX callback. Sends the save editor AJAX command and closes the dialog.
 *
 * @see filter_format_editor_link_form()
 * @see filter_format_editor_image_form()
 */
function editor_ckeditor_link_dialog_save($form, &$form_state) {
  $commands = array();

  $values = $form_state['values'];

  if ($entity_type = $values['attributes']['data-entity-type']) {
    $title = trim($values['attributes']['data-entity-id']);

    $entity_info = entity_get_info($entity_type);

    // Prevent errors if the entity type has no label key.
    if (empty($entity_info['entity keys']['label'])) {
      form_set_error('data-entity-id', t('Unable to find label for @type entity @id.', array('@type' => $entity_type, '@id' => $title)));
    }

    // The entity ID may be either the ID (integer) of the entity or the
    // entity's title (string).
    if (is_numeric($title)) {
      $entities = entity_load($entity_type, array($title));
    }
    else {
      $entities = entity_load($entity_type, FALSE, array($entity_info['entity keys']['label'] => array($title)));
    }

    $entity = reset($entities);

    if (!empty($entity)) {
      list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

      if (!entity_access('view', $entity_type, $entity)) {
        form_set_error('data-entity-id', t('Unable to access @type entity @id.', array('@type' => $entity_type, '@id' => $title)));
      }
      else {
        $label = entity_label($entity_type, $entity);

        $form_state['values']['attributes']['data-entity-id'] = $id;
        $form_state['values']['attributes']['data-entity-label'] = $label;

        $uri = entity_uri($entity_type, $entity);
        $url = $uri && !empty($uri['path']) ? url($uri['path'], $uri['options']) : FALSE;

        $form_state['values']['attributes']['href'] = $url;
      }
    }
    else {
      form_set_error('data-entity-id', t('Unable to load @type entity @id.', array('@type' => $entity_type, '@id' => $title)));
    }
  }
  else {
    $form_state['values']['attributes']['data-entity-label'] = $values['attributes']['href'];
  }

  if (form_get_errors()) {
    unset($form['#prefix'], $form['#suffix']);
    $status_messages = array('#theme' => 'status_messages');
    $output = drupal_render($form);
    $output = '<div>' . drupal_render($status_messages) . $output . '</div>';
    $commands[] = ajax_command_html('#editor-ckeditor-link-dialog-form', $output);
  }
  else {
    $commands[] = editor_command_editor_dialog_save($form_state['values']);
    $commands[] = dialog_command_close_modal_dialog();
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Form callback: Display a form for inserting/editing a link.
 */
function editor_ckeditor_image_dialog_form($form, &$form_state, $format) {
  editor_format_ensure_additional_properties($format);

  // This form is special, in that the default values do not come from the
  // server side, but from the client side, from a text editor. We must cache
  // this data in form state, because when the form is rebuilt, we will be
  // receiving values from the form, instead of the values from the text
  // editor. If we don't cache it, this data will be lost.
  if (isset($form_state['input']['editor_object'])) {
    // By convention, the data that the text editor sends to any dialog is in
    // the 'editor_object' key. And the image dialog for text editors expects
    // that data to be the attributes for an <img> element.
    $image_element = $form_state['input']['editor_object'];
    $form_state['image_element'] = $image_element;
  }
  else {
    // Retrieve the image element's attributes from form state.
    $image_element = $form_state['image_element'] ? $form_state['image_element'] : array();
  }

  // Set the dialog title.
  if (!empty($image_element['src'])) {
    drupal_set_title(t('Edit image'));
  }
  else {
    drupal_set_title(t('Insert image'));
  }

  $form['#tree'] = TRUE;
  $form['#prefix'] = '<div id="editor-ckeditor-image-dialog-form">';
  $form['#suffix'] = '</div>';

  // Construct strings to use in the upload validators.
  $upload_settings = isset($format->editor_settings['image_upload']) ? $format->editor_settings['image_upload'] : array();
  $upload_settings += array(
    'status' => 0,
    'dimensions' => array('max_width' => '', 'max_height' => ''),
    'max_size' => NULL,
    'scheme' => 'public',
    'directory' => 'inline-images',
  );

  if (!empty($upload_settings['max_dimensions']['width']) && !empty($upload_settings['max_dimensions']['height'])) {
    $max_dimensions = $upload_settings['max_dimensions']['width'] . 'x' . $upload_settings['max_dimensions']['height'];
  }
  else {
    $max_dimensions = 0;
  }

  $max_filesize = !empty($upload_settings['max_size']) ? min(parse_size($upload_settings['max_size']), file_upload_max_size()) : file_upload_max_size();
  $existing_file = !empty($image_element['data-entity-id']) ? file_load($image_element['data-entity-id']) : NULL;
  $fid = $existing_file ? $existing_file->fid : NULL;

  $form['fid'] = array(
    '#title' => t('Image'),
    '#type' => 'managed_file',
    '#upload_location' => $upload_settings['scheme'] . '://' . $upload_settings['directory'],
    '#default_value' => $fid ? $fid : NULL,
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
      'file_validate_size' => array($max_filesize),
      'file_validate_image_resolution' => array($max_dimensions),
    ),
    '#required' => TRUE,
  );

  $form['attributes']['src'] = array(
    '#title' => t('URL'),
    '#type' => 'textfield',
    '#default_value' => isset($image_element['src']) ? $image_element['src'] : '',
    '#maxlength' => 2048,
    '#required' => TRUE,
  );

  // If the editor has image uploads enabled, show a managed_file form item,
  // otherwise show a (file URL) text form item.
  if ($upload_settings['status']) {
    $form['attributes']['src']['#access'] = FALSE;
    $form['attributes']['src']['#required'] = FALSE;
  }
  else {
    $form['fid']['#access'] = FALSE;
    $form['fid']['#required'] = FALSE;
  }

  // The alt attribute is *required*, but we allow users to opt-in to empty
  // alt attributes for the very rare edge cases where that is valid by
  // specifying two double quotes as the alternative text in the dialog.
  // However, that *is* stored as an empty alt attribute, so if we're editing
  // an existing image (which means the src attribute is set) and its alt
  // attribute is empty, then we show that as two double quotes in the dialog.
  // @see https://www.drupal.org/node/2307647
  $alt = isset($image_element['alt']) ? $image_element['alt'] : '';

  if ($alt === '' && !empty($image_element['src'])) {
    $alt = '""';
  }

  $form['attributes']['alt'] = array(
    '#title' => t('Alternative text'),
    '#attributes' => array(
      'placeholder' => t('Short description for the visually impaired'),
    ),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => t('Alternative text is required.<br />(Only in rare cases should this be left empty. To create empty alternative text, enter <code>""</code> — two double quotes without any content).'),
    '#default_value' => $alt,
    '#parents' => array('attributes', 'alt'),
    '#maxlength' => 2048,
  );

  // When Drupal core's filter_align is being used, the text editor may
  // offer the ability to change the alignment.
  if (isset($image_element['data-align']) && $format->filters['editor_align']->status) {
    $form['align'] = array(
      '#title' => t('Align'),
      '#type' => 'radios',
      '#options' => array(
        'none' => t('None'),
        'left' => t('Left'),
        'center' => t('Center'),
        'right' => t('Right'),
      ),
      '#default_value' => $image_element['data-align'] === '' ? 'none' : $image_element['data-align'],
      '#attributes' => array('class' => array('container-inline')),
      '#parents' => array('attributes', 'data-align'),
    );
  }

  // When Drupal core's filter_caption is being used, the text editor may
  // offer the ability to in-place edit the image's caption: show a toggle.
  if (isset($image_element['hasCaption']) && $format->filters['editor_caption']->status) {
    $form['caption'] = array(
      '#title' => t('Caption'),
      '#type' => 'checkbox',
      '#default_value' => $image_element['hasCaption'] === 'true',
      '#parents' => array('attributes', 'hasCaption'),
    );
  }

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    // No regular submit-handler. This form only works via JavaScript.
    '#submit' => array(),
    '#ajax' => array(
      'callback' => 'editor_ckeditor_image_dialog_save',
      'event' => 'click',
    ),
    '#attributes' => array(
      'class' => array('button--primary'),
    ),
  );

  return $form;
}

/**
 * Form AJAX callback. Sends the save editor AJAX command and closes the dialog.
 *
 * @see filter_format_editor_link_form()
 * @see filter_format_editor_image_form()
 */
function editor_ckeditor_image_dialog_save($form, &$form_state) {
  $commands = array();

  // Convert any uploaded files from the FID values to data-entity-uuid
  // attributes and set data-entity-type to 'file'.
  $fid = isset($form_state['values']['fid']) ? $form_state['values']['fid'] : 0;

  if (!empty($fid)) {
    $file = file_load($fid);

    // Transform absolute image URLs to relative image URLs: prevent problems
    // on multisite set-ups and prevent mixed content errors.
    $scheme = file_uri_scheme($file->uri);

    if ($scheme && file_stream_wrapper_valid_scheme($scheme)) {
      $wrapper = file_stream_wrapper_get_instance_by_scheme($scheme);
      // Transform absolute image URLs to relative image URLs: prevent problems
      // on multisite set-ups and prevent mixed content errors.
      $file_url = base_path() . $wrapper->getDirectoryPath() . '/' . file_uri_target($file->uri);

      $form_state['values']['attributes']['src'] = $file_url;
      $form_state['values']['attributes']['data-entity-id'] = $fid;
      $form_state['values']['attributes']['data-entity-type'] = 'file';
    }
  }

  // When the alt attribute is set to two double quotes, transform it to the
  // empty string: two double quotes signify "empty alt attribute". See above.
  if (trim($form_state['values']['attributes']['alt']) === '""') {
    $form_state['attributes']['alt'] = '';
  }

  if (form_get_errors()) {
    unset($form['#prefix'], $form['#suffix']);
    $status_messages = array('#theme' => 'status_messages');
    $output = drupal_render($form);
    $output = '<div>' . drupal_render($status_messages) . $output . '</div>';
    $commands[] = ajax_command_html('#editor-ckeditor-image-dialog-form', $output);
  }
  else {
    $commands[] = editor_command_editor_dialog_save($form_state['values']);
    $commands[] = dialog_command_close_modal_dialog();
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}
