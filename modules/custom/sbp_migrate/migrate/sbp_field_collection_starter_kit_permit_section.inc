<?php
/**
 * @file
 *  Hero Field Collection migration for incentives content types.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFieldCollectionStarterKitPermitSection extends SBPMigration {

  public function __construct($arguments) {
    $this->setProperties('field_collection_starter_kit_permit_section');
    parent::__construct($arguments);

    $this->destination = new MigrateDestinationFieldCollection(
      'field_permit_section',
      array('host_entity_type' => 'node')
    );

    $this->addFieldMapping('host_entity_id', 'id')
      ->sourceMigration('SBPMigrationNodeStarterKit');

    $columns = array(
      array('id', 'StarterKitID'),
      array('field_section_description', 'Section Description'),
      array('field_permit_links', 'Calls to Action Link Field'),
      array('field_permit_links_title', 'Calls to Action URL title'),
      array('field_permit_links_attributes', 'Calls to Action URL attributes'),
      array('field_agency', 'Agency Term Reference Field'),
    );

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'sbp_migrate') . '/import/field_collection_starter_kit_permit_section.csv',
      $columns,
      array('header_rows' => 1)
    );

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'int',
          'not null' => true,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('field_section_description', 'field_section_description');

    $this->addPermitSectionStarterKitPermitLinksFieldMapping('field_permit_links');
    $this->addPermitSectionStarterKitAgencyFieldMapping('field_agency');

  }

  protected function addPermitSectionStarterKitPermitLinksFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_permit_links');
    $this->addFieldMapping($field_name . ':title', 'field_permit_links_title');
    $this->addFieldMapping($field_name . ':attributes', 'field_permit_links_attributes');
    $this->addFieldMapping($field_name . ':language')->defaultValue(LANGUAGE_NONE);
  }

  protected function addPermitSectionStarterKitAgencyFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_agency');
    $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');
  }
}