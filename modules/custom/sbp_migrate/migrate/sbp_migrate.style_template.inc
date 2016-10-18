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
    $this->addStyleClassFieldMapping('field_style_class');
    
    $this->description = t('Import Style Template terms.');
  }

  protected function addStyleClassFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_style_class');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }

}