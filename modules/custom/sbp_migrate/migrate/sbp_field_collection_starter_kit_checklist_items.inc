<?php
/**
 * @file
 *  Hero Field Collection migration for incentives content types.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFieldCollectionStarterKitChecklistItems extends SBPMigration {

  public function __construct($arguments) {
    $this->setProperties('field_collection_starter_kit_checklist_items');
    parent::__construct($arguments);

    $this->destination = new MigrateDestinationFieldCollection(
      'field_checklist_items',
      array('host_entity_type' => 'node')
    );

    $this->addFieldMapping('host_entity_id', 'id')
      ->sourceMigration('SBPMigrationNodeStarterKit');

    $columns = array(
      array('id', 'StarterKitID'),
      array('field_section_title', 'Title'),
      array('field_section_tip', 'Tip'),
      array('field_checklist_hierarchy', 'Checklist Hierarchy'),
      array('field_checklist_hierarchy_parent', 'Checklist Hierarchy Parent'),
    );

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'sbp_migrate') . '/import/field_collection_starter_kit_checklist_items.csv',
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

    $this->addFieldMapping('field_section_title', 'field_section_title');
    

    $this->addStarterKitSectionTipFieldMapping('field_section_tip');
    $this->addStarterKitChecklistHierarchyFieldMapping('field_checklist_hierarchy');

  }

  protected function addStarterKitSectionTipFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_section_tip');
    $this->addFieldMapping($field_name . ':format')->defaultValue('filtered_html');
  }

  protected function addStarterKitChecklistHierarchyFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_checklist_hierarchy');
    $this->addFieldMapping($field_name . ':parent', 'field_checklist_hierarchy_parent');
    $this->addFieldMapping($field_name . ':format')->defaultValue('filtered_html');
  }
}