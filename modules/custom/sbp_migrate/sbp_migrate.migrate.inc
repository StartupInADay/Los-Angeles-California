<?php

/**
 * @file
 *  Base migrations used in SBP.
 */

/**
 * Base migration for SBP.
 */
abstract class SBPMigration extends Migration {

  /**
   * The directory from which CSV files and media assets will be imported.
   *
   * To replace the default imports with customized versions, create a 'custom'
   * directory in the sbp_migrate directory and place custom import
   * files there.
   */
  protected $baseImportDirectory;

  /**
   * The relative path from the $baseImportDirectory to a directory with images
   * for import.
   */
  protected $imageDirectory;

  /**
   * The name of a CSV file to be imported.
   */
  protected $importFileName;

  /**
   * The type of migration, used as a bundle value where appropriate and also
   * for the base filename of the import CSV file.
   */
  protected $bundle;

  public function processImport(array $options = array()) {
    parent::processImport($options);
    // Do not force menu rebuilding. Otherwise pathauto will try to rebuild
    // in each node_insert invocation.
    variable_set('menu_rebuild_needed', FALSE);
  }

  /**
   * Suppress distracting install-time messages.
   */
  protected function progressMessage($result) {
    if (!drupal_installation_attempted()) {
      parent::progressMessage($result);
    }
  }

  protected function setProperties($bundle) {
    $path = drupal_get_path('module', 'sbp_migrate');
    $this->baseImportDirectory = (file_exists($path . '/local') ? $path . '/local' : $path . '/import');
    $this->importFileName = $bundle . '.csv';
    $this->bundle = $bundle;
  }

  protected function addImageCsvColumns(&$columns) {
    $columns[] = array('image', 'Image');
    $columns[] = array('image_title', 'Image title');
    $columns[] = array('image_alt', 'Image alt');
  }

  protected function addLogoCsvColumns(&$columns) {
    $columns[] = array('field_logo', 'Logo filename');
    $columns[] = array('field_logo_alt', 'Logo alt');
    $columns[] = array('field_logo_title', 'Logo title');
  }

  protected function addUrlCsvColumns(&$columns) {
    $columns[] = array('field_sponsor_url', 'Sponsor Link Field');
    $columns[] = array('field_sponsor_url_title', 'Sponsor URL title');
    $columns[] = array('field_sponsor_url_attributes', 'Sponsor URL attributes');
  }

  protected function addSponsorCustCsvColumns(&$columns) {
    $columns[] = array('comment', 'Comment');
  }

  //TODO
  protected function addResourcesCust(&$columns) {
    $columns[] = array('field_document_display', 'Resource Document Display');
    $columns[] = array('field_document_description', 'Resource Document Description');
  }

  protected function addIncentivesCust(&$columns) {
    $columns[] = array('field_incentive_url', 'Field Incentive URL Field');
    $columns[] = array('field_incentive_url_title', 'Field Incentive Title');
    $columns[] = array('field_incentive_url_attributes', 'Field Incentive Attributes');
  }

  protected function addResourcesIncentivesSharedCust(&$columns) {
    $columns[] = array('field_main_feature', 'Resource Main Feature');
    $columns[] = array('field_secondary_feature', 'Resource Secondary Feature');
    $columns[] = array('field_business_need', 'Resource Business Needs Field');
    $columns[] = array('field_industry_tag', 'Resource Industry Tags Field');
    $columns[] = array('field_business_ownership', 'Resource Business Ownership Field');
    $columns[] = array('field_resource_type', 'Resource Resource Type Field');
  }

  protected function addStarterKitCust(&$columns) {
    $columns[] = array('field_guide_title', 'Guide Title');
    $columns[] = array('field_guide_description', 'Guide Description');
    $columns[] = array('field_permit_description', 'Permit Description');
    $columns[] = array('field_checklist_description', 'Checklist Description');
    $columns[] = array('field_guide_pdf_description', 'Guide PDF Description');
    $columns[] = array('field_guide_pdf_display', 'Guide PDF Display');
    $columns[] = array('field_permit_pdf_description', 'Permit PDF Description');
    $columns[] = array('field_permit_pdf_display', 'Permit PDF Display');
    $columns[] = array('field_guide_image', 'Guide filename');
    $columns[] = array('field_guide_image_alt', 'Guide alt');
    $columns[] = array('field_guide_image_title', 'Guide title');
    $columns[] = array('field_permit_image', 'Permit filename');
    $columns[] = array('field_permit_image_alt', 'Permit alt');
    $columns[] = array('field_permit_image_title', 'Permit title');
  }

