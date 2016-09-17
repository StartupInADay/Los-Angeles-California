<?php

/**
 * @file
 * Support for Universally Unique Identifier (uuid) module.
 */

/**
 * Generic UUID Handler.
 */
class MigrateUuidHandler extends MigrateDestinationHandler {
  public function __construct() {
    $this->registerTypes(array('user', 'comment', 'taxonomy_term'));
  }

  /**
   * Make the destination field visible.
   */
  public function fields() {
    if (module_exists('uuid')) {
      return array(
        'uuid' => t('Universally Unique IDentifier'),
      );
    }
    else {
      return array();
    }
  }
}

/**
 * Node specific UUID Handler. This needs to be separate since nodes have the
 * revision_uuid field as well.
 */
class MigrateUuidNodeHandler extends MigrateDestinationHandler {
  public function __construct() {
    $this->registerTypes(array('node'));
  }

  /**
   * Make the destination field visible.
   */
  public function fields() {
    if (module_exists('uuid')) {
      return array(
        'uuid' => t('Universally Unique IDentifier'),
        'revision_uuid' => t('Universally Unique IDentifier (revision)'),
      );
    } else {
      return array();
    }
  }
}
