(function ($) {

  // Triggered toggle select modifies the DOM elements in the Panels Preview form.
  function previewToggleSelect(value) {
    switch(value) {
      case 'split':
        $('.modal-content .panels-preview-ctools-form div').children('fieldset').show();
        $('.modal-content .widget-preview-single').show().removeClass('panels-preview-full');
      break;
      case 'edit':
        $('.modal-content .panels-preview-ctools-form div').children('fieldset').show();
        $('.modal-content .widget-preview-single').hide();
      break;
      case 'full':
        $('.modal-content .panels-preview-ctools-form div').children('fieldset').hide();
        $('.modal-content .widget-preview-single').show().addClass('panels-preview-full');
      break;
    }
  }

  // Behavior for Preview Toggle during Drupal page bootstrap.
  Drupal.behaviors.panelsPreviewToggle = {
    attach: function (context, settings) {
      // Event listener fires when the user changes the Preview toggle.
      $('.modal-content select.widget-preview-select-type').on('change', function() {
        // Trigger the DOM shifting in the form.
        previewToggleSelect(this.value);
        // Set most recently used preview type for later usage.
        localStorage.setItem('Drupal.panelsPreview.selectedType', this.value);
      });
      // Check for recent usage and set select preview type.
      var type = localStorage.getItem('Drupal.panelsPreview.selectedType');
      // Trigger a shift if a recent preview type is available.
      if (type != null) {
        // Update the selector and trigger the DOM shifting.
        $('.modal-content select.widget-preview-select-type').val(type);
        previewToggleSelect(type);
      }
    }
  }

})(jQuery);
