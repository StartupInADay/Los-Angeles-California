<?php

/**
 *  @file
 *   Contains base user migration class.
 */

class ImportBaseUsers extends Migration {

  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'name' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationUser::getKeySchema()
    );

    $this->destination = new MigrateDestinationUser();
    
    $import_path = drupal_get_path('module', 'import') . '/import/';
    $this->source = new MigrateSourceCSV($import_path . 'import.base.users.csv', $this->csvcolumns(), array('header_rows' => 1));

    $this->addFieldMapping('name', 'name')->dedupe('users', 'name');
    $this->addFieldMapping('pass', 'pass');
    $this->addFieldMapping('mail', 'mail')->dedupe('users', 'mail');
    $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('roles', 'roles');

    $this->addFieldMapping('picture', 'picture')->sourceMigration('ImportBaseUserPictures');
    $this->addFieldMapping('field_user_bio', 'bio');
    $this->addFieldMapping('field_user_bio:format')->defaultValue('full_html');

  }

  function csvcolumns() {
    $columns[0] = array('name', 'Name');
    $columns[1] = array('pass', 'Pass');
    $columns[2] = array('mail', 'Mail');
    $columns[3] = array('status', 'Status');
    $columns[4] = array('roles', 'Roles');
    $columns[5] = array('picture', 'Picture');
    $columns[6] = array('bio', 'Bio');
    return $columns;
  }

  public function prepareRow($row) {
    $user_roles = explode(",", $row->roles);
    $roles = array('2' => '2');
    foreach ($user_roles as $role_name) {
      $rid = db_query('SELECT rid FROM {role} WHERE name = :name', array(':name' => $role_name))->fetchField();
      $roles[$rid] = $rid;
    }
    $row->roles = $roles;
    return TRUE;
  }

}

class ImportBaseUserPictures extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'picture' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationFile::getKeySchema()
    );
    $this->destination = new MigrateDestinationFile('file');
    $import_path = drupal_get_path('module', 'import') . '/import/';
    $this->source = new MigrateSourceCSV($import_path . 'import.base.users.csv', $this->csvcolumns(), array('header_rows' => 1));
    $this->addFieldMapping('value', 'picture');
    $this->addFieldMapping('source_dir')->defaultValue($import_path . 'images');
    $this->addFieldMapping('file_replace')->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('destination_file', 'filename');
    $this->addFieldMapping('status')->defaultValue(1);
   }

  function csvcolumns() {
    $columns[5] = array('picture', 'Picture');
    return $columns;
  }

}
