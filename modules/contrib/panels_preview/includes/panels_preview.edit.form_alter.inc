<?php

/**
 * Implements hook_form_alter()
 */
function panels_preview_form_alter(&$form, &$form_state, $form_id) {
  /**
   * Add a custom preview to the content type edit form and style edit form.
   * This is applied in all cases, except for cases where we are customizing an
   * entity (i.e. fieldable panels panes) because we cannot reliable regenerate
   * a preview without saving.
   */
  $extra_form_ids_with_preview = array(
    'panels_edit_style_settings_form',
    'panels_edit_style_type_form',
    'ctools_entity_field_content_type_formatter_options',
    'ctools_entity_field_content_type_formatter_styles',
  );
  if (variable_get('panels_preview_live_preview', 1) && (strpos($form_id, '_content_type_edit_form') || in_array($form_id, $extra_form_ids_with_preview))) {

    $pane = $form_id == 'panels_edit_style_settings_form' ? $form_state['display']->content[$form_state['pid']] : $form_state['pane'];
    $display = $form_state['display'];
    ctools_include('plugins', 'panels');
    ctools_include('content');

    // Process stylizer settings forms to ensure previews work correctly.
    if ($form_id == 'panels_edit_style_settings_form') {

      if (!empty($form_state['style']) && is_array($form_state['style'])) {
        $form_state['style'] = $form_state['style']['name'];
      }

      if (!empty($form_state['values']['settings'])) {
        $form_state['conf'] = $form_state['values']['settings'];
      }

      if (!empty($form_state['values']['settings'])) {
        $pane->style = array(
          'settings' => $form_state['values']['settings'],
          'style' => $form_state['style'],
        );
      }
    }

    // Determine the style
    if (strpos($form_id, '_content_type_edit_form') || $form_id == 'ctools_entity_field_content_type_formatter_options') {
      $style = (isset($pane->style['style'])) ? panels_get_style($pane->style['style']) : NULL;
    }
    elseif ($form_id == 'panels_edit_style_type_form') {
      $style = ($form_state['rebuild'] && $form_id == 'panels_edit_style_type_form') ? panels_get_style($form_state['input']['style']) : panels_get_style($form_state['style']);
    }
    elseif ($form_id == 'panels_edit_style_settings_form') {
      $style = panels_get_style($form_state['style']);
    }
    else {
      $style = NULL;
    }

    // EXPERIEMENTAL: Handling Saving a Fieldable Panel Pane To Create a Live Preview. The trick
    // here is to make sure we properly save a version to get a vid, but don't make it the real one.
    // NOTE - I doubt this will be very good for performance. Let's call it RC1.
    $clicked_button = empty($form_state['triggering_element']['#value']) ? '' : $form_state['triggering_element']['#value'];
    if ($form_id == 'fieldable_panels_panes_fieldable_panels_pane_content_type_edit_form' && ($clicked_button == 'Update Preview' || ($form_state['op'] == 'add' && empty($form_state['input']['form_build_id'])))) {

      // Run entity information through standard submission form if this is a new or updated object
      // Update the pane settngs with the VID afterwards
      if (!$form_state['entity']->reusable || $form_state['op'] != 'add' || $form_state['rebuild'] == TRUE) {
        // Process the entity to create a preview.
        $default_values = array(
          'language' => '',
          'title' => '',
          'link' => '',
          'path' => '',
          'reusable' => '',
          'category' => '',
          'admin_title' => '',
          'admin_description' => '',
          'revision' => '',
        );

        if (!empty($form_state['values'])) {
          $form_state['values'] = array_merge($default_values, $form_state['values']);
        }
        else {
          $form_state['values'] = $default_values;
        }

        // Begin code from fieldable_panels_panes_entity_edit_form_submit().
        $entity = $form_state['entity'];

        // Copy hardcoded fields.
        $entity->title = $form_state['values']['title'];
        $entity->link = $form_state['values']['link'];
        $entity->path = $form_state['values']['path'];
        $entity->language = $form_state['values']['language'];
        $entity->reusable = $form_state['values']['reusable'];
        $entity->category = $form_state['values']['category'];
        $entity->admin_title = $form_state['values']['admin_title'];
        $entity->admin_description = $form_state['values']['admin_description'];

        $entity->revision = $form_state['values']['revision'];
        // Only set a log message if there was a new revision. This prevents
        // overwriting a log message on the current revision
        if ($entity->revision) {
          $entity->log = $form_state['values']['log'];
        }
        // End code from fieldable_panels_panes_entity_edit_form_submit().

        // Hide revision field.
        $form_state['values']['revision']['#access'] = FALSE;

        if (!isset($entity->fpid)) {
          $entity->fpid = NULL;
        }

        field_attach_form_validate('fieldable_panels_pane', $entity, $form, $form_state);
        field_attach_submit('fieldable_panels_pane', $entity, $form, $form_state);

        // Set the callback to use for rendering the preview.
        $preview_callback = 'panels_preview_render_fieldable_panels_pane_preview';

        // Stash the entity stub for the preview content type.
        $pane->configuration['fieldable_panels_pane'] = $entity;
      }
    }

    // Add our own submit handler ALWAYS, not just when we are generating
    // a preview.
    if ($form_id == 'fieldable_panels_panes_fieldable_panels_pane_content_type_edit_form') {
      $form['#submit'][] = '_panels_preview_fieldable_panels_panes_fieldable_panels_pane_content_type_edit_form_submit';
    }

    // Set the configuration and determine the content for the pane
    $configuration_ids = array(
      'ctools_entity_field_content_type_formatter_options',
      'ctools_entity_field_content_type_formatter_styles',
      'panels_edit_style_settings_form',
    );
    $configuration = ($form_state['rebuild'] && (strpos($form_id, 'content_type_edit_form') || in_array($form_id, $configuration_ids))) ? array_merge($pane->configuration, $form_state['input']) : $pane->configuration;

    $keywords = array();
    $args = array();
    if (!empty($form_state['contexts'])) {
      $context = $form_state['contexts'];
    }
    elseif (!empty($form_state['display']->context)) {
      $context = $form_state['display']->context;
    }
    else {
      $context = array();
    }

    // Special case for panelizer fields being edited through the IPE.
    if (!empty($pane->configuration['formatter']) && $pane->configuration['formatter'] == 'image') {
      list($entity_type, $field_name) = explode(':', $pane->subtype);
      if (!empty($form_state['entity']) && !empty($form_state['entity']->$field_name)) {
        $form_state['entity']->{$field_name}[LANGUAGE_NONE][0] = array_merge((array) file_load($form_state['entity']->{$field_name}[LANGUAGE_NONE][0]['fid']), $form_state['entity']->{$field_name}[LANGUAGE_NONE][0]);
      }

      if (!empty($configuration['image_style']) ) {
        $configuration['formatter_settings']['image_style'] = $configuration['image_style'];
        unset($configuration['image_style']);
      }

      if (!empty($configuration['image_link'])) {
        $configuration['formatter_settings']['image_link'] = $configuration['image_link'];
        unset($configuration['image_link']);
      }
    }

    // Only render preview for Panels Panes and not Regions.
    if (isset($pane)) {
      // Creates preview outside of form itself.
      $form['#post_render'][] = 'panels_preview_form_post_render_preview';
      $form['#prefix'] = panels_preview_form_preview_type_select();
      $form['#panels_preview_preview_info'] = array(
        'preview_callback' => isset($preview_callback) ? $preview_callback : NULL,
        'pane' => $pane,
        'configuration' =>  $configuration,
        'keywords' => $keywords,
        'args' => $args,
        'context' => $context,
        'style' => $style,
        'display' => $display,
      );

      // Remove the clearfix for preview floating.
      if (isset($form['aligner_start'])) {
        $form['aligner_start']['#markup'] = str_replace('clearfix', '', $form['aligner_start']['#markup']);
      }

      $preview_attributes = array(
        'class' => array(
          'widget-preview-button',
        ),
      );

      // If live previewing, don't show button, and autoupdate.
      if (variable_get('panels_preview_live_preview', 1) == 1) {
        $preview_attributes['style'] = 'display: none';
        $preview_attributes['class'][] = 'ctools-auto-submit-click';
      }

      // Create the preview refresh button
      $form['buttons']['preview'] = array(
        '#type' => 'button',
        '#value' => t('Update Preview'),
        '#attributes' => $preview_attributes,
        '#ajax' => array(
          'callback' => 'panels_preview_ajax_update_preview',
          'wrapper' => 'panels-preview-form-widget-preview',
          'path' => 'system/panels-preview',
        ),
      );

      // Autosubmit the form
      ctools_add_js('auto-submit');
      $form['#attributes']['class'][] = 'ctools-auto-submit-full-form';
      $form['#attributes']['class'][] = 'panels-preview-ctools-form';
    }
    // Convert any other #ajax enabled buttons to use system/panels-preview.
    _panels_preview_add_path_to_ajax($form);
  }

  /**
   * Modify buttons for the CTools Content Type Settings Forms.
   */
  if (strpos($form_id, 'content_type_edit_form') || $form_id == 'ctools_entity_field_content_type_formatter_options') {
    // Change 'Finish' to 'Save'
    if (!empty($form['buttons']['return'])) {
      $form['buttons']['return']['#value'] = t('Save');
    }
  }

  /**
   * Globally make sure that all options are inside a fieldset.
   * Currently just supporting a known list of forms, but persumably this can be
   * applied to all items eventually.
   */
  $forms_to_improve = array(
    'ctools_block_content_type_edit_form',
    'ctools_custom_content_type_edit_form',
    'ctools_entity_field_content_type_formatter_options',
    'ctools_entity_field_content_type_formatter_styles',
    'ctools_entity_form_field_content_type_edit_form',
    'entity_entity_view_content_type_edit_form',
    'menu_block_menu_tree_content_type_edit_form',
    'panels_preview_search_facet_content_type_edit_form',
    'panels_edit_style_settings_form',
    'panels_preview_search_search_box_content_type_edit_form',
    'views_content_views_content_type_edit_form',
  );

  drupal_alter('panels_preview_forms_to_improve', $forms_to_improve);

  if (in_array($form_id, $forms_to_improve) || fnmatch('ctools_node_*_edit_form', $form_id)) {
    // Grab child elements and create the fieldset.
    $children = element_children($form);
    if (count($children) > 1) {
      $form['general_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t('General Settings'),
        '#weight' => -30,
      );

      // Move all elements which aren't fieldsets or the submit buttons into the general settings fieldset.
      foreach ($children as $child) {
        if ((!empty($form[$child]['#type']) || !empty($form[$child]['#tree'])) && $form[$child]['#type'] != 'fieldset' && $child != 'buttons' && $child != 'submit') {
          // If the form has #tree => TRUE set, then we need to make sure the
          // submitted data has the same keys.
          if (empty($form[$child]['#parents'])) {
            $form[$child]['#parents'] = array($child);
          }
          $form['general_settings'][$child] = $form[$child];
          unset($form[$child]);
        }
      }

      // Don't show the context when it is the only one
      if (!empty($form['general_settings']['context']) && count($form['general_settings']['context']['#options']) == 1) {
        $form['general_settings']['context']['#type'] = 'value';
        $form['general_settings']['context']['#value'] = $form['general_settings']['context']['#default_value'];
      }

      // Don't show the override title text field unless we are overriding
      if (!empty($form['general_settings']['override_title_text'])) {
        $form['general_settings']['override_title_text']['#dependency_type'] = 'hide';
      }

      // Deal with cases where there is override title markup
      if (!empty($form['override_title_markup'])) {
        $form['override_title_markup']['#access'] = FALSE;
      }
    }
  }
}

/**
 * Add the preview style selector to the form output.
 */
function panels_preview_form_preview_type_select() {
  $selector = array(
    '#type' => 'select',
    '#options' => array('split' => 'Split Preview', 'edit' => 'No Preview (form only)', 'full' => 'Full Preview (no form)'),
    '#attributes' => array(
      'id' => 'panels-preview-form-preview-select-type',
      'class' => array('widget-preview', 'widget-preview-select-type'),
    ),
  );
  return drupal_render($selector);
}
