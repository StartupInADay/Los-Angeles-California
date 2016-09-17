<?php
/**
 * @file
 * Hooks provided by Panels Preview.
 */

/**
 * Alter the list of forms to improve.
 *
 * @param &$form_ids
 *   The list of form ids for the forms that panels_preview will improve. Add or
 *   remove items from this list.
 *
 * @see https://www.drupal.org/node/2261061
 */
function hook_panels_preview_forms_to_improve_alter(&$form_ids) {
  // If you added your own content type and want the extra form fields added to
  // the fieldsets that panels_preview creates, you can add the form id of your
  // settings form here.
  $form_ids[] = 'my_module_widget_content_type_edit_form';
}

