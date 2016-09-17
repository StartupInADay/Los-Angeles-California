<?php

/**
 * @file form_builder.properties.inc
 * Implementations of hook_form_builder_properties in separate functions.
 */

/**
 * Configuration form for the "key" property.
 *
 * The key property is special in that it's not actually part of the element,
 * but instead the array key that is used to reference the element in the
 * Form API structure.
 */
function form_builder_property_key_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['key'] = array(
    '#title' => t('Form key'),
    '#type' => 'machine_name',
    '#default_value' => preg_match('/^new_[0-9]+$/', $element['#key']) ? '' : $element['#key'],
    '#maxlength' => 128,
    '#description' => t('The form key is used in the field "name" attribute. Must be alphanumeric and underscore characters.'),
    '#machine_name' => array(
      'source' => array('title'),
      'label' => t('Form key'),
    ),
    '#weight' => -9,
    '#element_validate' => array('form_builder_property_key_form_validate'),
  );

  return $form;
}

/**
 * Element validate function for the "key" property. Ensure safe characters.
 *
 * @todo make part of this a form method.
 */
function form_builder_property_key_form_validate($element, $form_state) {
  if (!preg_match('/^[a-z0-9_]+$/', $element['#value'])) {
    form_error($element, t('The form key may only contain lowercase alphanumeric characters and underscores.'));
  }

  // Check that the new key does not conflict with an existing key.
  if ($element['#value'] != $element['#default_value']) {
    // Assuming to be called from form_builder_field_configure().
    list($form_type, $form_id, $element_id) = $form_state['build_info']['args'];
    $form_obj = FormBuilderLoader::instance()->fromCache($form_type, $form_id);
    $parent = _form_builder_get_element_parent($form_obj, $element_id);
    $new_key = $form_state['values']['key'];
    if (isset($parent[$new_key])) {
      form_set_error('key', t('The form key %key is already in use.', array('%key' => $new_key)));
    }
  }
}

/**
 * Get the parent element of an element from the form.
 */
function _form_builder_get_element_parent($form_obj, $element_id) {
  $element = $form_obj->getElementArray($element_id);
  $parent_id = $element['#form_builder']['parent_id'];
  return $form_obj->getElementArray($parent_id);
}

/**
 * Configuration form for the "title" property.
 */
function form_builder_property_title_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#default_value' => $element['#title'],
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -10,
  );

  return $form;
}

/**
 * Configuration form for the "title_display" property.
 */
function form_builder_property_title_display_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['title_display'] = array(
    '#form_builder' => array('property_group' => 'display'),
    '#title' => t('Title display'),
    '#type' => 'select',
    '#default_value' => $element['#title_display'],
    '#options' => array(
      'before' => t('Before'),
      'after' => t('After'),
      'invisible' => t('Invisible'),
      'attribute' => t('Attribute'),
    ),
    '#required' => TRUE,
    '#weight' => -10,
  );

  return $form;
}


/**
 * Configuration form for the "weight" property.
 *
 * This field is in the "hidden" builder group, meaning it's never shown in
 * the main editing interface. However, it's still there if editing without JS.
 */
function form_builder_property_weight_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  if (!isset($_REQUEST['js'])) {
    $form['weight'] = array(
      '#form_builder' => array('property_group' => 'hidden'),
      '#type' => 'textfield',
      '#size' => 6,
      '#title' => t('Weight'),
      '#default_value' => $element['#weight'],
    );
  }

  return $form;
}

/**
 * Configuration form for the "description" property.
 */
function form_builder_property_description_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => $element['#description'],
    '#weight' => 5,
  );

  return $form;
}

/**
 * Configuration form for the "disabled" property.
 */
function form_builder_property_disabled_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['disabled'] = array(
    '#form_builder' => array('property_group' => 'display'),
    '#title' => t('Disabled (read-only)'),
    '#type' => 'checkbox',
    '#default_value' => $element['#disabled'],
    '#weight' => 12,
  );

  return $form;
}

/**
 * Configuration form for the "required" property.
 */
function form_builder_property_required_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['required'] = array(
    '#form_builder' => array('property_group' => 'validation'),
    '#title' => t('Required'),
    '#type' => 'checkbox',
    '#default_value' => $element['#required'],
    '#weight' => -1,
  );

  return $form;
}

/**
 * Configuration form for the "options" property.
 */
function form_builder_property_options_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  // Checkboxes have an implied "multiple" property.
  if ($element['#type'] == 'checkboxes') {
    $element['#multiple'] = TRUE;
  }

  $form['options'] = array(
    '#form_builder' => array('property_group' => 'options'),
    '#title' => t('Options'),
    '#type' => 'options',
    '#default_value' => $element['#default_value'],
    '#options' => $element['#options'],
    '#required' => TRUE,
    '#multiple' => isset($element['#multiple']) ? $element['#multiple'] : FALSE,
    '#multiple_toggle' => isset($element['#multiple_toggle']) ? $element['#multiple_toggle'] : FALSE,
    '#optgroups' => $element['#type'] == 'select' ? TRUE : FALSE,
    '#limit' => 500,
    '#key_type' => isset($element['#key_type']) ? $element['#key_type'] : 'mixed',
    '#key_type_toggle' => isset($element['#key_type_toggle']) ? $element['#key_type_toggle'] : TRUE,
    '#key_type_toggled' => isset($element['#key_type_toggled']) ? $element['#key_type_toggled'] : FALSE,
  );

  // Remove the default value field, since it's handled by the options field.
  $form['default_value'] = array();

  return $form;
}

