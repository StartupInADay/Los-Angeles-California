<?php

/**
 * @file form_builder.admin.inc
 * Administrative interface for editing forms.
 */

/**
 * Main form building interface. Can be used as a menu callback.
 *
 * @param $form_type
 *   The type of form being edited. Usually the name of the providing module.
 * @param $form_id
 *   The unique identifier for the form being edited with the type.
 */
function form_builder_interface($form_type, $form_id) {
  module_load_include('inc', 'form_builder', 'includes/form_builder.api');
  module_load_include('inc', 'form_builder', 'includes/form_builder.cache');

  // Set the current form type (used for display of the sidebar block).
  form_builder_active_form($form_type, $form_id);

  // Load the current state of the form, or create a new cache if needed.
  $loader = FormBuilderLoader::instance();
  $form_obj = $loader->fromCache($form_type, $form_id);
  if (!$form_obj) {
    $form_obj = $loader->fromStorage($form_type, $form_id);
    $form_obj->save();
  }

  $output = array();
  $output[] = drupal_get_form('form_builder_preview', $form_obj, $form_type, $form_id);
  $output[] = drupal_get_form('form_builder_positions', $form_obj, $form_type, $form_id);

  return $output;
}

/**
 * Menu callback for adding a field.
 */
function form_builder_add_page($form_type, $form_id, $element_type) {
  _form_builder_add_element($form_type, $form_id, $element_type);
}

/**
 * Menu callback for cloning a field.
 */
function form_builder_clone_page($form_type, $form_id, $element_id) {
  _form_builder_add_element($form_type, $form_id, NULL, $element_id);
}

/**
 * Menu callback helper for adding or cloning a field.
 *
 * If the $element_id parameter is provided, the new field will be cloned from
 * the corresponding field (and the $element_type parameter will be ignored).
 * Otherwise, a new field of type $element_type will be added from scratch.
 */
function _form_builder_add_element($form_type, $form_id, $element_type, $element_id = NULL, $sid = NULL, $return = FALSE) {
  module_load_include('inc', 'form_builder', 'includes/form_builder.api');
  module_load_include('inc', 'form_builder', 'includes/form_builder.cache');

  $loader = FormBuilderLoader::instance();
  $fields = $loader->getElementTypeInfo($form_type, $form_id);
  $form_obj = $loader->fromCache($form_type, $form_id, $sid);
  $element = NULL;
  // Keep track of the number of elements added this requests.
  $i = &drupal_static(__FUNCTION__, 1);

  // If the field is being cloned, copy the original from the current form
  // structure.
  if (isset($element_id)) {
    $element = $form_obj->getElementArray($element_id);
    if ($element) {
      // @todo implement cloning as an element method.
      // Remove the key so that a new one will be generated.
      unset($element['#key']);
      // Change the title to avoid confusion, and to avoid duplicate
      // auto-generated machine names.
      if (isset($element['#title'])) {
        $element['#title'] = t('Copy of !title', array('!title' => $element['#title']));
      }
      // Set the element type to the correct value so it will be used
      // consistently throughout this function.
      $element_type = $element['#type'];
    }
  }
  // Otherwise, create a new default field.
  elseif (isset($fields[$element_type]['default'])) {
    // @todo make this an element static method.
    $element = $fields[$element_type]['default'];
  }

  if ($element) {
    // @todo make this part of the element object's initialization.
    $element_id = isset($_REQUEST['element_id']) ? $_REQUEST['element_id'] : 'new_' . REQUEST_TIME . $i++;

    // Set the element ID to a hard-coded value if a unique field type.
    if (isset($fields[$element_type]['unique']) && $fields[$element_type]['unique']) {
      $element_id = $element_type;
    }

    $element['#key'] = $element_id;
    $element['#form_builder']['element_id'] = $element_id;
    $element['#form_builder']['is_new'] = TRUE;
    $f = $form_obj->getFormArray();
    $element['#weight'] = count(element_children($f));

    $element_id = $form_obj->setElementArray($element, FORM_BUILDER_ROOT, TRUE);
    $form_obj->save();

    if (isset($_REQUEST['js']) || $return) {
      $form = drupal_get_form('form_builder_positions', $form_obj, $form_type, $form_id);

      $data = array(
        'formType' => $form_type,
        'formId' => $form_id,
        'elementId' => $element_id,
        'html' => form_builder_field_render($form_type, $form_id, $element_id),
        'positionForm' => drupal_render($form),
      );

      if ($return) {
        return $data;
      }
      form_builder_json_output($data);
      exit();
    }
  }

  // Otherwise return to the previous page.
  drupal_goto();
}

