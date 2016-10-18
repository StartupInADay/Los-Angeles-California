<?php
/**
 * @file
 *  Taxonomy Department Type migration.
 */

class SBPMigrationTermDepartmentType extends SBPMigrationTerm {

  public function __construct($arguments) {
    $this->setProperties('department_type');
    parent::__construct($arguments);
    $this->description = t('Import Department Type terms.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }

}