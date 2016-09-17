/**
 * @file visitor_actions_ui.launcher.js
 */

(function (Drupal, $) {

"use strict";

Drupal.behaviors.visitorActionsEditorLauncher = {
  attach: function (context) {
    // Clicking toggles the visitor actions edit mode. If the mode was initially
    // disabled, the JavaScript resources to build the edit mode will be
    // loaded.
    if (!getAppView()) {
      var $triggers = $('[href*="' + Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/structure/visitor_actions/add"]')
      var $placeholders = $triggers.once('visitor-actions-ui-launcher');
      // Filter out the triggers that have been processed and unbind them.
      if ($placeholders.length) {
        $placeholders.each(function (index, item) {
          var $trigger = $(item).attr('data-visitor-actions-ui', 'loader');
          var id = 'visitorActionsUI-' + (new Date()).getTime();
          $trigger.attr('data-visitor-actions-ui-loader', id)
          $('<div id="' + id + '" class="element-invisible visitor-actions-ui-loader" data-visitor-actions-ui="loader"></div>').insertAfter($trigger.eq(0));
        });
      }
      // Set up a blank placeholder to be replaced with the action editor
      // assets.
      $triggers.each(function (index, item) {
        var $trigger = $(item);
        $trigger.unbind('click.visitorActionsUILoad');
        var id = $trigger.attr('data-visitor-actions-ui-loader');
        var $placeholder = $('#' + id);
        $trigger.one('click.visitorActionsUILoad', function (event) {
          event.preventDefault();
          $trigger.unbind('click.visitorActionsUILoad');

          /**
           * Override the Drupal.ajax error handler for the form redirection error.
           *
           * Remove the alert() call.
           */
          var ajaxError = Drupal.ajax.prototype.error;
          Drupal.ajax.prototype.error = function (response, uri) {
            // Remove the progress element.
            if (this.progress.element) {
              $(this.progress.element).remove();
            }
            if (this.progress.object) {
              this.progress.object.stopMonitoring();
            }
            // Undo hide.
            $(this.wrapper).show();
            // Re-enable the element.
            $(this.element).removeClass('progress-disabled').removeAttr('disabled');
            // Reattach behaviors, if they were detached in beforeSerialize().
            if (this.form) {
              var settings = response.settings || this.settings || Drupal.settings;
              Drupal.attachBehaviors(this.form, settings);
            }
          };

          /**
           * Remove refs to scope variables and the Drupal.ajax instance.
           */
          function cleanup () {
            delete Drupal.ajax[id];
            id = null;
            $placeholder.unbind().remove();
            $placeholder = null;
            $trigger = null;
          }

          Drupal.ajax[id] = new Drupal.ajax(id, $('#' + id), {
            url: Drupal.settings.basePath + Drupal.settings.pathPrefix + 'visitor_actions_ui/edit-mode-toggle/assets',
            event: 'load.visitorActionsUI',
            wrapper: id,
            progress: {
              type: null
            },
            success: function (response, status) {
              // Call the parent insert function
              Drupal.ajax.prototype.success.call(this, response, status);
              // Toggle the app on.
              attachVisitorActionsToggleEditMode();
              $trigger.trigger('click.visitorActionsUI');
              cleanup();
            },
            complete: function () {
              // Put the original Drupal.ajax error handler back.
              Drupal.ajax.prototype.error = ajaxError;
              ajaxError = null;
            }
          });
          // This placeholder only needs to trigger this event once, so unbind
          // the event immediately.
          if (!getAppView()) {
            $placeholder.trigger('load.visitorActionsUI');
          }
          else {
            attachVisitorActionsToggleEditMode();
            cleanup();
          }
        });
      });

      // Allow other modules to shut down in-place visitor action definition.
      $('body').once('visitorActionsUIShutdown').each(function() {
        $(document).bind('visitorActionsUIShutdown', function (event) {
          var appView = getAppView();
          if (appView) {
            var model = appView.model;
            if (model.get('editMode')) {
              // Make sure the toggle click handlers have been bound. This is not
              // ideal, but loading order of JS files dictates that we be very
              // conservative here.
              attachVisitorActionsToggleEditMode();
              // Find one of the triggers and click it.
              $('[href*="' + Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/structure/visitor_actions/add"]').eq(0).trigger('click.visitorActionsUI');
            }
          }
        });
      });

    }
    // The app is already loaded.
    else {
      $('[data-visitor-actions-ui="loader"]')
        .unbind('click.visitorActionsUILoad load.visitorActionsUI')
        .filter('.visitor-actions-ui-loader')
        .remove();
      attachVisitorActionsToggleEditMode();
    }
  }
};

/**
 *
 */
function attachVisitorActionsToggleEditMode () {
  var $onceler = $('body').once('visitor-actions-ui');
  if ($onceler.length) {
    var trigger = '[href*="' + Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/structure/visitor_actions/add"]';

    $(document).delegate(trigger, 'click.visitorActionsUI', function (event) {

      var element = this;

      // Toggle the edit mode.
      // First, make sure each trigger has a unique ID.
      if (!element.id) {
        element.id = 'visitorActionsUI-' + (new Date()).getTime();
      }
      // Toggle edit mode on the server. The response will update
      // Drupal.settings.visitor_actions.edit_mode.
      Drupal.ajax[element.id] = new Drupal.ajax(element.id, element, {
        url: Drupal.settings.basePath + Drupal.settings.pathPrefix + 'visitor_actions_ui/edit-mode-toggle',
        event: 'toggle.visitorActionsUI',
        progress: {
          type: 'none'
        }
      });

      $(element).trigger('toggle.visitorActionsUI');

      event.preventDefault();
      event.stopPropagation();
    });

    // Toggles edit mode in the Visitor Actions app.
    Drupal.ajax.prototype.commands.visitor_actions_ui_toggle_edit_mode = function (ajax, response, status) {
      // Clean up.
      delete Drupal.ajax[ajax.element.id];
      $(ajax.element).unbind('toggle.visitorActionsUI');
      // Toggle the app.
      var appView = getAppView();
      if (appView) {
        // Change the editMode value into a true Boolean.
        // jQuery.fn.toggleClass can't deal with truthy/falsey values.
        var editMode = !!response.edit_mode;
        appView.model.set('editMode', editMode);
        $(document).trigger('visitorActionsUIEditMode', editMode);
      }
    };
  }
};

Drupal.visitorActions = Drupal.visitorActions || {};

/**
 * Returns the Backbone View of the Visitor Actions add action controller.
 *
 * @return Backbone.View
 */
function getAppView () {
  return Drupal.visitorActions && Drupal.visitorActions.ui && Drupal.visitorActions.ui.views && Drupal.visitorActions.ui.views.appView;
}

}(Drupal, Drupal.jQuery));
