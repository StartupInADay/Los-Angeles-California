<?php

/**
 * @addtogroup hooks
 * @{
 */

/**
 * @file
 * These are the hooks that are invoked by Form Builder.
 */

/**
 * Define form builder form types and their element types.
 *
 * @return
 *   An associative array of form type definitions. Each definition is an
 *   associative array:
 *    - class: This has to be the fully qualified name of an autoloadable class
 *      implementing the FormBuilderFormInterface. If no class is passed it
 *      defaults to 'FormBuilderFormBase'.
 *    - element class: The default class used for elements.
 *    - property class: The default class used for properties.
 *   The full definition is passed to the constructor of the class, so other
 *   array keys can be used to pass additional parameters.
 */
function hook_form_builder_form_types() {
  $types['example'] = array(
    'class' => 'ExampleFormBuilderForm',
    'element class' => 'ExampleFormBuilderElement',
    'property class' => 'ExampleFormBuilderProperty',
    'parameter1' => 'test',
  );
  return $types;
}

/**
 * Alter the form types defined by other modules.
 *
 * @see hook_form_builder_form_types().
 */
function hook_form_builder_form_types_alter(&$types) {
  $types['webform']['element class'] = 'ExtendedWebformElementBase';
}

/**
 * Define the elements and properties supported by a form.
 *
 * All modules that wish to create an configurable form need implement this hook. It
 * defines to Form Builder what types of elements the implementing module knows
 * how to modify. Within each element that is modifiable, the properties that
 * may be changed are also listed.
 *
 * @param string $form_type
 *   The form type of the form as declared in hook_form_builder_form_types().
 * @param mixed $form_id
 *   The ID of the form.
 *
 * @return
 *   An array of available elements types for this form. Each field contains
 *   the following properties:
 *   - class: The class used to handle this element type. Defaults to the
 *     'element class' attribute of the form type.
 *   - title: The name of the field type that is displayed in the new fields
 *     block.
 *   - properties: An array of properties that are configurable. Configuration
 *     of these properties is defined by hook_form_builder_properties().
 *   - default: A complete sample form element that will be used when a new
 *     element of this type is added to the form. Further modification of this
 *     default element may be done in hook_form_builder_element_alter().
 */
function hook_form_builder_element_types($form_type, $form_id) {
  if ($form_type != 'node') {
    return;
  }

  $types = array();

  // The #type property of the field is used as the key.
  $types['textfield'] = array(
    'title' => t('Textfield'),
    // Properties that may be edited on this field type.
    'properties' => array(
      'title',
      'description',
      'field_prefix',
      'field_suffix',
      'default_value',
      'required',
      'size',
    ),
    // A complete default form element used when a new field of this type
    // is added to a form.
    'default' => array(
      '#title' => t('New textfield'),
      '#type' => 'textfield',
    ),
    // If needing only a single field of this type in an entire form, specify
    // the "unique" property. The form element will be remove from the new
    // field pallette when added. If the field is deleted from the form, the
    // field option will reappear in the new field block.
    // 'unique' => TRUE,
  );

  // Return the array of supported element types.
  return $types;
}

/**
 * Modify fields and properties that are declared by other modules.
 *
 * @see hook_form_builder_element_types()
 */
function hook_form_builder_element_types_alter(&$types, $form_type, $form_id) {
  if ($form_type == 'webform') {
    // Add our new placeholder properties for the webform textfield component.
    $types['textfield']['properties'][] = 'placeholder';
  }
}

/**
 * Defined globally available Form API properties.
 *
 * The hook_form_builder_properties() hook allows modules to define properties
 * that are configurable within form elements. Properties defined by any module may be
 * used inside of any form element, so unique property names are advised.
 *
 * Typically, this hook only needs to implemented if your module also has an
 * implementation of hook_elements(). In which case you would implement
 * hook_form_builder_properties to inform Form Builder of the new properties
 * that are configurable.
 *
 * @param $form_type
 *   The type of form for which these properties apply. You may choose to ignore
 *   the value of this parameter if your properties apply globally to all forms.
 *
 * @return
 *   An array of properties mapped to their configuration:
 *   - 'class' (optional): Specifies the class that defines the property
 *     behavior. It defaults to the default 'property class' defined in
 *     @see hook_form_builder_form_types().
 *   Additional parameters may be specified. Their semantics is defined and
 *   implemented by their property class. Usually the following keys may be
 *   used:
 *   - 'form': Form callback that generates the form elements for configuring
 *     this property.
 *   - 'validate': Array of validate callbacks.
 *   - 'submit': Array of submit callbacks.
 *
 * @ingroup form_builder
 */
