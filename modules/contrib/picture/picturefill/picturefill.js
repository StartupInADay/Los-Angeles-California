/*jshint loopfunc: true, browser: true, curly: true, eqeqeq: true, expr: true, forin: true, latedef: true, newcap: true, noarg: true, trailing: true, undef: true, unused: true */
/*! Picturefill - Author: Scott Jehl, 2012 | License: MIT/GPLv2 */
(function(w, parent){

  // Enable strict mode.
  "use strict";

  w.picturefill = function(parent) {
    // Copy attributes from the source to the destination.
    function _copyAttributes(src, tar) {
      if (src.getAttribute('data-width') && src.getAttribute('data-height')) {
        tar.width = src.getAttribute('data-width');
        tar.height = src.getAttribute('data-height');
      }
      else {
        tar.removeAttribute('width');
        tar.removeAttribute('height');
      }
    }

    // Get all picture tags.
    if (!parent || !parent.getElementsByTagName) {
      parent = w.document;
    }
    var ps = parent.getElementsByTagName('span');

    // Loop the pictures.
    for (var i = 0, il = ps.length; i < il; i++ ) {
      if (ps[i].getAttribute('data-picture') !== null) {
        var sources = ps[i].getElementsByTagName('span');
        var picImg = null;
        var matches = [];

        // See which sources match.
        for (var j = 0, jl = sources.length; j < jl; j++ ) {
          var media = sources[j].getAttribute('data-media');

          // If there's no media specified or the media query matches, add it.
          if (!media || (w.matchMedia && w.matchMedia(media).matches)) {
            matches.push(sources[j]);
          }
        }

        if (matches.length) {
          // Grab the most appropriate (last) match.
          var match = matches.pop();

          // Find any existing img element in the picture element.
          picImg = ps[i].getElementsByTagName('img')[0];

          // Add a new img element if one doesn't exists.
          if (!picImg) {
            picImg = w.document.createElement('img');
            picImg.alt = ps[i].getAttribute('data-alt') || '';
            picImg.title = ps[i].getAttribute('data-title') || '';
            ps[i].appendChild(picImg);
          }

          // Set the source if it's different.
          if (picImg.getAttribute('src') !== match.getAttribute('data-src')) {
            picImg.src = match.getAttribute('data-src');
            _copyAttributes(match, picImg);
          }
        }
      }
    }
  };

  // Run on resize and domready (w.load as a fallback)
  if (w.addEventListener) {
    w.addEventListener('resize', w.picturefill, false);
    w.addEventListener('DOMContentLoaded', function() {
      w.picturefill();
      // Run once only.
      w.removeEventListener('load', w.picturefill, false);
    }, false);
    w.addEventListener('load', w.picturefill, false);
  }
  else if (w.attachEvent) {
    w.attachEvent('onload', w.picturefill);
  }
})(this);