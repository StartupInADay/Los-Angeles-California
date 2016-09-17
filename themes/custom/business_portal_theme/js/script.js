/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {


// To understand behaviors, see https://drupal.org/node/756722#behaviors

Drupal.behaviors.layout = {
  attach: function(context, settings) {
    $(".panel-2col-stacked .panel-col-top").wrapInner( "<div class='outer'></div>");
  }
};

Drupal.behaviors.accordion = {
  attach: function(context, settings) {

    var els = $(".field-starter-kit-key-points");

    // A single DIV container needs to be after each field-title header.
    els.each(function(){
        $(this).find(".field-content-components").wrapAll("<div class='js-component-wrapper' />");
    });

    els.accordion({
     collapsible: true,
     active: false,
     header: ".field-title",
     animated: false});
  }
}

Drupal.behaviors.rotator = {
  attach: function(context, settings) {

    var rotatingWrapper = $(".front .panels-flexible-2 .field-image ");
    var rotatingItems = rotatingWrapper.find("img");

    rotatingItems.hide();
    rotatingWrapper.attr("id", "rotating-items-wrapper");
    rotatingItems.addClass("rotating-item");
    //count number of items
    var numberOfItems = rotatingItems.length;

    if (!numberOfItems) return;
    console.log(numberOfItems);
    var InfiniteRotator =
        {
          init: function()
          {
            //initial fade-in time (in milliseconds)
            var initialFadeIn = 1000;

            //interval between items (in milliseconds)
            var itemInterval = 5000;

            //cross-fade time (in milliseconds)
            var fadeTime = 2500;



            //set current item
            var currentItem = 0;

            //show first item
            rotatingItems.first().eq(currentItem).fadeIn(initialFadeIn);

            //loop through the items
            var infiniteLoop = setInterval(function(){
              rotatingItems.eq(currentItem).fadeOut(fadeTime);

              if(currentItem == numberOfItems -1){
                currentItem = 0;
              }else{
                currentItem++;
              }
              rotatingItems.eq(currentItem).fadeIn(fadeTime);

            }, itemInterval);
          }
        };

    InfiniteRotator.init();
  }
};

  Drupal.behaviors.toggleFacets = {
    attach: function(context, settings) {
      var width = $(window).width();
      var buttonWrapper = $("form#views-exposed-form-sbp-resource-incentives-panel-pane-1");
      var toggleButton = $("<a>Search or filter documents</a>");

      if(width < 960) {
        // Create the toggle button.
        $(".pane-facetapi").hide();
        $("input#edit-search-api-views-fulltext").hide();
        // Use jquery.once to prevent multiple button creation.
        buttonWrapper.once('createtoggle').prepend(toggleButton);

        //Add class to button.
        toggleButton.addClass("facetapi__toggle");

        //Toggle function.
        $(toggleButton).click(function() {
          $('.pane-facetapi').toggle("slow");
          $("input#edit-search-api-views-fulltext").toggle("slow");
          return false;
        });

      }
    }
  };

  /**
   * LA-369 Mobile Menu plugin
   * https://github.com/meanthemes/meanMenu
   * @type {{attach: Drupal.behaviors.mobileMenu.attach}}
   */
  Drupal.behaviors.mobileMenu = {
    attach: function(context, settings) {
      $('#meanMenu--wrapper-block').once('revealMenu').meanmenu({
        meanScreenWidth: "960",
        meanMenuContainer: '#mean-menu--mobile',
        meanMenuCloseSize: "22px"
      });
    }
  };

  Drupal.behaviors.changeHeaderBg = {
    attach: function(context, settings) {
      $("a.meanmenu-reveal").once('addHeaderClass').click(function () {
        $("#header").toggleClass("dark-blue-bg");
      });
     }
    };


  /**
   * LA-335: Adding a dynamic column layout for the featured resources widget.
   * Uses the Isotope.js library
   */
  Drupal.behaviors.dynamicLayout = {
    attach: function(context, settings) {

      $('.dynamic-layout--wrapper').isotope({
        // options
        itemSelector: '.dynamic-layout--item',
        layoutMode: 'fitRows'
      });
    }

  };

  /**
   * LA-335: Code to make all blocks in the same row default to the height of the longest block.
   * Assumes that each row contains 2 blocks.
   * @type {{attach: Drupal.behaviors.equalHeightColumns.attach}}
     */

  Drupal.behaviors.equalHeightColumns = {
    attach: function(context, settings) {
      var windowWidth = $(window).width();
      // Select the node block.
      var dynamicBlock = $('.dynamic-layout--wrapper').find('.dynamic-layout--item');

      //Set this behaviour to only apply in desktop widths. Smaller widths default to a stacked layout.
      if(windowWidth > 959) {
        $.each(dynamicBlock, function (i) {
          // Get the height of each node block
          var blockHeight = $(dynamicBlock[i]).outerHeight();

          if (i == 0 || i % 2 == 0) {
            // For the first block and every other block with an even index
            function compareHeights() {
              // Compare heights and assign the largest height to both blocks in the row.
              var nextBlockHeight = $(dynamicBlock[i + 1]).outerHeight();
              if (blockHeight < nextBlockHeight) {
                $(dynamicBlock[i]).height(nextBlockHeight);

              } else {
                $(dynamicBlock[i + 1]).height(blockHeight);
              }
            }
            return compareHeights();
          }
        });
      }

      // Applying the function on window resize
      $(window).resize(function() {
        if(windowWidth > 959) {
          $.each(dynamicBlock, function (i) {
            // Get the height of each node block
            var blockHeight = $(dynamicBlock[i]).outerHeight();

            if (i == 0 || i % 2 == 0) {
              // For the first block and every other block with an even index
              function compareHeights() {
                // Compare heights and assign the largest height to both blocks in the row.
                var nextBlockHeight = $(dynamicBlock[i + 1]).outerHeight();
                if (blockHeight < nextBlockHeight) {
                  $(dynamicBlock[i]).height(nextBlockHeight);

                } else {
                  $(dynamicBlock[i + 1]).height(blockHeight);
                }
              }
              return compareHeights();
            }
          });
        }
      });

    }
  };

  /**
   * LA-374 building the timeline using slick carousel
   * http://kenwheeler.github.io/slick/
   */
  Drupal.behaviors.timelineCarousel = {
    attach: function(context, settings) {
      $('.slick-carousel').not('.slick-initialized').slick({
        infinite: false,
        slidesToShow: 10,
        slidesToScroll: 2,
        appendArrows: '.timeline__container',
        focusOnSelect: true,
        easing: 'easeOutCubic',
        swipeToSlide: true,
        prevArrow: '<button class = "timeline__arrow prev"></button>',
        nextArrow: '<button class = "timeline__arrow next"></button>',
        responsive: [
          {
            breakpoint: 2401,
            settings: {
              slidesToShow: 7,
              slidesToScroll: 3
            }
          },
          {
            breakpoint: 1801,
            settings: {
              slidesToShow: 5,
              slidesToScroll: 3
            }
          },
          {
            breakpoint: 961,
            settings: {
              slidesToShow: 4,
              slidesToScroll: 3
            }
          },
          {
            breakpoint: 721,
            settings: {
              slidesToShow: 3,
              slidesToScroll: 2
            }
          },
          {
            breakpoint: 481,
            settings: {
              slidesToShow: 2,
              slidesToScroll: 1
            }
          },
          {
            breakpoint: 361,
            settings: {
              slidesToShow: 1,
              slidesToScroll: 1
            }
          }
        ]
      });
    }
  };
  
  
  /**
   * LA-245: Code to make left and right blocks in the same height.
   */
  $.fn.equalizeHeights = function(){
    return this.height( Math.max.apply(this, $(this).map(function(i,e){return $(e).height();}).get() ) );
  };
  
//  $(document).ready(function() {
//    // Equalize columns on page load
//    var $panels = $('.center-wrapper .panel-panel');
//    $(window).load(function() {
//      if ($(window).width() > 959) {
//        $panels.equalizeHeights();
//      }
//    });
//    
//    // Equalize columns on page resize
//    $(window).resize(function() {
//      if ($(window).width() > 959) {
//        $panels.equalizeHeights();
//      } else {
//        $panels.css({'height': 'auto'});
//      }
//    });
//  });
  
  /**
   * Code to make search and translation icon clickable to show/hide search form
   * or google translation widget.
   */
  $(document).ready(function() {
    $('#block-sbp-search-sbp-search-icon').click(function(e) {
      e.preventDefault();
      if($(this).hasClass('active')) {
        $(this).removeClass('active');
        $('#header, .meanmenu-reveal').removeClass('dark-blue-background');
        $('#block-sbp-search-sbp-search-form').stop().hide();
      } else {
        $(this).addClass('active');
        $('.meanclose').trigger('click');
        $('#header, .meanmenu-reveal').addClass('dark-blue-background');
        $('#block-sbp-translation-sbp-translation-icon').removeClass('active');
        $('#block-sbp-search-sbp-search-form').stop().show();
        $('#block-sbp-translation-sbp-translation-form').stop().hide();
        $(this).addClass('active');
      }
    });
    $('#block-sbp-translation-sbp-translation-icon').click(function(e) {
      e.preventDefault();
      if($(this).hasClass('active')) {
        $(this).removeClass('active');
        $('#header, .meanmenu-reveal').removeClass('dark-blue-background');
        $('#block-sbp-translation-sbp-translation-form').stop().hide();
      } else {
        $('.meanclose').trigger('click');
        $('#header, .meanmenu-reveal').addClass('dark-blue-background');
        $('#block-sbp-search-sbp-search-icon').removeClass('active');
        $('#block-sbp-translation-sbp-translation-form').stop().show();
        $('#block-sbp-search-sbp-search-form').stop().hide();
        $(this).addClass('active');
      }
    });
    
    $('.sbp-search-intro-text').click(function(e) {
      e.preventDefault();
      $(this).hide();
      $('#edit-search').focus();
    });
    $('#edit-search').blur(function() {
      if($(this).val()==='') {
        $('.sbp-search-intro-text').show();
      }
    });
    
    $('.meanmenu-reveal').click(function(e) {
        $('#block-sbp-translation-sbp-translation-icon.active').trigger('click');
        $('#block-sbp-search-sbp-search-icon.active').trigger('click');
    });
    
    
    $('.tool-tip').each(function() {
      var $parent = $('.tool-tip').parents('li');
      $parent.css({'background-image': 'none'});
    })
    
  });

})(jQuery, Drupal, this, this.document);
