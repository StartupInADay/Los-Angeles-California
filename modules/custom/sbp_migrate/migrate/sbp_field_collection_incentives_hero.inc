<?php
/**
 * @file
 *  Hero Field Collection migration for online_resource content types.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFieldCollectionOnlineResourcesHero extends SBPMigration {

  public function __construct($arguments) {
    $this->setProperties('field_collection_online_resources_hero');
    parent::__construct($arguments);


    $this->destination = new MigrateDestinationFieldCollection(
      'field_hero',
      array('host_entity_type' => 'node')
    );
    
    $columns = array(
      array('id', 'ResourceID'),
      array('title', 'Title'),
      array('field_description', 'Description'),
    );

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'sbp_migrate') . '../import/field_collection_online_resources_hero.csv',
      $columns,
      array('header_rows' => 1)
    );

    $this->destination = new MigrateDestinationNode('online_resource');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'int',
          'not null' => true,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('uid', NULL)->defaultValue(1);
    $this->addSimpleMappings(array(
      'title',
      'field_description',
    ));
  
  }
  

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}