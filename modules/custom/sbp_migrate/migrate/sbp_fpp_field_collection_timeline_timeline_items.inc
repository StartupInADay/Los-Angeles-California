<?php
/**
 * @file
 *  Calls to Actions Field Collection migration for Four Column CTA FPP.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFPPFieldCollectionTimelineTimelineItems extends SBPMigration {

  public function __construct($arguments) {
    $this->setProperties('field_collection_timeline_timeline_items');
    parent::__construct($arguments);

    $this->destination = new MigrateDestinationFieldCollection(
      'field_timeline_items',
      array('host_entity_type' => 'fieldable_panels_panes')
    );

    $this->addFieldMapping('host_entity_id', 'id')
      ->sourceMigration('SBPMigrationFPPTimeline');

    $columns = array(
      array('id', 'TimelineID'),
      array('field_timeline_title', 'Timeline Title'),
      array('field_timeline_body', 'Timeline Body'),
      array('field_timeline_cta', 'Timeline CTA'),
      array('field_timeline_cta_title', 'Timeline CTA URL title'),
      array('field_timeline_cta_attributes', 'Timeline CTA URL attributes'),
    );

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'sbp_migrate') . '/import/field_collection_timeline_timeline_items.csv',
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

    $this->addFieldMapping('field_timeline_title', 'field_title');

    $this->addTimelineItemsTimelineBodyFieldMapping('field_timeline_body');
    $this->addTimelineItemsTimelineCTAFieldMapping('field_timeline_cta');

  }

  protected function addTimelineItemsTimelineCTAFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_link');
    $this->addFieldMapping($field_name . ':title', 'field_link_title');
    $this->addFieldMapping($field_name . ':attributes', 'field_link_attributes');
    $this->addFieldMapping($field_name . ':language')->defaultValue(LANGUAGE_NONE);
  }
  
  protected function addTimelineItemsTimelineBodyFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_timeline_body');
    $this->addFieldMapping($field_name . ':format')
      ->defaultValue('filtered_html');
  }
}