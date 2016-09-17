(function ($) {

// Auto-submit main search input after autocomplete
if (typeof Drupal.jsAC != 'undefined') {

  var getSetting = function (input, setting, defaultValue) {
    // Earlier versions of jQuery, like the default for Drupal 7, don't properly
    // convert data-* attributes to camel case, so we access it via the verbatim
    // name from the attribute (which also works in newer versions).
    var search = $(input).data('search-api-autocomplete-search');
    if (typeof search == 'undefined'
        || typeof Drupal.settings.search_api_autocomplete == 'undefined'
        || typeof Drupal.settings.search_api_autocomplete[search] == 'undefined'
        || typeof Drupal.settings.search_api_autocomplete[search][setting] == 'undefined') {
      return defaultValue;
    }
    return Drupal.settings.search_api_autocomplete[search][setting];
  };

  var oldJsAC = Drupal.jsAC;
  /**
   * An AutoComplete object.
   *
   * Overridden to set the proper "role" attribute on the input element.
   */
  Drupal.jsAC = function ($input, db) {
    if ($input.data('search-api-autocomplete-search')) {
      $input.attr('role', 'combobox');
      $input.parent().attr('role', 'search');
    }
    oldJsAC.call(this, $input, db);
  };
  Drupal.jsAC.prototype = oldJsAC.prototype;

  /**
   * Handler for the "keyup" event.
   *
   * Extend from Drupal's autocomplete.js to automatically submit the form
   * when Enter is hit.
   */
  var default_onkeyup = Drupal.jsAC.prototype.onkeyup;
  Drupal.jsAC.prototype.onkeyup = function (input, e) {
    if (!e) {
      e = window.event;
    }
    // Fire standard function.
    default_onkeyup.call(this, input, e);

    if (13 == e.keyCode && $(input).hasClass('auto_submit')) {
      var selector = getSetting(input, 'selector', ':submit');
      $(selector, input.form).trigger('click');
    }
  };

  /**
   * Handler for the "keydown" event.
   *
   * Extend from Drupal's autocomplete.js to avoid ajax interfering with the
   * autocomplete.
   */
  var default_onkeydown = Drupal.jsAC.prototype.onkeydown;
  Drupal.jsAC.prototype.onkeydown = function (input, e) {
    if (!e) {
      e = window.event;
    }
    // Fire standard function.
    default_onkeydown.call(this, input, e);

    // Prevent that the ajax handling of Views fires too early and thus
    // misses the form update.
    if (13 == e.keyCode && $(input).hasClass('auto_submit')) {
      e.preventDefault();
      return false;
    }
  };

  Drupal.jsAC.prototype.select = function(node) {
    var autocompleteValue = $(node).data('autocompleteValue');
    // Check whether this is not a suggestion but a "link".
    if (autocompleteValue.charAt(0) == ' ') {
      window.location.href = autocompleteValue.substr(1);
      return false;
    }
    this.input.value = autocompleteValue;
    $(this.input).trigger('autocompleteSelect', [node]);
    if ($(this.input).hasClass('auto_submit')) {
      if (typeof Drupal.search_api_ajax != 'undefined') {
        // Use Search API Ajax to submit
        Drupal.search_api_ajax.navigateQuery($(this.input).val());
      }
      else {
        var selector = getSetting(this.input, 'selector', ':submit');
        $(selector, this.input.form).trigger('click');
      }
      return true;
    }
  };

  /**
   * Overwrite default behaviour.
   *
   * Just always return true to make it possible to submit even when there was
   * an autocomplete suggestion list open.
   */
  Drupal.autocompleteSubmit = function () {
    $('#autocomplete').each(function () {
      this.owner.hidePopup();
    });
    return true;
  };
}

/**
* Performs a cached and delayed search.
*/
Drupal.ACDB.prototype.search = function (searchString) {
  this.searchString = searchString;

  // Check allowed length of string for autocomplete.
  var data = $(this.owner.input).first().data('min-autocomplete-length');
  if (data && searchString.length < data) {
    return;
  }

  // See if this string needs to be searched for anyway.
  if (searchString.match(/^\s*$/)) {
    return;
  }

  // Prepare search string.
  searchString = searchString.replace(/^\s+/, '');
  searchString = searchString.replace(/\s+/g, ' ');

  // See if this key has been searched for before.
  if (this.cache[searchString]) {
    return this.owner.found(this.cache[searchString]);
  }

  var db = this;
  this.searchString = searchString;

  // Initiate delayed search.
  if (this.timer) {
    clearTimeout(this.timer);
  }
  var sendAjaxRequest = function () {
    db.owner.setStatus('begin');

    var url;

    // Allow custom Search API Autocomplete overrides for specific searches.
    if (getSetting(db.owner.input, 'custom_path', false)) {
      var queryChar = db.uri.indexOf('?') >= 0 ? '&' : '?';
      url = db.uri + queryChar + 'search=' + encodeURIComponent(searchString);
    }
    else {
      // We use Drupal.encodePath instead of encodeURIComponent to allow
      // autocomplete search terms to contain slashes.
      url = db.uri + '/' + Drupal.encodePath(searchString);
    }

    // Ajax GET request for autocompletion.
    $.ajax({
      type: 'GET',
      url: url,
      dataType: 'json',
      success: function (matches) {
        if (typeof matches.status == 'undefined' || matches.status != 0) {
          db.cache[searchString] = matches;
          // Verify if these are still the matches the user wants to see.
          if (db.searchString == searchString) {
            db.owner.found(matches);
          }
          db.owner.setStatus('found');
        }
      },
      error: function (xmlhttp) {
        if (xmlhttp.status) {
          alert(Drupal.ajaxError(xmlhttp, db.uri));
        }
      }
    });
  };
  // Make it possible to override the delay via a setting.
  var delay = getSetting(this.owner.input, 'delay', this.delay);
  if (delay > 0) {
    this.timer = setTimeout(sendAjaxRequest, delay);
  }
  else {
    sendAjaxRequest.apply();
  }
};

})(jQuery);