  protected function addDepartmentCust(&$columns) {
    $columns[] = array('field_phone', 'Department Phone');
    $columns[] = array('field_description', 'Department Description');
    $columns[] = array('field_link', 'Department Link');
    $columns[] = array('field_link_title', 'Department Link Title');
    $columns[] = array('field_link_attributes', 'Department Link Attributes');
    $columns[] = array('field_department_type', 'Checklist Description');
    $columns[] = array('field_main_feature', 'Guide PDF Description');
    $columns[] = array('field_secondary_feature', 'Guide PDF Display');
  }

  protected function addWizardScreenCust(&$columns) {
    $columns[] = array('field_screen_type', 'Screen Type');
    $columns[] = array('field_section', 'Screen Section');
    $columns[] = array('field_description', 'Screen Description');
    $columns[] = array('field_tip', 'Section Tip');
  }

  protected function getImportFilePath() {
    return $this->baseImportDirectory . '/' . $this->importFileName;
  }

  protected function getImageDirectoryPath() {
    return $this->baseImportDirectory . '/' . $this->imageDirectory;
  }
}

/**
 * Base migration for nodes in SBP.
 */
abstract class SBPMigrationNode extends SBPMigration {

  /**
   * Whether or not the node type supports a field_tags taxonomy term reference
   * field.
   */
  protected $useTags = TRUE;

  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->description = t('Import node content.');
    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV($this->getImportFilePath(), $this->csvcolumns(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationNode($this->bundle);

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('uid')->defaultValue(1);
    $this->addFieldMapping('status')->defaultValue(1);
    $this->addFieldMapping('language')->defaultValue(LANGUAGE_NONE);
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('promote', 'promote');
    $this->addFieldMapping('path', 'path');

    if ($this->useImages = FALSE) {
      if (isset($this->useSponsorCust) && $this->useSponsorCust != 1) {
        $this->addImageFieldMappings('field_image');
      }
      if (!isset($this->useSponsorCust)) {
        $this->addImageFieldMappings('field_image');
      }
    }
    if ($this->useImages = FALSE && isset($this->useSponsorCust) && $this->useSponsorCust != 1) {
      $this->addImageFieldMappings('field_image');
    }
    if(!isset($this->useNotBody) || isset($useNotBody) && $this->useNotBody != 1) {
      $this->addFieldMapping('body', 'body');
      $this->addFieldMapping('body:format')->defaultValue('filtered_html');
    }

    if ($this->useTags) {
      $this->addFieldMapping('field_tags', 'tags')
         ->separator(', ');
      $this->addFieldMapping('field_tags:create_term')
        ->defaultValue(TRUE);
    }
    
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
  }

  protected function addImageFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'image');
    $this->addFieldMapping($field_name . ':source_dir')
      ->defaultValue($this->getImageDirectoryPath());
    $this->addFieldMapping($field_name . ':file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping($field_name . ':alt', 'image_alt');
    $this->addFieldMapping($field_name . ':title', 'image_title');
  }

  protected function addLogoFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_logo');
    $this->addFieldMapping($field_name . ':alt', 'field_logo_alt');
    $this->addFieldMapping($field_name . ':title', 'field_logo_title');
    $this->addFieldMapping($field_name . ':file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping($field_name . ':source_dir')
      ->defaultValue($this->getImageDirectoryPath());
  }

