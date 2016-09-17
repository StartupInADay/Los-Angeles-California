QUnit.test( "Executor test", function( assert ) {
  expect(22);
  // Provide a mock of the attach behaviors function for purposes of tracking
  // that it is correctly called.
  Drupal.attachBehaviors = function(bread, circus) {
    assert.ok(true, 'Attach behaviors called.');
  }

  // Test the executor for a regular option set.
  assert.equal(0, $('.osid-1-first-option').length);
  assert.equal(0, $('.osid-1-second-option').length);
  Drupal.personalize.executors.show.execute($('[data-personalize=osid-1]'), 'first-choice', 1);
  assert.equal(1, $('.osid-1-first-option').length);
  assert.equal(0, $('.osid-1-second-option').length);
  Drupal.personalize.executors.show.execute($('[data-personalize=osid-1]'), 'second-choice', 1);
  assert.equal(0, $('.osid-1-first-option').length);
  assert.equal(1, $('.osid-1-second-option').length);

  // Test an option set that appears multiple times on the page.
  assert.equal(0, $('.osid-2-first-option').length);
  assert.equal(0, $('.osid-2-second-option').length);
  Drupal.personalize.executors.show.execute($('[data-personalize=osid-2]'), 'first-choice', 2);
  assert.equal(2, $('.osid-2-first-option').length);
  assert.equal(0, $('.osid-2-second-option').length);
  Drupal.personalize.executors.show.execute($('[data-personalize=osid-2]'), 'second-choice', 2);
  assert.equal(0, $('.osid-2-first-option').length);
  assert.equal(2, $('.osid-2-second-option').length);

  // Test the executor for an option set with an empty html attribute
  Drupal.personalize.executors.show.execute($('[data-personalize=osid-3]'), 'second-choice', 3);
  assert.equal(0, $('.osid-3-first-option').length);
  assert.equal(1, $('.osid-3-noscript-option').length);

  // Test the executor for an option set with an empty html attribute
  // that is rendered multiple times
  Drupal.personalize.executors.show.execute($('[data-personalize=osid-4]'), 'second-choice', 4);
  assert.equal(0, $('.osid-4-first-option').length);
  assert.equal(2, $('.osid-4-noscript-option').length);
});

QUnit.test( "Evaluate contexts test", function( assert ) {
  var agentType = 'js_test_agent';
  Drupal.personalize.agents = Drupal.personalize.agents || {};

  Drupal.personalize.agents[agentType] = {
    'featureToContext': function(featureName) {
      var contextArray = featureName.split('--');
      return {
        'key': contextArray[0],
        'value': contextArray[1]
      };
    }
  };
  var visitorContext = {
    'some_plugin': {
      'some-context-key': 'some-value'
    },
    'some_other_plugin': {
      'ohai': 'stuff',
      'numeric-context': 43
    }
  };
  var featureRules = {
    'some-context-key--sc-some-value': {
      'context': 'some-context-key',
      'match': 'value',
      'operator': 'contains',
      'plugin': 'some_plugin'
    },
    'ohai--stuff': {
      'context': 'ohai',
      'match': 'stuff',
      'operator': 'equals',
      'plugin': 'some_other_plugin'
    },
    'numeric-context--gt-42': {
      'context': 'numeric-context',
      'match': 42,
      'operator': 'numgt',
      'plugin': 'some_other_plugin'
    }
  };
  // Try with a non-existent agent type, we should get an empty object.
  var evaluated = Drupal.personalize.evaluateContexts('non_existent_type', visitorContext, featureRules);
  assert.ok( evaluated !== null);
  assert.ok( $.isEmptyObject(evaluated) );
  // Now try with our dummy agent type.
  var evaluated = Drupal.personalize.evaluateContexts(agentType, visitorContext, featureRules);
  assert.equal( evaluated['some-context-key'].length, 2, "First context key has the correct number of values" );
  assert.equal( evaluated['some-context-key'][0], 'some-value', "First context key has the basic value" );
  assert.equal( evaluated['some-context-key'][1], 'sc-some-value', "First context key has the operator-derived value" );
  assert.equal( evaluated['ohai'].length, 1, "Second context key has the correct number of values" );
  assert.equal( evaluated['ohai'][0], 'stuff', "Second context key has the basic value" );
  assert.equal( evaluated['numeric-context'].length, 2, "Third context key has the correct number of values" );
  assert.equal( evaluated['numeric-context'][0], 43, "Third context key has the basic value" );
  assert.equal( evaluated['numeric-context'][1], 'gt-42', "Third context key has the operator-derived value" );
});