/**
 * Menu callback for configuring a field.
 */
function form_builder_configure_page($form_type, $form_id, $element_id) {
  $output = drupal_get_form('form_builder_field_configure', $form_type, $form_id, $element_id);

  if (isset($_REQUEST['js'])) {
    // Return the newly changed field.
    if (isset($_REQUEST['return'])) {
      form_builder_field_json($form_type, $form_id, $element_id);
    }
    // Display the configuration form for a field.
    else {
      $data = array(
        'formType' => $form_type,
        'formId' => $form_id,
        'elementId' => $element_id,
        'html' => drupal_render($output),
        'errors' => form_get_errors(),
      );
      form_builder_json_output($data);
      exit();
    }
  }

  return $output;
}

/**
 * Menu callback for removing a field.
 */
function form_builder_remove_page($form_type, $form_id, $element_id) {
  $output = drupal_get_form('form_builder_field_remove', $form_type, $form_id, $element_id);

  if (isset($_REQUEST['js']) && !isset($_REQUEST['return'])) {
    // This after build function immediately returns the form as JSON.
    $data = array(
      'formType' => $form_type,
      'formId' => $form_id,
      'elementId' => $element_id,
      'html' => drupal_render($output),
    );

    form_builder_json_output($data);
    exit();
  }

  return $output;
}

/**
 * Render the palette of fields to add to a form.
 */
function form_builder_field_palette() {
  $active = form_builder_active_form();
  $output = NULL;
  if (isset($active)) {
    $form_type = $active['form_type'];
    $form_id = $active['form_id'];
    $loader = FormBuilderLoader::instance();
    $fields = $loader->getElementTypeInfo($form_type, $form_id);
    $groups = $loader->getPaletteGroupInfo($form_type);
    // TODO: We shouldn't have to clear the cache here.
    $form = $loader->fromCache($form_type, $active['form_id'], NULL, TRUE);
    $active_fields = $form->getElementTypes();
    foreach ($fields as $key => $field) {
      if ($field['unique'] && in_array($key, $active_fields)) {
        $fields[$key]['in_use'] = TRUE;
      }
      if ($field['addable'] == FALSE) {
        unset($fields[$key]);
      }
    }
    $output = theme('form_builder_field_palette', array('fields' => $fields, 'groups' => $groups, 'form_type' => $active['form_type'], 'form_id' => $active['form_id']));
  }
  return $output;
}

/**
 * Form. Given a form array, present it for editing in a preview.
 */
