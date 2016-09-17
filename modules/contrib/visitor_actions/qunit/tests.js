/**
 * Helper function to clean up after tests.
 */
function cleanup() {
  var $ = jQuery;
  Drupal.settings.visitor_actions.actions = {};
  Drupal.visitorActions.publisher.reset();
  Drupal.visitorActions.page.reset();
  $('a').unbind('click');
  $('form').unbind('submit');
}

/**
 * Set up to initialize base settings.
 */
QUnit.module("Visitor Actions", {
  setup: function() {
    // Set up the Drupal settings.
    Drupal.settings.visitor_actions.pageContext = 'quint/tests.html';
    Drupal.settings.visitor_actions.content_wrapper = Drupal.settings.visitor_actions.content_wrapper || '#page-wrapper';
    Drupal.settings.visitor_actions.actionableElementTypes = Drupal.settings.visitor_actions.actionableElementTypes || [
      { selector: null, type: 'page' },
      { selector: 'form[action]', type: 'form' },
      { selector: 'form[action]', type: 'base_form' },
      { selector: 'a[href]', type: 'link' }
    ];
    Drupal.settings.visitor_actions.actions = Drupal.settings.visitor_actions.actions || {};
    Drupal.settings.visitor_actions.ignoreClasses = Drupal.settings.visitor_actions.ignoreClasses || '';
    Drupal.settings.visitor_actions.ignoreIds = Drupal.settings.visitor_actions.ignoreIds || '';
  }
});

/**
 * P A G E  E L E M E N T S
 */

/**
 * Page view.
 */
QUnit.test('Visitor actions page elements', function (assert) {
  $ = jQuery;
  expect(3);

  Drupal.settings.visitor_actions.actions['pageView'] = {
    actionable_element: 'page',
      event: 'view'
  };

  function subscribeCallback(name, event, context) {
    assert.equal(name, 'pageView', 'Callback called for pageView.');
    assert.ok(event.type === 'PageView', 'pageView called as a page view event.');
    assert.equal(context.PageView, Drupal.settings.visitor_actions.pageContext, 'Page view set in context.');
  }

  // Subscribe to the actions.
  Drupal.visitorActions.publisher.subscribe(subscribeCallback);

  // Attach the behaviors.
  Drupal.behaviors.visitorActions.attach($('#qunit-fixture'), Drupal.settings);

  cleanup();
});

/**
 * Page view with delayed subscriber.
 */
QUnit.test('Visitor actions page view with delayed subscription', function (assert) {
  $ = jQuery;
  expect(6);

  Drupal.settings.visitor_actions.actions['pageView'] = {
    actionable_element: 'page',
    event: 'view'
  };

  function subscribeCallback(name, event, context) {
    assert.equal(name, 'pageView', 'Callback called for pageView.');
    assert.ok(event.type === 'PageView', 'pageView called as a page view event.');
    assert.equal(context.PageView, Drupal.settings.visitor_actions.pageContext, 'Page view set in context.');
  }

  function subscribeCallbackDelay(name, event, context) {
    assert.equal(name, 'pageView', 'Delayed callback called for pageView.');
    assert.ok(event.type === 'PageView', 'pageView called as a page view event.');
    assert.equal(context.PageView, Drupal.settings.visitor_actions.pageContext, 'Page view set in context.');
  }

  // Subscribe to the actions.
  Drupal.visitorActions.publisher.subscribe(subscribeCallback);

  // Attach the behaviors (triggering the page view event).
  Drupal.behaviors.visitorActions.attach($('#qunit-fixture'), Drupal.settings);

  // Subscribe to the actions after the event occurs.
  Drupal.visitorActions.publisher.subscribe(subscribeCallbackDelay);

  cleanup();
});

/**
 * Page stay.
 */
QUnit.asyncTest('Visitor actions page stay event', function (assert) {
  $ = jQuery;
  expect(2);

  Drupal.settings.visitor_actions.actions['pageStay'] =  {
    actionable_element: 'page',
      event: 'stay',
      options: {remains_for: 1}
  };

  function subscribeCallback(name, event, context) {
    QUnit.start();
    assert.equal(name, 'pageStay', 'Callback called for pageStay.');
    assert.equal(context.PageView, Drupal.settings.visitor_actions.pageContext, 'Page view set in page context.');
  }

  // Subscribe to the actions.
  Drupal.visitorActions.publisher.subscribe(subscribeCallback);

  // Attach the behaviors.
  Drupal.behaviors.visitorActions.attach($('#qunit-fixture'), Drupal.settings);

  QUnit.stop();
  setTimeout(function() {
    // Should have called the callback by now.
    QUnit.start();

    cleanup();
  }, 2000);
});

/**
 * Page scroll to bottom.
 */