function hook_form_builder_properties($form_type) {
  return array(
    'title' => array(
      'form' => 'form_builder_property_title_form',
    ),
    'description' => array(
      'form' => 'form_builder_property_description_form',
    ),
    'options' => array(
      'form' => 'form_builder_property_options_form',
      'submit' => array('form_builder_property_options_form_submit'),
    ),
  );
}

/**
 * Define globally available #element_validate functions.
 *
 * @param $form_type
 *   The form type for which this validator will be available. You may
 *   optionally check this value if you'd like to limit this validator to only
 *   certain form types.
 */
function hook_form_builder_validators($form_type) {
  return array(
    'form_validate_integer' => array(
      'form' => 'form_builder_validate_integer',
    ),
    'form_validate_decimal' => array(
      'form' => 'form_builder_validate_decimal',
    ),
    'form_validate_email' => array(
      'form' => 'form_builder_validate_email',
    ),
    'form_validate_url' => array(
      'form' => 'form_builder_validate_url',
    ),
  );
}

/**
 * Designate groups of properties. Displayed as tabs when editing a field.
 *
 * Most properties will fall into one of the predefined categories created by
 * Form Builder, but it may be desired that some properties be split into
 * entirely different groups to separate them from other property options.
 *
 * Form Builder provides the following groups by default:
 *  - default: The "Properties" tab, used if no group is specified.
 *  - hidden: Not displayed at all unless JavaScript is disabled.
 *  - display: The "Display" tab. Use for properties that are purely cosmetic.
 *  - options: The "Options" tab. Typically used for select list, radio,
 *    or checkbox options.
 *  - validation: The "Validation" tab. Use for properties or configuration
 *    that enables validation functions on the element.
 *
 * @param $form_type
 *   The form type for which this group will be available. You may optionally
 *   check this value if you'd like to limit this group to only certain form
 *   types.
 *
 * @return
 *   An array of property groups, keyed by the value used in the
 *   #form_builder['property_group'] property.
 *
 * @see hook_form_builder_load()
 *
 * @ingroup form_builder
 */
function hook_form_builder_property_groups($form_type) {
  return array(
    'my_group' => array(
      // Low weight values will appear as first tabs.
      'weight' => 0,
      // The title will be used as the tab title.
      'title' => t('My group'),
    ),
    'my_hidden_group' => array(
      'weight' => 100,
      'title' => t('Advanced'),
       // When JavaScript is enabled, collapsed groups will not be rendered.
       // This group will only be displayed when JavaScript is disabled.
      'collapsible' => TRUE,
      'collapsed' => TRUE,
    ),
  );
}

/**
 * Modify an individual element before it is displayed in the form preview.
 *
 * This function is typically used to cleanup a form element just before it
 * is rendered. The most important purpose of this function is to filter out
 * dangerous markup from unfiltered properties, such as #description.
 * Properties like #title and #options are filtered by the Form API.
 */
function hook_form_builder_preview_alter(&$element, $form_type, $form_id) {
  if ($form_type == 'node') {
    if (isset($element['#description'])) {
      $element['#description'] = filter_xss($element['#description']);
    }
  }
}

/**
 * Modify an individual element before it is added to a new form.
 *
 * This function may be helpful for setting a new element #key,
 * #form_builder['element_id'], or adjusting access in the
 * #form_builder['configurable'] and #form_builder['removable'] properties.
 */
function hook_form_builder_add_element_alter(&$element, $form_type, $form_id) {
  if ($form_type == 'node') {
    $element['#key'] = 'something';
  }
}

/**
 * Take a Form API array and save settings for changed elements.
 *
 * @param $form_type
 *   The type of form being loaded.
 * @param $form_id
 *   The unique identifier for the form being edited.
 */
function hook_form_builder_load($form_type, $form_id) {
  if ($form_type == 'node') {
    $node = (object) array(
      'type' => preg_replace('/_node_form/', '', $form_id),
    );

    // Load the form, usually by calling it's function directly.
    $form = node_form(array(), $node);

    // Allow other modules to extend the form.
    $form_state = array();
    drupal_alter('form', $form, $form_state, $form_id);

    // Loop through the form and add #form_builder properties to each element
    // that is configurable.
    foreach (element_children($form) as $key) {
      $form[$key]['#form_builder'] = array(
        'configurable' => TRUE,
        'removable' => TRUE,
        // If unique, when this element is deleted, a new one will appear in the
        // new field pallette.
        //'unique' => TRUE,
      );
    }

    return $form;
  }
}

/**
 * Take a form builder array and save changes permanently.
 */
function hook_form_builder_save(&$form, $form_type, $form_id) {
  if ($form_type == 'node') {
    foreach (element_children($form) as $key) {
      if (isset($form[$key]['#form_builder']['element_id'])) {
        // Save settings for this element.
      }
    }
  }
}

/**
 * @} End of "addtogroup hooks".
 */
