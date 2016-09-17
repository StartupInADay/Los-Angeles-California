(function ($, Drupal) {

    Drupal.behaviors.exampleModule = {
        attach: function (context, settings) {
            $('#collection-usage-tabs', context).tabs();
        }
    };

})(jQuery, Drupal);
