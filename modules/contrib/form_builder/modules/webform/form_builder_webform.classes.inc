<?php


class FormBuilderWebformForm extends FormBuilderFormBase {
  /**
   * Create a webform component array based the form_builder cache.
   *
   * @param string $element_id
   *   Unique ID of the element.
   * @return array
   *   A webform component array.
   */
  public function getComponent($element_id) {
    module_load_include('inc', 'form_builder_webform', 'form_builder_webform.components');

    $element = $this->getElementArray($element_id);
    $component = $element['#webform_component'];
    $type = $component['type'];

    $component['email'] = 0;
    $component['nid'] = $this->formId;
    $component['weight'] = $element['#weight'];

    // Set the parent ID to default to the form base.
    $component['pid'] = FORM_BUILDER_ROOT;

    // Set the parent ID for the component if it is nested inside another component.
    $parent = $this->getElementArray($element['#form_builder']['parent_id']);
    if ($parent && isset($parent['#webform_component']['cid'])) {
      $component['pid'] = $parent['#webform_component']['cid'];
    }

    // Allow each component to set any specific settings that can't be mapped.
    if ($saved_component = form_builder_webform_component_invoke($type, 'form_builder_save', $component, $element)) {
      $component = $saved_component;
    }

    return $component;
  }
}

class FormBuilderWebformElement extends FormBuilderElementBase {

  /**
   * {@inheritdoc}
   */
  protected function setProperty($property, $value) {
    $component = &$this->element['#webform_component'];
    $properties = $this->getProperties();
    $properties[$property]->setValue($component, $value);
  }

  /**
   * {@inheritdoc}
   */
  public function render() {
    $element = &$this->element;
    if (isset($element['#webform_component'])) {
      $component = &$element['#webform_component'];
      $new_element = webform_component_invoke($component['type'], 'render', $component, NULL, FALSE);
      // Preserve the #weight. It may have been changed by the positions form.
      $new_element['#weight'] = $element['#weight'];
      return $new_element + $element;
    }
    return $element;
  }

}

class FormBuilderWebformProperty extends FormBuilderPropertyBase {

  protected $storageParents;

  /**
   * {@inheritdoc}
   */
  public function __construct($property, $params, $form_type_name) {
    $params += array('storage_parents' => array($property));
    parent::__construct($property, $params, $form_type_name);
    $this->storageParents = $params['storage_parents'];
  }

  /**
   * {@inheritdoc}
   */
  public function setValue(&$component, $value) {
    drupal_array_set_nested_value($component, $this->storageParents, $value);
  }
}

/**
 * Special handling for the mandatory -> required rename in webform4.
 */
class FormBuilderWebformPropertyRequired extends FormBuilderPropertyBase {
  public function setValue(&$component, $value) {
    $component['required'] = $value; // webform 4
    $component['mandatory'] = $value; // webform 3
  }
}

/**
 * Special handling for $component['value'].
 */
class FormBuilderWebformPropertyValue extends FormBuilderWebformProperty {
  /**
   * {@inheritdoc}
   */
  public function setValue(&$component, $value) {
    if (is_array($value)) {
      $value = implode(',', $value);
    }
    parent::setValue($component, $value);
  }
}

