<?php

class CollectionWebTestCase extends DrupalWebTestCase {
  protected $profile = 'testing';

  public function setUp() {
    parent::setUp(array('collection', 'collection_field', 'xautoload', 'entity'));

    $field = array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_collection',
      'foreign keys' => array(),
      'indexes' => array(
        'collection' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'collection_field',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'collection',
    );
    field_create_field($field);

    $instance = array(
      'bundle' => 'article',
      'entity_type' => 'node',
      'field_name' => 'field_collection',
      'label' => 'Collection',
      'widget' => array(
        'module' => 'collections',
        'type' => 'collection',
      ),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
        ),
      ),
    );
    field_create_instance($instance);

    $this->createCollection();
    $this->node = $this->createNode();
  }

  protected function createCollection() {
    $values = array(
      'label' => 'Test Collecction 1',
      'name' => 'test_collection_1',
    );

    $collection = new \Drupal\collection\Collection($values);
    $collection->save();
  }


  protected function createNode() {
    $node = array();
    $node['type'] = 'article';
    $node['field_collection'][LANGUAGE_NONE][0]['value'] = "test_collection_1";
    $node['uid'] = 1;
    $node['language'] = LANGUAGE_NONE;
    $node['status'] = 1;

    return $this->drupalCreateNode($node);
  }
}
