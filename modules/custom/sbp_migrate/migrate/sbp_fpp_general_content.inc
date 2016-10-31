<?php
/**
 * @file
 *  Fieldable Panel Pane Panel - General Content migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFPPGeneralContent extends SBPMigrationFPP {

  public function __construct($arguments) {
    $this->setProperties('general_content');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useGeneralContent = TRUE;
    
    parent::__construct($arguments);

    $this->addFieldMapping('field_intro_title','field_intro_title');
    $this->addFieldMapping('field_description','field_description');

    $this->addPaneStyleFieldMapping('field_pane_style');
    $this->addCallToActionFieldMapping('field_call_to_action');

    $this->description = t('Import General Content FPP.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}