<?php

/**
 * Implementation of hook_form_FORM_ID_alter()
 *
 * Provides customization to entity formatter option form.
 */
function panels_preview_form_ctools_entity_field_content_type_formatter_options_alter(&$form, &$form_state, $form_id) {

  // Get the configuration
  $conf = $form_state['conf'];

  // Move the title for the label and formatter to be the field prefix
  foreach (array('label', 'formatter') as $field) {
    if (!empty($form['general_settings'][$field])) {
      $form['general_settings'][$field]['#field_prefix'] = $form['general_settings'][$field]['#title'];
      $form['general_settings'][$field]['#title'] = '';
    }
  }

  // Add in the field edit fields with FAPE if we are not editing a default
  if (empty($form_state['entity']) && !empty($form_state['display']->context['panelizer']->data)) {
    // Determine what entity type and field name we are editing based on the form subtype and URL args.
    $arg_parts = explode(':', arg(4));
    list($entity_type, $field_name) = explode(':', $form_state['subtype_name']);
    $entity = reset(entity_load($entity_type, array($arg_parts[2])));

    // Unfortunately the above code is not as reliable as we'd like. There are cases where the field we're editing
    // doesn't have an corrosponding entity (say, comments). Let's make sure we have an entity before adding FAPE specific
    // data into the form.
    if (!empty($entity)) {
      $form_state['entity'] = $entity;
      $form_state['entity_type'] = $entity_type;
      $form_state['field_name'] = $field_name;
      $form_state['bundle'] = empty($entity->bundle) ? $entity->type : $entity->bundle;
      $form_state['field_instance'] = field_info_instance($entity_type, $field_name, $form_state['bundle']);
      $form_state['langcode'] = LANGUAGE_NONE;
      $form_state['subform_id'] = 'fape_field_edit_field_form';
    }
  }

  // Ensure form id, form token, and form build id are on the root of the form.
  // Image fields in particular have issues with this.
  $form['form_id'] = $form['general_settings']['form_id'];
  $form['form_token'] = $form['general_settings']['form_token'];
  $form['form_build_id'] = $form['general_settings']['form_build_id'];
  unset($form['general_settings']['form_id']);
  unset($form['general_settings']['form_token']);
  unset($form['general_settings']['form_build_id']);

  if (!empty($form_state['entity'])) {
    // Add the field edit form.
    fape_field_edit_field_form($form, $form_state);

    // Remove the default submitter in favor of our own custom submit callback.
    array_pop($form['#submit']);
    $form['#submit'][] = 'panels_preview_fape_submit';
  }
  // Pre-rende fields into a fieldset.
  $form['#pre_render'][] = 'panels_preview_ctools_entity_pre_render';

  // When moving backwards through form steps, ensure image widgets properly retain their value.
  if (!empty($form_state['field_instance']) && $form_state['triggering_element']['#value'] == 'Back') {
    $field_name = $form_state['field_instance']['field_name'];
    $field_type = $form_state['field_instance']['widget']['module'];
    $entity = $form_state['entity'];

    // Reset the #default_value to the (currently) unsaved entity value.
    if ($field_type == 'image' && !empty($form[$field_name])) {
      $field = $entity->$field_name;
      foreach ($field[LANGUAGE_NONE] as $delta => $value) {
        $form[$field_name][LANGUAGE_NONE][$delta]['#default_value'] = $value;
      }
    }
  }

  // Enable auto submit functionality
  if (variable_get('panels_preview_live_preview', 1)) {
    panels_preview_autosubmit_configure($form);
  }

  // Add a custom submit handler to our preview and submit option
  $form['#submit'][] = 'panels_preview_ctools_content_type_modal_submit';
  $form['buttons']['preview']['#submit'][] = 'panels_preview_ctools_content_type_modal_submit';
}

/**
 * Implementation of hook_form_FORM_ID_alter()
 *
 * Provides customization to rendering entity fields as panes.
 */
function panels_preview_form_ctools_entity_field_content_type_formatter_styles_alter(&$form, &$form_state, $form_id) {
  // Push the panels preview submitter onto the front of the submit array.
  // We need it at the beginning so the entity is saved before the IPE
  // reloads pane content.
  if (!empty($form_state['entity'])) {
    $form['#submit'][] = 'panels_preview_fape_submit';
  }
}

/**
 * Implement the "Content Settings" fieldset in a pre-render. This fixes issues with image caused by initially
 * doing this in a form_alter.
 *
 * @see http://drupal.org/node/1567704
 */
function panels_preview_ctools_entity_pre_render($element) {
  $exclude_list = array(
    'form_id',
    'form_token',
    'form_build_id',
    'buttons',
  );

  // Add any remaining fields to the content settings fieldset.
  foreach (element_children($element) as $key) {
    $value = $element[$key];
    if (!in_array($key, $exclude_list) && !empty($value['#type']) && $value['#type'] != 'fieldset') {
      if (empty($element['content_settings'])) {
        // Add a content settings fieldset.
        $element['content_settings'] = array(
          '#type' => 'fieldset',
          '#title' => t('Content Settings'),
          '#weight' => 1,
        );
      }

      $element['content_settings'][$key] = $value;
      unset($element[$key]);
    }
  }

  return $element;
}

/**
 * Custom submit callback for FAPE enabled stylizer forms.
 */
function panels_preview_fape_submit(&$form, &$form_state) {
  // If there isn't a next step save the entity.
  if ((empty($form_state['triggering_element']['#next']) || !empty($form_state['field']['body'])) && !empty($form_state['entity']) && !empty($form_state['display_cache'])) {
    entity_save($form_state['entity_type'], $form_state['entity']);
    $form_state['display_cache']->display->context['panelizer']->data = $form_state['entity'];
  }
  // Otherwise we're on step 1, so just run the FAPE submitter.
  else {
    fape_field_edit_field_form_submit($form, $form_state);
  }
}
