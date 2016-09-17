(function ($) {
    Drupal.behaviors.iib = {
      attach : function (context, settings) {
        // Toolbar click handlers
        $('.iib-trigger', context).click(function() {
          iibActive = Boolean(!$('.page-iib').hasClass('iib-active'));
          $('.iib-trigger').toggleClass('active', iibActive);
          $('.page-iib', context).toggleClass('iib-active', iibActive);
          return false;
        });
        // Force IIB to be active when displaying an error.
        if ($('.page-iib .error')[0]) {
          $('.iib-trigger').addClass('active');
          $('.page-iib', context).addClass('iib-active');
        }
        var spsActive = Boolean($('.page-iib #edit-cancel').length);
        $('.iib-trigger').toggleClass('sps-active', spsActive);
        $('.page-iib').toggleClass('sps-active', spsActive);
      }
    };
})(jQuery);
