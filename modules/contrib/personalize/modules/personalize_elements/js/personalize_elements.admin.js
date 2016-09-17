(function ($) {

  Drupal.personalizeElements = Drupal.personalizeElements || {};

  /**
   * Handles setting the initial value for option labels as element content is entered.
   *
   * This is to allow a default option label that has meaning and rather than
   * forcing the user to adjust this for each content variation.
   */
  Drupal.behaviors.personalizeElementsOptionLabel = {
    attach: function(context, settings) {
      var maxOptionLabelLength = 20;

      Drupal.personalizeElements.changedOptionLabels = Drupal.personalizeElements.changedOptionLabels || [];

      // Update the appropriate option label when an element is selected so long
      // as the user has not yet changed the label.
      $('.personalize-elements-add-content:not(.personalize-elements-add-processed)', context).keyup(function(e) {
        if ($(':input[name="variation_type"]').val() === 'runJS') {
          // We don't do anything for run JavaScript operations as the content
          // is not suitable for labels.
          // @todo generalize this using a settings value.
          return;
        }
        var $variationText = $(this).parents('.personalize-elements-option-content-element').prev('.personalize-elements-option-label-element').find('.personalize-elements-add-option-label');
        var $variationLabel = $(this).parents('.personalize-elements-option-content-element').prev('.personalize-elements-option-label-element').find('label');
        if (Drupal.personalizeElements.changedOptionLabels.indexOf($variationText.attr('id')) >= 0) {
          // The user has already changed the label.
          return;
        }
        if (e.keyCode === 9) {
          // This is a tab key that could have been used just to navigate in and
          // out of the field.  If there were other content-related changes they
          // would already be caught anyway, so ignore this.
          return;
        }
        // Limit the text used as the label.
        var newLabel = $(this).val().replace(/<(?:.|\n)*?>/gm, '');
        var truncateLength = Math.min(maxOptionLabelLength, newLabel.length);
        var truncated = newLabel.substring(0, truncateLength);
        $variationText.val(truncated);
        $variationLabel.text(truncated);
      });

      // Once the user updates the option label it should no longer be changed
      // automatically.
      // Note that the context may be the actual form if "Add another" is used.
      var labelSelector = '#personalize-elements-form .personalize-elements-add-option-label';
      if (context !== document && $(context).attr('id') === 'personalize-elements-form') {
        labelSelector = '.personalize-elements-add-option-label';
      }
      $(labelSelector, context).change(function(e) {
        Drupal.personalizeElements.changedOptionLabels.push($(this).attr('id'));
      });
      $(this).addClass('personalize-elements-add-processed');
    }
  }

  /**
   * Handles the renaming of content variations.
   */
  Drupal.behaviors.personalizeElementsRenameVariation = {
    attach: function (context, settings) {
      $('.personalize-elements-option-label-element:not(.personalize-elements-rename-processed)', context).each(function() {
        var $text = $('input[type="text"]', this);
        var $label = $('label', this);
        var current = this;
        $(this).addClass('personalize-elements-rename-processed');

        // Set the label to be the same as the text input field.
        $label.text($text.val());
        $text.hide();

        // Don't add rename functionality to the control option.
        if ($(this).parent().hasClass('personalize-elements-control')) {
          return;
        }

        // Add a link to rename the content variation.
        $label.after(' <a href="#" class="personalize-elements-rename">' + Drupal.t('Rename') + '</a>');
        $('.personalize-elements-rename', this).on('click', function(e) {
          $label.hide();
          // Reset the input text to receive any automated changes from new
          // content variations.
          $text.val($label.text());
          $text.show();
          $text.focus();
          $(this).hide();
          return false;
        });
        $text.on('blur', function(e) {
          $label.text($text.val());
          $label.show();
          $text.hide();
          $('.personalize-elements-rename', current).show();
        });
      });
    }
  }

})(jQuery);
