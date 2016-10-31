<?php
/**
 * @file
 *  Fieldable Panel Pane Panel - Four Column CTA migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFPPFourColumnCTA extends SBPMigrationFPP {

  public function __construct($arguments) {
    $this->setProperties('four_column_cta');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useFourCol = TRUE;
    
    parent::__construct($arguments);

    $this->addPaneStyleFieldMapping('field_pane_style');

    $this->description = t('Import Four Column CTA.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}