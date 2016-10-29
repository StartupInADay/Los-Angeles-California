<?php
/**
 * @file
 *  Online Resources migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationNodeOnlineResources extends SBPMigrationNode {

  public function __construct($arguments) {
    $this->setProperties('online_resource');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useResourcesCust = TRUE;
    $this->useResourcesIncentivesSharedCust = TRUE;
    $this->addFieldMapping('pathauto')->defaultValue(0);

    $this->addDocumentFieldMapping('field_document');
    $this->addMainFeatureFieldMapping('field_main_feature');
    $this->addSecondaryFeatureFieldMapping('field_secondary_feature');
    $this->addBusinessNeedsFieldMapping('field_business_need');
    $this->addIndustryTagFieldMapping('field_industry_tag');
    $this->addBusinessOwnershipFieldMapping('field_business_ownership');
    $this->addResourceTypeFieldMapping('field_resource_type');
    parent::__construct($arguments);

    $this->description = t('Import Resource Pages.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}