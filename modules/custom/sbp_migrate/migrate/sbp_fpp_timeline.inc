<?php
/**
 * @file
 *  Fieldable Panel Pane Panel - Timeline migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFPPTimeline extends SBPMigrationFPP {

  public function __construct($arguments) {
    $this->setProperties('timeline');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useTimeline = TRUE;
    
    parent::__construct($arguments);

    $this->addPaneStyleFieldMapping('field_pane_style');

    $this->description = t('Import Timeline FPP.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}