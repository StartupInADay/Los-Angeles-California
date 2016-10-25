<?php
/**
 * @file
 *  Hero Field Collection migration for Department content types.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFieldCollectionDepartmentAddresses extends SBPMigration {

  public function __construct($arguments) {
    $this->setProperties('field_collection_department_addresses');
    parent::__construct($arguments);

    $this->dependencies = array('SBPMigrationNodeDepartment');
    $this->addSoftDependencies(array('SBPMigrationNodeDepartment'));
    
    $this->destination = new MigrateDestinationFieldCollection(
      'field_addresses',
      array('host_entity_type' => 'node')
    );

    $this->addFieldMapping('host_entity_id', 'id')
      ->sourceMigration('SBPMigrationNodeDepartment');

    $columns = array(
      array('id', 'DepartmentID'),
      array('field_title', 'Title'),
      array('field_address', 'Address'),
      array('field_address_administrative_area', 'Address Administrative Area'),
      array('field_address_sub_administrative_area', 'Address Sub Administrative Area'),
      array('field_address_locality', 'Address Locality'),
      array('field_address_dependent_locality', 'Address Sub Locality'),
      array('field_address_postal_code', 'Address Postal Code'),
      array('field_address_thoroughfare', 'Address Thoroughfare'),
      array('field_address_premise', 'Address Premise'),
      array('field_address_sub_premise', 'Address Sub Premise'),
      array('field_address_organisation_name', 'Address Organisation Name'),
      array('field_address_name_line', 'Address Name Line'),
      array('field_address_first_name', 'Address First Name'),
      array('field_address_last_name', 'Address Las Name'),
      array('field_address_data', 'Address Data'),
    );

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'sbp_migrate') . '/import/field_collection_department_addresses.csv',
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

    $this->addSimpleMappings(array('field_title',));
    $this->addAddressesAddressFieldMapping('field_address');

  }

  protected function addAddressesAddressFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_address');
    $this->addFieldMapping($field_name . ':administrative_area', 'field_address_administrative_area');
    $this->addFieldMapping($field_name . ':sub_administrative_area', 'field_address_sub_administrative_area');
    $this->addFieldMapping($field_name . ':locality', 'field_address_locality');
    $this->addFieldMapping($field_name . ':dependent_locality', 'field_address_dependent_locality');
    $this->addFieldMapping($field_name . ':postal_code', 'field_address_postal_code');
    $this->addFieldMapping($field_name . ':thoroughfare', 'field_address_thoroughfare');
    $this->addFieldMapping($field_name . ':premise', 'field_address_premise');
    $this->addFieldMapping($field_name . ':sub_premise', 'field_address_sub_premise');
    $this->addFieldMapping($field_name . ':organisation_name', 'field_address_organisation_name');
    $this->addFieldMapping($field_name . ':name_line', 'field_address_name_line');
    $this->addFieldMapping($field_name . ':first_name', 'field_address_first_name');
    $this->addFieldMapping($field_name . ':last_name', 'field_address_last_name');
    $this->addFieldMapping($field_name . ':data', 'field_address_data');
  }
}