function form_builder_preview($f, &$form_state, $form_obj, $form_type, $form_id) {

  // @todo: think about this more. We basically get the form from outside, so
  // while normally the first argument would be $form, we use the third, which
  // is provided by the caller.

  // Make modifications to all form elements recursively.
  $element_ids = $form_obj->getElementIds();

  $form = $form_obj->getFormArray();
  // Record all the element IDs within the entire form.
  $form['#form_builder']['element_ids'] = $element_ids;
  $form['#form_builder']['form_type'] = $form_type;
  $form['#form_builder']['form_id'] = $form_id;

  // Add a pre_render to the entire form itself.
  $form['#pre_render'][] = 'form_builder_pre_render_form';
  $form['#theme_wrappers'] = array('form_builder_wrapper');

  // Add required jQuery UI elements.
  $form['#attached']['library'][] = array('system', 'ui.draggable');
  $form['#attached']['library'][] = array('system', 'ui.droppable');
  $form['#attached']['library'][] = array('system', 'ui.sortable');

  $form['#attached']['library'][] = array('system', 'form');
  $form['#attached']['js'][] = drupal_get_path('module', 'form_builder') .'/form_builder.js';

  // TODO: This JS file should be loaded dynamically as needed.
  $form['#attached']['js'][] = drupal_get_path('module', 'options_element') .'/options_element.js';
  // TODO: Use libraries if available. see http://drupal.org/node/954804
  $form['#attached']['js'][] = 'misc/tabledrag.js';
  $form['#attached']['js'][] = 'misc/jquery.cookie.js';
  $form['#attached']['library'][] = array('system', 'form');
  $form['#attached']['js'][] = 'misc/form.js';
  $form['#attached']['js'][] = 'misc/collapse.js';

  $form['#attached']['js'][] = drupal_get_path('module', 'filter') . '/filter.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'filter') . '/filter.css';

  $form['#attached']['js'][] = array('data' => array('machineName' => array()), 'type' => 'setting');
  $form['#attached']['js'][] = 'misc/machine-name.js';

  $settings = array(
    'emptyForm' => theme('form_builder_empty_form'),
    'emptyFieldset' => theme('form_builder_empty_fieldset'),
    'noFieldSelected' => theme('form_builder_no_field_selected'),
    'fieldLoading' => theme('form_builder_field_loading'),
  );

  $form['#attached']['js'][] = array('data' => array('formBuilder' => $settings), 'type' => 'setting');

  $form['#attached']['css'][] = drupal_get_path('module', 'form_builder') .'/form_builder.css';
  $form['#attached']['css'][] = drupal_get_path('module', 'options_element') .'/options_element.css';

  return $form;
}

/**
 * Form containing all the current weights and parents of elements.
 */
function form_builder_positions($form, &$form_state, $form_obj, $form_type, $form_id) {
  form_load_include($form_state, 'inc', 'form_builder', 'includes/form_builder.admin');
  $form = array(
    '#tree' => TRUE,
    '#form_builder' => array(
      'form_type' => $form_type,
      'form_id' => $form_id,
    ),
  );

  foreach ($form_obj->getElementIds() as $element_id) {
    $element = $form_obj->getElementArray($element_id);
    $form[$element_id]['weight'] = array(
      '#type' => 'hidden',
      '#default_value' => isset($element['#weight']) ? $element['#weight'] : 0,
      '#attributes' => array('class' => array('form-builder-weight form-builder-element-' . $element_id)),
    );
    $form[$element_id]['parent'] = array(
      '#type' => 'hidden',
      '#default_value' => $element['#form_builder']['parent_id'],
      '#attributes' => array('class' => array('form-builder-parent form-builder-element-' . $element_id)),
    );
  }

  // Drupal MUST have a button to register submissions.
  // Add a button even though the form is only submitted via AJAX.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#ajax' => array(
      'callback' => 'form_builder_positions_ajax',
      'wrapper' => 'form_builder_positions',
    ),
  );

  return $form;
}

/**
 * AJAX callback for form_builder_positions.
 */
function form_builder_positions_ajax($form, &$form_state) {
  // We don't want to change anything in the form.
  return array(
    '#type' => 'ajax',
    '#commands' => array(),
  );
}

/**
 * Submit handler for the form_builder_positions form.
 */
function form_builder_positions_submit(&$form, &$form_state) {
  module_load_include('inc', 'form_builder', 'includes/form_builder.api');
  module_load_include('inc', 'form_builder', 'includes/form_builder.cache');

  $form_type = $form['#form_builder']['form_type'];
  $form_id = $form['#form_builder']['form_id'];
  $form_cache = FormBuilderLoader::instance()->fromCache($form_type, $form_id);

  foreach (element_children($form) as $element_id) {
    // Skip items without weight value (like the form token, build_id, etc).
    if (!isset($form[$element_id]['weight'])) {
      continue;
    }

    // Check for changed weights or parents.
    $element = $form_cache->getElementArray($element_id);
    $element['#weight'] = $form_state['values'][$element_id]['weight'];
    $element['#form_builder']['parent_id'] = $form_state['values'][$element_id]['parent'];
    $form_cache->setElementArray($element);
  }

  // Save all the changes made.
  $form_cache->save();

  // Don't redirect, which will cause an unnecessary HTTP request.
  $form_state['redirect'] = FALSE;
}

/**
 * Output the wrapper around the form_builder preview.
 *
 * Optionally outputs the field palette if it is not already available as a
 * block.
 */
