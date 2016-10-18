<?php
/**
 * @file
 *  Incentives migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationNodeIncentives extends SBPMigrationNode {

  public function __construct($arguments) {
    $this->setProperties('incentives');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useIncentivesCust = TRUE;
    $this->useResourcesIncentivesSharedCust = TRUE;
    $this->addFieldMapping('pathauto')->defaultValue(0);
    $this->addIncentiveUrlFieldMapping('field_incentive_url');
    $this->addMainFeatureFieldMapping('field_main_feature');
    $this->addSecondaryFeatureFieldMapping('field_secondary_feature');
    $this->addBusinessNeedFieldMapping('field_business_need');
    $this->addIndustryTagFieldMapping('field_industry_tag');
    $this->addResourceTypeFieldMapping('field_resource_type');
    $this->addBusinessOwnershipFieldMapping('field_business_ownership');
    parent::__construct($arguments);

    $this->description = t('Import Resource Pages.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}