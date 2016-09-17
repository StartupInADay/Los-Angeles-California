/**
 * @file visitor_actions_ui.dialog.js
 *
 * Requires ducktape.position.js from visitor actions ui.
 */
(function (Drupal, $, _, Backbone) {

  "use strict";

  Drupal.visitorActions = Drupal.visitorActions || {};
  Drupal.visitorActions.ui = Drupal.visitorActions.ui || {};
  Drupal.visitorActions.ui.dialog = Drupal.visitorActions.ui.dialog || {};

  /*******************************************************************
   * M O D E L S
   *******************************************************************/
  Drupal.visitorActions.ui.dialog.models = Drupal.visitorActions.ui.dialog.models || {};
  Drupal.visitorActions.ui.dialog.models.DialogModel = Backbone.Model.extend({
    defaults: {
      // True if the item is being configured.
      active: false,
      // The path to a form to load for configuration.
      formPath: '',
      // A query selector to find this item.
      selector: null
    },


    /**
     * {@inheritdoc}
     */
    destroy: function (options) {
      this.trigger('destroy', this, this.collection, options);
    }
  });

  /*******************************************************************
   * V I E W S
   *******************************************************************/
  Drupal.visitorActions.ui.dialog.views = Drupal.visitorActions.ui.dialog.views || {};
  Drupal.visitorActions.ui.dialog.views.ElementDialogView = Backbone.View.extend({

    /**
     * {@inheritdoc}
     */
    initialize: function(options) {
      options = options || {};
      // The anchor determines the placement of the window.
      this.anchor = this.el;
      if (!this.anchor.id || this.anchor.id.length === 0) {
        // Assign a temporary id for purposes of manipulation within this dialog.
        this.anchor.id = 'visitorActionsUIDialog-' + new Date().getTime();
      }
      this.delay = (options.delay && !isNaN(options.delay)) ? options.delay : 100;
      if (options.myHorizontalEdge && ['left','right','center'].indexOf(options.myHorizontalEdge) >= 0) {
        this.myHorizontalEdge = options.myHorizontalEdge;
      } else {
        this.myHorizontalEdge = (document.documentElementDir === 'rtl') ? 'right' : 'left';
      }
      if (options.anchorHorizontalEdge && ['left','right','center'].indexOf(options.anchorHorizontalEdge) >= 0) {
        this.anchorHorizontalEdge = options.anchorHorizontalEdge;
      } else {
        this.anchorHorizontalEdge = this.myHorizontalEdge;
      }
      this.myVerticalEdge = (options.myVerticalEdge && ['top','bottom','center'].indexOf(options.myVerticalEdge) >= 0) ? options.myVerticalEdge : 'top';
      this.anchorVerticalEdge = (options.anchorVerticalEdge && ['top','bottom','center'].indexOf(options.anchorVerticalEdge) >= 0) ? options.anchorVerticalEdge : 'bottom';
      this.horizontalPadding = options.horizontalPadding && !isNaN(options.horizontalPadding) ? options.horizontalPadding : -4;
      this.collision = (options.collision && ['flip','fit','flipfit','none'].indexOf(options.collision) >= 0) ? options.collision : 'flipfit';

      this.listenTo(this.model, 'change:active', this.render);
      this.listenTo(this.model, 'change:active', this.deactivate);
      this.listenTo(this.model, 'destroy', this.remove);
    },

    render: function (model, active) {
      if (!active) {
        return;
      }
      var that = this;
      // Create the dialog and set it as the element for this view.
      this.setElement($(Drupal.theme('visitorActionsUIElementDialog', {
        id: this.model.id
      })));
      if (this.className) {
        this.$el.addClass(this.className);
      }
      this.$el.appendTo('body');
      // Reposition the dialog on window scroll and resize.
      $(window).off('.visitorActionsUI.elementDialogView');
      $(window).on('resize.visitorActionsUI.elementDialogView scroll.visitorActionsUI.elementDialogView', function (event) {
        that.position();
      });

      // The rest of this processing is specific to AJAX forms within the
      // dialog.
      var formPath = this.model.get('formPath');
      if (typeof formPath !== 'string' || formPath.length === 0) {
        return;
      }

      /**
       * Dismisses this instance of ElementDialogView
       */
      Drupal.ajax.prototype.commands.visitor_actions_ui_dismiss = function (ajax, response, status) {
        Drupal.ajax.prototype.commands.visitor_actions_ui_dismiss = null;
        Drupal.ajax[that.anchor.id] = null;
        that.model.set('active', false);
      }

      /**
       * Override the Drupal.ajax error handler.
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

      // We need to know when the insert command is called and position the
      // dialog after the form DOM elements have been inserted.
      var insert = Drupal.ajax.prototype.commands.insert;
      /**
       * Hooks into the Drupal.ajax insert command.
       */
      Drupal.ajax.prototype.commands.insert = function (ajax, response, status) {
        // Deal with incremented form IDs.
        if (ajax.wrapper === '#visitor-actions-form') {
          ajax.wrapper = '#' + ajax.form[0].id;
        }
        // Call the position method.
        if (ajax.event === 'formAction.visitorActionsUI.elementDialogView') {
          that.position(function () {
            that.show();
          });
        }
        // Call the original insert command.
        insert.call(this, ajax, response, status);
        // Put the original insert command back.
        Drupal.ajax.prototype.commands.insert = insert;
      }

      // Perform an AJAX request to get a specific form.
      Drupal.ajax[this.anchor.id] = new Drupal.ajax(this.anchor.id, this.anchor, {
        url: formPath,
        event: 'formAction.visitorActionsUI.elementDialogView',
        wrapper: that.model.id + '-dialog .visitor-actions-ui-placeholder',
        progress: {
          type: null
        },
        success: function (response, status) {
          $('#' + that.anchor.id).off('formAction.visitorActionsUI.elementDialogView');
          that.formSuccessHandler.call(that, this, response, status);
        },
        complete: function () {
          // Put the original Drupal.ajax error handler back.
          Drupal.ajax.prototype.error = ajaxError;
          ajaxError = null;
        }
      });

      // Trigger the form load after the dialog has rendered.
      $('#' + this.anchor.id).trigger('formAction.visitorActionsUI.elementDialogView');
    },

    /**
     * A callback to be overridden to handle any events when a form is loaded.
     */
    formSuccessHandler: function (ajax, response, status) {
      Drupal.ajax.prototype.success.call(ajax, response, status);
    },

    /**
     * Removes the form dialog from the DOM.
     *
     * @param Backbone.Model model
     *   The model for this view.
     * @param Boolean active
     *   Indicates if the model is active or not.
     */
    deactivate: function (model, active) {
      if (active) {
        return;
      }
      this.el.parentNode.removeChild(this.el);
    },

    /**
     * {@inheritDoc}
     */
    remove: function (model) {
      $(window).off('.visitorActionsUI.elementDialogView');
      this.setElement(null);
      Backbone.View.prototype.remove.call(this);
    },

    /**
     * Uses the jQuery.ui.position() method to position the dialog.
     *
     * @param function callback
     *   (optional) A function to invoke after positioning has finished.
     */
    position: function (callback) {
      clearTimeout(this.timer);

      var that = this;
      if (typeof(that.callback) !== 'undefined') {
        that.callback = callback ? callback : that.callback;
      } else {
        that.callback = callback;
      }

      /**
       * Refines the positioning algorithm of jquery.ui.position().
       *
       * Invoked as the 'using' callback of jquery.ui.position() in
       * positionDialog().
       *
       * @param Object suggested
       *   A hash of top and left values for the position that should be set. It
       *   can be forwarded to .css() or .animate().
       * @param Object info
       *   The position and dimensions of both the 'my' element and the 'of'
       *   elements, as well as calculations to their relative position. This
       *   object contains the following properties:
       *     - Object element: A hash that contains information about the HTML
       *     element that will be positioned. Also known as the 'my' element.
       *     - Object target: A hash that contains information about the HTML
       *     element that the 'my' element will be positioned against. Also known
       *     as the 'of' element.
       */
      function refinePosition (suggested, info) {
        var $pointer = info.element.element.find('.visitor-actions-ui-dialog-pointer');
        var coords = {
          left: Math.floor(suggested.left),
          top: Math.floor(suggested.top)
        };

        /**
         * Calculates the position of the pointer in relation to the target.
         */
        function pointerPosition () {
          var swag = info.target.left  - info.element.left;
          var element = info.element.element;
          var elWidth = element.outerWidth();
          var pointerWidth = $pointer.outerWidth();
          var gutter = parseInt(element.css('padding-left').slice(0, -2), 10) + parseInt(element.css('padding-right').slice(0, -2), 10);
          // Don't let the value be less than zero.
          swag = (swag > gutter) ? swag : gutter;
          // Don't let the value be greater than the width of the element.
          swag = (swag + pointerWidth < (elWidth - gutter)) ? swag : elWidth - pointerWidth - gutter;
          return swag;
        }

        // Determine if the pointer should be on the top or bottom.
        info.element.element.toggleClass('visitor-actions-ui-dialog-pointer-top', info.vertical === 'top');
        // Determine if the pointer should be on the left or right.
        $pointer.css('left', pointerPosition());
        // Apply the positioning.
        info.element.element.css(coords);
      }

      /**
       * Calls the jquery.ui.position() method of the $el of this view.
       *
       * @param function callback
       * (optional) A function to invoke after positioning has finished.
       */
      function positionDialog () {
        that.$el.position_visitor_actions_ui({
          my: that.myHorizontalEdge + ' ' + that.myVerticalEdge,
          // Move the toolbar 1px towards the start edge of the 'of' element,
          // plus any horizontal padding that may have been added to the element
          // that is being added, to prevent unwanted horizontal movement.
          at: that.anchorHorizontalEdge + '+' + (1 + that.horizontalPadding) + ' ' + that.anchorVerticalEdge,
          of: that.anchor,
          collision: that.collision,
          using: refinePosition
        });
        // Invoke an optional callback after the positioning has finished.
        if (that.callback) {
          that.callback();
        }
        // Clear the callback reference now that it has been called.
        delete that.callback;
      }

      // Uses the jQuery.ui.position() method. Use a timeout to move the toolbar
      // only after the user has focused on an editable for 250ms. This prevents
      // the toolbar from jumping around the screen.
      this.timer = setTimeout(function () {
        // Render the position in the next execution cycle, so that animations on
        // the field have time to process. This is not strictly speaking, a
        // guarantee that all animations will be finished, but it's a simple way
        // to get better positioning without too much additional code.
        _.defer(positionDialog);
      }, that.delay);
    },

    /**
     * Reveals the dialog after it has been positioned.
     *
     * Called as part of the render sequence.
     */
    show: function () {
      this.el.style.display = 'block';
    }
  });

  /**
   * Theme function for an element dialog.
   *
   * @param Object options
   *   Contains the following key:
   *   - id: The id associated with the actionable element.
   *
   * @return String
   *   The corresponding HTML.
   */
  Drupal.theme.visitorActionsUIElementDialog = function (options) {
    var html = '';
    html += '<div id="' + options.id + '-dialog" class="visitor-actions-ui-dialog clearfix" style="display:none;">';
    html += '<i class="visitor-actions-ui-dialog-pointer"></i>';
    html += '<div class="visitor-actions-ui-dialog-content">';
    html += '<div class="visitor-actions-ui-placeholder"></div>';
    html += '</div>';
    return html;
  };

}(Drupal, Drupal.jQuery, _, Backbone));
