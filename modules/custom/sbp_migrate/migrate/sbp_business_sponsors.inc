<?php
/**
 * @file
 *  Business Portal Landing Page migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationNodeBusinessSponsor extends SBPMigrationNode {

  public function __construct($arguments) {
    $this->setProperties('sbp_business_sponsors');
    $this->useTags = FALSE;
    $this->useImage = TRUE;
    $this->addFieldMapping('pathauto')->defaultValue(0);
    parent::__construct($arguments);
    $this->description = t('Import Sponsor Pages.');
  }
  
  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}