<?php
/**
 * @file
 *  Taxonomy Business Portal Wizard Section migration.
 */

class SBPMigrationTermBusinessPortalWizardSections extends SBPMigrationTerm {

  public function __construct($arguments) {
    $this->setProperties('business_portal_wizard_sections');
    $this->useBPWSCust = TRUE;
    parent::__construct($arguments);
    $this->description = t('Import Business Portal Wizard Section terms.');
    
    $this->addFieldMapping('field_color', 'field_color');
    $this->addFieldMapping('field_contextual_color', 'field_contextual_color');
    $this->addFieldMapping('field_drawer_color', 'field_drawer_color');
    $this->addFieldMapping('field_jurisdiction', 'field_jurisdiction');


    $this->addImageFieldMapping('field_image');
    $this->addIllustrationFieldMapping('field_illustration');

    $this->dependencies = array('SBPMigrationTermDepartmentType','SBPMigrationTermBusinessNeeds','SBPMigrationTermBusinessOwnership','SBPMigrationTermIndustryTags');
    $this->addSoftDependencies(array('SBPMigrationTermDepartmentType','SBPMigrationTermBusinessNeeds','SBPMigrationTermBusinessOwnership','SBPMigrationTermIndustryTags'));
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }

}