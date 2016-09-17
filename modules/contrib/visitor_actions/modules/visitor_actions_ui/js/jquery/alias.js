/**
 * Some sites use multiple versions of the jQuery library.  This can mean
 * that some plugins can be loaded into one instance or another.  This
 * file allows us to make a reference to the original library.
 */
(function (Drupal, $) {
  // Make a reference to the original jQuery in case it gets overwritten.
  Drupal.jQuery = $.noConflict();

})(Drupal, jQuery);