QUnit.test('Visitor actions page scroll event', function (assert) {
  $ = jQuery;
  expect(2);

  Drupal.settings.visitor_actions.actions['pageScroll'] =  {
    actionable_element: 'page',
    event: 'scrollToBottom',
    options: {bottomOffset: 50}
  };

  function subscribeCallback(name, event, context) {
    assert.equal(name, 'pageScroll', 'Callback called for pageScroll.');
    assert.equal(context.PageView, Drupal.settings.visitor_actions.pageContext, 'Page view set in page context.');
  }

  // Subscribe to the actions.
  Drupal.visitorActions.publisher.subscribe(subscribeCallback);

  // Attach the behaviors.
  Drupal.behaviors.visitorActions.attach($('#qunit-fixture'), Drupal.settings);

  // Scroll the page, but not enough to trigger the callback.
  // If triggered, then the expected number of test results will be off.
  window.scroll(0, $(document).height() - 75);
  $(window).trigger('scroll');

  // Scroll far enough to trigger the callback.
  window.scroll(0, $(document).height()-25);
  $(window).trigger('scroll');
  cleanup();
});

/**
 * L I N K  E L E M E N T S
 */
QUnit.test('Visitor actions link elements', function (assert) {
  $ = jQuery;
  expect(8);

  Drupal.settings.visitor_actions.actions['clicks-link-a'] =  {
    actionable_element: 'link',
      event: 'click',
      identifier: 'a.link-a',
      options: []
  };

  $('a.link-a').bind('click', function() {
    assert.ok(true, 'A link was clicked.');
    return false;
  });

  function subscribeCallback(name, event, context) {
    assert.equal(name, 'clicks-link-a', 'Callback called for clicks-link-a.');
    assert.ok(event.type === 'click', 'clicks-link-a called as a click event.');
    assert.equal(context.Click.AnchorText, 'Link A', 'Link anchor text in context');
    assert.deepEqual(context.Click.DataAttributes, {'visitor-actions': 'test'}, 'Link data attributes in context.');
    assert.equal(context.Click.DestinationUrl, 'test.html', 'Link destination URL in context.');
    assert.deepEqual(context.Click.LinkClasses, ['link-a', 'visitorActions-clicks-link-a-processed'], 'Link classes set in context.');
    assert.equal(context.PageView, Drupal.settings.visitor_actions.pageContext, 'Page view set in context.');
  }

  // Subscribe to the actions.
  Drupal.visitorActions.publisher.subscribe(subscribeCallback);

  // Attach the behaviors.
  Drupal.behaviors.visitorActions.attach($('#qunit-fixture'), Drupal.settings);

  // Simulate a link click.
  $('a.link-a').click();

  // Clean-up.
  cleanup();
});

/**
 * F O R M  E L E M E N T S
 */
QUnit.test('Visitor actions form elements', function (assert) {
  $ = jQuery;
  expect(8);

  Drupal.settings.visitor_actions.actions = {
    'submit-form-a': {
      actionable_element: 'form',
      event: 'submit_client',
      identifier: 'form-a',
      options: []
    },
    'submit-form-b': {
      actionable_element: 'form',
      event: 'submit_client',
      identifier: 'form-b',
      options: []
    }
  };

  $('form.form-a').bind('submit', function() {
    assert.ok(true, 'A form was submitted.');
    return false;
  });

  $('form.form-b').bind('submit', function(event) {
    // Simulate a drupal ajax form handling.
    Drupal.ajax['form-b'].eventResponse(this, event);
    return false;
  })

  // Set up the Drupal Ajax callback information.
  Drupal.ajax.prototype = Drupal.ajax.prototype || {};
  Drupal.ajax.prototype.eventResponse = function(element, event) {
    assert.ok(true, 'Drupal ajax form submission call made.');
    return false;
  };
  Drupal.settings.ajax['form-b'] = {};
  Drupal.ajax['form-b'] = {form: $('#form-b')};

  // Generic callback for subscriber notifications.
  function subscribeCallback(name, event, context) {
    assert.ok(['submit-form-a', 'submit-form-b'].indexOf(name) !== -1, 'Callback called for ' + name);
    assert.ok(event.type === 'submit', name + ' called as a form submit event.');
    assert.equal(context.PageView, Drupal.settings.visitor_actions.pageContext, 'Page view set in context.');
  }

  // Subscribe to the actions.
  Drupal.visitorActions.publisher.subscribe(subscribeCallback);

  // Attach the behaviors.
  Drupal.behaviors.visitorActions.attach($('#qunit-fixture'), Drupal.settings);

  // Simulate a form submit.
  $('form.form-a').submit();
  $('form.form-b').submit();

  cleanup();
});
