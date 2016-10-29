<?php
/**
 * @file
 *  Hero Field Collection migration for Department content types.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFieldCollectionDepartmentHero extends SBPMigration {

  public function __construct($arguments) {
    $this->setProperties('field_collection_department_hero');
    parent::__construct($arguments);

    $this->destination = new MigrateDestinationFieldCollection(
      'field_hero',
      array('host_entity_type' => 'node')
    );

    $this->addFieldMapping('host_entity_id', 'id')
      ->sourceMigration('SBPMigrationNodeDepartment');

    $columns = array(
      array('id', 'DepartmentID'),
      array('field_hero_title', 'Title'),
      array('field_description', 'Description'),
      array('field_background', 'Background'),
      array('field_width', 'Width'),
      array('field_subtitle_long', 'Subtitle Long'),
      array('field_subtitle_2', 'Subtitle_2'),
      array('field_pane_style', 'Pane Style'),
      array('field_calls_to_action', 'Calls to Action Link Field'),
      array('field_calls_to_action_title', 'Calls to Action URL title'),
      array('field_calls_to_action_attributes', 'Calls to Action URL attributes'),
      array('field_background_image', 'Background Image filename'),
      array('field_background_image_alt', 'Background Image alt'),
      array('field_background_image_title', 'Background Image title'),
      array('field_background_image_source_dir', 'Background Image source directory'),
      array('field_icon', 'Icon filename'),
      array('field_icon_alt', 'Icon alt'),
      array('field_icon_title', 'Icon title'),
      array('field_icon_source_dir', 'Icon source directory'),
    );

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'sbp_migrate') . '/import/field_collection_department_hero.csv',
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

    $this->addSimpleMappings(array(
      'field_hero_title',
      'field_description',
      'field_background',
      'field_width',
      'field_subtitle_long',
      'field_subtitle_2',
    ));
    $this->addHeroDepartmentPaneStyleFieldMapping('field_pane_style');
    $this->addHeroDepartmentCallsToActionFieldMapping('field_calls_to_action');
    $this->addHeroDepartmentBackgroundImageFieldMapping('field_background_image');
    $this->addHeroDepartmentIconFieldMapping('field_icon');

  }

  protected function addHeroDepartmentPaneStyleFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_pane_style');
    $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');
  }

  protected function addHeroDepartmentCallsToActionFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_calls_to_action');
    $this->addFieldMapping($field_name . ':title', 'field_calls_to_action_title');
    $this->addFieldMapping($field_name . ':attributes', 'field_calls_to_action_attributes');
    $this->addFieldMapping($field_name . ':language')->defaultValue(LANGUAGE_NONE);
  }

  protected function addHeroDepartmentBackgroundImageFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_background_image');
    $this->addFieldMapping($field_name . ':alt', 'field_background_image_alt');
    $this->addFieldMapping($field_name . ':title', 'field_background_image_title');
    $this->addFieldMapping($field_name . ':file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping($field_name . ':source_dir')
      ->defaultValue($this->getImageDirectoryPath());
  }

  protected function addHeroDepartmentIconFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_icon');
    $this->addFieldMapping($field_name . ':alt', 'field_icon_alt');
    $this->addFieldMapping($field_name . ':title', 'field_icon_title');
    $this->addFieldMapping($field_name . ':file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping($field_name . ':source_dir')
      ->defaultValue($this->getImageDirectoryPath());
  }
}