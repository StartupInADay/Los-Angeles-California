(function ($) {

  var eventNamespace = 'visitorActions';

  /**
   * IE8 Compatibility functions.
   */

  /**
   * Duct tape for Array.prototype.forEach.
   *
   * @usage forEach([], function () {}, {});
   *
   * @param array arr
   * @param function iterator
   *   The iterator is provided with the following signature and context.
   *   function (item, index, scope) { this === scope }
   * @param object scope
   *   (optional) The value of the iterator invocation object aka 'this'.
   */
  var forEach = Array.prototype.forEach && function (arr, iterator, scope) {
    Array.prototype.forEach.call(arr, iterator, scope);
  } || function (arr, iterator, scope) {
    'use strict';
    var i, len;
    for (i = 0, len = arr.length; i < len; ++i) {
      iterator.call(scope, arr[i], i, arr);
    }
  };

  /**
   * Duct tape for Array.prototype.some.
   *
   * @usage some([], function () {}, {});
   *
   * @param array arr
   * @param function comparator
   *   The comparator is provided with the following signature and context.
   *   function (item, index, scope) { this === scope }
   * @param object scope
   *   (optional) The value of the iterator invocation object aka 'this'.
   */
  var some = Array.prototype.some && function (arr, comparator, scope) {
    return Array.prototype.some.call(arr, comparator, scope);
  } || function (arr, comparator, scope) {
    'use strict';
    if (arr === null) {
      throw new TypeError();
    }

    var thisp, i,
      t = Object(arr),
      len = t.length >>> 0;
    if (typeof comparator !== 'function') {
      throw new TypeError();
    }

    thisp = arguments[2];
    for (i = 0; i < len; i++) {
      if (i in t && comparator.call(thisp, t[i], i, t)) {
        return true;
      }
    }

    return false;
  }

  /**
   * End of IE8 Compatibility functions.
   */

  /**
   * Provides client side visitor action tracking.
   */
  var Publisher = function () {
    this.subscribers = [];
    this.events = [];
  };

  Publisher.prototype = {
    /**
     * Publishes events to listeners.
     *
     * @param string name
     *   The name of the event being delivered.
     * @param Event event
     *   The JavaScript event object
     * @param Object pageContext
     *   An object of page context data.
     */
    deliver: function (name, event, pageContext) {
      forEach(this.subscribers, function (subscriber) {
       subscriber.call(this, name, event, pageContext);
      });
      this.events.push({
        'name': name,
        'event': event,
        'pageContext': pageContext
      });
    },

    /**
     * Registers subscribers to published data.
     *
     * @param Function subscriber
     *   The subscriber callback for event notifications
     * @param boolean receivePreviousEvents
     *   True to get have the subscriber callback invoked with notifications
     *   for all events of this page request that occurred prior to subscribing.
     *   False to only receive notifications for future events.
     *   Defaults to true.
     */
    subscribe: function (subscriber, receivePreviousEvents) {
      receivePreviousEvents = typeof(receivePreviousEvents) == 'undefined';

      var alreadyExists = some(this.subscribers,
        function(el) {
          return el === subscriber;
        }
      );
      if (!alreadyExists) {
        this.subscribers.push(subscriber);

        if (receivePreviousEvents) {
          forEach(this.events, function (eventData) {
            subscriber.call(this, eventData.name, eventData.event, eventData.pageContext);
          })
        }
      }
    },

    /**
     * Clears a publisher's subscriber and event listings.
     *
     * Used for testing purposes.
     */
    reset: function() {
      this.subscribers = [];
      this.events = [];
    }
  };

  Drupal.visitorActions = Drupal.visitorActions || {};
  Drupal.visitorActions.publisher = new Publisher();

  /**
   * Binds events to selectors for client-side visitor actions.
   */
  Drupal.behaviors.visitorActions = {
    attach: function (context, settings) {
      var name, action, callback, boundActions = {};
      for (name in Drupal.settings.visitor_actions.actions) {
        if (Drupal.settings.visitor_actions.actions.hasOwnProperty(name)) {
          action = Drupal.settings.visitor_actions.actions[name];
          if (Drupal.visitorActions.hasOwnProperty(action.actionable_element) && typeof Drupal.visitorActions[action.actionable_element].bindEvent === 'function') {
            callback = (function (innerName){
              return function (event, actionContext) {
                Drupal.visitorActions.publisher.deliver(innerName, event, actionContext);
              }
            })(name);
            // Keep track of what we have bound actions to.
            boundActions[name] = Drupal.visitorActions[action.actionable_element].bindEvent(name, action, context, callback);
          }
        }
      }
      $(document).trigger('visitorActionsBindActions', [boundActions]);
    }
  };

  /**
   * Generates the page context data for visitor actions.
   */
  Drupal.visitorActions.getPageContext = function() {
    // Start with server-side context settings.
    var actionContext = {};
    actionContext['PageView'] = Drupal.settings.visitor_actions.pageContext;

    // Add client-side context settings.
    var clientContext = {};
    clientContext.ReferralPath = document.referrer;

    // Get the query string as a map
    var queryMap = {}, keyValuePairs = location.search.slice(1).split('&');

    forEach(keyValuePairs, function(keyValuePair) {
      keyValuePair = keyValuePair.split('=');
      queryMap[keyValuePair[0]] = keyValuePair[1] || '';
    });
    // Add query string parameters
    clientContext.Campaign = queryMap.hasOwnProperty('utm_campaign') ? queryMap.utm_campaign : '';
    clientContext.Source = queryMap.hasOwnProperty('utm_source') ? queryMap.utm_source : '';
    clientContext.Medium = queryMap.hasOwnProperty('utm_medium') ? queryMap.utm_medium : '';
    clientContext.Term = queryMap.hasOwnProperty('utm_term') ? queryMap.utm_term : '';
    clientContext.Content = queryMap.hasOwnProperty('utm_content') ? queryMap.utm_content : '';
    actionContext['PageView']['TrafficSource'] = clientContext;
    return actionContext;
  };

  Drupal.visitorActions.link = {
    'bindEvent': function (name, action, context, callback) {
      var actionContext = Drupal.visitorActions.getPageContext();
      // Make sure the selector is valid.
      try {
        var $selector = $(action.identifier, context);
      } catch (error) {
        // Can't add a bind event because the selector is invalid.
        return;
      }
      $selector
        .once('visitorActions-' + name)
        .bind(action.event + '.' + eventNamespace, {'eventNamespace' : eventNamespace}, function (event) {
          // Add link click context to action context.
          if (event.type === 'click') {
            var linkContext = {};
            linkContext.DestinationUrl = $(this).attr('href');
            linkContext.AnchorText = $(this).text();
            linkContext.LinkClasses = $(this).attr('class').split(' ');
            linkContext.DataAttributes = {};
            var linkData = $selector.data();
            for (var dataKey in linkData) {
              var typeData = typeof(linkData[dataKey]);
              if (typeData !== 'object' && typeData !== 'undefined' && typeData !== 'function') {
                linkContext.DataAttributes[dataKey] = linkData[dataKey];
              }
            }
            actionContext.Click = linkContext;
          }
          if (typeof callback === 'function') {
            callback.call(null, event, actionContext)
          }
        });
      return $selector;
    }
  };

  Drupal.visitorActions.form = {
    'bindEvent': function (name, action, context, callback) {
      // Drupal form IDs get their underscores converted to hyphens when
      // output as element IDs in markup.
      var formId = action.identifier.replace(/_/g, '-');
      try {
        var $selector = $('form#' + formId, context);
      } catch (error) {
        // Can't add a bind event because the selector is invalid.
        return null;
      }
      var pageContext = Drupal.visitorActions.getPageContext();
      if ($selector.length == 0 || typeof callback !== 'function') {
        return null;
      }
      if (action.event === 'submit_client') {
        // If the selector is within a Drupal AJAX form then we have to
        // work within the Drupal form processing.
        for (var id in Drupal.settings.ajax) {
          var ajaxed = Drupal.ajax[id];
          if (typeof ajaxed !== 'undefined' && typeof ajaxed.form !== 'undefined' && ajaxed.form.length > 0) {
            if (ajaxed.form[0] === $selector[0]) {
              // Re-assign the eventResponse prototype method so that it can
              // be called from the scope of the instance later.
              ajaxed.drupalEventResponse = Drupal.ajax.prototype.eventResponse;
              // Now use the eventResponse prototype to inject visitor actions
              // callback prior to calling the Drupal event handler.
              ajaxed.eventResponse = function(element, event) {
                // Notify of this action.
                callback.call(null, event, pageContext);
                // Now invoke Drupal's event handling.
                return this.drupalEventResponse(element, event);
              };
              return $selector;
            }
          }
        }

        // Otherwise just bind to the form submit event for a regular client-
        // side form submission.
        $selector
          .once('visitorActions-' + name)
          .bind('submit.' + eventNamespace, {'eventNamespace' : eventNamespace}, function (event) {
            callback.call(null, event, pageContext);
          });
      }
      return $selector;
    }
  };

  var pageViewed = false;

  Drupal.visitorActions.page = {
    'view': function (name, action, context, callback) {
      if (!pageViewed) {
        // Create a dummy "event" object to pass as the first parameter (this
        // is usually a js event representing e.g. a click or a form submission).
        var event = {'type': 'PageView'};
        if (typeof callback === 'function') {
          callback.call(null, event, Drupal.visitorActions.getPageContext());
        }
        pageViewed = true;
      }
    },
    'stay': function (name, action, context, callback) {
      var time = 5; // Default time in seconds.
      if (action.options.hasOwnProperty('remains_for')) {
        time = action.options.remains_for;
      }
      var pageContext = Drupal.visitorActions.getPageContext();
      setTimeout(function (event) {
        if (typeof callback === 'function') {
          callback.call(null, event, pageContext);
        }
      }, time * 1000);
    },
    'scrollToBottom': function (name, action, context, callback) {
      var $windowProcessed = $('html').once('visitorActionsPageBindEvent-' + name);
      if ($windowProcessed.length > 0) {
        var bottomOffset = 100; // Default bottom offset in pixels
        var pageContext = Drupal.visitorActions.getPageContext();
        if (action.hasOwnProperty('options') && action.options.hasOwnProperty('bottom_offset')) {
          bottomOffset = action.options.bottom_offset;
        }
        $(window)
          .bind('scroll.visitorActions-' + name, function (event) {
            var $window = $(event.currentTarget);
            // Fire the event when the user scrolls to within a set number of
            // pixels from the bottom of the page.
            // Taken from the example here http://goo.gl/XfMaZZ
            if($window.scrollTop() + $window.height() > $(document).height() - bottomOffset) {
              $window.unbind('scroll.visitorActions-' + name);
              if (typeof callback === 'function') {
                callback.call(null, event, pageContext);
              }
            }
          });
      }
    },
    'bindEvent': function (name, action, context, callback) {
      if (typeof(this[action.event]) === 'function') {
        this[action.event].call(this, name, action, context, callback);
      }
      return $(window);
    },
    'reset': function() {
      pageViewed = false;
    }
  };

})(jQuery);
