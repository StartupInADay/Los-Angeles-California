<?php

/**
 * @file
 * Definition of ctools_export_ui_entity_embed.class.php.
 */

/**
 * Export UI for entity_embed buttons.
 */
class ctools_export_ui_entity_embed extends ctools_export_ui {
  /**
   * {@inheritdoc}
   */
  function edit_form(&$form, &$form_state) {
    parent::edit_form($form, $form_state);

    $preset = $form_state['item'];

    // Determine the currently selected entity type.
    $selected = isset($form_state['values']['entity_type']) ? $form_state['values']['entity_type'] : $preset->entity_type;

    // Disable form submission if an entity type has not been selected.
    if (empty($selected)) {
      $form['buttons']['submit']['#disabled'] = TRUE;
    }

    // Wrap the submit button in markup to allow it to be targeted with JS.
    $form['buttons']['submit']['#prefix'] = '<span id="save-button">';
    $form['buttons']['submit']['#suffix'] = '</span>';
  }
}
