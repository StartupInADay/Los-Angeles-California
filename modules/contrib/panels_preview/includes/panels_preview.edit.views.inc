<?php

/**
 * Implements hook_views_pre_view()
 */
function panels_preview_views_pre_view(&$view) {
  if (isset($view->display_handler->options['pane_conf'])) {
    $conf = $view->display_handler->options['pane_conf'];

    if (isset($conf['widget_title'])) {
      $view->display_handler->options['defaults']['title'] = FALSE;
      $view->display_handler->options['title'] = $conf['widget_title'];
      $view->build_info['title'] = $conf['widget_title'];
    }

    // Deal with legacy 'nodes' and others (such as 'files') view settings so
    // that other entity types can be included.
    if (!empty($conf['view_settings'])) {
      $conf['view_settings'] = panels_preview_convert_view_settings($conf['view_settings']);
    }

    // Set the style plugin to a table style.
    // Determine that this was previously a field view, which has been overridden to a node view in the pane config.
    if (!empty($conf['view_settings']) && $conf['view_settings'] == 'rendered_entity') {
      $view->display_handler->options['defaults']['row_plugin'] = FALSE;
      $view->display_handler->options['row_plugin'] = 'entity';
    }
    elseif (!empty($conf['view_settings']) && $conf['view_settings'] == 'table') {
      // Find the currently active field defination, else break out as table
      // needs fields.
      if (empty($view->display_handler->options['defaults']['fields']) && isset($view->display_handler->options['fields'])) {
        $fields = &$view->display_handler->options['fields'];
      }
      elseif (!empty($view->display_handler->default_display->options['fields'])) {
        $fields = &$view->display_handler->default_display->options['fields'];
      }
      else {
        // If no fields, don't try to display as table.
        return;
      }

      $view->display_handler->options['defaults']['style_plugin'] = FALSE;
      $view->display_handler->options['style_plugin'] = 'table';

      // Set or remove header labels depending on user selection.
      $use_header_titles = !empty($conf['header_type']) && $conf['header_type'] == 'titles';
      foreach ($fields as $field_key => &$field) {
        if ($use_header_titles && !empty($field['ui_name']) && empty($field['label'])) {
          $field['label'] = $field['ui_name'];
        }
        elseif (!$use_header_titles) {
          $field['label'] = '';
        }
        // Hide empty columns.
        if (!empty($view->display_handler->options['row_plugin']['hide_empty'])) {
          $view->display_handler->options['style_options'][$field_key]['empty_column'] = TRUE;
        }
      }
    }
    if ((empty($conf['view_settings']) || $conf['view_settings'] == 'rendered_entity') && !empty($conf['view_mode'])) {
      // Transfer over the row options from default if set to use.
      if (!empty($view->display_handler->options['defaults']['row_options'])) {
        $view->display_handler->options['defaults']['row_options'] = FALSE;
        $view->display_handler->options['row_options'] = $view->display_handler->default_display->options['row_options'];
      }
      $view->display_handler->options['row_options']['view_mode'] = $conf['view_mode'];
    }
  }
}

/**
 * Convert with legacy 'nodes' and others (such as 'files') view settings.
 */
function panels_preview_convert_view_settings($view_settings) {
  // The 'fields' and 'table' view settings apply to any entity type.
  if (in_array($view_settings, array('fields', 'table'))) {
    return $view_settings;
  }

  // We convert other view settings to 'rendered_entity' (which could be
  // 'node' or 'files' or others that are specific to an entity type).
  return 'rendered_entity';
}

/**
 * Implementation of hook_form_FORM_ID_alter()
 *
 * Provides customizations to the views content type settings form
 */
