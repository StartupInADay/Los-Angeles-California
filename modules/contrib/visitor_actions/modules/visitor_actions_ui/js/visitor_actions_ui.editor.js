/**
 * @file visitor_actions_ui.editor.js
 */
(function (Drupal, $, _, Backbone, Dialog, Utilities) {

"use strict";
$ = Drupal.jQuery;

// If we have a version of jQuery > 1.7, then use $.proxy.
// $.proxy_visitor_actions_ui will be defined when jQuery < 1.7.
$.proxy_visitor_actions_ui = $.proxy_visitor_actions_ui || $.proxy;

Drupal.behaviors.visitorActionsUIEditor = {
  attach: function () {
    var model = Drupal.visitorActions.ui.models.appModel || new Drupal.visitorActions.ui.AppModel({
      editMode: Drupal.settings.visitor_actions.edit_mode
    });
    if (!Drupal.visitorActions.ui.views.appView) {
      Drupal.visitorActions.ui.models.appModel = model;
      Drupal.visitorActions.ui.views.appView = new Drupal.visitorActions.ui.AppView({
        el: Drupal.settings.visitor_actions.content_wrapper || 'body',
        model: model,
        actionableElements: new Backbone.Collection([], {
          model: Drupal.visitorActions.ui.ActionModel
        }),
        ActionView: Drupal.visitorActions.ui.ActionView,
        ActionDialogView: Drupal.visitorActions.ui.ActionDialogVisualView
      });
    }
    // Destroy the toggle views and rebuild them in each pass.
    _.each(Drupal.visitorActions.ui.views.toggleViews, function (view) {
      view.remove();
      view = null;
    });
    var toggleViews = Drupal.visitorActions.ui.views.toggleViews = [];
    // Process the visitor actions edit mode toggle.
    $('[href*="' + Drupal.settings.basePath + Drupal.settings.pathPrefix + 'admin/structure/visitor_actions/add"]')
      .each(function (index, element) {
        toggleViews.push(
          (new Drupal.visitorActions.ui.AppToggleView({
            el: element,
            model: model
          }))
        );
      });
    Drupal.visitorActions.ui.views.toggleViews = toggleViews;
    // Process actionable elements on every invocation of attach().
    var view = Drupal.visitorActions.ui.views.appView;
    var modelDefinitions = view.buildActionableElementModels(Drupal.settings.visitor_actions.actionableElementTypes, view.el);
    view.actionableElements.add(modelDefinitions);

    // Close the editor dialog if the the overlay is opened.
    $(document).bind('drupalOverlayOpen.visitoractionsui', function () {
      _.each(view.actionableElements.where({active: true}), function (model) {
        model.set('active', false);
      });
    });
  }
};

Drupal.visitorActions = Drupal.visitorActions || {};
Drupal.visitorActions.ui = Drupal.visitorActions.ui || {};
$.extend(Drupal.visitorActions.ui, {

  // A hash of View instances.
  views: {},

  // A hash of Model instances.
  models: {},

  /**
   * Backbone model for the context page.
   */
  AppModel: Backbone.Model.extend({
    defaults: {
      // If this app is being loaded, it is because it is being launched into
      // an edit mode.
      editMode: true
    },

    /**
     * {@inheritdoc}
     */
    destroy: function (options) {
      this.trigger('destroy', this, this.collection, options);
    }
  }),

  /**
   * Backbone controller view for page-level interactions.
   */
  AppView: Backbone.View.extend({

    /**
     * {@inheritdoc}
     */
    initialize: function (options) {

      this.actionableElements = options.actionableElements;
      this.$pageActionsContainer = $();

      this.model.on('change:editMode', this.render, this);
      this.model.on('change:editMode', this.toggleEditMode, this);

      // When actionable elements are added to the collection,
      var decorate = $.proxy_visitor_actions_ui(this.decorateActionableElement, this, options.ActionView, options.ActionDialogView);
      this.actionableElements.on('add', decorate, this);
      this.actionableElements.on('change:active', this.switchActiveElement, this);

      // Run the setup methods once on initialization.
      for (var i = 0, methods = ['render', 'toggleEditMode'], len = methods.length; i < len; i++) {
        this[methods[i]](this.model, this.model.get('editMode'));
      }
    },

    /**
     * {@inheritdoc}
     */
    render: function (model, editMode) {
      this.$pageActionsContainer.toggle(editMode);
    },

    /**
     * {@inheritdoc}
     */
    remove: function () {
      this.actionableElements.each(function (model) {
        model.set({
          enabled: false,
          active: false
        });
        model.destroy();
      });
      this.setElement(null);
      Backbone.View.prototype.remove.call(this);
    },

    /**
     * Toggles the app on and off.
     *
     * @param Backbone.Model model
     * @param Boolean editMode
     *   true is active and false is inactive.
     */
    toggleEditMode: function (model, editMode) {
      this.actionableElements.each(function (model) {
        if (editMode) {
          model.set({
            enabled: editMode
          });
        }
        else {
          model.set({
            enabled: false,
            active: false
          });
        }
      });
    },

    /**
     *
     */
    buildActionableElementModels: function (settings, context) {
      var definitions = [];
      var instanceCount = 0;
      var item, type, selector;
      context = context || 'body';

      /**
       * Returns a page-unique ID for the provided element.
       *
       * @param DOM element
       *
       * @return String
       */
      function getElementID (element) {
        // Use an id to reference the HTML element. If the element does not have
        // an id attribute, create one from the current time. This value will
        // never be stored, so it does not need to be globally unique.
        var id;
        if (element.id) {
          id = element.id;
        }
        else {
          id = 'visitorActionsUI-' + (new Date()).getTime() + '-' + (instanceCount++);
          element.id = id;
        }
        return id;
      }

      /**
       * Creates and inserts a trigger of adding actions that have no
       * identifier.
       */
      function insertErsatzLink(view, context, item) {
        var $container = $('#visitor-actions-ui-actionable-elements-without-identifiers');
        if (!$container.length) {
          $container = view.$pageActionsContainer = $(Drupal.theme('visitorActionsUIActionableElementsWithoutIdentifiers', {
            id: 'visitor-actions-ui-actionable-elements-without-identifiers'
          })).prependTo(context);
        }
        var $button = $(Drupal.theme('visitorActionsUIButton', {
          text: Drupal.t('Add @type action', {'@type': item.type})
        })).appendTo($container);
        // Give this element an id.
        item.id = getElementID($button.get(0));
        item.selector = '#' + item.id;
        return item;
      }


      /**
       * Returns a definition structure for an ActionModel.
       *
       * @param String type
       *   The type of ActionModel e.g. 'link'
       * @param String selector
       *   A query selector to find this element.
       * @param DOM element
       *   A DOM element associated with this ActionModel. The association is
       *   maintained by the element's ID.
       *
       * @return Object
       */
      function defineModel (type, selector, element) {
        return {
          id: getElementID(element),
          type: type,
          selector: selector
        };
      }

      for (var i = 0, len = settings.length; i < len; i++) {
        item = settings[i];
        type = item.type;
        selector = item.selector;
        // Create a temporary link for this actionable element.
        if (!selector) {
          item = insertErsatzLink(this, context, item);
          selector = item.selector;
        }
        // Create standard model definitions for the actionable elements.
        $(context).find(selector)
          .once('visitor-actions-ui')
          // Give developers a chance to opt out on a one-by-one basis.
          .not('.visitor-actions-ui-ignore')
          // Try to eliminate administration elements.
          .filter(function () {
            var el = this;
            // Filter on the id for blacklisted components.
            var id = el.id || '';
            // Filter for blacklisted class name components.
            var className = typeof this.className === 'string' && this.className || '';
            // Filter for blacklisted href components.
            var href = this.attributes['href'] && this.attributes['href'].value || '';
            // Eliminate any visitor actions components.
            var rVA = /^visitor-actions/;
            // Eliminate local tasks and contextual links.
            var rTask = /local-task|contextual/;
            // Eliminate admin links.
            var rAdmin = /^\/?admin/;
            // Eliminate node action links.
            var rNode = /^\/?node(\/)?(\d)*(?=\/add|\/edit|\/delete)/;
            // Reject the element if any tests match.
            if (rVA.test(id) || rTask.test(className) || rAdmin.test(href) || rNode.test(href)) {
              return false;
            }
            // Check to see if it is within visitor actions dialog.
            if ($(this).parents('.visitor-actions-ui-dialog').length > 0) {
              return false;
            }
            // Keep the element as the default.
            return true;
          })
          .each(function (index, element) {
            definitions.push(defineModel(type, selector, element));
          });
      }

      return definitions;
    },

    /**
     * Associates an ActionView with an ActionModel.
     */
    decorateActionableElement: function (ActionView, ActionDialogView, model) {
      // Create an ActionView and ActionDialogView for the model.
      var options = {
        el: document.getElementById(model.id),
        model: model
      };
      (new ActionView(options));
      (new ActionDialogView(options));
      model.set('enabled', this.model.get('editMode'));
    },

    /**
     * Ensures that only one actionable element is active at a time.
     */
    switchActiveElement: function (model, active) {
      if (!active) {
        return;
      }
      _.chain(this.actionableElements.models)
        .reject(function (m) {
          return m.id === model.id;
        })
        .each(function (m) {
          m.set('active', false);
        });
    }
  }),

  /**
   *
   */
  AppToggleView: Backbone.View.extend({

    /**
     * {@inheritdoc}
     */
    initialize: function () {
      this.model.on('change:editMode', this.render, this);
      this.model.on('destroy', this.remove, this);
      // Set the toggle based on the initial value of the AppModel editMode.
      this.render(this.model, this.model.get('editMode'));
    },

    /**
     * {@inheritdoc}
     */
    render: function (model, editMode) {
      this.$el.toggleClass('visitor-actions-ui-toggle-active', editMode);
      var text = (editMode) ? Drupal.t('Exit add goal mode') : Drupal.t('Add goal');
      this.$el.text(text);
    },

    /**
     * {@inheritdoc}
     */
    remove: function () {
      this.undelegateEvents();
      this.$el.removeData().off();
      this.setElement(null);
      Backbone.View.prototype.remove.call(this);
    }
  }),

  /**
   * The model for an actionable element.
   */
  ActionModel: Dialog.models.DialogModel.extend({
    defaults: _.extend({}, Dialog.models.DialogModel.prototype.defaults,
      {
        // True if the Action element is available for activation.
        enabled: false,
        // A type of element, e.g. 'link', or 'form'.
        type: null
      }
    )
  }),

  /**
   * Backbone View/Controller for creating Actions.
   */
  ActionView: Backbone.View.extend({

    events: {
      'click': 'onClick'
    },

    /**
     * {@inheritdoc}
     */
    initialize: function (options) {
      this.model.on('change', this.render, this);
      this.model.on('destroy', this.remove, this);

      this.render();
    },

    /**
     * {@inheritdoc}
     */
    render: function (model) {
      var enabled = this.model.get('enabled');
      this.$el.toggleClass('visitor-actions-ui-enabled', enabled);
      this[(enabled) ? 'delegateEvents' : 'undelegateEvents']();
      this.$el.toggleClass('visitor-actions-ui-active', this.model.get('active'));
    },

    /**
     * {@inheritdoc}
     */
    remove: function () {
      this.setElement(null);
      // Remove the processed marker.
      this.$el.removeClass('visitor-actions-ui-processed');
      Backbone.View.prototype.remove.call(this);
    },

    /**
     * Responds to clicks.
     *
     * @param jQuery.Event event
     */
    onClick: function (event) {
      this.model.set('active', true);
      event.preventDefault();
      event.stopPropagation();
      event.stopImmediatePropagation();
    }
  }),

  /**
   * Backbone view for the dialog element that contains the interaction UI.
   */
  ActionDialogVisualView: Dialog.views.ElementDialogView.extend({

    initialize: function(options) {
      options.delay = 100;
      this.parent('inherit', options);
    },

    /**
     * {@inheritdoc}
     */
    render: function (model, active) {
      if (!active) {
        return;
      }
      var that = this;
      var type = this.model.get('type');
      var formPath = Drupal.settings.basePath + Drupal.settings.pathPrefix +
        'visitor_actions/add/' +
        Drupal.encodePath(type) +
        '?path=' + Drupal.encodePath(Drupal.settings.visitor_actions.currentPath);
      model.set('formPath', formPath);

      /**
       * Redirects to the callback that disables edit mode.
       *
       * This in turn redirects to where the user came from before they enabled
       * edit mode.
       */
      Drupal.ajax.prototype.commands.visitor_actions_ui_command_redirect = function (ajax, response, status) {
        Drupal.ajax.prototype.commands.visitor_actions_ui_command_redirect = null;
        var destination = document.location.href;
        document.location.href = response.redirect_url + '?destination=' + destination;
      };

      /**
       * Prefills the form with known values after it is loaded into the DOM.
       */
      Drupal.ajax.prototype.commands.visitor_actions_ui_prefill_form = function (ajax, response, status) {
        Drupal.ajax.prototype.commands.visitor_actions_ui_prefill_form = null;
        // Prefill the title for the action.
        var $item = $(ajax.selector);
        var $dialog = $(ajax.selector + '-dialog');
        var title = '';
        switch (that.model.get('type')) {
          case 'form':
            if (ajax.element && ajax.element.id) {
              title = ajax.element.id
                .replace(/-/g, ' ')
                .replace(/^[a-z]/, function (char) {
                  return char.toUpperCase();
                })
                .replace(/\s?form/, '')
                .trim();
              title = Drupal.t('@title form', {'@title': title});
            }
            break;
          case 'page':
            title = /^(.*)\s\|.*/.exec(document.title)[1].trim();
            title = Drupal.t('@title page', {'@title': title});
            break;
          default:
            if (ajax.element && ajax.element.innerText && ajax.element.innerText.length > 0) {
              title = ajax.element.innerText.replace(/^[a-z]/, function (char) {
                return char.toUpperCase();
              }).trim();
              title = Drupal.t('@title link', {'@title': title});
            }
            break;
        }
        title = Drupal.t('@title action', {'@title': title});
        $dialog
          .find('[name="title"]')
          .val(title)
          // Trigger a keyup to produce a machine name.
          .trigger('keyup');


        // Prefill the selector for the element.
        var ignoreClassesRegex = new RegExp(Drupal.settings.visitor_actions.ignoreClasses, 'g');
        var ignoreIdRegex = new RegExp(Drupal.settings.visitor_actions.ignoreIds);
        var selector = Utilities.getSelector(ajax.element, ignoreIdRegex, ignoreClassesRegex);
        $dialog.find('[name="identifier[' + type + ']"]').val(Drupal.formatString(selector));
      };

      this.parent('render', model, active);
    },

    /**
     * Uses the jQuery.ui.position() method to position the dialog.
     *
     * @param function callback
     *   (optional) A function to invoke after positioning has finished.
     */
    position: function (callback) {
      this.parent('position', callback);
    },

    formSuccessHandler: function (ajax, response, status) {
      this.parent('formSuccessHandler', ajax, response, status);
      $('input[name="identifier[form]"], input[name="identifier[link]"]', this.el).on('change', function(e) {
        var $idForm = $(this).closest('form');
        try {
          var test = $(this).val();
          var valid = $(test);
        } catch (error) {
          $(this).addClass('error');
          if ($('.visitor-actions-ui-js-message', $idForm).length == 0) {
            var errorMessage = '<div class="visitor-actions-ui-js-message"><div class="messages error"><h2 class="element-invisible">' + Drupal.t('Error message') + '</h2>';
            errorMessage += Drupal.t('Selector field must contain a valid jQuery selector.');
            errorMessage += '</div></div>';
            $idForm.prepend(errorMessage);
          }
          $('#edit-submit-form', $idForm).attr('disabled', 'disabled').addClass('form-button-disabled');
          return;
        }
        // If we are still here then validation passed and any previous
        // error messages should be removed.
        $(this).removeClass('error');
        $('.visitor-actions-ui-js-message', $idForm).remove();
        $('#edit-submit-form', $idForm).removeAttr('disabled').removeClass('form-button-disabled');
      });
    }
  })
});

/**
 * Theme function for a container to present actionable elements without IDs.
 *
 * @param Object options
 *   Contains the following key:
 *   - id: The id associated with the container.
 *
 * @return String
 *   The corresponding HTML.
 */
Drupal.theme.visitorActionsUIActionableElementsWithoutIdentifiers = function (options) {
  return '<div id="' + options.id + '" class="visitor-actions-ui-actionable-elements-without-identifiers clearfix"></div>';
};

/**
 * Theme function for a simple button.
 *
 * @param Object options
 *   Contains the following key:
 *   - text: The button text.
 *
 * @return String
 *   The corresponding HTML.
 */
Drupal.theme.visitorActionsUIButton = function (options) {
  return '<button>' + options.text + '</button>';
};

}(Drupal, Drupal.jQuery, _, Backbone, Drupal.visitorActions.ui.dialog, Drupal.utilities));
