<?php

class FormBuilderFormBaseTest extends DrupalUnitTestCase {

  public static function getInfo() {
    return array(
      'name' => 'FormBuilderFormBase unit tests.',
      'description' => 'Tests form element handling.',
      'group' => 'Form builder',
    );
  }

  protected function emptyForm() {
    return new FormBuilderFormBase('webform', 'test', NULL, array(), array(), NULL);
  }

  public function tearDown() {
    parent::tearDown();
    FormBuilderFormBase::purge(0);
    FormBuilderLoader::instance()->fromCache(NULL, NULL, NULL, TRUE);
  }

  /**
   * @cover FormBuilderLoader::fromCache
   * @cover FormBuilderFormBase::load
   * @cover FormBuilderFormBase::save
   */
  public function testSaveAndLoad() {
    $loader = FormBuilderLoader::instance();
    $form = $loader->getForm('webform', 'test', 'test', array());
    $form->save();
    $this->assertEqual(
      $form->getFormArray(),
      $loader->fromCache('webform', 'test', 'test')->getFormArray()
    );

  }

  /**
   * @covers FormBuilderFormBase::setElementArray
   * @covers FormBuilderFormBase::getElement
   * @covers FormBuilderFormBase::getElementArray
   * @covers FormBuilderFormBase::getFormArray
   * @covers FormBuilderFormBase::addDefaults
   */
  public function testSetElementArray() {
    $form = $this->emptyForm();
    $a['#form_builder']['element_id'] = 'A';
    $a['#key'] = 'a';
    $a['#type'] = 'textfield';
    $this->assertEqual('A', $form->setElementArray($a));
    $rform = $form->getFormArray();
    $this->assertArrayHasKey('a', $rform);

    $a['#key'] = 'x';
    $this->assertEqual('A', $form->setElementArray($a));
    $rform = $form->getFormArray();
    $this->assertArrayNotHasKey('a', $rform);
    $this->assertArrayHasKey('x', $rform);

    $b['#key'] = 'b';
    $b['#type'] = 'textfield';
    $b['#form_builder'] = array('element_id' => 'B', 'parent_id' => 'A');
    $this->assertEqual('B', $form->setElementArray($b));
    $this->assertArrayNotHasKey('b', $form->getFormArray());
    $this->assertArrayHasKey('b', $form->getElementArray('A'));

    $b['#form_builder']['parent_id'] = 'NON EXISTING';
    $this->assertFalse($form->setElementArray($b));
    $this->assertArrayHasKey('b', $form->getElementArray('A'));

    $b['#form_builder']['parent_id'] = FORM_BUILDER_ROOT;
    $this->assertEqual('B', $form->setElementArray($b));
    $this->assertArrayHasKey('b', $form->getFormArray());
    $this->assertArrayNotHasKey('b', $form->getElementArray('A'));
  }

  /**
   * @covers FormBuilderFormBase::getElementIds
   * @covers FormBuilderFormBase::unsetElement
   * @covers FormBuilderFormBase::unindexElements
   */
  public function test_unsetElementArray() {
    $form['a']['#type'] = 'textfield';
    $form['a']['#form_builder'] = array('element_id' => 'A');
    $form['a']['b'] = array('#type' => 'textfield');
    $form['a']['b']['#form_builder'] = array('element_id' => 'B');
    $form_obj =  new FormBuilderFormBase('webform', 'test', NULL, array(), $form);
    $this->assertEqual(array('A', 'B'), $form_obj->getElementIds());
    $form_obj->unsetElement('A');
    $this->assertEqual(array(), $form_obj->getElementIds());
  }

  /**
   * @covers FormBuilderFormBase::__construct
   * @covers FormBuilderFormBase::indexElements
   */
  public function testElementIdIndexing() {
    $form['a']['#type'] = 'textfield';
    $form['a']['#form_builder'] = array('element_id' => 'A');
    $form['a']['b'] = array('#type' => 'textfield');
    $form['a']['b']['#form_builder'] = array('element_id' => 'B');
    $form_obj = new FormBuilderFormBase('webform', 'test', NULL, array(), $form);
    $this->assertNotEmpty($form_obj->getElementArray('A'));
    $this->assertNotEmpty($form_obj->getElementArray('B'));
  }

  /**
   * Integration test _form_builder_add_element().
   *
   * @covers ::_form_builder_add_element
   * @covers ::form_builder_field_render
   * @covers FormBuilderFormBase::load
   * @covers FormBuilderFormBase::save
   * @covers FormBuilderFormBase::serialize
   * @covers FormBuilderFormBase::unserialize
   */
  public function test_form_builder_add_element() {
    module_load_include('inc', 'form_builder', 'includes/form_builder.admin');
    $loader = FormBuilderLoader::instance();
    $form = $loader->getForm('webform', 'test', 'test', array());
    $form->save();
    $data = _form_builder_add_element('webform', 'test', 'email', NULL, 'test', TRUE);
    $this->assertNotEmpty($data);
    $this->assertNotEmpty($data['html']);
  }

  /**
   * Integration test: Render textfield inside fieldset.
   *
   * @covers ::_form_builder_add_element
   * @covers ::form_builder_field_render
   * @covers FormBuilderFormBase::load
   * @covers FormBuilderFormBase::fromArray
   * @covers FormBuilderFormBase::setElementArray
   */
  public function test_render_fieldset() {
    module_load_include('inc', 'form_builder', 'includes/form_builder.admin');
    $loader = FormBuilderLoader::instance();
    $form = $loader->getForm('webform', 'test', 'test', array());
    $form->save();
    drupal_static_reset('drupal_html_id');
    $data = _form_builder_add_element('webform', 'test', 'fieldset', NULL, 'test', TRUE);
    $wrapper = simplexml_load_string($data['html']);
    // Test if element is properly wrapped.
    $this->assertEqual('form-builder-wrapper', (string) $wrapper['class']);
    $this->assertEqual('form-builder-title-bar', (string) $wrapper->div[0]['class']);
    $element = $wrapper->div[1];
    $this->assertEqual('form-builder-element form-builder-element-fieldset', (string) $element['class']);
    $this->assertNotEmpty($element->fieldset);
    $fieldset_id = $data['elementId'];

    $data = _form_builder_add_element('webform', 'test', 'textfield', NULL, 'test', TRUE);
    $this->assertNotEquals($fieldset_id, $data['elementId']);
    $textfield_id = $data['elementId'];

    $form = $loader->fromCache('webform', 'test', 'test');
    $element = $form->getElementArray($textfield_id);
    $element['#weight'] = 1;
    $element['#form_builder']['parent_id'] = $fieldset_id;
    $form->setElementArray($element);

    $form_array = $form->getFormArray();
    $this->assertEqual(array($fieldset_id), element_children($form_array));
    $this->assertEqual(array($textfield_id), element_children($form_array[$fieldset_id]));
  }
}
