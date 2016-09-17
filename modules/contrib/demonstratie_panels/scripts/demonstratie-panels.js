(function ($) {
  function featureHeight(page, imageMultiplier, contentPosition) {
    return Math.round(page.width() * imageMultiplier) - contentPosition.top;
  }

  function resetHero(page, image, imageParent) {
    page.data('background', '').css({
      'background-image': '',
      'background-repeat': ''
    }).removeClass('has-hero-image');
    if (imageParent.hasClass('demonstratie-hero-hero')) {
      image.show();
      imageParent.css('min-height', '');
    }
  }

  function processHero(page, contentPosition, image, imageParent) {
    var imageSrc = image.attr('src'),
      imageWidth = image.width(),
      imageHeight = image.height(),
      imageMultiplier = imageHeight / imageWidth;

    // If the size is appropriate, load the image and process.
    if ($(window).width() >= 768 && imageHeight > 0 && imageSrc != page.data('background')) {
      image.load(imageSrc, function () {
        imageWidth = image.width(),
        imageHeight = image.height(),
        imageMultiplier = imageHeight / imageWidth;

        page.data('background', imageSrc).css({
          'background-image': 'url("' + imageSrc + '")',
          'background-repeat': 'no-repeat'
        }).addClass('has-hero-image');
        image.hide();
        if (imageParent.hasClass('demonstratie-hero-hero')) {
          imageParent.css('min-height', featureHeight(page, imageMultiplier, contentPosition));
        }
      });
    }
    // If the image is loaded, only size the parent properly.
    else if ($(window).width() >= 768 && imageParent.hasClass('demonstratie-hero-hero')) {
      imageParent.css('min-height', featureHeight(page, imageMultiplier, contentPosition));
    }
    // If the window is too small, make sure all the changes are stripped.
    else if ($(window).width() < 768) {
      resetHero(page, image, imageParent);
    }
  }

  /**
   * Overrides Drupal.behaviors.picture
   *
   * This is a duplicate of the behavior in the picture module. It is being
   * overriden here so that Drupal.behaviors.heroImage can work off of the data provided.
   */
  Drupal.behaviors.picture = {
    attach: function (context) {
      window.picturefill(context);
    }
  };

  /**
   * Place hero images as a background on the .page element.
   */
  Drupal.behaviors.heroImage = {
    attach: function (context, settings) {
      var page = $('.page', context),
        image = page.find('.hero-image img').first(),
        imageParent = image.parents('.hero-image, .node').addClass('hero-image-parent'),
        contentPosition = $('.site-header').siblings('.content').offset();

      if (image.length > 0) {
        image.get(0).onload = processHero.bind(this, page, contentPosition, image, imageParent);

        $(window).resize(function () {
          contentPosition = $('.site-header').siblings('.content').offset();
          processHero(page, contentPosition, image, imageParent);
        });

        $('.panels-ipe-button-container a.panels-ipe-startedit').click(function () {
          resetHero(page, image, imageParent);
        });

        // Make the background replacement work better with IPE panels.
        $('.panels-ipe-form-container').on('DOMNodeInserted', function () {
          $(this).find('input').click(function () {
            setTimeout(function () {
              image = page.find('.hero-image img').first(),
              imageParent = image.parents('.hero-image, .node').addClass('hero-image-parent'),
              contentPosition = $('.site-header').siblings('.content').offset();

              processHero(page, contentPosition, image, imageParent);
            }, 2000);
          });
        });
      }

    }
  };
})(jQuery);