QUnit.asyncTest( "Get visitor contexts test", function( assert ) {
  expect(7);
  // Set-up
  function assignDummyValues(contexts) {
    var values = {
      'some-context': 'some-value',
      'some-other-context': 'some-other-value',
      'ohai': 42,
      'kthxbai': 0,
      'mahna': 'dodoo'
    };
    var myValues = {};
    for (var i in contexts) {
      if (contexts.hasOwnProperty(i) && values.hasOwnProperty(i)) {
        myValues[i] = values[i];
      }
    }
    return myValues;
  }
  Drupal.personalize = Drupal.personalize || {};
  Drupal.personalize.contextTimeout = 3000;

  Drupal.personalize.visitor_context = Drupal.personalize.visitor_context || {};
  Drupal.personalize.visitor_context.my_first_plugin = {
    'getContext': function(contexts) {
      return assignDummyValues(contexts);
    }
  };
  Drupal.personalize.visitor_context.my_second_plugin = {
    'getContext': function(contexts) {
      return assignDummyValues(contexts);
    }
  };

  Drupal.personalize.visitor_context.my_promise_plugin = {
    'getContext': function(contexts) {
      return new Promise(function(resolve, reject) {
        setTimeout(function() {
          resolve(assignDummyValues(contexts));
          QUnit.start();
        }, (Drupal.personalize.contextTimeout - 1000));
      });
    }
  }
  // End of set-up.

  var contexts = {
    'my_first_plugin': {
      'some-context': 'some-context',
      'some-other-context': 'some-other-context'
    },
    'my_second_plugin': {
      'ohai': 'ohai',
      'kthxbai': 'kthxbai'
    },
    'my_promise_plugin': {
      'mahna': 'mahna'
    },
    'my_nonexistent_plugin': {
      'foo': 'foo'
    }
  };
  var callback = function(contextValues) {
    assert.ok(contextValues.hasOwnProperty('my_first_plugin'));
    assert.equal(contextValues.my_first_plugin['some-context'], 'some-value');
    assert.equal(contextValues.my_first_plugin['some-other-context'], 'some-other-value');
    assert.equal(contextValues.my_second_plugin['ohai'], 42);
    assert.equal(contextValues.my_second_plugin['kthxbai'], 0);
    assert.equal(contextValues.my_nonexistent_plugin, null);
    assert.equal(contextValues.my_promise_plugin['mahna'], 'dodoo');
    QUnit.start();
  };
  QUnit.stop();
  Drupal.personalize.getVisitorContexts(contexts, callback);
});

QUnit.asyncTest( "Get visitor contexts timeout test", function( assert ) {
  expect(3);
  // Set-up
  function assignDummyValues(contexts) {
    var values = {
      'some-context': 'some-value',
      'some-other-context': 'some-other-value',
      'mahna': 'dodoo'
    };
    var myValues = {};
    for (var i in contexts) {
      if (contexts.hasOwnProperty(i) && values.hasOwnProperty(i)) {
        myValues[i] = values[i];
      }
    }
    return myValues;
  }
  Drupal.personalize = Drupal.personalize || {};
  Drupal.personalize.contextTimeout = 300;

  Drupal.personalize.visitor_context = Drupal.personalize.visitor_context || {};
  Drupal.personalize.visitor_context.my_first_plugin = {
    'getContext': function(contexts) {
      return assignDummyValues(contexts);
    }
  };
  Drupal.personalize.visitor_context.my_promise_plugin = {
    'getContext': function(contexts) {
      return new Promise(function(resolve, reject) {
        setTimeout(function() {
          resolve(assignDummyValues(contexts));
        }, (Drupal.personalize.contextTimeout+100));
      });
    }
  }
  // End of set-up.

  var contexts = {
    'my_first_plugin': {
      'some-context': 'some-context',
      'some-other-context': 'some-other-context'
    },
    'my_promise_plugin': {
      'mahna': 'mahna'
    }
  };
  var callback = function(contextValues) {
    assert.ok(contextValues.hasOwnProperty('my_first_plugin'));
    // Contexts should still be returned when there wasn't an error.
    assert.equal(contextValues.my_first_plugin['some-context'], 'some-value');
    assert.equal(contextValues.my_first_plugin['some-other-context'], 'some-other-value');
    QUnit.start();
  };
  Drupal.personalize.getVisitorContexts(contexts, callback);
});