function theme_form_builder_wrapper($variables) {
  $element = $variables['element'];

  $output = '';
  $output .= '<div id="form-builder-wrapper" class="' . ($element['#show_palette'] ? 'with-palette' : 'no-palette') . '">';
  if ($element['#show_palette']) {
    $output .= '<div id="form-builder-fields">';
    $output .= '<div class="block">';
    $output .= form_builder_field_palette();
    $output .= '</div>';
    $output .= '</div>';
  }

  $output .= '<div id="form-builder">';
  if (isset($element['#title'])) {
    $output .= '<h3 class="form-builder-title">' . $element['#title'] . '</h3>';
  }

  // Add the contents of the form and close the wrappers.
  $output .= $element['#children'];
  $output .= '</div>'; // #form-builder.
  $output .= '</div>'; // #form-builder-wrapper.
  return $output;
}

/**
 * Output the wrapper around a form_builder element with configure/remove links.
 */
function theme_form_builder_element_wrapper($variables) {
  $element = $variables['element'];

  $removable = isset($element['#form_builder']['removable']) ? $element['#form_builder']['removable'] : TRUE;
  $configurable  = isset($element['#form_builder']['configurable']) ? $element['#form_builder']['configurable'] : TRUE;
  $cloneable = form_builder_menu_field_access('clone', $element['#form_builder']['form_type'], $element['#form_builder']['form_id'], $element['#form_builder']['element_id']);

  $output = '';
  $output .= '<div class="form-builder-wrapper">';
  if ($removable || $configurable) {
    $output .= '<div class="form-builder-title-bar">';
    $output .= '<span class="form-builder-links">';
    if ($removable) {
      $output .= l('<span class="remove">'. t('Remove') .'</span>', 'admin/build/form-builder/remove/' . $element['#form_builder']['form_type'] . '/' . $element['#form_builder']['form_id'] . '/' . $element['#form_builder']['element_id'], array('html' => TRUE, 'attributes' => array('class' => array('remove'), 'title' => t('Remove')), 'query' => drupal_get_destination()));
    }
    if ($removable && $configurable) {
      $output .= ' ';
    }
    if ($configurable) {
      $output .= l('<span class="configure">'. t('Configure') .'</span>', 'admin/build/form-builder/configure/' . $element['#form_builder']['form_type'] . '/' . $element['#form_builder']['form_id'] . '/' . $element['#form_builder']['element_id'], array('html' => TRUE, 'attributes' => array('class' => array('configure'), 'title' => t('Configure')), 'query' => drupal_get_destination()));
    }
    $output .= '</span>';
    $output .= '</div>';
  }

  if ($cloneable) {
    $output .= '<div class="form-builder-bottom-title-bar">';
    $output .= '<span class="form-builder-bottom-links">';
    $output .= l('<span class="clone">'. t('Copy') .'</span>', 'admin/build/form-builder/clone/' . $element['#form_builder']['form_type'] . '/' . $element['#form_builder']['form_id'] . '/' . $element['#form_builder']['element_id'], array('html' => TRUE, 'attributes' => array('class' => array('clone'), 'title' => t('Copy')), 'query' => drupal_get_destination()));
    $output .= '</span>';
    $output .= '</div>';
  }

  $output .= '<div class="form-builder-element form-builder-element-' . $element['#type'] . '" id="form-builder-element-' . $element['#form_builder']['element_id'] . '">';
  // TODO: Overlay image: good idea or bad idea? Prevents any interaction with
  // form elements in the preview.
  //$output .= theme('image', drupal_get_path('module', 'form_builder') .'/images/blank.gif', '', '', array('width' => '1', 'height' => '1', 'class' => 'form-builder-disable'));
  $output .= $element['#children'];

  $output .= '</div></div>';

  return $output;
}

/**
 * Placeholder for an entirely empty form before any fields are added.
 */
function theme_form_builder_empty_form($variables) {
  $output = '';
  $output .= '<div class="form-builder-wrapper form-builder-empty-form form-builder-empty-placeholder">';
  $output .= '<span>' . t('Drag a field from the palette to add it to this form.') . '</span>';
  $output .= '</div>';
  return $output;
}

/**
 * Placeholder for empty fieldsets during drag and drop.
 */
