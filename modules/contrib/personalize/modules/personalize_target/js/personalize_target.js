(function ($) {

Drupal.personalize_target = (function() {

  var decision_points = {}, initialized = false;

  function init() {
    var settings = Drupal.settings.personalize.option_sets;
    for (var i in settings) {
      if (settings.hasOwnProperty(i)) {
        var option_set = settings[i];
        var decision_point = option_set.decision_point;
        var decision_name = option_set.decision_name;
        if (option_set.hasOwnProperty('targeting')) {
          for (var j in option_set.targeting) {
            if (option_set.targeting.hasOwnProperty(j) && option_set.targeting[j].hasOwnProperty('option_id') && option_set.targeting[j].hasOwnProperty('targeting_features')) {
              decision_points[decision_point] = decision_points[decision_point] || {};
              decision_points[decision_point][decision_name] = decision_points[decision_point][decision_name] || {
                'mapped_features' : {}
              };
              // Loop through all features specified for a targeting rule and add them to the
              // features map for this decision.
              for (var k in option_set.targeting[j].targeting_features) {
                if (option_set.targeting[j].targeting_features.hasOwnProperty(k)) {
                  var feature_name = option_set.targeting[j].targeting_features[k];
                  decision_points[decision_point][decision_name].mapped_features[feature_name] = option_set.targeting[j].option_id;
                }
              }
            }
          }
        }
      }
    }
  }

  function contextToFeatureString(key, value) {
    return key + '::' + value;
  }

  function convertContextToFeatureStrings(visitor_context) {
    var i, j, feature_strings = [];
    for (i in visitor_context) {
      if (visitor_context.hasOwnProperty(i)) {
        for (j in visitor_context[i]) {
          if (visitor_context[i].hasOwnProperty(j)) {
            feature_strings.push(contextToFeatureString(i, visitor_context[i][j]));
          }
        }
      }
    }
    return feature_strings;
  }

  return {
    'getDecision': function(name, visitor_context, choices, decision_point, fallbacks, callback) {
      if (!initialized) {
        init();
      }
      var decisions = {};
      var feature_strings = convertContextToFeatureStrings(visitor_context);
      for (var j in choices) {
        if (choices.hasOwnProperty(j)) {
          // Initialize the decision to the fallback option.
          var fallbackIndex = fallbacks.hasOwnProperty(j) ? fallbacks[j] : 0;
          decisions[j] = choices[j][fallbackIndex];
          if (decision_points.hasOwnProperty(decision_point) && decision_points[decision_point].hasOwnProperty(j)) {
            // See if any of the visitor context features has an option mapped to it.
            for (var i in feature_strings) {
              if (feature_strings.hasOwnProperty(i) && decision_points[decision_point][j].mapped_features.hasOwnProperty(feature_strings[i])) {
                decisions[j] = decision_points[decision_point][j].mapped_features[feature_strings[i]];
                break; // No need to look at any other feature strings.
              }
            }
          }
        }
      }
      callback(decisions);
    }
  }
})();

Drupal.personalize = Drupal.personalize || {};
Drupal.personalize.agents = Drupal.personalize.agents || {};
Drupal.personalize.agents.personalize_target = {
  'getDecisionsForPoint': function(agent_name, visitor_context, choices, decision_point, fallbacks, callback) {
    Drupal.personalize_target.getDecision(agent_name, visitor_context, choices, decision_point, fallbacks, callback);
  },
  'sendGoalToAgent': function(agent_name, goal_name, value) {
    if (window.console) {
      console.log('Sending goal ' + goal_name + ' to agent ' + agent_name + ' with value ' + value);
    }
  },
  'featureToContext': function(featureString) {
    var contextArray = featureString.split('::');
    return {
      'key': contextArray[0],
      'value': contextArray[1]
    }
  }
};

})(jQuery);