QUnit.module("Personalize page tests", {
  'restore': {},
  'setup': function() {
    Drupal.personalize.resetAll();
    Drupal.settings.personalize = {
      'cacheExpiration': {
        'decisions': 'session'
      },
      'agent_map': {
        'my-agent': {
          'active': 1,
          'cache_decisions': false,
          'enabled_contexts': [],
          'type': 'test_agent'
        },
        'my-inactive-agent': {
          'active': 0,
          'cache_decisions': false,
          'enabled_contexts': [],
          'type': 'test_agent'
        }
      },
      'option_sets': {
        'osid-0': {
          'agent': 'my-inactive-agent',
          'data': [],
          'decision_name': 'osid-0',
          'decision_point': 'osid-0',
          'executor': 'test',
          'label': 'My Test',
          'mvt': null,
          'option_names': ['first-option', 'second-option'],
          'options': [
            {
              'option_id': 'first-option',
              'option_label': 'First Option'
            },
            {
              'option_id': 'second-option',
              'option_label': 'Second Option'
            }
          ],
          'osid': 'osid-0',
          'plugin': 'my_os_plugin',
          'selector': '.some-class',
          'stateful': 0,
          'winner': null
        },
        'osid-1': {
          'agent': 'my-agent',
          'data': [],
          'decision_name': 'osid-1',
          'decision_point': 'osid-1',
          'executor': 'show',
          'label': 'My Test',
          'mvt': null,
          'option_names': ['first-option', 'second-option'],
          'options': [
            {
              'option_id': 'first-option',
              'option_label': 'First Option'
            },
            {
              'option_id': 'second-option',
              'option_label': 'Second Option'
            }
          ],
          'osid': 'osid-1',
          'plugin': 'my_os_plugin',
          'selector': '.some-class',
          'stateful': 0,
          'winner': null
        }
      }
    };
    Drupal.personalize.agents.test_agent = {};
    this.restore.execute = Drupal.personalize.executors.show.execute;
  },
  'teardown': function() {
    if (this.restore.hasOwnProperty('execute')) {
      Drupal.personalize.executors.show.execute = this.restore.execute;
    }
    delete(Drupal.personalize.executors.test);
    $.bbq.removeState();
    Drupal.decisionQueuerAgent.reset();
  }
});

QUnit.asyncTest("Personalize page simple", function( assert ) {
  expect(7);
  QUnit.start();

  Drupal.personalize.agents.test_agent.getDecisionsForPoint = function(name, visitor_context, choices, decision_point, fallbacks, callback) {
    QUnit.start();
    assert.equal(name, 'my-agent');
    assert.ok($.isEmptyObject(visitor_context));
    assert.ok(choices.hasOwnProperty('osid-1'));
    assert.equal(choices['osid-1'][0], 'first-option');
    assert.equal(choices['osid-1'][1], 'second-option');
    assert.equal(decision_point, 'osid-1');
    callback.call(null, {'osid-1': 'second-option'});
  };
  Drupal.personalize.executors.show.execute = function ($option_sets, choice_name, osid, preview) {
    assert.equal('second-option', choice_name);
  };

  QUnit.stop();
  Drupal.personalize.personalizePage(Drupal.settings);
});

QUnit.asyncTest("trigger decision event", function( assert ) {
  expect(3);
  QUnit.start();
  var simplePersonalizeDecisionFunc = function( e, $option_set, decision, osid, agent_name ) {
    assert.equal(decision, 'second-option');
    assert.equal(osid, 'osid-1');
    assert.equal(agent_name, 'my-agent');
    $(document).unbind("personalizeDecision", simplePersonalizeDecisionFunc);
  };
  $(document).bind("personalizeDecision", simplePersonalizeDecisionFunc);
  Drupal.personalize.agents.test_agent.getDecisionsForPoint = function(name, visitor_context, choices, decision_point, fallbacks, callback) {
    QUnit.start();
    callback.call(null, {'osid-1': 'second-option'});
  };

  QUnit.stop();
  Drupal.personalize.personalizePage(Drupal.settings);
});

