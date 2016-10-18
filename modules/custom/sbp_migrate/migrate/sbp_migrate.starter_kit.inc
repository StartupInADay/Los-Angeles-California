<?php
/**
 * @file
 *  Migration of Demo Business Portal Starter Kits.
 */

class SBPMigrationNodeStarterKit extends SBPMigrationNode {

  public function __construct($arguments) {
    $this->setProperties('sbp_starter_kit');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useStarterKitCust = TRUE;
    $this->addFieldMapping('pathauto')->defaultValue(0);
    
    $this->dependencies = array('SBPMigrationTermDepartmentType','SBPMigrationTermTermsDefinitions');
    
    // Text fields
    $this->addFieldMapping('field_guide_title', 'field_guide_title');
    $this->addFieldMapping('field_guide_description', 'field_guide_description');
    $this->addFieldMapping('field_permit_description', 'field_permit_description');
    $this->addFieldMapping('field_checklist_description', 'field_checklist_description');
    
    // Files
    $this->addGuidePdfFieldMapping('field_guide_pdf');
    $this->addPermitPdfFieldMapping('field_permit_pdf');
    
    // Images
    $this->addGuideImageFieldMapping('field_guide_image');
    $this->addPermitImageFieldMapping('field_permit_image');
    
    parent::__construct($arguments);

    $this->description = t('Import Starter Kit Pages.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}

