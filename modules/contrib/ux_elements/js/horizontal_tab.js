
(function ($) {

/**
 * Copy of the functionality of vertical tabs in Drupal 7 core
 */
Drupal.behaviors.horizontalTabs = {
  attach: function (context) {
    $('.horizontal-tabs-panes', context).once('horizontal-tabs', function () {
      var focusID = $(':hidden.horizontal-tabs-active-tab', this).val();
      var tab_focus;

      // Check if there are some fieldsets that can be converted to horizontal-tabs
      var $fieldsets = $('> fieldset', this);
      if ($fieldsets.length == 0) {
        return;
      }

      // Create the tab column.
      var tab_list = $('<ul class="horizontal-tabs-list"></ul>');
      $(this).wrap('<div class="horizontal-tabs clearfix"></div>').before(tab_list);

      // Transform each fieldset into a tab.
      $fieldsets.each(function () {
        var horizontal_tab = new Drupal.horizontalTab({
          title: $('> legend', this).text(),
          fieldset: $(this)
        });
        tab_list.append(horizontal_tab.item);
        $(this)
          .removeClass('collapsible collapsed')
          .addClass('horizontal-tabs-pane')
          .data('horizontalTab', horizontal_tab);
        if (this.id == focusID) {
          tab_focus = $(this);
        }
      });

      $('> li:first', tab_list).addClass('first');
      $('> li:last', tab_list).addClass('last');

      if (!tab_focus) {
        // If the current URL has a fragment and one of the tabs contains an
        // element that matches the URL fragment, activate that tab.
        if (window.location.hash && $(window.location.hash, this).length) {
          tab_focus = $(window.location.hash, this).closest('.horizontal-tabs-\n\
\n\
pane');
        }
        else {
          tab_focus = $('> .horizontal-tabs-pane:first', this);
        }
      }
      if (tab_focus.length) {
        tab_focus.data('horizontalTab').focus();
      }
      
      // Set a min width so the tabs won't float below
      var minWidth = 0;
      $('> li', tab_list).each(function () {
        var outer = $(this).outerWidth(true);
        minWidth = minWidth + outer;
      });
      $(this).parent().css('min-width', minWidth);

    });
  }
};

/**
 * The horizontal tab object represents a single tab within a tab group.
 *
 * @param settings
 *   An object with the following keys:
 *   - title: The name of the tab.
 *   - fieldset: The jQuery object of the fieldset that is the tab pane.
 */
Drupal.horizontalTab = function (settings) {
  var self = this;
  $.extend(this, settings, Drupal.theme('horizontalTab', settings));

  this.link.click(function () {
    self.focus();
    return false;
  });

  // Keyboard events added:
  // Pressing the Enter key will open the tab pane.
  this.link.keydown(function(event) {
    if (event.keyCode == 13) {
      self.focus();
      // Set focus on the first input field of the visible fieldset/tab pane.
      $("fieldset.horizontal-tabs-pane :input:visible:enabled:first").focus();
      return false;
    }
  });

  // Pressing the Enter key lets you leave the tab again.
  this.fieldset.keydown(function(event) {
    // Enter key should not trigger inside <textarea> to allow for multi-line entries.
    if (event.keyCode == 13 && event.target.nodeName != "TEXTAREA") {
      // Set focus on the selected tab button again.
      $(".horizontal-tab-button.selected a").focus();
      return false;
    }
  });
};

Drupal.horizontalTab.prototype = {
  /**
   * Displays the tab's content pane.
   */
  focus: function () {
    this.fieldset
      .siblings('fieldset.horizontal-tabs-pane')
        .each(function () {
          var tab = $(this).data('horizontalTab');
          tab.fieldset.hide();
          tab.item.removeClass('selected');
        })
      .end()
      .show()
      .siblings(':hidden.horizontal-tabs-active-tab')
      .val(this.fieldset.attr('id'));

    this.item.addClass('selected');
    // Mark the active tab for screen readers.
    $('#active-horizontal-tab').remove();
    this.link.append('<span id="active-horizontal-tab" class="element-invisible">' + Drupal.t('(active tab)') + '</span>');
    $(this.fieldset).trigger('horizontalTabActivate', this);
  }
};

/**
 * Theme function for a horizontal tab.
 *
 * @param settings
 *   An object with the following keys:
 *   - title: The name of the tab.
 * @return
 *   This function has to return an object with at least these keys:
 *   - item: The root tab jQuery element
 *   - link: The anchor tag that acts as the clickable area of the tab
 *       (jQuery version)
 */
Drupal.theme.prototype.horizontalTab = function (settings) {
  var tab = {};
  tab.item = $('<li class="horizontal-tab-button" tabindex="-1"></li>')
    .append(tab.link = $('<a href="#"></a>')
    .append(tab.title = $('<span></span>').text(settings.title))
  );
  return tab;
};

})(jQuery);
