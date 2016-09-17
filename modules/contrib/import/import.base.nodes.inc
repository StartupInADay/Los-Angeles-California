<?php

/**
 * @file
 *  Migrations for Basic Nodes.
 */

class ImportBaseNodes extends Migration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Import nodes.');

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'title' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Create a MigrateSource object.
    if (isset($arguments['path'])) {
      $import_path = $arguments['path'];
    }
    else {
      $import_path = drupal_get_path('module', 'import') . '/import/import.base.nodes.csv';
    }
    $this->source = new MigrateSourceCSV($import_path, $this->csvcolumns(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationNode('article');

    $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    $this->addFieldMapping('status', 'status')->defaultValue(1);
    $this->addFieldMapping('language', 'language')->defaultValue('en');
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('body:format')->defaultValue('filtered_html');
  }

  function csvcolumns() {
    $columns[0] = array('title', 'Title');
    $columns[1] = array('body', 'Body');
    return $columns;
  }

}

