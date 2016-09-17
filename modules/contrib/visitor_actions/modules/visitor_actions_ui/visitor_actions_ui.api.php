<?php

/**
 * Provides a listing of JavaScript regular expressions to be ignored when selecting
 * elements and/or generating an element selector.
 *
 * Returns an array with two keys:
 * - classes: An array of regular expressions to ignore in classes.  This should
 *   match the entire class name.
 * - ids: An array of regular expressions to test for ids.  Note that this only
 *   needs to match any part of the ID name rather than the entire name.
 */
function hook_visitor_actions_ui_selector_ignore() {
  return array(
    'classes' => array(
      'someclass',
      '[a-zA-Z0-9]-processed',
    ),
    'ids' => array(
      'visitorActions-[a-zA-Z0-9\_]',
    ),
  );
}
