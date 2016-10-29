<?php
/**
 * @file
 *  Incentives migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationNodeWizardScreen extends SBPMigrationNode {

  public function __construct($arguments) {
    $this->setProperties('sbp_wizard_screen');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useWizardScreenCust = TRUE;
    $this->useNotBody = TRUE;
    $this->addFieldMapping('pathauto')->defaultValue(0);

    $this->addFieldMapping('field_screen_type', 'field_screen_type');
    
    $this->addSectionFieldMapping('field_section');
    $this->addDescriptionFieldMapping('field_description');
    $this->addTipFieldMapping('field_tip');
    
    parent::__construct($arguments);

    $this->description = t('Import Resource Pages.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}