  protected function addUrlFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_sponsor_url');
    $this->addFieldMapping($field_name . ':title', 'field_sponsor_url_title');
    $this->addFieldMapping($field_name . ':attributes', 'field_sponsor_url_attributes');
    $this->addFieldMapping($field_name . ':language')->defaultValue(LANGUAGE_NONE);
  }

  protected function addDocumentFieldMapping($field_name) {
    $this->addFieldMapping($field_name . ':file_class')->defaultValue('MigrateFileUri');
    $this->addFieldMapping($field_name . ':file_replace')->defaultValue(FILE_EXISTS_RENAME);
    $this->addFieldMapping($field_name . ':preserve_files')->defaultValue(FALSE);
    $this->addFieldMapping($field_name . ':source_dir')->defaultValue($this->getImageDirectoryPath());
    $this->addFieldMapping($field_name . ':destination_file')->defaultValue('sample.png');
    $this->addFieldMapping($field_name . ':description', 'field_document_description');
    $this->addFieldMapping($field_name . ':display', 'field_document_display');
  }

  protected function addIncentiveUrlFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_incentive_url');
    $this->addFieldMapping($field_name . ':title', 'field_incentive_url_title');
    $this->addFieldMapping($field_name . ':attributes', 'field_incentive_url_attributes');
    $this->addFieldMapping($field_name . ':language')->defaultValue(LANGUAGE_NONE);
  }

  protected function addMainFeatureFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_main_feature');
  }

  protected function addSecondaryFeatureFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_secondary_feature');
  }

  protected function addBusinessNeedsFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_business_need');
    $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');
  }

  protected function addIndustryTagFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_industry_tag');
    $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');
  }

  protected function addBusinessOwnershipFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_business_ownership');
    $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');

  }

  protected function addResourceTypeFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_resource_type');
    $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');
  }

  protected function addGuidePdfFieldMapping($field_name) {
    $this->addFieldMapping($field_name . ':file_class')->defaultValue('MigrateFileUri');
    $this->addFieldMapping($field_name . ':file_replace')->defaultValue(FILE_EXISTS_RENAME);
    $this->addFieldMapping($field_name . ':preserve_files')->defaultValue(FALSE);
    $this->addFieldMapping($field_name . ':source_dir')->defaultValue($this->getImageDirectoryPath());
    $this->addFieldMapping($field_name . ':destination_file')->defaultValue('sample.png');
    $this->addFieldMapping($field_name . ':description', 'field_guide_pdf_description');
    $this->addFieldMapping($field_name . ':display', 'field_guide_pdf_display');
  }

  protected function addPermitPdfFieldMapping($field_name) {
    $this->addFieldMapping($field_name . ':file_class')->defaultValue('MigrateFileUri');
    $this->addFieldMapping($field_name . ':file_replace')->defaultValue(FILE_EXISTS_RENAME);
    $this->addFieldMapping($field_name . ':preserve_files')->defaultValue(FALSE);
    $this->addFieldMapping($field_name . ':source_dir')->defaultValue($this->getImageDirectoryPath());
    $this->addFieldMapping($field_name . ':destination_file')->defaultValue('sample.png');
    $this->addFieldMapping($field_name . ':description', 'field_permit_pdf_description');
    $this->addFieldMapping($field_name . ':display', 'field_permit_pdf_display');
  }

  protected function addGuideImageFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_guide_image');
    $this->addFieldMapping($field_name . ':alt', 'field_guide_image_alt');
    $this->addFieldMapping($field_name . ':title', 'field_guide_image_title');
    $this->addFieldMapping($field_name . ':file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping($field_name . ':source_dir')
      ->defaultValue($this->getImageDirectoryPath());
  }

  protected function addPermitImageFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_permit_image');
    $this->addFieldMapping($field_name . ':alt', 'field_permit_image_alt');
    $this->addFieldMapping($field_name . ':title', 'field_permit_image_title');
    $this->addFieldMapping($field_name . ':file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping($field_name . ':source_dir')
      ->defaultValue($this->getImageDirectoryPath());
  }

  protected function addDescriptionFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_description');
    $this->addFieldMapping($field_name . ':format')->defaultValue('filtered_html');
  }

  protected function addTipFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_tip');
    $this->addFieldMapping($field_name . ':format')->defaultValue('filtered_html');
  }

  protected function addLinkFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_link');
    $this->addFieldMapping($field_name . ':title', 'field_link_title');
    $this->addFieldMapping($field_name . ':attributes', 'field_link_attributes');
    $this->addFieldMapping($field_name . ':language')->defaultValue(LANGUAGE_NONE);
  }

  protected function addDepartmentTypeFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_department_type');
    $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');
  }

  protected function addSectionFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_section');
    $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');
  }

  function csvcolumns() {
    $columns[] = array('id', 'ID');
    $columns[] = array('title', 'Title');
    $columns[] = array('promote', 'Promote');
    $columns[] = array('path', 'Path');
    if ($this->useImage = FALSE) {
      $this->addImageCsvColumns($columns);
    }
    if ($this->useTags) {
      $columns[] = array('tags', 'Tags');
    }
    if(!isset($this->useNotBody) || isset($useNotBody) && $this->useNotBody != 1) {
      $columns[] = array('body', 'Body');
    }
    if (isset($this->useSponsorCust) && $this->useSponsorCust = 1) {
      $this->addSponsorCustCsvColumns($columns);
    }
    if (isset($this->useUrl) && $this->useUrl = 1) {
      $this->addUrlCsvColumns($columns);
    }
    if (isset($this->useLogo) && $this->useLogo = 1) {
      $this->addLogoCsvColumns($columns);
    }
    if (isset($this->useResourcesCust) && $this->useResourcesCust = 1) {
      $this->addResourcesCust($columns);
    }
    if (isset($this->useIncentivesCust) && $this->useIncentivesCust = 1) {
      $this->addIncentivesCust($columns);
    }
    if (isset($this->useResourcesIncentivesSharedCust) && $this->useResourcesIncentivesSharedCust = 1) {
      $this->addResourcesIncentivesSharedCust($columns);
    }
    if (isset($this->useStarterKitCust) && $this->useStarterKitCust = 1) {
      $this->addStarterKitCust($columns);
    }
    if (isset($this->useDepartmentCust) && $this->useDepartmentCust = 1) {
      $this->addDepartmentCust($columns);
    }
    if (isset($this->useWizardScreenCust) && $this->useWizardScreenCust = 1) {
      $this->addWizardScreenCust($columns);
    }
    return $columns;
  }

  protected function setProperties($bundle) {
    parent::setProperties($bundle);
    $this->bundle = $bundle;
    $this->imageDirectory = 'images/' . $bundle;
  }
}

