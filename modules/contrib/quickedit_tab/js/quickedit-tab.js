/**
 * @file
 * Provide a helper tab for easier in-place editing.
 */

(function ($, Backbone, Drupal, undefined) {

"use strict";

Drupal.behaviors.quickEditTab = {
  attach: function (context) {
    // Use backbone in scenarios where navbar tab is used.
    var $body = $(window.parent.document.body).once('quickedit-tab');
    if ($body.length) {
      var tabModel = Drupal.quickEditTab.models.tabModel = new Drupal.quickEditTab.TabStateModel();
      var $tab = $('#quickedit-navbar-tab').once('quickedit-tab');
      if ($tab.length > 0) {
        Drupal.quickEditTab.views.tabView = new Drupal.quickEditTab.TabView({
          el: $tab.get(),
          model: tabModel
        });
      }
    }
  }
};

Drupal.quickEditTab = {

  // Storage for view and model instances.
  models: {},
  views: {},

  // Backbone Model for the navbar tab state.
  TabStateModel: Backbone.Model.extend({
    defaults: {
      // Track how many in-place editable entities exist on the page.
      entityCount: 0,
      // The entity currently being in-place edited, if any.
      activeEntity: null,
      // The contextual links triggers are visible. (This only happens when
      // there are multiple in-place editable entities on the page.)
      contextualLinksTriggersVisible: false,
      // When there is either an active entity, or contextual links triggers are
      // visible, we consider the tab "active".
      isActive: false
    }
  }),

  // Handles the navbar tab interactions.
  TabView: Backbone.View.extend({
    events: {
      'click #quickedit-trigger-link': 'toggleQuickEdit'
    },

    /**
     * {@inheritdoc}
     */
    initialize: function (options) {
      this.listenTo(this.model, {
        'change:activeEntity change:contextualLinksTriggersVisible': this.updateActiveness,
        'change:entityCount change:isActive': this.render,
        'change:contextualLinksTriggersVisible': this.renderContextualLinkTriggers
      });

      this.listenTo(Drupal.quickedit.collections.entities, {
        'add remove reset': this.countEntities,
        'change:state': this.entityStateChange
      });
      // In Drupal 7, we cannot use asset library dependencies to make this
      // JavaScript execute before quickedit.module's, so upon initialization we need
      // to update the entity count manually.
      this.model.set('entityCount', Drupal.quickedit.collections.entities.length);
    },

    /**
     * Toggle in-place editing.
     */
    toggleQuickEdit: function () {
      // Activate!
      if (!this.model.get('isActive')) {
        // If there's only one in-place editable entity, start in-place editing.
        var editableEntities = Drupal.quickedit.collections.entities;
        if (editableEntities.length === 1) {
          editableEntities.at(0).set('state', 'launching');
        }
        // Otherwise, show all contextual links triggers.
        else {
          this.model.set('contextualLinksTriggersVisible', true);
        }
      }
      // Deactivate!
      else {
        // If there's an entity being in-place edited, stop in-place editing.
        var activeEntity = this.model.get('activeEntity');
        if (activeEntity) {
          activeEntity.set('state', 'deactivating');
        }
        // Otherwise, hide all contextual links triggers.
        else {
          this.model.set('contextualLinksTriggersVisible', false);
        }
      }
    },

    /**
     * Tracks the number of in-place editable entities on the page.
     *
     * @param Drupal.quickedit.EntityModel entityModel
     *   The entity model that was added or removed.
     * @param Drupal.quickedit.EntityCollection entityCollection
     *    The collection of entity models.
     */
    countEntities: function (entityModel, entityCollection) {
      this.model.set('entityCount', entityCollection.length);
    },

    /**
     * Tracks whether an entity is actively being in-place edited, and if that's
     * the case, hide all contextual links triggers.
     *
     * @param Drupal.quickedit.EntityModel entityModel
     *   The entity model that whose state has changed.
     * @param String state
     *   One of Drupal.quickedit.EntityModel.states.
     */
    entityStateChange: function (entityModel, state) {
      if (state === 'opened') {
        this.model.set('activeEntity', entityModel);
        this.model.set('contextualLinksTriggersVisible', false);
      }
      else if (state === 'closed') {
        this.model.set('activeEntity', null);
      }
    },

    /**
     * Update activeness based on two factors, makes it
     */
    updateActiveness: function () {
      // We mark the toolbar tab as active when either an entity is being edited
      // in-place, or we're showing all contextual links triggers.
      var hasActiveEntity = !!this.model.get('activeEntity');
      this.model.set('isActive', hasActiveEntity || this.model.get('contextualLinksTriggersVisible'));
    },

    /**
     * Renders visibility & activeness of tab.
     */
    render: function () {
      this.$el.toggleClass('element-invisible', this.model.get('entityCount') === 0);
      this.$el.toggleClass('active', this.model.get('isActive'));
      return this;
    },

    /**
     * Renders contextual links triggers visibility.
     */
    renderContextualLinkTriggers: function (model, show) {
      var classes = 'contextual-links-trigger-active quickedit-contextual-link';
      Drupal.quickedit.collections.entities.forEach(function (editableEntity) {
        var contextualLinkView = editableEntity.get('contextualLinkView');
        contextualLinkView.$el
          .closest('.contextual-links-wrapper')
          .find('.contextual-links-trigger')
          .toggleClass(classes, show);
      });
    }

  })

};

}(jQuery, Backbone, Drupal));
