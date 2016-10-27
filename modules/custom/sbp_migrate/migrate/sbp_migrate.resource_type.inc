<?php
/**
 * @file
 *  Taxonomy Industry Tags migration.
 */

class SBPMigrationTermResourceType extends SBPMigrationTerm {

  public function __construct($arguments) {
    $this->setProperties('resource_type');
    parent::__construct($arguments);
    $this->description = t('Import Business Needs terms.');

    //$this->dependencies = array('SBPMigrationTermDepartmentType','SBPMigrationTermBusinessNeeds','SBPMigrationTermBusinessOwnership','SBPMigrationTermIndustryTags');
    //$this->addSoftDependencies(array('SBPMigrationTermDepartmentType','SBPMigrationTermBusinessNeeds','SBPMigrationTermBusinessOwnership','SBPMigrationTermIndustryTags'));
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }

}