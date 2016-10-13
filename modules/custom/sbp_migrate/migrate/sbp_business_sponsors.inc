<?php
/**
 * @file
 *  Business Portal Landing Page migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationNodeBusinessSponsor extends SBPMigrationNode {

  public function __construct($arguments) {
    $this->setProperties('business_portal_sponsors');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useUrl = TRUE;
    $this->useSponsorCust = TRUE;
    $this->addFieldMapping('pathauto')->defaultValue(0);
    $this->addFieldMapping('comment', 'comment');
    $this->addUrlFieldMappings('field_sponsor_url');
    parent::__construct($arguments);

    $this->description = t('Import Sponsor Pages.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}