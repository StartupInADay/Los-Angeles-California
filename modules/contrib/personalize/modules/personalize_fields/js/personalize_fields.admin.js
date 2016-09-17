(function ($) {

  Drupal.personalizeFields = Drupal.personalizeFields || {};
  /**
   * Handles setting the initial value for option labels as fields are added.
   *
   * This is to allow a default option label that has meaning and rather than
   * forcing the user to adjust this for each field added.
   */
  Drupal.behaviors.personalizeFieldsOptionLabel = {
    attach: function(context, settings) {
      Drupal.personalizeFields.changedOptionLabels = Drupal.personalizeFields.changedOptionLabels || [];

      var maxOptionLabelLength = 20;

      // Update the appropriate option label when a block is selected so long
      // as the user has not yet changed the label.
      $('.personalize-fields-add-option-label', context).each(function(e) {
        var $label = $(this);
        if ($(this).parents('.image-widget-data').length > 0) {
          // Set the option label to the uploaded filename.
          $(this).parents('.image-widget-data').find('input.form-file').change(function(e) {
            if (Drupal.personalizeFields.changedOptionLabels.indexOf($label.attr('id')) >= 0) {
              return;
            }
            // Only set the filename itself (not the whole path).
            $label.val($(this).val().replace(/^.*(\\|\/|\:)/, ''));
          });
        } else {
          // Set the option label to the field text.
          $(this).parent().prev().find('input.form-text').keyup(function(e) {
            if (Drupal.personalizeFields.changedOptionLabels.indexOf($label.attr('id')) >= 0) {
              return;
            }
            var newLabel = $(this).val();
            var truncateLength = Math.min(maxOptionLabelLength, newLabel.length);
            $label.val(newLabel.substring(0, truncateLength));
          });
        }
      });

      // Once the user updates the option label it should no longer be changed
      // automatically.
      $('.personalize-fields-add-option-label', context).change(function(e) {
        Drupal.personalizeFields.changedOptionLabels.push($(this).attr('id'));
      });
    }
  }

})(jQuery);
