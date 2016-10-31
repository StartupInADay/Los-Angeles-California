<?php
/**
 * @file
 *  Fieldable Panel Pane Panel - Pull Quote migration.
 * 
 * No custom imports necessary at this point.
 */

class SBPMigrationFPPPullQuote extends SBPMigrationFPP {

  public function __construct($arguments) {
    $this->setProperties('pull_quote');
    $this->useTags = FALSE;
    $this->useImage = FALSE;
    $this->usePullQuote = TRUE;
    
    parent::__construct($arguments);

    $this->addFieldMapping('field_quote', 'field_quote');
    $this->addFieldMapping('field_quote_subtitle', 'field_quote_subtitle');
    $this->addFieldMapping('field_author', 'field_author');

    $this->addPaneStyleFieldMapping('field_pane_style');
    $this->addQuoteImageFieldMapping('field_quote_image');

    $this->description = t('Import Pull Quote FPP.');
  }

  function csvcolumns() {
    $columns = parent::csvcolumns();
    return $columns;
  }
}