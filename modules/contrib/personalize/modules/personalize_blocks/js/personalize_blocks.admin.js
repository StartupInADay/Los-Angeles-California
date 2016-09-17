(function ($) {

  Drupal.personalizeBlocks = Drupal.personalizeBlocks || {};

  Drupal.personalizeBlocks.setOptionLabelFromExistingBlock = function (event) {
    var $optionLabel = $(event.currentTarget).parents('tr').find('.personalize-blocks-add-option-label');
    // Text is displayed in the format "Friendly (module:machine)".
    var blockRe = /\s\(([a-zA-Z]|\d)+:([a-zA-Z]|\d)+\)$/;
    var newLabel = $(event.currentTarget).find('option:selected').text().replace(blockRe, '');
    Drupal.personalizeBlocks.setOptionLabel($optionLabel, newLabel);
  };

  Drupal.personalizeBlocks.setOptionLabelFromNewBlock = function (event) {
    var $optionLabel = $(event.currentTarget).parents('tr').find('.personalize-blocks-add-option-label');
    Drupal.personalizeBlocks.setOptionLabel($optionLabel, $(event.currentTarget).val());
  };

  Drupal.personalizeBlocks.setOptionLabel = function ($optionLabel, newLabel) {
    var maxOptionLabelLength = 20;
    if (Drupal.personalizeBlocks.changedOptionLabels.indexOf($optionLabel.attr('id')) >= 0) {
      // The user has already changed the label.
      return;
    }
    // Limit the text used as the label.
    var truncateLength = Math.min(maxOptionLabelLength, newLabel.length);
    $optionLabel.val(newLabel.substring(0, truncateLength));
  }

  /**
   * Handles setting the initial value for option labels as blocks are selected.
   *
   * This is to allow a default option label that has meaning and rather than
   * forcing the user to adjust this for each block selected.
   */
  Drupal.behaviors.personalizeBlockOptionLabel = {
    attach: function(context, settings) {
      Drupal.personalizeBlocks.changedOptionLabels = Drupal.personalizeBlocks.changedOptionLabels || [];

      // Update the appropriate option label when a block is selected so long
      // as the user has not yet changed the label.
      $('.personalize-blocks-add-block-select', context).on('change', function(e) {
        if ($(this).val() === 'add') {
          // Take the option label from the new block info field.
          var $optionLabel = $(event.currentTarget).parents('tr').find('.personalize-blocks-add-option-label');
          $addInfo = $(this).closest('td').find('.personalize-blocks-add-block-info');
          // Set the the default to the current text in the add block field.
          Drupal.personalizeBlocks.setOptionLabel($optionLabel, $addInfo.val());
          // Set a listener for any changes to that text.
          $addInfo.on('keyup', Drupal.personalizeBlocks.setOptionLabelFromNewBlock);
        } else {
          // Take the option label from the selected block and stop listening
          // to changes in the add block info field.
          $(this).closest('td').find('.personalize-blocks-add-block-info').off('keyup', Drupal.personalizeBlocks.setOptionLabelFromNewBlock);
          Drupal.personalizeBlocks.setOptionLabelFromExistingBlock(e);
        }
      });

      // Once the user updates the option label it should no longer be changed
      // automatically.
      $('.personalize-blocks-add-option-label', context).once().change(function(e) {
        Drupal.personalizeBlocks.changedOptionLabels.push($(this).attr('id'));
      });
    }
  };

})(jQuery);
