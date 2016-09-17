<?php

/**
 * Defines the variation types available for personalize elements.
 *
 * @param $filter_by_perms
 *   Indicates if the array of types should be filtered based on the current
 *   user's permissions.
 * @param bool $filter_active
 *   True to restrict the returned variations to those having an active status.
 * @return array
 *   An array of variation types.  The key should be the same as the key for
 *   JavaScript functionality to execute in order to personalize.  Each type
 *   should include:
 *   - label: The label to display within the personalize elements options
 *   - needs_selector: boolean indicating if this personalization requires
 *     a selector to act upon
 *   - contextual: An array of information for inclusion within the contextual
 *     content variation creation process.  FALSE if this variation should
 *     not be included.  In order to be included the type must also have
 *     needs_selector => TRUE.
 *   - active: True to indicate the variation type can be used for new
 *     variations and false for variation types that have been retired.
 *     - label: The label for this item to be displayed within contextual
 *       creation menu
 *     - formitem: A form field render array for the
 *       personalize_elements_content to be collected for this variation.
 */
function hook_personalize_elements_variation_types($filter_by_perms = TRUE, $filter_active = TRUE) {
  return array(
    'myVariationType' => array(
      'label' => t('Change something'),
      'needs_selector' => TRUE,
      'active' => TRUE,
      'contextual' => array(
        'label' => t('Change something'),
        'formitem' => array(
          '#type' => 'textarea',
          '#title' => t('New stuff'),
          '#rows' => 4,
        ),
      ),
    ),
  );
}
