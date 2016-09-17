<?php

/**
 * @file form_builder.cache.inc
 * Functions for temporary storage of form builder structures while editing.
 */

/**
 * Load a form configuration cache.
 *
 * @deprecated Use FormBuilderLoader::fromCache() instead.
 *
 * @see FormBuilderLoader::fromCache()
 */
function form_builder_cache_load($form_type, $form_id, $sid = NULL, $reset = FALSE) {
  $form = FormBuilderLoader::instance()
    ->fromCache($form_type, $form_id, $sid, $reset);
  return $form ? $form->getFormArray() : FALSE;
}

/**
 * Save a form builder cache based on the form structure.
 *
 * @deprecated Use FormBuilderFormBase::save() instead.
 *
 * @see FormBuilderFormBase::save()
 */
function form_builder_cache_save($form_type, $form_id, $form, $sid = NULL) {
  $form = FormBuilderLoader::instance()->getForm($form_type, $form_id, $sid, $form);
  return $form->save();
}

function form_builder_cache_delete($form_type, $form_id, $sid = NULL) {
  $form = FormBuilderLoader::instance()
    ->fromCache($form_type, $form_id, $sid)
    ->delete();
}

/**
 * Delete outdated cache entries.
 *
 * @param $expire_threshold
 *   The maximum amount of time allowed for entries to be kept, in seconds.
 */
function form_builder_cache_purge($expire_threshold = NULL) {
  FormBuilderFormBase::purge($expire_threshold);
}

/**
 * Compare the cached form with the original and return all changed elements.
 *
 * @return
 *   An array of elements keyed by the element_id of the changed element. Each
 *   value contains an array of the 'original' and 'modified' elements.
 *
 * @todo Implement this as a form object method.
 */
function form_builder_cache_difference($form_type, $form_id, $exclude_weight = TRUE) {
  $loader = FormBuilderLoader::instance();
  $modified_form = $loader->fromCache($form_type, $form_id);
  if ($modified_form) {
    $original_form = $loader->fromStorage($form_type, $form_id);
    $original_ids = $original_form->getElementIds();
    $modified_ids = $modified_form->getElementIds();

    // Find the union of IDs in both arrays.
    $element_ids = array_keys(array_flip($original_ids) + array_flip($modified_ids));

    // Build a list of all elements that have changed.
    $differences = array();
    foreach ($element_ids as $element_id) {
      $original = $original_form->getElementArray($element_id);
      $modified = $modified_form->getElementArray($element_id);

      if ($exclude_weight) {
        if (isset($original['#weight'])) {
          unset($original['#weight']);
        }
        if (isset($modified['#weight'])) {
          unset($modified['#weight']);
        }
      }

      if ($original != $modified) {
        $differences[$element_id] = array(
          'original' => $original,
          'modified' => $modified,
        );
      }
    }

    return $differences;
  }
  return FALSE;
}

/**
 * Retrieve a single field from a form cache.
 */
function form_builder_cache_field_load($form_type, $form_id, $element_id, $sid = NULL, $reset = FALSE) {
  $form = FormBuilderLoader::instance()
    ->fromCache($form_type, $form_id, $sid, $reset);
  return $form ? $form->getElementArray($element_id) : FALSE;
}

/**
 * Add or update a single field in a form builder cache.
 */
function form_builder_cache_field_save($form_type, $form_id, $element, $sid = NULL) {
  $form = FormBuilderLoader::instance()
    ->fromCache($form_type, $form_id, $sid);
  $form->setElementArray($element);
  $form->save();
}

/**
 * Remove a single field from a form builder cache.
 */
function form_builder_cache_field_delete($form_type, $form_id, $element_id, $sid = NULL) {
  $form = FormBuilderLoader::instance()
    ->fromCache($form_type, $form_id, $sid);
  $form->unsetElement($element_id);
  $form->save();
}