QUnit.asyncTest( "Invalid selector test", function ( assert ) {
  expect(7);
  // Set an option with an invalid selector and test that all data is still
  // passed up without generating a JavaScript error.
  Drupal.settings.personalize.option_sets['osid-1']['selector'] = '#myinvalid selector#';

  QUnit.start();
  Drupal.personalize.agents.test_agent.getDecisionsForPoint = function(name, visitor_context, choices, decision_point, fallbacks, callback) {
    QUnit.start();
    assert.equal(name, 'my-agent');
    assert.ok($.isEmptyObject(visitor_context));
    assert.ok(choices.hasOwnProperty('osid-1'));
    assert.equal(choices['osid-1'][0], 'first-option');
    assert.equal(choices['osid-1'][1], 'second-option');
    assert.equal(decision_point, 'osid-1');
    callback.call(null, {'osid-1': 'second-option'});
  };
  Drupal.personalize.executors.show.execute = function ($option_sets, choice_name, osid, preview) {
    assert.equal('second-option', choice_name);
    // Put the selector back to a normal selector.
    Drupal.settings.personalize.option_sets['osid-1']['selector'] = '.some-class';
  };

  QUnit.stop();
  Drupal.personalize.personalizePage(Drupal.settings);
});


QUnit.asyncTest("Personalize page 2 option sets", function( assert ) {
  // The getDecisionsForPoint method should be called twice, once for each option set.
  expect(8);
  QUnit.start();

  // Add a second option set.
  addOptionSetToDrupalSettings('osid-2', 'osid-2', 'osid-2');
  Drupal.personalize.agents.test_agent.getDecisionsForPoint = function(name, visitor_context, choices, decision_point, fallbacks, callback) {
    switch(decision_point) {
      case 'osid-1':
        assert.equal(name, 'my-agent');
        assert.ok($.isEmptyObject(visitor_context));
        assert.ok(choices.hasOwnProperty('osid-1'), 'Got osid-1');
        callback.call(null, {'osid-1': 'second-option'});
        break;
      case 'osid-2':
        assert.equal(name, 'my-agent');
        assert.ok($.isEmptyObject(visitor_context));
        assert.ok(choices.hasOwnProperty('osid-2'), 'Got osid-2');
        callback.call(null, {'osid-2': 'first-option'});
        QUnit.start();
        break;
    }
  };
  Drupal.personalize.executors.show.execute = function ($option_sets, choice_name, osid, preview) {
    if (osid == 'osid-1') {
      assert.equal('second-option', choice_name);
    }
    if (osid == 'osid-2') {
      assert.equal('first-option', choice_name);
    }
  };

  QUnit.stop();
  Drupal.personalize.personalizePage(Drupal.settings);
});

QUnit.asyncTest("Personalize page 2 option sets one decision", function( assert ) {
  // The getDecisionsForPoint method should be called only once for the two option sets.
 expect(6);
  QUnit.start();
  // Create 2 option sets with the same decision name.
  addOptionSetToDrupalSettings('osid-2', 'osid-2', 'osid-2');
  Drupal.settings.personalize.option_sets['osid-1'].decision_name = Drupal.settings.personalize.option_sets['osid-2'].decision_name = 'my_decision';
  Drupal.settings.personalize.option_sets['osid-1'].decision_point = Drupal.settings.personalize.option_sets['osid-2'].decision_point = 'my_decision_point';

  Drupal.personalize.agents.test_agent.getDecisionsForPoint = function(name, visitor_context, choices, decision_point, fallbacks, callback) {
    QUnit.start();
    assert.equal(name, 'my-agent');
    assert.ok($.isEmptyObject(visitor_context));
    assert.ok(choices.hasOwnProperty('my_decision'));
    assert.equal(decision_point, 'my_decision_point');
    callback.call(null, {'my_decision': 'first-option'});
  };
  Drupal.personalize.executors.show.execute = function ($option_sets, choice_name, osid, preview) {
    assert.equal('first-option', choice_name);
  };
  QUnit.stop();
  Drupal.personalize.personalizePage(Drupal.settings);
});

