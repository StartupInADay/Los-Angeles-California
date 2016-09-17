<?php

/**
 * @file
 * Theme functions for Editor module.
 */

/**
 * Returns HTML for the text format administration overview form.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function editor_admin_overview($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form['formats']) as $id) {
    $form['formats'][$id]['weight']['#attributes']['class'] = array('text-format-order-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form['formats'][$id]['name']),
        drupal_render($form['formats'][$id]['editor']),
        drupal_render($form['formats'][$id]['roles']),
        drupal_render($form['formats'][$id]['weight']),
        drupal_render($form['formats'][$id]['configure']),
        drupal_render($form['formats'][$id]['disable']),
      ),
      'class' => array('draggable'),
    );
  }
  $header = array(t('Name'), t('Editor'), t('Roles'), t('Weight'), array('data' => t('Operations'), 'colspan' => 2));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'text-format-order')));
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('text-format-order', 'order', 'sibling', 'text-format-order-weight');

  return $output;
}

/**
 * Returns HTML for a textarea form element.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #description, #rows, #cols, #required,
 *     #attributes, #resizable.
 *
 * @ingroup themeable
 */
function editor_textarea($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'cols', 'rows'));
  _form_set_class($element, array('form-textarea'));

  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper'),
  );

  // Add resizable behavior.
  if (!empty($element['#resizable'])) {
    $element['#attributes']['class'][] = 'resize-' . $element['#resizable'];
  }

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
}

/**
 * Returns HTML for a captioned item, usually an image.
 */
function theme_editor_caption($variables) {
  $output = '';
  $output .= '<figure' . drupal_attributes($variables['attributes']) . '>';
  $output .= $variables['item'];
  $output .= '<figcaption>' . $variables['caption'] . '</figcaption>';
  $output .= '</figure>';
  return $output;
}