/**
 * Base migration for terms in SBP.
 */
abstract class SBPMigrationTerm extends SBPMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Import taxonomy terms.');

    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV($this->getImportFilePath(), $this->csvcolumns(), array('header_rows' => 1));
    $this->destination = new MigrateDestinationTerm($this->bundle);

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
        'name' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('description', 'description');
    $this->addFieldMapping('format', 'format')->defaultValue('filtered_html');
    $this->addFieldMapping('weight', 'weight');
    $this->addFieldMapping('parent_name', 'parent_name');
  }

  protected function addImageFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_image');
    $this->addFieldMapping($field_name . ':source_dir')
      ->defaultValue($this->getImageDirectoryPath());
    $this->addFieldMapping($field_name . ':file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping($field_name . ':alt', 'field_image_alt');
    $this->addFieldMapping($field_name . ':title', 'field_image_title');
  }

  protected function addIllustrationFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_illustration');
    $this->addFieldMapping($field_name . ':source_dir')
      ->defaultValue($this->getImageDirectoryPath());
    $this->addFieldMapping($field_name . ':file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping($field_name . ':alt', 'field_illustration_alt');
    $this->addFieldMapping($field_name . ':title', 'field_illustration_title');
  }

  function csvcolumns() {
    $columns[] = array('id', 'ID');
    $columns[] = array('name', 'Name');
    $columns[] = array('description', 'Description');
    $columns[] = array('format', 'Format');
    $columns[] = array('weight', 'Weight');
    $columns[] = array('parent_name', 'Parent name');

    if (isset($this->useStyleClass) && $this->useStyleClass = 1) {
      $columns[] = array('field_style_class', 'Style Class');
    }

    if (isset($this->useBPWSCust) && $this->useBPWSCust = 1) {
      $columns[] = array('field_color', 'Wizard Color');
      $columns[] = array('field_contextual_color', 'Wizard Contextual Color');
      $columns[] = array('field_drawer_color', 'Wizard Drawer Color');
      $columns[] = array('field_jurisdiction', 'Wizard Jurisdiction');
      $columns[] = array('field_image', 'Wizard Image');
      $columns[] = array('field_image_title', 'Wizard Image Title');
      $columns[] = array('field_image_alt', 'Wizard Image Alternate Text');
      $columns[] = array('field_illustration', 'Wizard Illustration');
      $columns[] = array('field_illustration_title', 'Wizard Illustration Title');
      $columns[] = array('field_illustration_alt', 'Wizard Illustration Alternate Text');
    }

    return $columns;
  }

  protected function setProperties($bundle) {
    parent::setProperties($bundle);
    $this->imageDirectory = 'images/' . $bundle;
  }
}

/**
 * BUSINESS PORTAL ITEMS
 */

/**
 * Starter Kit migration for nodes in SBP.
 */
class SBPMigrationStarterKit extends SBPMigrationNode {