QUnit.asyncTest("Personalize page MVT", function( assert ) {
  // The getDecisionsForPoint method should be called only once for the two option sets.
  expect(7);
  QUnit.start();
  // Create an MVT.
  var mvt_name = 'mymvt';
  addMVTToDrupalSettings(mvt_name, 2);

  Drupal.personalize.agents.test_agent.getDecisionsForPoint = function(name, visitor_context, choices, decision_point, fallbacks, callback) {
    QUnit.start();
    assert.equal(name, 'my-agent');
    assert.ok($.isEmptyObject(visitor_context));
    assert.ok(choices.hasOwnProperty('osid-1'));
    assert.ok(choices.hasOwnProperty('osid-2'));
    assert.equal(decision_point, mvt_name);
    callback.call(null, {'osid-1': 'second-option', 'osid-2': 'first-option'});
  };
  Drupal.personalize.executors.show.execute = function ($option_sets, choice_name, osid, preview) {
    if (osid == 'osid-1') {
      assert.equal('second-option', choice_name);
    }
    if (osid == 'osid-2') {
      assert.equal('first-option', choice_name);
    }
  };
  QUnit.stop();
  Drupal.personalize.personalizePage(Drupal.settings);
});

QUnit.asyncTest("Personalize page with visitor context", function( assert ) {
  // The getDecisionsForPoint method should be called only once for the two option sets.
  expect(7);
  QUnit.start();
  // Set up the agent to have a user profile context enabled.
  Drupal.settings.personalize.agent_map['my-agent'].enabled_contexts = {'user_profile_context': {'my_user_profile_field': 'my_user_profile_field'}};
  // Now add a value for that context to the settings.
  Drupal.settings.personalize_user_profile_context = {
    'my_user_profile_field': 'my_user_profile_value'
  };
  Drupal.personalize.agents.test_agent.getDecisionsForPoint = function(name, visitor_context, choices, decision_point, fallbacks, callback) {
    QUnit.start();
    assert.equal(name, 'my-agent');
    assert.ok(choices.hasOwnProperty('osid-1'));
    assert.equal(decision_point, 'osid-1');
    assert.ok(visitor_context.hasOwnProperty('my_user_profile_field'));
    assert.equal(1, visitor_context.my_user_profile_field.length);
    assert.equal("my_user_profile_value", visitor_context.my_user_profile_field[0]);
    callback.call(null, {'osid-1': 'second-option'});
  };
  Drupal.personalize.executors.show.execute = function ($option_sets, choice_name, osid, preview) {
    assert.equal('second-option', choice_name);
  };
  QUnit.stop();
  Drupal.personalize.personalizePage(Drupal.settings);
});

QUnit.asyncTest("Stateful option set", function( assert ) {
  expect(8);
  QUnit.start();
  // Add a stop call for each asynchronous function we want to put
  // an assertion in.
  QUnit.stop();
  QUnit.stop();
  QUnit.stop();
  Drupal.settings.personalize.option_sets['osid-1'].stateful = 1;
  Drupal.personalize.agents.test_agent.getDecisionsForPoint = function(name, visitor_context, choices, decision_point, fallbacks, callback) {
    QUnit.start();
    assert.equal(name, 'my-agent');
    assert.ok($.isEmptyObject(visitor_context));
    assert.ok(choices.hasOwnProperty('osid-1'));
    assert.equal(choices['osid-1'][0], 'first-option');
    assert.equal(choices['osid-1'][1], 'second-option');
    assert.equal(decision_point, 'osid-1');
    callback.call(null, {'osid-1': 'second-option'});
  };
  Drupal.personalize.executors.show.execute = function ($option_sets, choice_name, osid, preview) {
    assert.equal('second-option', choice_name);
    QUnit.start();
  };

  Drupal.personalize.personalizePage(Drupal.settings);
  $(window).bind( 'hashchange', function(e) {
    var state = $.param.fragment();
    if (state == 'osid-1=second-option') {
      // This assertion may seem meaningless given the condition
      // above, but we declare at the start of the test how many
      // assertions we expect, so the test will fail if we don't
      // get here.
      assert.ok(state, 'stateful option set works');
      QUnit.start();
    }
  });

});

