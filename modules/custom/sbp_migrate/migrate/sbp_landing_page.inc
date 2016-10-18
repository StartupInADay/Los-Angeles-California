<?php
/**
 * @file
 *  Business Portal Landing Page migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationNodeLandingPage extends SBPMigrationNode {

  public function __construct($arguments) {
    $this->setProperties('sbp_landing_page');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->addFieldMapping('pathauto')->defaultValue(0);
    parent::__construct($arguments);
    $this->description = t('Import Landing Pages.');

    $this->dependencies = array('SBPMigrationTermTermsDefinitions');
  }
}