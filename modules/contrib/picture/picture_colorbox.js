/*
 * Lazy load picture element.
 */
 
(function(win) {
  'use strict';

  function _lazyload_pictures() {
    var pics    = win.document.getElementsByTagName('span'),
        picl    = pics.length,
        pic     = null;

    while (picl-- && (pic = pics[picl])) {
      if (pic.getAttribute('data-picture-lazy') === 'lazy') {
        pic.setAttribute('data-picture', '');
        pic.removeAttribute('data-picture-lazy');
      }
    }
  }

  if (win.addEventListener) {
      win.addEventListener('load', _lazyload_pictures);
  } else {
      win.attachEvent('onload', _lazyload_pictures);
  }  
})(window);
