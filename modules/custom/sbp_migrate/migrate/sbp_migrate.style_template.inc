<?php
/**
 * @file
 *  Taxonomy Style Template migration.
 */

class SBPMigrationTermStyleTemplate extends SBPMigrationTerm {

  public function __construct($arguments) {
    $this->setProperties('style_template');
    parent::__construct($arguments);
    $this->description = t('Import Style Template terms.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }

}