function form_builder_property_options_form_submit(&$form, &$form_state) {
  $options = $form_state['values']['options']['options_field'];
  $default_value = $form_state['values']['options']['default_value'];

  if (isset($form_state['values']['options']['multiple'])) {
    $multiple = $form_state['values']['options']['multiple'];
    $form_state['values']['multiple'] = $multiple;
  }

  if (isset($form_state['values']['options']['custom_keys'])) {
    $form_state['values']['key_type_toggled'] = $form_state['values']['options']['custom_keys'];
  }

  $form_state['values']['options'] = $options;
  $form_state['values']['default_value'] = $default_value;
}

/**
 * Configuration form for the "default_value" property.
 */
function form_builder_property_default_value_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['default_value'] = array(
    // Most fields have default values that can be stored on one line, so we
    // use a textfield to configure their default value. Textareas are an
    // exception, though.
    '#type' => $element['#type'] == 'textarea' ? 'textarea' : 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $element['#default_value'],
    '#weight' => 1,
  );

  return $form;
}

/**
 * Configuration form for the "markup" property.
 */
function form_builder_property_markup_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  // TODO: This is a placeholder until "#markup" becomes available in D7.
  $form['markup'] = array(
    '#type' => 'text_format',
    '#title' => t('Markup'),
    '#default_value' => $element['#markup'],
    '#format' => isset($element['#format']) ? $element['#format'] : NULL,
    '#weight' => 1,
  );

  return $form;
}

/**
 * Submit handler for the "markup" property.
 */
function form_builder_property_markup_form_submit(&$form, &$form_state) {
  // The submitted data from an element of type 'text_format' is an array that
  // contains the text and its format in separate keys. We want these to wind
  // up in #markup and #format, respectively.
  $markup_array = $form_state['values']['markup'];
  $form_state['values']['markup'] = $markup_array['value'];
  $form_state['values']['format'] = $markup_array['format'];
}

/**
 * Configuration form for the "maxlength" property.
 */
function form_builder_property_maxlength_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['maxlength'] = array(
    '#form_builder' => array('property_group' => 'validation'),
    '#type' => 'textfield',
    '#size' => 6,
    '#title' => t('Max length'),
    '#default_value' => $element['#maxlength'],
    '#field_suffix' => ' ' . t('characters'),
    '#weight' => 3,
    '#maxlength' => 7,
    '#element_validate' => array('form_validate_integer'),
  );

  return $form;
}

/**
 * Configuration form for the "size" property.
 */
function form_builder_property_size_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['size'] = array(
    '#form_builder' => array('property_group' => 'display'),
    '#type' => 'textfield',
    '#size' => 6,
    '#title' => t('Size'),
    '#default_value' => $element['#size'],
    '#weight' => 2,
    '#maxlength' => 5,
    '#element_validate' => array('form_validate_integer'),
  );

  return $form;
}

/**
 * Configuration form for the "rows" property.
 */
function form_builder_property_rows_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['rows'] = array(
    '#form_builder' => array('property_group' => 'display'),
    '#type' => 'textfield',
    '#size' => 6,
    '#title' => t('Rows'),
    '#default_value' => $element['#rows'],
    '#weight' => 2,
  );

  return $form;
}

/**
 * Configuration form for the "cols" property.
 */
function form_builder_property_cols_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['cols'] = array(
    '#form_builder' => array('property_group' => 'display'),
    '#type' => 'textfield',
    '#size' => 6,
    '#title' => t('Columns'),
    '#default_value' => $element['#cols'],
    '#weight' => 3,
    '#description' => t('The width of the textarea. This property might not have a visual impact depending on the CSS of your site.'),
  );

  return $form;
}


/**
 * Configuration form for the "field_prefix" property.
 */
function form_builder_property_field_prefix_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['field_prefix'] = array(
    '#form_builder' => array('property_group' => 'display'),
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#default_value' => $element['#field_prefix'],
    '#weight' => -2,
  );

  return $form;
}

/**
 * Configuration form for the "field_suffix" property.
 */
function form_builder_property_field_suffix_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['field_suffix'] = array(
    '#form_builder' => array('property_group' => 'display'),
    '#type' => 'textfield',
    '#title' => t('Suffix'),
    '#default_value' => $element['#field_suffix'],
    '#weight' => -1,
  );

  return $form;
}

/**
 * Configuration form for the "collapsible" property.
 */
function form_builder_property_collapsible_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['collapsible'] = array(
    '#form_builder' => array('property_group' => 'display'),
    '#type' => 'checkbox',
    '#title' => t('Collapsible'),
    '#default_value' => $element['#collapsible'],
    '#weight' => -2,
  );

  return $form;
}

/**
 * Configuration form for the "collapsed" property.
 */
function form_builder_property_collapsed_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['collapsed'] = array(
    '#form_builder' => array('property_group' => 'display'),
    '#type' => 'checkbox',
    '#title' => t('Collapsed'),
    '#default_value' => $element['#collapsed'],
    '#weight' => -1,
    '#description' => t('This property will not affect the preview immediately.'),
  );

  return $form;
}
