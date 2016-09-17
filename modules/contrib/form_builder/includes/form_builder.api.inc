<?php

/**
 * @file form_builder.api.inc
 * Universally used API functions within the Form builder module.
 */

/**
 * Get a list of all properties that are supported within a form type.
 *
 * @deprecated Deprecated since 1.8 and will be removed in 2.0.
 *   Use the Form / Element or Property classes instead of the info-arrays.
 */
function form_builder_get_properties($form_type, $reset = FALSE) {
  return FormBuilderLoader::instance()->getPropertyInfo($form_type, $reset);
}

/**
 * Function to retrieve a single element within a form structure.
 *
 * If needing to retreive multiple elements at once, use
 * form_builder_get_elements().
 *
 * @param $form
 *   A complete hierarchical FAPI structure.
 * @param $element_id
 *   The unique identifier for an element that is to be retrieved.
 * @return
 *   A single Form API element array.
 *
 * @see form_builder_get_elements().
 *
 * @deprecated Use FormBuilderFormInterface::getElement() instead.
 *   Will be removed in 2.0.
 */
function form_builder_get_element(&$form, $element_id) {
  return FormBuilderFormBase::fromArray($form)->getElementArray($element_id);
}

/**
 * Retrieve multiple elements within a form structure.
 *
 * @param $form
 *   A complete hierarchical FAPI structure.
 * @param $element_ids
 *   An array of unique identifiers for elements that are to be retreived. These
 *   identifiers match against the special property
 *   "#form_builder['element_id']", which is not available in normal FAPI
 *   structures. It must be added by the respective module that is providing
 *   support for a certain field type.
 *
 *   For example, CCK provides a unique identifier for each field such as
 *   "field_my_name". This field name must be added to the form array as
 *   #form_builder['element_id'] = 'field_my_name' in CCK's implementation of
 *   hook_form_builder_load().
 * @return
 *   A single Form API element array.
 */
function form_builder_get_elements(&$form, $element_ids) {
  return FormBuilderFormBase::fromArray($form)->getFormArray();
}

/**
 * Set an element within a form structure.
 *
 * @deprecated Deprecated since 1.8 and will be removed in 2.0.
 *   Use the FormBuilderElement class instead of the info-arrays.
 *
 * @return
 *   TRUE if an element was updated, FALSE if it was not found.
 */
function form_builder_set_element(&$form, $element, &$entire_form = NULL, $parent_id = FORM_BUILDER_ROOT) {
  $form_obj = FormBuilderFormBase::fromArray($form);
  $r = $form_obj->setElementArray($element);
  $form = $form_obj->getFormArray();
  return $r;
}

/**
 * Unset an element within a form structure.
 *
 * @deprecated Deprecated since 1.8 and will be removed in 2.0.
 *   Use the FormBuilderElement class instead of the info-arrays.
 */
function form_builder_unset_element(&$form, $element_id) {
  $form_obj = FormBuilderFormBase::fromArray($form);
  $r = $form_obj->unsetElement($element_id);
  $form = $form_obj->getFormArray();
  return $r;
}

/**
 * Recursive function to check if an element exists at all within a form.
 *
 * @deprecated Use FormBuilderFormInterface::getElementIds() instead.
 */
function form_builder_get_element_ids($form) {
  return FormBuilderFormBase::fromArray($form)->getElementIds();
}

/**
 * Recursive function to get the types of all element within a form.
 *
 * @deprecated Use FormBuilderFormInterface::getElementTypes() instead.
 */
function form_builder_get_element_types($form) {
  return FormBuilderFormBase::fromArray($form)->getElementTypes();
}

/**
 * Loader function to retrieve a form builder configuration array.
 *
 * @deprecated Use FormBuilderLoader::fromStorage() instead.
 *
 * @param $form_type
 *   The type of form being edited. Usually the name of the providing module.
 * @param $form_id
 *   The unique identifier for the form being edited with the type.
 */
function form_builder_load_form($form_type, $form_id) {
  return FormBuilderLoader::instance()
    ->fromStorage($form_type, $form_id)
    ->getFormArray();
}

/**
 * Execute the save methods for a form array.
 *
 * @deprecated Use FormBuilderFormInterface::saveToStorage().
 */
function form_builder_save_form(&$form, $form_type, $form_id) {
  FormBuilderFormBase::fromArray($form)->saveToStorage();
}

/**
 * Helper function to sort elements by 'weight' and 'title'.
 */
function _form_builder_sort($a, $b) {
  $a_weight = (is_array($a) && isset($a['weight'])) ? $a['weight'] : 0;
  $b_weight = (is_array($b) && isset($b['weight'])) ? $b['weight'] : 0;
  if ($a_weight == $b_weight) {
    if (!isset($b['title'])) {
      return -1;
    }
    if (!isset($a['title'])) {
      return 1;
    }
    return strcasecmp($a['title'], $b['title']);
  }
  return ($a_weight < $b_weight) ? -1 : 1;
}
