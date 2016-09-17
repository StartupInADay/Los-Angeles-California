<?php

/**
 * @file
 * A handler to provide a Views Fieldsets field.
 */

class views_fieldsets_fieldset_field_handler extends views_handler_field {

  function query() {
    // Do nothing -- to override the parent query.
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['element_default_classes']['default'] = FALSE;

    // Add the hierarchical fieldset structure storage.
    $options['children'] = array('default' => array());

    // Add (cosmetic) user options.
    $options['fieldset']['contains'] = array();
    $options['fieldset']['contains']['type'] = array('default' => 'fieldset');
    $options['fieldset']['contains']['classes'] = array('default' => '');
    $options['fieldset']['contains']['id'] = array('default' => '');
    $options['fieldset']['contains']['collapsible'] = array('default' => 0);
    $options['fieldset']['contains']['collapsed'] = array('default' => 0);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    // These options are required by Views.
    $keep_form_elements = element_children($form);

    // These options are useful for a fieldset.
    $keep_form_elements[] = 'custom_label';
    $keep_form_elements[] = 'label';
    $keep_form_elements[] = 'alter';

    // These options are useful 'alter' options. Most aren't.
    $keep_rewrite_elements = array('alter_text', 'text', 'help', 'trim_whitespace', 'nl2br');

    // Get the plentiful usual suspects.
    parent::options_form($form, $form_state);

    // Remove options that aren't relevant.
    foreach (element_children($form) as $name) {
      if (!in_array($name, $keep_form_elements)) {
        $form[$name]['#access'] = FALSE;
      }
    }

    // Alter custom_label (always ON).
    $element = &$form['custom_label'];
    $element['#default_value'] = $element['#value'] = TRUE;
    $element['#type'] = 'hidden';

    // Alter label to make it fit our needs.
    $element = &$form['label'];
    unset($element['#dependency']);
    $element['#title'] = t('Fieldset title');
    $element['#required'] = TRUE;
    $element['#description'] = t('The back-end admin label and default front-end fieldset legend. You can override the fieldset legend by rewriting below. Will be translated either way.');

    // Open 'alter' fieldset.
    $form['alter']['#collapsed'] = $form['alter']['#collapsible'] = FALSE;
    $form['alter']['#description'] = t('This will alter the fieldset <code>&lt;legend&gt;</code>. Default would be the above <em>Fieldset title</em>.');

    // Hide most 'alter' stuff.
    foreach (element_children($form['alter']) as $name) {
      if (!in_array($name, $keep_rewrite_elements)) {
        $form['alter'][$name]['#access'] = FALSE;
      }
    }

    // Don't hide 'hide_empty', because it's useful for empty fieldsets (due to empty fields).
    unset($form['hide_empty']['#access'], $form['hide_empty']['#fieldset']);
    $form['hide_empty']['#weight'] = 200;

    // Add 'fieldset' tree with the usual fieldset stuff.
    $form['fieldset'] = array(
      '#tree' => TRUE,
    );

    $fieldset_types = array(
      'fieldset' => '<fieldset>',
      'div' => '<div>',
    );
    $form['fieldset']['type'] = array(
      '#type' => 'select',
      '#title' => t('Display type'),
      '#options' => $fieldset_types,
      '#default_value' => $this->options['fieldset']['type'],
      '#description' => t('A <code>&lt;fieldset&gt;</code> will always show its title as <code>&lt;legend&gt;</code>. A <code>&lt;div&gt;</code> will never show the title.'),
    );

    $form['fieldset']['classes'] = array(
      '#type' => 'textfield',
      '#title' => t('HTML classes'),
      '#default_value' => $this->options['fieldset']['classes'],
	  '#description' => t('The usual Views row tokens are available. If you leave this empty, the untranslated <em>Fieldset title</em> will be added.'),
    );

    $form['fieldset']['id'] = array(
      '#type' => 'textfield',
      '#title' => t('HTML ID'),
      '#default_value' => $this->options['fieldset']['id'],
	  '#description' => t('The usual Views row tokens are available.') . ' ' . t('You want to make sure this ID is very unique by using tokens.'),
    );

    $form['fieldset']['collapsible'] = array(
      '#type' => 'checkbox',
      '#title' => t('Collapsible'),
      '#default_value' => $this->options['fieldset']['collapsible'],
    );

    $form['fieldset']['collapsed'] = array(
      '#type' => 'checkbox',
      '#title' => t('Collapsed'),
      '#default_value' => $this->options['fieldset']['collapsed'],
    );
  }

  /**
   * This is a dummy that has to return a truthy value. Otherwise the No Results Behavior will kick in.
   */
  function render($values) {
    if (isset($this->options['children'])) {
      return '[' . implode(', ', $this->options['children']) . ']';
    }
  }

}