function panels_preview_form_views_content_views_panes_content_type_edit_form_alter(&$form, &$form_state, $form_id) {

  // Get the configuration
  $conf = $form_state['conf'];

  // Add a widget title setting if views allows this to be customized
  if (!empty($form['override_title'])) {
    $form['override_title_markup']['#access'] = FALSE;
    $form['override_title']['#access'] = FALSE;
    $form['override_title_text']['#access'] = FALSE;
    $form['override_title_heading']['#access'] = FALSE;
    $form['exposed']['widget_title'] = array(
      '#field_prefix' => t('Title'),
      '#type' => 'textfield',
      '#size' => '',
      '#weight' => -100,
      '#states' => array(
        'visible' => array(
          ':input[name="override_title"]' => array('value' => '1'),
        ),
      ),
      '#default_value' => (isset($conf['widget_title'])) ? $conf['widget_title'] : $form_state['view']->display_handler->options['title'],
      '#parents' => array('widget_title'),
    );
  }

  // Add an option to make the widget title a link if views allows this to happen
  if (!empty($form['link_to_view']) && !empty($form['path'])) {
    $form['link_to_view']['#title'] = t('Make title a link');
    $form['link_to_view']['#description'] =  '';
    $form['link_to_view']['#id'] = 'edit-link';
    $form['exposed']['link_to_view'] = $form['link_to_view'];
    $form['exposed']['link_to_view']['#parents'] = array('link_to_view');
    unset($form['link_to_view']);
  }

  // Add an option to make the more link available if views allows this to happen
  if (!empty($form['more_link'])) {
    $form['more_link']['#id'] = 'more-link';
    $form['more_link']['#description'] = '';
    $form['exposed']['more_link'] = $form['more_link'];
    $form['exposed']['more_link']['#parents'] = array('more_link');
    unset($form['more_link']);
  }


  // Update the field settings for pagers
  if (!empty($form['use_pager']) && !empty($form['pager_id'])) {
    $form['use_pager']['#prefix'] = '<div class="form-item container-inline">';
    $form['exposed']['use_pager'] = $form['use_pager'];
    $form['exposed']['use_pager']['#parents'] = array('use_pager');
    $form['exposed']['pager_id'] = $form['pager_id'];
    $form['exposed']['pager_id']['#parents'] = array('pager_id');
    unset($form['use_pager']);
    unset($form['pager_id']);
  }

  // Handle the path value that views gives us
  if (!empty($form['path'])) {
    $form['path']['#description'] = t('The URL path used for linking and paging purposes. Leave blank to use the current page.');
    $form['path']['#title'] = '';
    $form['path']['#dependency'] = array('edit-link' => array(1), 'more-link' => array(1), 'use-pager-checkbox' => array(1));
    $form['exposed']['path'] = $form['path'];
    $form['exposed']['path']['#parents'] = array('path');
    unset($form['path']);
  }

  // Adjust the items per page and offset settings
  if (!empty($form['items_per_page']) && !empty($form['offset'])) {
    $form['items_per_page']['#field_prefix'] = t('Items to Show');
    $form['items_per_page']['#title'] = '';
    $form['items_per_page']['#description'] = t('Enter 0 to display all.');
    $form['display_settings']['items_per_page'] = $form['items_per_page'];
    unset($form['items_per_page']);
    $form['offset']['#field_prefix'] = t('Items to Skip');
    $form['offset']['#title'] = '';
    $form['offset']['#description'] = t('Enter 0 to skip none.');
    $form['offset']['#prefix'] = '<div class="clearfix">';
    $form['offset']['#suffix'] = '</div>';
    $form['display_settings']['offset'] = $form['offset'];
    unset($form['offset']);
  }
  elseif (!empty($form['items_per_page']) && empty($form['offset'])) {
    $form['items_per_page']['#field_prefix'] = t('Items to Show');
    $form['items_per_page']['#title'] = '';
    $form['items_per_page']['#description'] = t('Enter 0 to display all.');
    $form['items_per_page']['#prefix'] = '<div class="clearfix">';
    $form['items_per_page']['#suffix'] = '</div>';
    $form['display_settings']['items_per_page'] = $form['items_per_page'];
    unset($form['items_per_page']);
  }
  elseif (empty($form['items_per_page']) && !empty($form['offset'])) {
    $form['offset']['#field_prefix'] = t('Items to Skip');
    $form['offset']['#title'] = '';
    $form['offset']['#description'] = t('Enter 0 to skip none.');
    $form['offset']['#prefix'] = '<div class="clearfix">';
    $form['offset']['#suffix'] = '</div>';
    $form['display_settings']['offset'] = $form['offset'];
    unset($form['offset']);
  }

  // Adjust the sort ordering and sort by options
  if (!empty($form['exposed']['sort_order']) && !empty($form['exposed']['sort_by'])) {
    $form['exposed']['sort_order']['#title'] = t('Sort order');
    $form['display_settings']['sort_order'] = $form['exposed']['sort_order'];
    $form['display_settings']['sort_order']['#parents'] = array('exposed', 'sort_order');
    unset($form['exposed']['sort_order']);
    $form['exposed']['sort_by']['#prefix'] = '<div class="clearfix">';
    $form['exposed']['sort_by']['#suffix'] = '</div>';
    $form['display_settings']['sort_by'] = $form['exposed']['sort_by'];
    $form['display_settings']['sort_by']['#parents'] = array('exposed', 'sort_by');
    unset($form['exposed']['sort_by']);
  }
  elseif (!empty($form['exposed']['sort_order']) && empty($form['exposed']['sort_by'])) {
    $form['exposed']['sort_order']['#title'] = t('Sort order');
    $form['exposed']['#prefix'] = '<div class="clearfix">';
    $form['exposed']['#suffix'] = '</div>';
    $form['display_settings']['sort_order'] = $form['exposed']['sort_order'];
    $form['display_settings']['sort_order']['#parents'] = array('exposed', 'sort_order');
    unset($form['exposed']['sort_order']);
  }
  elseif (empty($form['exposed']['sort_order']) && !empty($form['exposed']['sort_by'])) {
    $form['exposed']['sort_by']['#prefix'] = '<div class="clearfix">';
    $form['exposed']['sort_by']['#suffix'] = '</div>';
    $form['display_settings']['sort_by'] = $form['exposed']['sort_by'];
    $form['display_settings']['sort_by']['#parents'] = array('exposed', 'sort_by');
    unset($form['exposed']['sort_by']);
  }

  // Adjust the field setting options.
  if (isset($form['fields_override'])) {
    $form['fields_override']['#title'] = t('Field Settings');
    $form['fields_override']['#collapsible'] = FALSE;
    foreach ($form['fields_override'] as &$field) {
      if (is_array($field)) {
        $field['#title'] = t('Display') . ' ' . $field['#title'];
      }
    }
  }

  // Determine if this is a fielded view. If so, add the display type toggle.
  $view_handler = $form_state['view']->display_handler;
  if ($view_handler->options['row_plugin'] == 'fields') {
    // Set to default view settings if there isn't one.
    if (empty($conf['view_settings'])) {
      if ($form_state['view']->style_plugin->plugin_name == 'table') {
        $conf['view_settings'] = 'table';
      }
      else {
        $conf['view_settings'] = 'fields';
      }
    }

    // Deal with legacy 'nodes' and others (such as 'files') view settings so
    // that other entity types can be included.
    $conf['view_settings'] = panels_preview_convert_view_settings($conf['view_settings']);

    // Add information about the View Mode.
    $form['display_settings']['view_settings'] = array(
      '#type' => 'radios',
      '#prefix' => '<div class="view-settings-wrapper"><span class="field-prefix">' . t('Display Type') . '</span>',
      '#suffix' => '</div>',
      '#default_value' => $conf['view_settings'],
      '#weight' => 10,
      '#options' => array(
        'fields' => t('Fields'),
        'rendered_entity' => t('Content'),
        'table' => t('Table'),
      ),
    );

    // Add header column options for table views.
    $form['display_settings']['header_type'] = array(
      '#type' => 'select',
      '#title' => t('Column Header'),
      '#options' => array(
        'none' => t('None'),
        'titles' => t('Titles'),
      ),
      '#default_value' => !empty($conf['header_type']) ? $conf['header_type'] : 'none',
      '#states' => array(
        'visible' => array(
          ':input[name="view_settings"]' => array('value' => 'table'),
        ),
      ),
      '#weight' => 11,
    );

    // Update the field overrides to be dependent on the view settings selection.
    $form['fields_override']['#states'] = array(
      // The inverted logic here isn't optimal, and in the future may be better achieved via OR'd conditions.
      // @link http://drupal.org/node/735528 @endlink
      'invisible' => array(
        ':input[name="view_settings"]' => array('value' => 'rendered_entity'),
      ),
    );
  }

  // Get view modes for entity
  $view_modes = panels_preview_view_mode_options(panels_preview_get_entity_type($form_state['view']));

  // Get the default view mode.
  $options_default_view_mode = ($view_handler->options['row_plugin'] == 'fields') ? 'teaser' : 'full';
  if (isset($form_state['view']->style_plugin->display->display_options['row_options']['view_mode'])) {
    $options_default_view_mode = $form_state['view']->style_plugin->display->display_options['row_options']['view_mode'];
  }
  if (!array_key_exists($options_default_view_mode, $view_modes)){
    $options_default_view_mode = current(array_keys($view_modes));
  }

  // Add specific style options.
  $form['content_settings']['view_mode'] = array(
    '#type' => 'radios',
    '#options' => $view_modes,
    '#default_value' => !empty($conf['view_mode']) ? $conf['view_mode'] : $options_default_view_mode,
    '#states' => array(
      'visible' => array(
        ':input[name="view_settings"]' => array('value' => 'rendered_entity'),
      ),
    ),
  );

  // Define a general settings fieldset if we have exposed values
  if (!empty($form['exposed']) && count(element_children($form['exposed'])) > 0) {
    $form['exposed']['#type'] = 'fieldset';
    $form['exposed']['#title'] = t('General Settings');
    $form['exposed']['#weight'] = -30;
    $form['exposed']['#attributes'] = array('class' => array('general-settings-fieldset'));
  }

  // Define a context settings fieldset if we have exposed values
  if (!empty($form['context']) && count(element_children($form['context'])) > 0) {
    $form['context']['#type'] = 'fieldset';
    $form['context']['#title'] = t('Context Settings');
    $form['context']['#weight'] = -29;
  }

  // Define a content settings fieldset if we have exposed values
  if (!empty($form['content_settings']) && count(element_children($form['content_settings'])) > 0) {
    $form['content_settings']['#type'] = 'fieldset';
    $form['content_settings']['#title'] = t('Content Settings');
    $form['content_settings']['#weight'] = -27;
  }

  // Define a display settings fieldset if we have display options
  if (!empty($form['display_settings']) && count(element_children($form['display_settings'])) > 0) {
    $form['display_settings']['#type'] = 'fieldset';
    $form['display_settings']['#title'] = t('Display Settings');
    $form['display_settings']['#weight'] = -28;
  }

  // Add auto submit functionality
  if (variable_get('panels_preview_live_preview', 1)) {
    panels_preview_autosubmit_configure($form);
  }

  // Use the ctools submit handler for Views Content.
  $form['#submit'][] = 'panels_preview_ctools_content_type_modal_submit';
}

/**
 * Helper function to get the entity type retrieved by a view
 */
function panels_preview_get_entity_type($view) {
  $result = 'node';

  if (isset($view->base_table) && $view->base_table != 'node') {
    // Find the entity type with the corresponding base table.
    $base_table = $view->base_table;
    $entity_types = entity_get_info();
    foreach ($entity_types as $entity_type => $entity_type_info) {
      if ($entity_type_info['base table'] == $base_table) {
        $result = $entity_type;
        break;
      }
    }
  }

  return $result;
}
