/**
 * @file
 * Provide a component that switches the sitewide language.
 */

(function ($, Backbone, Drupal, undefined) {

Drupal.behaviors.interactiveInfoBar = {
  attach: function (context) {
    var $body = $(window.parent.document.body).once('iib');
    if ($body.length) {
      var tabModel = Drupal.interactiveInfoBar.models.tabModel = new Drupal.interactiveInfoBar.TabStateModel();
      var $tab = $('#iib-navbar-tab');
      if ($tab.length > 0) {
        Drupal.interactiveInfoBar.views.tabView = new Drupal.interactiveInfoBar.TabView({
          el: $tab.get(),
          tabModel: tabModel,
        });
      }
      var spsActive = $('.page-iib #edit-cancel').length;
      $tab.toggleClass('sps-active', Boolean(spsActive));
    }
  }
};

Drupal.interactiveInfoBar = Drupal.interactiveInfoBar || {

  // Storage for view and model instances.
  views: {},
  models: {},

  // Backbone Model for the navbar tab state.
  TabStateModel: Backbone.Model.extend({
    defaults: {
      isIIBOpen: false
    }
  }),

  // Handles the navbar tab interactions.
  TabView: Backbone.View.extend({
    events: {
      'click .iib-trigger': 'toggleIIB',
      'mouseleave': 'toggleIIB',
    },

    initialize: function(options) {
      this.tabModel = options.tabModel;
      this.tabModel.on('change:isIIBOpen', this.render, this);
    },

    render: function() {
      var isIIBOpen = this.tabModel.get('isIIBOpen');
      this.$el.toggleClass('open', isIIBOpen);
      return this;
    },

    toggleIIB: function(event){
      if (event.type === 'mouseleave') {
        this.tabModel.set('isIIBOpen', false);
      }
      else {
        this.tabModel.set('isIIBOpen', !this.tabModel.get('isIIBOpen'));
      }
      event.stopPropagation();
      event.preventDefault();
      },
    }),
  };

}(jQuery, Backbone, Drupal));
