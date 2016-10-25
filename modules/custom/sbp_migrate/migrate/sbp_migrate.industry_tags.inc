<?php
/**
 * @file
 *  Taxonomy Industry Tags migration.
 */

class SBPMigrationTermIndustryTags extends SBPMigrationTerm {

  public function __construct($arguments) {
    $this->setProperties('industry_tags');
    parent::__construct($arguments);
    $this->description = t('Import Industry Tag terms.');

    $this->addSoftDependencies(array('SBPMigrationTermDepartmentType','SBPMigrationTermBusinessNeeds','SBPMigrationTermBusinessOwnership'));
    $this->dependencies = array('SBPMigrationTermDepartmentType','SBPMigrationTermBusinessNeeds','SBPMigrationTermBusinessOwnership');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }

}