QUnit.asyncTest('Decision caching', function( assert ) {
  // Since decision caching is turned on for this test, make sure we remove anything we
  // add to session storage at the start of the test.
  sessionStorage.removeItem("Drupal.personalize:decisions:my-agent:osid-1");
  // Set decision caching to true for our test agent.
  Drupal.settings.personalize.agent_map['my-agent'].cache_decisions = true;

  expect(8);
  QUnit.start();
  Drupal.personalize.agents.test_agent.getDecisionsForPoint = function(name, visitor_context, choices, decision_point, fallbacks, callback) {
    QUnit.start();
    assert.equal(name, 'my-agent');
    assert.ok($.isEmptyObject(visitor_context));
    assert.ok(choices.hasOwnProperty('osid-1'));
    assert.equal(choices['osid-1'][0], 'first-option');
    assert.equal(choices['osid-1'][1], 'second-option');
    assert.equal(decision_point, 'osid-1');
    callback.call(null, {'osid-1': 'second-option'});
  };
  var reran = false;
  Drupal.personalize.executors.show.execute = function ($option_sets, choice_name, osid, preview) {
    // This should be called twice.
    assert.equal('second-option', choice_name, 'Executor got the expected option');
    if (!reran) {
      reran = true;
      Drupal.personalize.resetAll();
      Drupal.personalize.personalizePage(Drupal.settings);
    }
  };

  QUnit.stop();
  Drupal.personalize.personalizePage(Drupal.settings);

});

QUnit.asyncTest("MVT decision caching", function( assert ) {
  // Since decision caching is turned on for this test, make sure we remove anything we
  // add to session storage at the start of the test.
  sessionStorage.removeItem("Drupal.personalize:decisions:my-agent:mymvt");
  // Set decision caching to true for our test agent.
  Drupal.settings.personalize.agent_map['my-agent'].cache_decisions = true;
  // The getDecisionsForPoint method should be called only once for the two option sets.
  expect(9);
  QUnit.start();
  // Create an MVT.
  var mvt_name = 'mymvt';
  addMVTToDrupalSettings(mvt_name, 2);

  Drupal.personalize.agents.test_agent.getDecisionsForPoint = function(name, visitor_context, choices, decision_point, fallbacks, callback) {
    QUnit.start();
    assert.equal(name, 'my-agent');
    assert.ok($.isEmptyObject(visitor_context));
    assert.ok(choices.hasOwnProperty('osid-1'));
    assert.ok(choices.hasOwnProperty('osid-2'));
    assert.equal(decision_point, mvt_name);
    callback.call(null, {'osid-1': 'second-option', 'osid-2': 'first-option'});
  };
  var reran = false;
  var needsStart = false;
  Drupal.personalize.executors.show.execute = function ($option_sets, choice_name, osid, preview) {
    // QUnit will not get restarted in the getDecisionsForPoint the second time
    // because decisions will come from the cache.  It can only be started
    // once, however for the mvt.
    if (needsStart) {
      QUnit.start();
      needsStart = false;
    }
    if (osid == 'osid-1') {
      assert.equal('second-option', choice_name);
    }
    if (osid == 'osid-2') {
      assert.equal('first-option', choice_name);
    }
    if (!reran) {
      reran = true;
      Drupal.personalize.resetAll();
      QUnit.stop();
      needsStart = true;
      Drupal.personalize.personalizePage(Drupal.settings);
    }
  };
  QUnit.stop();
  Drupal.personalize.personalizePage(Drupal.settings);
});

QUnit.asyncTest('Decision caching expiration', function( assert ) {
  // Clear out the storage.
  Drupal.personalize.storage.utilities.maintain();
  // Since decision caching is turned on for this test, make sure we remove
  // anything we add to local storage at the start of the test.
  localStorage.removeItem("Drupal.personalize:decisions:my-agent:osid-1");
  // Set decision caching to true for our test agent.
  Drupal.settings.personalize.agent_map['my-agent'].cache_decisions = true;
  // Set the expiration for decisions to 3 seconds.
  Drupal.settings.personalize.cacheExpiration.decisions = .05;

  // The expect assertion is actually what determines the success or failure of
  // this test as total amount of callbacks indicates the expiration.
  expect(15);
  QUnit.start();
  // This callback should be called twice.
  Drupal.personalize.agents.test_agent.getDecisionsForPoint = function(name, visitor_context, choices, decision_point, fallbacks, callback) {
    QUnit.start();
    assert.equal(name, 'my-agent');
    assert.ok($.isEmptyObject(visitor_context));
    assert.ok(choices.hasOwnProperty('osid-1'));
    assert.equal(choices['osid-1'][0], 'first-option');
    assert.equal(choices['osid-1'][1], 'second-option');
    assert.equal(decision_point, 'osid-1');
    callback.call(null, {'osid-1': 'second-option'});
  };
  var reran = false;
  var complete = false;
  // Show executor should be called three times.
  Drupal.personalize.executors.show.execute = function ($option_sets, choice_name, osid, preview) {
    assert.equal('second-option', choice_name, 'Executor got the expected option');
    if (complete) {
      return;
    }
    if (!reran) {
      reran = true;
      Drupal.personalize.resetAll();
      Drupal.personalize.personalizePage(Drupal.settings);
    } else {
      // Now expire the decision.
      Drupal.personalize.resetAll();
      QUnit.stop();
      setTimeout(function () {
        complete = true;
        // In a new page call the storage would be pruned.
        Drupal.personalize.storage.utilities.maintain();
        Drupal.personalize.personalizePage(Drupal.settings);
      }, 4000);
    }
  };

  QUnit.stop();
  Drupal.personalize.personalizePage(Drupal.settings);

});


