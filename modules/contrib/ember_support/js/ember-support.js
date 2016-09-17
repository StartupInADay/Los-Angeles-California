(function ($) {
  var controls = '#panels-ipe-control-container';

  $(document).on('mousemove', function () {
    // Show the IPE menu when the mouse is actively in the window.
    $(controls).fadeIn();
  }).on('mouseleave', function () {
    // Hide the menu when the mouse leaves the window.
    $(controls).delay(200).fadeOut();
  });

  Drupal.behaviors.ember_support = {
    attach: function (context, settings) {
      // Add in slider support for image sizing in the library.
      // Find the exposed filters and add the slider inline with them.
      if ($(".media-browser-wrapper").length) {
        $(".media-browser-wrapper .views-exposed-form .views-exposed-widgets").once('media_preview_sizer').append("<div class='views-exposed-widget slide-widget'><label>Image Size</label><div class='slide-image'></div></div>");
        var valued = (!localStorage.getItem("slideWidth")) ? 200 : localStorage.getItem("slideWidth");
        // Set a default CSS size for the list items.
        $('#media-browser-library-list li').css('width', localStorage.getItem('slideWidth') + 'px');
        // Using a preset image style with a max width of 300, set the minimum and starting value.
        $('.slide-image').once('media_preview_sizer').slider({
          value: valued,
          min: 100,
          max: 300,
          step: 2,
          // When the slider moves, resize the image according to the px amount.
          slide: function (event, ui) {
            // Store value in localstorage.
            localStorage.setItem('slideWidth', ui.value);
            $('#media-browser-library-list li').css('width', localStorage.getItem('slideWidth') + 'px');
          }
        });
      }
    }
  };

})(jQuery);
