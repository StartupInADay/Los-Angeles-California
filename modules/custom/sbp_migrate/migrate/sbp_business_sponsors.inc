<?php
/**
 * @file
 *  Business Portal Sponsor Page migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationNodeBusinessSponsor extends SBPMigrationNode {

  public function __construct($arguments) {
    $this->setProperties('business_portal_sponsors');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useUrl = TRUE;
    $this->useLogo = TRUE;
    $this->useSponsorCust = TRUE;
    $this->useNotBody = TRUE;
    $this->addFieldMapping('pathauto')->defaultValue(0);
    $this->addFieldMapping('comment', 'comment');
    $this->addUrlFieldMapping('field_sponsor_url');
    $this->addLogoFieldMapping('field_logo');
    parent::__construct($arguments);

    $this->description = t('Import Sponsor Pages.');
    $this->dependencies = array('SBPMigrationTermTermsDefinitions');
    $this->addSoftDependencies(array('SBPMigrationTermTermsDefinitions'));
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}