  public function __construct($arguments) {
    $this->setProperties('sbp_starter_kit');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    parent::__construct($arguments);
    $this->description = t('Import Starter Kits.');
    //$this->addFieldMapping('field_document', 'docs');
    //$this->addFieldMapping('field_related_documents', 'rel_docs');
    //$this->addFieldMapping('field_related_permits', 'permits');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    //$columns[] = array('kicker', 'Kicker');
    //$columns[] = array('link', 'Link');
    //$columns[] = array('link_title', 'Link title');
    return $columns;
  }

}

/**
 * Starter Kit migration for nodes in SBP.
 */
class SBPMigrationNodeContent extends SBPMigrationNode {

  public function __construct($arguments) {
    $this->setProperties('sbp_content');
    $this->useTags = FALSE;
    $this->useImage = TRUE;
    parent::__construct($arguments);
    $this->description = t('Import Content.');
    //$this->addFieldMapping('field_document', 'docs');
    //$this->addFieldMapping('field_related_documents', 'rel_docs');
    //$this->addFieldMapping('field_related_permits', 'permits');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    //$columns[] = array('kicker', 'Kicker');
    //$columns[] = array('link', 'Link');
    //$columns[] = array('link_title', 'Link title');
    return $columns;
  }

}


/**
 * Migration for menu links in SBP.
 * Adapted from OpenOutreach
 * sample-migration-module-importing-menu-csv.html.
 */
abstract class SBPMigrationMenuLinks extends SBPMigration {
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Creates sample menus.');

    $this->source = new MigrateSourceCSV($this->getImportFilePath(), $this->csvcolumns(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationMenuLinks();

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'mlid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'ID of destination link',
        ),
      ),
      MigrateDestinationMenuLinks::getKeySchema()
    );

    $this->addFieldMapping('menu_name', 'menu_name');
    $this->addFieldMapping('plid', 'ref_parent')->sourceMigration($this->getMachineName());
    $this->addFieldMapping('link_path', 'path');
    $this->addFieldMapping('router_path')->defaultValue('node/%');
    $this->addFieldMapping('link_title', 'title');
    $this->addFieldMapping('link_description', 'description');
    $this->addFieldMapping('external')->defaultValue('0');
    $this->addFieldMapping('expanded')->defaultValue('0');
    $this->addFieldMapping('weight','weight');
    $this->addFieldMapping('customized')->defaultValue('1');
    $this->addFieldMapping('has_children')->defaultValue('0');
    // Will be overridden automatically.
    $this->addFieldMapping('depth')->defaultValue('1');

    $this->addUnmigratedDestinations(array('module', 'hidden','options','p1', 'p2', 'p3', 'p4', 'p5', 'p6', 'p7', 'p8', 'p9', 'updated'));

  }

  function prepareRow($row) {
    // Convert the alias to the node path.
    $node_path = drupal_lookup_path('source', $row->path);
    if (empty($node_path)) {
      return FALSE;
    }

    $row->path = $node_path;
  }

  /**
   * Creates a stub menu link, for when a child is imported before its parent.
   *
   * See http://drupal.org/node/1403044#comment-5790748.
   *
   * @param $migration
   *  The source migration
   * @return
   *  int $mlid on success
   *  FALSE on failure
   */
  protected function createStub($migration) {
    // If ref_parent is 0, that means it has no parent, so don't create a stub.
    if (!$migration->sourceValues->ref_parent) {
      return FALSE;
    }
    $menu_link = array (
      'menu_name' => $migration->sourceValues->menu_name,
      'link_path' => 'stub-path',
      'router_path' => 'stub-path',
      'link_title' => t('Stub title'),
    );
    $mlid = menu_link_save($menu_link);
    if ($mlid) {
      return array($mlid);
    }
    else {
      return FALSE;
    }
  }

  function csvcolumns() {
    $columns[] = array('mlid', 'Menu link ID');
    $columns[] = array('ref_parent', 'Parent menu link ID');
    $columns[] = array('menu_name', 'Menu name');
    $columns[] = array('path', 'Path');
    $columns[] = array('title', 'Title');
    $columns[] = array('description', 'Description');
    $columns[] = array('weight', 'Weight');
    return $columns;
  }

}

/**
 * Menu link migration for Landing Page in SBP.
 */
class SBPMigrationMenuLinksSection extends SBPMigrationMenuLinks {

  public function __construct($arguments) {
    $this->setProperties('landing_page_menu');
    parent::__construct($arguments);
    // Require the nodes that are referenced.
    $this->dependencies[] = 'SBPMigrationNodeLandingPage';
  }

}
