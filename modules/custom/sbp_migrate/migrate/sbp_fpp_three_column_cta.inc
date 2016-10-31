<?php
/**
 * @file
 *  Fieldable Panel Pane Panel - Three Column CTA migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFPPThreeColumnCTA extends SBPMigrationFPP {

  public function __construct($arguments) {
    $this->setProperties('three_column_cta');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useThreeColumn = TRUE;
    
    parent::__construct($arguments);
    
    $this->addFieldMapping('field_component_body', 'field_component_body');

    $this->addPaneStyleFieldMapping('field_pane_style');

    $this->description = t('Import Three Column CTA.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}