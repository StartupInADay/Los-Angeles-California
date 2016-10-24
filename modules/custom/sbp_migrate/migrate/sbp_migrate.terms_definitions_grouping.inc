<?php
/**
 * @file
 *  Taxonomy Terms Definitions Groupings migration.
 */

class SBPMigrationTermTermsDefinitionsGrouping extends SBPMigrationTerm {

  public function __construct($arguments) {
    $this->setProperties('terms_definitions_grouping');
    parent::__construct($arguments);
    $this->description = t('Import Business Needs terms.');
    
    $this->dependencies = array('SBPMigrationTermDepartmentType','SBPMigrationTermBusinessNeeds','SBPMigrationTermBusinessOwnership','SBPMigrationTermIndustryTags','SBPMigrationTermResourceType','SBPMigrationTermStyleTemplate');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }

}