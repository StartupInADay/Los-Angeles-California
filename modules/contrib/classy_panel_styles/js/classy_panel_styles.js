/**
 * @file
 * Classy Panel Styles javascript file.
 *
 * Adds classes to regions and panes in the editor UI.
 */

(function ($) {

  "use strict";

  Drupal.classyPanels = Drupal.classyPanels || {};
  Drupal.classyPanels.applied = Drupal.classyPanels.applied || {};
  Drupal.settings.classyPanels = Drupal.settings.classyPanels || {};

  Drupal.behaviors.classyPanels = {
    attach: function (context, settings) {

      var id, classToAdd, classToRemove;

      // Iterate over Classy Panels styles.
      for (id in Drupal.settings.classyPanels) {
        if (Drupal.settings.classyPanels.hasOwnProperty(id)) {

          classToAdd = Drupal.settings.classyPanels[id];

          // Drupal.settings makes an array on subsequent (ajax) calls.
          if ($.isArray(classToAdd)) {
            classToAdd = classToAdd[0];
          }

          // Remove existing applied class, if any.
          if (Drupal.classyPanels.applied.hasOwnProperty(id)) {
            classToRemove = Drupal.classyPanels.applied[id];
            // Even if the classToAdd === classToRemove, go ahead with the
            // removal + addition, in case something else (like IPE) interfered
            // with the markup.
            // Use global context instead of the context passed into this
            // behavior, since the calling context does not include the Panels
            // markup except on first load.
            $('#' + id).removeClass(classToRemove);
          }

          // Add the class.
          // Use global context instead of the context passed into this
          // behavior, since the calling context does not include the Panels
          // markup except on first load.
          $('#' + id).addClass(classToAdd);

          // Keep track of the applied class.
          Drupal.classyPanels.applied[id] = classToAdd;
        }
      }
    }
  };

}(jQuery));
