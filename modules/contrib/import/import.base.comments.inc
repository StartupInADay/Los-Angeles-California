<?php

/**
 * @file
 *  Migrations for Basic Comments.
 */

class ImportBaseComments extends Migration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Import comments.');

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'subject' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationComment::getKeySchema()
    );

    $import_path = drupal_get_path('module', 'import') . '/import/';

    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV($import_path . 'import.base.comments.csv', $this->csvcolumns(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationComment('comment_node_article');

    $this->addFieldMapping('subject', 'subject');
    $this->addFieldMapping('nid', 'node')->sourceMigration('ImportBaseNodes');
    $this->addFieldMapping('uid', 'author')->sourceMigration('ImportBaseUsers');
    $this->addFieldMapping('comment_body', 'body');
    $this->addFieldMapping('comment_body:format')->defaultValue('filtered_html');
  }

  function csvcolumns() {
    $columns[0] = array('subject', 'Subject');
    $columns[1] = array('node', 'Node');
    $columns[2] = array('author', 'Author');
    $columns[3] = array('body', 'Body');
    return $columns;
  }

}

