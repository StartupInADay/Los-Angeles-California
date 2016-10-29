<?php
/**
 * @file
 *  Taxonomy Style Template migration.
 */

class SBPMigrationTermStyleTemplate extends SBPMigrationTerm {

  public function __construct($arguments) {
    $this->setProperties('style_template');
    $this->useStyleClass = TRUE;
    parent::__construct($arguments);
    
    $this->description = t('Import Style Template terms.');
    
    $this->addFieldMapping('field_style_class', 'field_style_class');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }

}