function theme_form_builder_empty_fieldset($variables) {
  $output = '';
  $output .= '<div class="form-builder-wrapper form-builder-empty-placeholder">';
  $output .= '<span>' . t('This fieldset is empty. Drag a form element into it.') . '</span>';
  $output .= '</div>';
  return $output;
}

/**
 * Message shown in custom field configure forms when no field is selected.
 *
 * Note that this message is not displayed using the default field presentation.
 * Modules or themes can set a custom field configuration form location by
 * specifying a Drupal.settings.formBuilder.configureFormSelector value.
 */
function theme_form_builder_no_field_selected($variables) {
  $output = '';
  $output .= '<div class="field-settings-message">';
  $output .= t('No field selected');
  $output .= '</div>';
  return $output;
}

/**
 * Message shown in custom field configure forms when a field is loading.
 *
 * @see theme_form_builder_no_field_selected().
 */
function theme_form_builder_field_loading($variables) {
  $output = '';
  $output .= '<div class="field-settings-message">';
  $output .= t('Loading...');
  $output .= '</div>';
  return $output;
}

/**
 * Block for adding new fields.
 *
 * @param $vars['fields']
 *   A list of all fields can be added to the current form type.
 * @param $vars['groups']
 *   A list of groups that fields may be sorted into. Each field is assigned
 *   a 'palette_group' property which corresponds to one of these groups.
 * @param $vars['form_type']
 *   The form type to which these blocks apply.
 * @param $vars['form_id']
 *   The form ID that is being edited.
 */
function theme_form_builder_field_palette($vars) {
  extract($vars);

  $output = '';
  $lists = array();
  foreach ($fields as $field_name => $field) {
    $class = array('field-' . $field_name, 'form-builder-palette-element');
    $style = '';

    // If a field is unique, add a special class to identify it.
    if ($field['unique']) {
      $class[] = 'form-builder-unique';
      $class[] = 'form-builder-element-' . $field_name;

      // If already in use, do not display it in the list.
      if (!empty($field['in_use'])) {
        $style = 'display: none;';
      }
    }

    $lists[$field['palette_group']]['#weight'] = $groups[$field['palette_group']]['weight'];
    $lists[$field['palette_group']][] = array(
      'data' => l($field['title'], 'admin/build/form-builder/add/' . $form_type . '/' . $form_id . '/' . $field_name, array('query' => drupal_get_destination())),
      'class' => $class,
      'style' => $style,
    );
  }

  // Sort the lists by weight.
  uasort($lists, 'element_sort');

  $output .= '<div id="form-builder-field-palette">';
  foreach ($lists as $group => $list) {
    unset($list['#weight']);
    $output .= theme('item_list', array('items' => $list, 'title' => (count($lists) > 1) ? $groups[$group]['title'] : t('Add a field'), 'type' => 'ul', 'attributes' => array('class' => array('form-builder-fields', 'clearfix'))));
  }
  $output .= '</div>';

  return $output;
}

/**
 * Take a form structure and add a prebuild function to every element.
 */
function form_builder_pre_render($element) {
  $element['#theme_wrappers'][] = 'form_builder_element_wrapper';

  if ($element['#form_builder']['element_type'] == 'fieldset') {
    $element['#attributes']['class'][] = 'form-builder-fieldset';
  }

  if (isset($element['#type']) && $element['#type'] == 'fieldset' && count(element_children($element)) == 0) {
    $element['#children'] = theme('form_builder_empty_fieldset');
  }

  // Allow modules to make modifications to this element.
  drupal_alter('form_builder_preview', $element, $element['#form_builder']['form_type'], $element['#form_builder']['form_id']);

  return $element;
}

/**
 * Pre-render function to alter the form being edited by Form builder.
 *
 * This function modifies the form element itself and sets a #title to label the
 * form preview and an #show_palette property to indicate to the theme wrapper
 * whether the field palette should be added.
 */
