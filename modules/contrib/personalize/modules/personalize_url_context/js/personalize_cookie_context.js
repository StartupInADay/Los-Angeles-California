(function ($) {

  Drupal.personalize = Drupal.personalize || {};
  Drupal.personalize.visitor_context = Drupal.personalize.visitor_context || {};
  Drupal.personalize.visitor_context.cookie_context = {
    'getContext': function(enabled) {
      var contextName, val, context_values = {}, exists_prefix = Drupal.settings.personalize_url_context.cookie_context_exists_prefix;
      for (contextName in enabled) {
        if (enabled.hasOwnProperty(contextName)) {
          // The context could just be a boolean, i.e. "does this cookie exist or not?" or
          // it could be that we need the actual value of the cookie.
          if (contextName.indexOf(exists_prefix) === 0) {
            var cookieName = contextName.substring(exists_prefix.length);
            val = $.cookie(cookieName);
            context_values[contextName] = val !== null ? 1 : 0;
          }
          else {
            val = $.cookie(contextName);
            if (val !== null) {
              context_values[contextName] = val;
            }
          }
        }
      }
      return context_values;
    }
  };

})(jQuery);
