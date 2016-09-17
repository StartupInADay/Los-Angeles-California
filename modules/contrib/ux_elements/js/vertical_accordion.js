
(function ($) {

/**
 * Call jQuery Accordion on vertical_accordion elements
 */
Drupal.behaviors.verticalAccordion = {
  attach: function (context) {
    $('.vertical-accordion', context).once('vertical-accordion', function(){
      $(this).accordion({
        header: '.pane-header',
        clearStyle: true,
        autoHeight: false
      });
    });
  }
};

})(jQuery);