function form_builder_pre_render_form($form) {
  global $theme;

  // We can't have forms inside of forms, so change this entire form a markup.
  $form['#type'] = 'markup';

  // Set a title for the preview if none exists.
  $form['#title'] = !isset($form['#title']) ? t('Form preview') : $form['#title'];

  // Remove unnecessary FAPI elements.
  unset($form['form_build_id']);
  unset($form['form_token']);
  unset($form['form_builder_preview']);

  // Check if the Form Builder block is enabled.
  // Otherwise make our own columns.
  if (!isset($form['#show_palette'])) {
    if (module_exists('block')) {
      $form['#show_palette'] = !db_query("SELECT status FROM {block} WHERE module = 'form_builder' AND theme = :theme", array(':theme' => $theme))->fetchField();
    }
    else {
      $form['#show_palette'] = TRUE;
    }
  }

  if ($theme == 'garland' || $theme == 'minnelli') {
    $form['#attached']['css'][] = drupal_get_path('module', 'form_builder') . '/form_builder.garland.css';
  }

  return $form;
}

function form_builder_after_build($element) {
  $element['#attributes']['readonly'] = 'readonly';
  foreach (element_children($element) as $key) {
    $element[$key] = form_builder_after_build($element[$key]);
  }

  return $element;
}

/**
 * Form for editing a field.
 */
function form_builder_field_configure($form, $form_state, $form_type, $form_id, $element_id) {
  module_load_include('inc', 'form_builder', 'includes/form_builder.api');
  module_load_include('inc', 'form_builder', 'includes/form_builder.cache');
  $form_obj = FormBuilderLoader::instance()->fromCache($form_type, $form_id);
  $element_obj = $form_obj->getElement($element_id);

  $form = $element_obj->configurationForm($form, $form_state);
  $form['#_edit_form_type'] = $form_type;
  $form['#_edit_form_id'] = $form_id;
  $form['#_edit_element_id'] = $element_id;
  $form['#pre_render'][] = 'form_builder_field_configure_pre_render';

  $form['form_builder_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 100,
  );

  return $form;
}

/**
 * Pre-render function for the field configuration form.
 */
function form_builder_field_configure_pre_render($form) {
  $groups = module_invoke_all('form_builder_property_groups', $form['#_edit_form_type']);

  foreach (element_children($form) as $key) {
    // If no group is specified, put the element into the default group.
    if (!isset($form[$key]['#form_builder']['property_group']) || !isset($groups[$form[$key]['#form_builder']['property_group']])) {
      if (!isset($form[$key]['#type']) || (isset($form[$key]['#type']) && !in_array($form[$key]['#type'], array('hidden', 'button', 'submit', 'value', 'token')))) {
        $form[$key]['#form_builder']['property_group'] = 'default';
      }
    }

    if (isset($form[$key]['#form_builder']['property_group'])) {
      $group = $form[$key]['#form_builder']['property_group'];

      // We add "_property_group" to the field key to prevent conflicts of
      // property names and group names.
      if (!isset($form[$group . '_property_group'])) {
        $form[$group . '_property_group'] = array(
          '#type' => 'fieldset',
          '#title' => $groups[$group]['title'],
          '#weight' => $groups[$group]['weight'],
          '#collapsible' => isset($groups[$group]['collapsible']) ? $groups[$group]['collapsible'] : FALSE,
          '#collapsed' => isset($groups[$group]['collapsed']) ? $groups[$group]['collapsed'] : FALSE,
          '#attributes' => array('class' => array('form-builder-group')),
        );
      }

      $form[$group .'_property_group'][$key] = $form[$key];
      unset($form[$key]);
    }
  }

  return $form;
}

function form_builder_field_configure_submit(&$form, &$form_state) {
  list($form_type, $form_id, $element_id) = $form_state['build_info']['args'];
  $form_obj = FormBUilderLoader::instance()->fromCache($form_type, $form_id);
  $element_obj = $form_obj->getElement($element_id);

  $element_obj->configurationSubmit($form, $form_state);

  // Update the form builder cache.
  $form_obj->setElementArray($element_obj->render());
  $form_obj->save();

  if (isset($_GET['js'])) {
    // Option A: Use the destination variable to do a drupal_goto(). Allows
    // other submit handlers to add on extra functionality.
    // The destination variable takes precedence over $form_state['redirect'].
    //$_REQUEST['destination'] = 'admin/build/form-builder/json/' . $form_type . '/' . $form_id . '/' . $element_id;

    // Option B: Immediately print the JSON and exit. Faster and requires only
    // one HTTP request instead of two. Other submit handlers must be before
    // this on.
    form_builder_field_json($form_type, $form_id, $element_id);
  }
}

