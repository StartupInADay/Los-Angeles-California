<?php
/**
 * @file
 *  Taxonomy Business Ownership migration.
 */

class SBPMigrationTermBusinessOwnership extends SBPMigrationTerm {

  public function __construct($arguments) {
    $this->setProperties('business_ownership');
    parent::__construct($arguments);
    $this->description = t('Import Industry Tag terms.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }

}