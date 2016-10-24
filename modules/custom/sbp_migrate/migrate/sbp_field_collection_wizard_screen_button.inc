<?php
/**
 * @file
 *  Hero Field Collection migration for Department content types.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFieldCollectionWizardScreenButton extends SBPMigration {

  public function __construct($arguments) {
    $this->setProperties('field_collection_wizard_screen_button');
    parent::__construct($arguments);

    $this->dependencies = array('SBPMigrationNodeWizardScreen');
    
    $this->destination = new MigrateDestinationFieldCollection(
      'field_button',
      array('host_entity_type' => 'node')
    );

    $this->addFieldMapping('host_entity_id', 'id')
      ->sourceMigration('SBPMigrationNodeWizardScreen');

    $columns = array(
      array('id', 'WizardScreenID'),
      array('field_title', 'Title'),
      array('field_button_result_order', 'Button Result Order'),
      array('field_button_style', 'Button Style'),
      array('field_information_text', 'Button Information Text'),
      array('field_button_links', 'Button Links'),
      array('field_button_links_title', 'Button Links Title'),
      array('field_button_links_attributes', 'Button Links Attribute'),
      array('field_cta_link_2', 'Button Links 2'),
      array('field_cta_link_2_title', 'Button Links 2 Title'),
      array('field_cta_link_2_attributes', 'Button Links 2 Attribute'),
      array('field_cta_link_3', 'Button Links 3'),
      array('field_cta_link_3_title', 'Button Links 3 Title'),
      array('field_cta_link_3_attributes', 'Button Links 3 Attribute'),
      array('field_wizard_screen', 'Wizard Screen'),
    );

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'sbp_migrate') . '/import/field_collection_wizard_screen_button.csv',
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
      'field_title',
      'field_button_result_order',
      'field_button_style',
    ));
    $this->addWizardScreenInformationTextFieldMapping('field_information_text');
    $this->addWizardScreenButtonLinks('field_button_links');
    $this->addWizardScreenCTALink2('field_cta_link_2');
    $this->addWizardScreenCTALink3('field_cta_link_3');
    $this->addWizardScreenWizardScreenFieldMapping('field_wizard_screen');

  }

  protected function addWizardScreenInformationTextFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_information_text');
    $this->addFieldMapping($field_name . ':format')->defaultValue('filtered_html');
  }

  protected function addWizardScreenButtonLinks($field_name) {
    $this->addFieldMapping($field_name, 'field_button_links');
    $this->addFieldMapping($field_name . ':title', 'field_button_links_title');
    $this->addFieldMapping($field_name . ':attributes', 'field_button_links_attributes');
    $this->addFieldMapping($field_name . ':language')->defaultValue(LANGUAGE_NONE);
  }

  protected function addWizardScreenCTALink2($field_name) {
    $this->addFieldMapping($field_name, 'field_cta_link_2');
    $this->addFieldMapping($field_name . ':title', 'field_cta_link_2_title');
    $this->addFieldMapping($field_name . ':attributes', 'field_cta_link_2_attributes');
    $this->addFieldMapping($field_name . ':language')->defaultValue(LANGUAGE_NONE);
  }

  protected function addWizardScreenCTALink3($field_name) {
    $this->addFieldMapping($field_name, 'field_cta_link_3');
    $this->addFieldMapping($field_name . ':title', 'field_cta_link_3_title');
    $this->addFieldMapping($field_name . ':attributes', 'field_cta_link_3_attributes');
    $this->addFieldMapping($field_name . ':language')->defaultValue(LANGUAGE_NONE);
  }

  protected function addWizardScreenWizardScreenFieldMapping($field_name) {
    $this->addFieldMapping($field_name, 'field_wizard_screen');
  }
}