(function ($) {

Drupal.personalize = Drupal.personalize || {};
Drupal.personalize.agents = Drupal.personalize.agents || {};
Drupal.personalize.agents.test_agent = {
  'getDecisionsForPoint': function(name, visitor_context, choices, decision_point, callback) {
    var j, decisions = {};
    for (j in choices) {
      if (choices.hasOwnProperty(j)) {
        decisions[j] = choices[j][0];
      }
    }
    callback(decisions);
  },
  'sendGoalToAgent': function(agent_name, goal_name, value, jsEvent) {
    if (window.console) {
      console.log('Sending goal ' + goal_name + ' to agent ' + agent_name + ' with value ' + value);
    }
    var session_id = Drupal.personalize.initializeSessionID();
    if (!session_id) {
      session_id = "anonymous";
    }
    var callback = null;
    $.ajax({
      "url": '/personalize-test/send-goal/ajax/' + agent_name + '/' + session_id + '/' + goal_name + '/' + value,
      "complete": function(jqXHR, status) {
        if (typeof callback === 'function') {
          callback();
          callback = null;
        }
      }
    });
  }
};

})(jQuery);
