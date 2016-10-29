<?php
/**
 * @file
 *  Incentives migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFPPSectionHomepage extends MigrateDestinationFieldablePanelsPanes {

  public function __construct($arguments) {
    $this->setProperties('fpp_section_homepage');
    $this->useTags = FALSE;
    $this->useImage = FALSE;

    parent::__construct($arguments);

    $this->description = t('Import Resource Pages.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}