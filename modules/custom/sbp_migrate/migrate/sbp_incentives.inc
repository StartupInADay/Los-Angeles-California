<?php
/**
 * @file
 *  Online Resources migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationNodeOnlineResource extends SBPMigrationNode {

  public function __construct($arguments) {
    $this->setProperties('online_resources');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->useResourceIncentiveSharedCust = TRUE;
    $this->useResourceCust = TRUE;
    $this->addFieldMapping('pathauto')->defaultValue(0);
    $this->addHeroFieldMapping('field_hero');
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