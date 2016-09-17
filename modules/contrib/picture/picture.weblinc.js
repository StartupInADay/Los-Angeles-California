if (typeof Drupal !== 'undefined' && typeof jQuery !== 'undefined') {
  // only load if Drupal and jQuery are defined.
  (function ($) {
    Drupal.behaviors.picture = {
      attach: function (context) {
        Picture.parse();
        Picture.watch();
      }
    };
  })(jQuery);
}