/**
 * Form for removing a field.
 */
function form_builder_field_remove($form, $form_state, $form_type, $form_id, $element_id) {
  module_load_include('inc', 'form_builder', 'includes/form_builder.api');
  module_load_include('inc', 'form_builder', 'includes/form_builder.cache');
  $element = form_builder_cache_field_load($form_type, $form_id, $element_id);

  $question = t('Remove the field %title?', array('%title' => $element['#title']));
  $path = isset($_GET['destination']) ? $_GET['destination'] : NULL;
  $description = t('Remove the field %title? This field will not be permanently removed until the form configuration is saved.', array('%title' => isset($element['#title']) ? $element['#title'] : $element['#form_builder']['element_id']));
  $yes = t('Remove');

  return confirm_form($form, $question, $path, $description, $yes);
}

function form_builder_field_remove_submit(&$form, &$form_state) {
  list($form_type, $form_id, $element_id) = $form_state['build_info']['args'];

  // Update the form builder cache.
  form_builder_cache_field_delete($form_type, $form_id, $element_id);

  if (isset($_GET['js'])) {
    // See form_builder_field_configure_submit() for comparison between using
    // redirect and immediately printing the JSON.
    //$form_state['redirect'] = 'admin/build/form-builder/json/' . $form_type . '/' . $form_id . '/' . $element_id;
    form_builder_field_json($form_type, $form_id, $element_id);
  }
}

/**
 * Render a single field independent of other settings.
 *
 * @todo form rendering a form method.
 */
function form_builder_field_render($form_type, $form_id, $element_id, $wrapper = FALSE) {
  module_load_include('inc', 'form_builder', 'includes/form_builder.api');
  module_load_include('inc', 'form_builder', 'includes/form_builder.cache');

  // Load the current state of the form and prepare it for rendering.
  $loader = FormBuilderLoader::instance();
  $form_obj = $loader->fromCache($form_type, $form_id);
  if (!$form_obj) {
    $form_obj = $loader->fromStorage($form_type, $form_id);
    $form_obj->save();
  }
  $form_state = array('submitted' => FALSE, 'build_info' => array('args' => array($form_obj, $form_type, $form_id)), 'values' => array(), 'method' => 'get');
  $form = drupal_retrieve_form('form_builder_preview', $form_state);
  drupal_prepare_form('form_builder_preview', $form, $form_state);
  $form['#post'] = array();
  $form = form_builder('form_builder_preview', $form, $form_state);

  // Get only the element wanted and render it.
  $element = form_builder_get_element($form, $element_id);

  if ($wrapper) {
    $element['#theme_wrappers'][] = 'form_builder_element_wrapper';
  }

  return drupal_render($element);
}

/**
 * Menu callback to display a field as a JSON string.
 */
function form_builder_field_json($form_type, $form_id, $element_id) {
  module_load_include('inc', 'form_builder', 'includes/form_builder.api');
  module_load_include('inc', 'form_builder', 'includes/form_builder.cache');
  $element = form_builder_cache_field_load($form_type, $form_id, $element_id);

  $data = array(
    'formType' => $form_type,
    'formId' => $form_id,
    'elementId' => $element_id,
    'html' => !empty($element) ? form_builder_field_render($form_type, $form_id, $element_id) : '',
    'errors' => form_get_errors(),
  );

  form_builder_json_output($data);
  exit();
}

/**
 * Generic function for outputing Form Builder JSON return responses.
 *
 * Adds status messages, settings, and timestamp to a form builder JSON response
 * and outputs it.
 */
function form_builder_json_output($data) {
  if (!isset($data['messages'])) {
    $data['messages'] = theme('status_messages');
  }
  if (!isset($data['settings'])) {
    $scripts = drupal_add_js();
    if (!empty($scripts['settings'])) {
      $data['settings'] = drupal_array_merge_deep_array($scripts['settings']['data']);
    }
  }
  if (!isset($data['time'])) {
    $data['time'] = time();
  }
  drupal_json_output($data);
}
