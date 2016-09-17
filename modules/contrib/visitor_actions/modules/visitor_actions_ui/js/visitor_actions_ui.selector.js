/**
 * @file visitor_actions_ui.selector.js
 * Inspired by https://github.com/ngs/jquery-selectorator
 */
(function ($, Drupal) {
  Drupal.utilities = Drupal.utilities || {};

  /**
   * Determines the unique selector for an element.
   *
   * @param element
   *   A DOM element to find the selector for.
   * @param ignoreId
   *   An ID that should not be used when generating the selector.  Can be a
   *   string for an exact match, or a regular expression object.
   * @param ignoreClasses
   *   Classes that should be ignored when generating the selector.  Can be a
   *   string for exact match (space delimited for multiple classes) or a regular
   *   expression object that returns a match for each class name.
   *   *-processed classes are ignored automatically.
   */
  Drupal.utilities.getSelector = Drupal.utilities.getSelector || function (element, ignoreId, ignoreClasses) {

    /**
     * Utility function to test if a string value empty.
     *
     * @param stringValue
     *   String value to test
     * @returns {boolean}
     *   True if not empty, false if null or empty.
     */
    function notEmpty(stringValue) {
      return (stringValue != null ? stringValue.length : void 0) > 0;
    };

    // Convert the ignoreID  and ignoreClasses to regular expressions if only
    // strings passed in.
    ignoreId = typeof ignoreId === 'undefined' ? '' : ignoreId;
    ignoreClasses = typeof ignoreClasses === 'undefined' ? '' : ignoreClasses;
    if (typeof ignoreId === 'string' && notEmpty(ignoreId)) {
      ignoreId = new RegExp('^' + ignoreId + '$', i);
    }
    if (typeof ignoreClasses === 'string' && notEmpty(ignoreClasses)) {
      temp = ignoreClasses.split(' ');
      ignoreClasses = '';
      for (var i=0; i<temp.length; i++) {
        ignoreClasses += '(' + temp[i] + ')';
        if (i < (temp.length-1)) {
          ignoreClasses += '|';
        }
      }
      ignoreClasses = new RegExp(ignoreClasses);
    }

    var result = $(element).getSelector({
      'ignore': {
        'classes': [ignoreClasses],
        'ids': [ignoreId]
      }
    });
    if ($.isArray(result) && result.length > 0) {
      return result[0];
    }
    return null;

  }
}(Drupal.jQuery, Drupal));
