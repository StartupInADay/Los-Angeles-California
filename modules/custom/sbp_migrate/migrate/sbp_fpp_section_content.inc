<?php
/**
 * @file
 *  Fieldable Panel Pane Panel - Section Content migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFPPSectionContent extends SBPMigrationFPP {

  public function __construct($arguments) {
    $this->setProperties('section_content');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useSectionContent = TRUE;
    
    parent::__construct($arguments);

    $this->addComponentBodyFieldMapping('field_component_body');
    $this->addPaneStyleFieldMapping('field_pane_style');
    $this->addCallsToActionFieldMapping('field_calls_to_action');
    $this->addBackgroundImageFieldMapping('field_background_image');

    $this->description = t('Import Section Content FPP.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}