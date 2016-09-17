/**
 * @file
 * Classy Panel Styles admin javascript file.
 *
 * Hides vertical tabs for layout selection on the admin page.
 */

(function ($) {

  "use strict";

  Drupal.behaviors.classyPanels = {
    attach: function (context, settings) {
      $('input[name="layouts_setting"]', context).once(function () {
        $(this, context).change(function () {
          if ('0' === $(this, context).attr('value')) {
            $('.vertical-tabs', context).hide();
          } else {
            $('.vertical-tabs', context).show();
          }
        });
      });
      $('input[name="layouts_setting"]:checked', context).change();
    }
  };

}(jQuery));
