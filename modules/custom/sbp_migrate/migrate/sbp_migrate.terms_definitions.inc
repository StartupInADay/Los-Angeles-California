<?php
/**
 * @file
 *  Taxonomy Industry Tags migration.
 */

class SBPMigrationTermTermsDefinitions extends SBPMigrationTerm {

  public function __construct($arguments) {
    $this->setProperties('terms_definitions');
    parent::__construct($arguments);
    $this->description = t('Import Terms and Definitions terms.');
    $this->addFieldMapping('field_term_definitions_grouping', 'grouping');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    $columns[] = array('grouping', 'Group term belongs to, ie A-H');
    return $columns;
  }

}