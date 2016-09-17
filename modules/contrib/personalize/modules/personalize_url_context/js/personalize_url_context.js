(function ($) {

  var initialized = false;
  var context = 'querystring_context';

  function init() {
    for (var name in Drupal.settings.personalize_url_context.querystring_params) {
      if (Drupal.settings.personalize_url_context.querystring_params.hasOwnProperty(name)) {
        Drupal.personalize.visitor_context_write(name, context, Drupal.settings.personalize_url_context.querystring_params[name]);
      }
    }
    var baseUrl = Drupal.settings.personalize_url_context.base_url, referrer = document.referrer;
    if (referrer && referrer.indexOf(baseUrl) == -1) {
      Drupal.personalize.visitor_context_write('referrer_url', context, referrer);
      Drupal.personalize.visitor_context_write('original_referrer_url', context, referrer, false);
    }
    initialized = true;
  }

  Drupal.behaviors.personalize_url_context = {
    attach: function (context, settings) {
      if (!initialized) {
        init();
      }
    }
  };

  Drupal.personalize = Drupal.personalize || {};
  Drupal.personalize.visitor_context = Drupal.personalize.visitor_context || {};
  Drupal.personalize.visitor_context.querystring_context = {
    'getContext': function(enabled) {
      if (!initialized) {
        init();
      }
      var i, context_values = {};
      for (i in enabled) {
        if (enabled.hasOwnProperty(i)) {
          var val = Drupal.personalize.visitor_context_read(i, context);
          if (val !== null) {
            context_values[i] = val;
          }
        }
      }
      return context_values;
    }
  };

})(jQuery);
