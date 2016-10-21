<?php
/**
 * @file
 *  Incentives migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationNodeDepartment extends SBPMigrationNode {

  public function __construct($arguments) {
    $this->setProperties('sbp_department');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useDepartmentCust = TRUE;
    $this->useNotBody = TRUE;
    $this->addFieldMapping('pathauto')->defaultValue(0);

    $this->dependencies = array('SBPMigrationTermDepartmentType');

    $this->addFieldMapping('field_phone', 'field_phone');
    
    $this->addDescriptionFieldMapping('field_description');
    $this->addLinkFieldMapping('field_link');
    $this->addDepartmentTypeFieldMapping('field_department_type');
    $this->addMainFeatureFieldMapping('field_main_feature');
    $this->addSecondaryFeatureFieldMapping('field_secondary_feature');
    
    parent::__construct($arguments);

    $this->description = t('Import Resource Pages.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}