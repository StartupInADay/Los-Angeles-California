/*!
 * jQuery.fn.proxy monkeypatch from 1.6.0
 * http://jquery.com
 *
 * Copyright 2013 jQuery Foundation and other contributors
 * Released under the MIT license.
 * http://jquery.org/license
 *
 * http://api.jqueryui.com/proxy/
 *
 * NOTE: Modified to live at $.fn.proxy_visitor_actions_ui rather than $.fn.proxy!
 */

// Bind a function to a context, optionally partially applying any
// arguments.
Drupal.jQuery.proxy_visitor_actions_ui = function( fn, context ) {
  if ( typeof context === "string" ) {
    var tmp = fn[ context ];
    context = fn;
    fn = tmp;
  }

  // Quick check to determine if target is callable, in the spec
  // this throws a TypeError, but we will just return undefined.
  if ( !jQuery.isFunction( fn ) ) {
    return undefined;
  }

  // Simulated bind
  var slice = Array.prototype.slice,
    args = slice.call( arguments, 2 ),
    proxy = function() {
      return fn.apply( context, args.concat( slice.call( arguments ) ) );
    };

  // Set the guid of unique handler to the same of original handler, so it can be removed
  proxy.guid = fn.guid = fn.guid || proxy.guid || jQuery.guid++;

  return proxy;
};
