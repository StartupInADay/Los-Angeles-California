<?php
/**
 * @file
 *  Hero Field Collection migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFieldCollectionHero extends SBPMigration {

  public function __construct($arguments) {
    $this->setProperties('field_collection_hero');
    parent::__construct($arguments);
    
    $columns = array(
      array('id', 'ResourceID'),
      array('title', 'Title'),
      array('field_description', 'Description'),
    );

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'fc_migration') . '/data/articles.csv',
      $columns,
      array('header_rows' => 1)
    );

    $this->destination = new MigrateDestinationNode('article');

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