QUnit.asyncTest("Complex option set processing", function( assert ) {
  // This entire test is about the expected number of assertions. It has multiple option sets
  // on the page, some inactive. The active option sets make use of mulitple promise-based
  // contexts, and the decision agent queues up decisions until it has a session ID available.
  expect(17);
  QUnit.start();

  // Set-up
  function assignDummyValues(contexts) {
    var values = {
      'mahna': 'dodoo',
      'ohai': 'kthxbai'
    };
    var myValues = {};
    for (var i in contexts) {
      if (contexts.hasOwnProperty(i) && values.hasOwnProperty(i)) {
        myValues[i] = values[i];
      }
    }
    return myValues;
  }

  Drupal.personalize = Drupal.personalize || {};
  Drupal.personalize.contextTimeout = 300;
  Drupal.personalize.visitor_context = Drupal.personalize.visitor_context || {};

  Drupal.personalize.visitor_context.my_promise_plugin = Drupal.personalize.visitor_context.my_other_promise_plugin = {
    'getContext': function (contexts) {
      return new Promise(function (resolve, reject) {
        setTimeout(function () {
          resolve(assignDummyValues(contexts));
        }, ((Drupal.personalize.contextTimeout - 100)));
      });
    }
  };

  // Set up the agent to have a couple of promise-based contexts enabled.
  var enabled_contexts = {
    'my_promise_plugin': {
      'mahna': 'mahna'
    },
    'my_other_promise_plugin': {
      'ohai': 'ohai'
    }
  };
  Drupal.settings.personalize.agent_map['my-agent'].enabled_contexts = enabled_contexts;
  // Add another agent that uses the same contexts.
  Drupal.settings.personalize.agent_map['my-other-agent'] = {
    'active': 1,
    'cache_decisions': false,
    'enabled_contexts': enabled_contexts,
    'type': 'test_agent'
  };
  // Add a second option set and assign it to the new agent.
  addOptionSetToDrupalSettings('osid-2', 'osid-2', 'osid-2');
  Drupal.settings.personalize.option_sets['osid-2'].agent = 'my-other-agent';
  // End of set-up.

  Drupal.personalize.agents.test_agent.getDecisionsForPoint = function (name, visitor_context, choices, decision_point, fallbacks, callback) {
    var callbackWrapper = function () {
      switch (decision_point) {
        case 'osid-1':
          assert.equal(name, 'my-agent');
          assert.ok(choices.hasOwnProperty('osid-1'));
          assert.equal(decision_point, 'osid-1');
          assert.ok(visitor_context.hasOwnProperty('mahna'));
          assert.ok(visitor_context.hasOwnProperty('ohai'));
          assert.equal(1, visitor_context.mahna.length);
          assert.equal("dodoo", visitor_context.mahna[0]);
          callback.call(null, {'osid-1': 'second-option'});
          break;
        case 'osid-2':
          assert.equal(name, 'my-other-agent');
          assert.ok(choices.hasOwnProperty('osid-2'));
          assert.equal(decision_point, 'osid-2');
          assert.ok(visitor_context.hasOwnProperty('mahna'));
          assert.ok(visitor_context.hasOwnProperty('ohai'));
          assert.equal(1, visitor_context.mahna.length);
          assert.equal("dodoo", visitor_context.mahna[0]);
          callback.call(null, {'osid-2': 'second-option'});
          QUnit.start();
          break;
      }
    };
    // Use the decisionQueuerAgent to get the decisions.
    Drupal.decisionQueuerAgent.getDecision(name, visitor_context, choices, decision_point, fallbacks, callbackWrapper);
  };
  Drupal.personalize.executors.show.execute = function ($option_sets, choice_name, osid, preview) {
    assert.equal('second-option', choice_name);
    // This simulates what happens when an executor kicks off the personalizePage
    // cycle again through Drupal.attachBehaviors.
    Drupal.personalize.personalizePage(Drupal.settings);
  };
  Drupal.personalize.executors.test = {
    'execute': function($option_sets, choice_name, osid, preview) {
      assert.equal('first-option', choice_name);
      // This simulates what happens when an executor kicks off the personalizePage
      // cycle again through Drupal.attachBehaviors.
      Drupal.personalize.personalizePage(Drupal.settings);
    }
  };
  QUnit.stop();
  Drupal.personalize.personalizePage(Drupal.settings);
});

