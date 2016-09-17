<?php

namespace Drupal\collection;

class Collection extends \Entity {
  public $name;
  public $label;
  public $status;
  public $module;
  public $cid;

  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, 'collection');
  }

  protected function defaultUri() {
    return array('path' => "admin/structure/collection/{$this->identifier()}");
  }
}
