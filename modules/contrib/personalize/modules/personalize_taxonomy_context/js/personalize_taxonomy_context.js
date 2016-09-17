(function ($) {

  Drupal.personalize = Drupal.personalize || {};
  Drupal.personalize.visitor_context = Drupal.personalize.visitor_context || {};
  Drupal.personalize.visitor_context.taxonomy_context = {
    'getContext': function(enabled) {
      var vocabularies = Drupal.settings.personalize_taxonomy_context.vocabularies, context_values = {}, vocabulary_machine_name;
      if (typeof vocabularies === 'undefined') {
        return context_values;
      }
      for (vocabulary_machine_name in enabled) {
        if (!vocabularies.hasOwnProperty(vocabulary_machine_name)) {
          continue;
        }
        context_values[vocabulary_machine_name] = vocabularies[vocabulary_machine_name];
      }
      return context_values;
    }
  };

})(jQuery);
