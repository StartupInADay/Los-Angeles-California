<?php
/**
 * @file
 *  Fieldable Panel Pane Panel - Featured Resources migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFPPFeaturedResources extends SBPMigrationFPP {

  public function __construct($arguments) {
    $this->setProperties('featured_resources');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useFeaturedResources = TRUE;
    
    parent::__construct($arguments);

    $this->addPaneStyleFieldMapping('field_pane_style');

    $this->description = t('Import Featured Resources FPP.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}