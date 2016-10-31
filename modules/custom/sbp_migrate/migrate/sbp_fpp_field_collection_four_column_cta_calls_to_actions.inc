<?php
/**
 * @file
 *  Calls to Actions Field Collection migration for Four Column CTA FPP.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFPPFieldCollectionFourColumnCTACallsToActions extends SBPMigration {

  public function __construct($arguments) {
    $this->setProperties('field_collection_four_columns_cta_calls_to_actions');
    parent::__construct($arguments);

    $this->destination = new MigrateDestinationFieldCollection(
      'field_calls_to_actions',
      array('host_entity_type' => 'fieldable_panels_panes')
    );

    $this->addFieldMapping('host_entity_id', 'id')
      ->sourceMigration('SBPMigrationFPPFourColumnCTA');

    $columns = array(
      array('id', 'FourColumnCTAID'),
      array('field_title', 'CTA Title'),
      array('field_description', 'CTA Description'),
      array('field_link', 'Calls to Action Link Field'),
      array('field_link_title', 'Calls to Action URL title'),
      array('field_link_attributes', 'Calls to Action URL attributes'),
      array('field_icon', 'Icon'),
      array('field_icon_title', 'Icon Title'),
      array('field_icon_alt', 'Icon Alt'),
    );

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'sbp_migrate') . '/import/field_collection_four_column_cta_calls_to_actions.csv',
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
      MigrateDestinationFieldablePanelsPanes::getKeySchema()
    );

    $this->addFieldMapping('field_title', 'field_title');
    $this->addFieldMapping('field_description', 'field_description');

    $this->addFourColumnCTALinkFieldMapping('field_link');
    $this->addFourColumnCTAIconFieldMapping('field_icon');

  }

  protected function addFourColumnCTALinkFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_link');
    $this->addFieldMapping($field_name . ':title', 'field_link_title');
    $this->addFieldMapping($field_name . ':attributes', 'field_link_attributes');
    $this->addFieldMapping($field_name . ':language')->defaultValue(LANGUAGE_NONE);
  }

  protected function addFourColumnCTAIconFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_icon');
    $this->addFieldMapping($field_name . ':alt', 'field_icon_alt');
    $this->addFieldMapping($field_name . ':title', 'field_icon_title');
    $this->addFieldMapping($field_name . ':file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping($field_name . ':source_dir')
      ->defaultValue($this->getImageDirectoryPath());
  }
}