function addOptionSetToDrupalSettings(osid, decision_name, decision_point, mvt) {

  Drupal.settings.personalize.option_sets[osid] = {
    'agent': 'my-agent',
    'data': [],
    'decision_name': decision_name,
    'decision_point': decision_point,
    'executor': 'show',
    'label': 'My Test',
    'mvt': mvt ? mvt : null,
    'option_names': ['first-option', 'second-option'],
    'options': [
      {
        'option_id': 'first-option',
        'option_lablel': 'First Option'
      },
      {
        'option_id': 'second-option',
        'option_label': 'Second Option'
      }
    ],
    'osid': osid,
    'plugin': 'my_os_plugin',
    'selector': '.some-other-class',
    'stateful': 0,
    'winner': null
  };
}

function addMVTToDrupalSettings(name, num_option_sets) {

  Drupal.settings.personalize.mvt = Drupal.settings.personalize.mvt || {};
  Drupal.settings.personalize.mvt[name] = {
    'agent': 'my-agent',
    'id': 1,
    'label': name,
    'machine_name': name,
    'option_sets': {},
    'stateful': 0
  }

  for (var i = 0; i < num_option_sets; i++) {
    var osid = 'osid-' + (i+1);
    addOptionSetToDrupalSettings(osid, osid, name, name);
    Drupal.settings.personalize.mvt[name]['option_sets'][osid] = Drupal.settings.personalize.option_sets[osid];
  }
}

var drupalPersonalizeSessionID = null;
Drupal.personalize.initializeSessionID = function() { return drupalPersonalizeSessionID };

Drupal.personalize.saveSessionID = function(session_id) {
  drupalPersonalizeSessionID = session_id;
};

/**
 * Provides a more sophisticated decision agent for testing. This agent queues up decisions
 * so that it can generate a session ID once and use it for all decisions.
 */
Drupal.decisionQueuerAgent = (function() {

  var sessionID = false, initializingSession = false, initialized = false, waitingDecisions = [];

  function readSessionID() {
    if (!sessionID) {
      sessionID = Drupal.personalize.initializeSessionID();
    }
    return sessionID;
  }

  function init() {
    initializingSession = !Boolean(readSessionID());
    initialized = true;
  }

  return {
    // Processes any decisions that have been queued up while the session was
    // initializing.
    'processWaitingDecisions': function() {
      while (waitingDecisions.length > 0) {
        var decision = waitingDecisions.shift();
        this.getDecision(decision.agent_name, decision.visitor_context, decision.choices, decision.point, decision.fallbacks, decision.callback);
      }
    },
    // Processes all decisions for a given decision point.
    'getDecision': function(agent_name, visitor_context, choices, point, fallbacks, callback) {
      var self = this;
      if (!initialized) {
        init();
      } else if (initializingSession) {
        // Add this decision to the queue of waiting decisions.
        waitingDecisions.push({
          'agent_name' : agent_name,
          'visitor_context' : visitor_context,
          'choices' : choices,
          'point' : point,
          'fallbacks' : fallbacks,
          'callback' : callback
        });
        return;
      }
      // Simulate getting a decision taking 100ms.
      setTimeout(function() {
        if (!sessionID) {
          var d = new Date().getTime();
          var mySession = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g,
            function(c) {
              var r = (d + Math.random()*16)%16 | 0;
              d = Math.floor(d/16);
              return (c=='x' ? r : (r&0x7|0x8)).toString(16);
            });
          Drupal.personalize.saveSessionID(mySession);
        }
        callback();

        // Now unblock all future decision requests.
        initializingSession = false;
        // Process any decisions that have been waiting in the queue.
        self.processWaitingDecisions();
      }, (100));
    },
    'reset': function() {
      sessionID = null;
      initializingSession = false;
      initialized = false;
      waitingDecisions = [];
    }
  }
})();
