
(function($) {

/**
 * @file form_builder.js
 * Provide enhancements to the form building user interface.
 */

Drupal.behaviors.formBuilderElement = {};
Drupal.behaviors.formBuilderElement.attach = function(context) {
  var $wrappers = $('div.form-builder-wrapper:not(.form-builder-processed)', context);
  var $elements = $('div.form-builder-element:not(.form-builder-processed)', context);

  // If the context itself is a wrapper, add it to the list.
  if ($(context).is('div.form-builder-wrapper:not(.form-builder-processed)')) {
    $wrappers = $wrappers.add(context);
  }
  // If the context itself is an element, add to the list.
  else if ($(context).is('div.form-builder-element:not(.form-builder-processed)')) {
    $elements = $elements.add(context);
  }

  // Add a guard class.
  $wrappers.addClass('form-builder-processed');
  $elements.addClass('form-builder-processed');

  // Add over effect on rollover.
  // The .hover() method is not used to avoid issues with nested hovers.
  $wrappers.not('div.form-builder-empty-placeholder')
    .bind('mouseover', Drupal.formBuilder.addHover)
    .bind('mouseout', Drupal.formBuilder.removeHover);

  // Add AJAX to edit links.
  $wrappers.find('span.form-builder-links a.configure').click(Drupal.formBuilder.editField);

  // Add AJAX to remove links.
  $wrappers.find('span.form-builder-links a.remove').click(Drupal.formBuilder.editField);

  // Add AJAX to clone links.
  $wrappers.find('span.form-builder-bottom-links a.clone').click(Drupal.formBuilder.cloneField);

  // Add AJAX to entire field for easy editing.
  $elements.each(function() {
    if ($(this).children('fieldset.form-builder-fieldset').length == 0) {
      var link = $(this).parents('div.form-builder-wrapper:first').find('a.configure').get(0);
      if (link) {
        $(this).click(Drupal.formBuilder.clickField).addClass('form-builder-clickable');
        $(this).find('div.form-builder-element label').click(Drupal.formBuilder.clickField);
      }
      else {
        $(this).addClass('form-builder-draggable');
      }
    }
  });

  // Disable field functionality on click.
  $elements.find('input, textarea').bind('mousedown', Drupal.formBuilder.disableField);
};

/**
 * Behavior to disable preview fields and instead open up the configuration.
 */
Drupal.behaviors.formBuilderFields = {};
Drupal.behaviors.formBuilderFields.attach = function(context) {
  // Bind a function to all elements to update the preview on change.
  var $configureForm = $('#form-builder-field-configure');

  $configureForm.find('input, textarea, select')
    .not('.form-builder-field-change')
    .addClass('form-builder-field-change')
    .bind('change', Drupal.formBuilder.elementPendingChange);

  $configureForm.find('input.form-text, textarea')
    .not('.form-builder-field-keyup')
    .addClass('form-builder-field-keyup')
    .bind('keyup', Drupal.formBuilder.elementPendingChange);
};

/**
 * Behavior for the entire form builder. Add drag and drop to elements.
 */
Drupal.behaviors.formBuilder = {};
Drupal.behaviors.formBuilder.attach = function(context) {
  var $formbuilder = $('#form-builder');
  var $elements = $('.form-builder-wrapper').not('.form-builder-empty-placeholder').not('.ui-draggable');
  $elements.draggable({
    distance: 4, // Pixels before dragging starts.
    handle: 'div.form-builder-title-bar, div.form-builder-element',
    helper: 'clone',
    appendTo: $formbuilder,
    opacity: 0.8,
    scope: 'form-builder',
    scroll: true,
    scrollSensitivity: 50,
    zIndex: 100,
    start: Drupal.formBuilder.startDrag,
    stop: Drupal.formBuilder.stopDrag
  });

  // This sets the height of the drag target to be at least as high as the field
  // palette so that field can be more easily dropped into an empty form.
  $formbuilder.css('min-height', $('#form-builder-fields').height());

  // Add the placeholder for an empty form.
  Drupal.formBuilder.checkForm();
};

/**
 * Behavior that renders fieldsets as tabs within the field configuration form.
 */
Drupal.behaviors.formBuilderTabs = {};
Drupal.behaviors.formBuilderTabs.attach = function(context) {
  var $fieldsets = $('fieldset.form-builder-group:not(.form-builer-tabs-processed)', context);
  var $close = $('<a class="close" href="#">' + Drupal.t('Close') + '</a>');
  var $tabs;
  var tabs = '';

  // Convert fieldsets to tabs.
  tabs = '<ul class="form-builder-tabs tabs clearfix">';
  $fieldsets.children('legend').each(function() {
    tabs += '<li>' + this.innerHTML + '</li>';
    $(this).remove();
  });
  tabs += '</ul>';

  // Add the new tabs to the page.
  $tabs = $(tabs);
  $fieldsets.filter(':first').before($close).before($tabs);

  // Remove 'fieldset-legend' class from tabs.
  $tabs.find('.fieldset-legend').removeClass('fieldset-legend');

  // Set clearfix on the parent div.
  $tabs.parent().addClass('clearfix');

  // Hide all the fieldsets except the first.
  $fieldsets.not(':first').css('display', 'none');
  $tabs.find('li:first').addClass('active').click(Drupal.formBuilder.clickCancel);

  // Enable tab switching by clicking on each tab.
  $tabs.find('li:not(.close)').each(function(index) {
    $(this).click(function() {
      $fieldsets.filter(':visible').css('display', 'none');
      $fieldsets.eq(index).css('display', 'block');
      $tabs.find('li.active').removeClass('active').unbind('click', Drupal.formBuilder.clickCancel);
      $(this).addClass('active').click(Drupal.formBuilder.clickCancel);
      Drupal.formBuilder.fixTableDragTabs($fieldsets.eq(index).get(0));
    });
  });

  $close.click(Drupal.formBuilder.clickCancel);

  // Add guard class.
  $fieldsets.addClass('form-builer-tabs-processed');
};

/**
 * Submit the delete form via AJAX or close the form with the cancel link.
 */
Drupal.behaviors.formBuilderDeleteConfirmation = {};
Drupal.behaviors.formBuilderDeleteConfirmation.attach = function(context) {
  var $confirmForm = $('form.confirmation', context);

  // If the confirmation form is the context.
  if ($(context).is('form.confirmation')) {
    $confirmForm = $(context);
  }

  if ($confirmForm.length) {
    $confirmForm.submit(Drupal.formBuilder.deleteField);
    $confirmForm.find('a').click(Drupal.formBuilder.clickCancel);
  }
};

/**
 * Keeps record of if a mouse button is pressed.
 */
Drupal.behaviors.formBuilderMousePress = {};
Drupal.behaviors.formBuilderMousePress.attach = function(context) {
  if (context == document) {
    $('body').mousedown(function() { Drupal.formBuilder.mousePressed = 1; });
    $('body').mouseup(function() { Drupal.formBuilder.mousePressed = 0; });
  }
};

/**
 * Scrolls the add new field block with the window.
 */
Drupal.behaviors.formBuilderBlockScroll = {};
Drupal.behaviors.formBuilderBlockScroll.attach = function(context) {
  var $list = $('ul.form-builder-fields', context);

  if ($list.length) {
    var $block = $list.parents('div.block:first').css('position', 'relative');
    var blockScrollStart = $block.offset().top;

    function blockScroll() {
      // Do not move the palette while dragging a field.
      if (Drupal.formBuilder.activeDragUi) {
        return;
      }

      var toolbarHeight = parseInt($('body.toolbar').css('padding-top'));
      var windowOffset = $(window).scrollTop() + (toolbarHeight ? toolbarHeight : 0);
      var blockHeight = $block.height();
      var formBuilderHeight = $('#form-builder').height();
      if (windowOffset - blockScrollStart > 0) {
        // Do not scroll beyond the bottom of the editing area.
        var newTop = Math.min(windowOffset - blockScrollStart + 20, formBuilderHeight - blockHeight);
        $block.animate({ top: (newTop + 'px') }, 'fast');
      }
      else {
        $block.animate({ top: '0px' }, 'fast');
      }
    }

    var timeout = false;
    function scrollTimeout() {
      if (timeout) {
        clearTimeout(timeout);
      }
      timeout = setTimeout(blockScroll, 100);
    }

    $(window).scroll(scrollTimeout);
  }
};

/**
 * Behavior for the Add a field block.
 * @param {Object} context
 */
Drupal.behaviors.formBuilderNewField = {};
Drupal.behaviors.formBuilderNewField.attach = function(context) {
  var $list = $('ul.form-builder-fields', context);

  if ($list.length) {
    // Allow items to be copied from the list of new fields.
    $list.children('li:not(.ui-draggable)').draggable({
      opacity: 0.8,
      helper: 'clone',
      scope: 'form-builder',
      scroll: true,
      scrollSensitivity: 50,
      tolerance: 'pointer',
      zIndex: 100,
      start: Drupal.formBuilder.startDrag,
      stop: Drupal.formBuilder.stopDrag
    });

    $list.click(Drupal.formBuilder.addField);
  }
};

Drupal.formBuilder = {
  // Variable to prevent multiple requests.
  updatingElement: false,
  // Variables to allow delayed updates on textfields and textareas.
  updateDelayElement: false,
  updateDelay: false,
  // Variable holding the actively edited element (if any).
  activeElement: false,
  // Variable holding the active drag object (if any).
  activeDragUi: false,
  // Variables to keep trak of the current and previous drop target. Used to
  // prevent overlapping targets from being shown as active at the same time.
  activeDropzone: false,
  previousDropzones: [],
  // Variable of the time of the last update, used to prevent old data from
  // replacing newer updates.
  lastUpdateTime: 0,
  // Status of mouse click.
  mousePressed: 0,
  // Selector for a custom field configuration form.
  fieldConfigureForm: false
};

/**
 * Event callback for mouseover of fields. Adds hover class.
 */
Drupal.formBuilder.addHover = function() {
  // Do not add hover effect while dragging over other fields.
  if (!Drupal.formBuilder.activeDragUi && !Drupal.formBuilder.mousePressed) {
    if ($(this).find('div.form-builder-hover').length == 0) {
      $(this).addClass('form-builder-hover');
    }
  }
};

/**
 * Event callback for mouseout of fields. Removes hover class.
 */
Drupal.formBuilder.removeHover = function() {
  // Do not add hover effect while dragging over other fields.
  if (!Drupal.formBuilder.activeDragUi && !Drupal.formBuilder.mousePressed) {
    $(this).removeClass('form-builder-hover');
  }
};

/**
 * Click handler for fields.
 *
 * Note this is applied to both the entire field and to the labels within the
 * field, as they have special browser behavior that needs to be overridden.
 */
Drupal.formBuilder.clickField = function(e) {
  // Allow select lists to be clicked on without opening the edit options.
  if ($(e.target).is('select')) {
    return;
  }

  // Find the first configure link for this field, ensuring we don't get a link
  // belonging to a nested form element within this element.
  var $wrapper = $(this).parents('.form-builder-wrapper:first');
  var link = $wrapper.find('a.configure').not($wrapper.find('.form-builder-element .form-builder-element a')).get(0);
  Drupal.formBuilder.editField.apply(link);

  return false;
};

/**
 * Mousedown event on element previews.
 */
Drupal.formBuilder.disableField = function(e) {
  return false;
};

/**
 * Load the edit form from the server.
 */
Drupal.formBuilder.editField = function() {
  var $element = $(this).parents('div.form-builder-wrapper');
  var $link = $(this);

  // Prevent duplicate clicks from taking effect if already handling a click.
  if (Drupal.formBuilder.updatingElement) {
    return false;
  }

  // Remove any highlight on this wrapper.
  $element.removeClass('highlighted');

  // Show loading indicators.
  $link.addClass('progress');

  // If clicking on the link a second time, close the form instead of open.
  if ($element.get(0) == Drupal.formBuilder.activeElement && $link.get(0) == Drupal.formBuilder.activeLink) {
    Drupal.formBuilder.closeActive(function() {
      $link.removeClass('progress');
    });
    Drupal.formBuilder.unsetActive();
    return false;
  }

  var getForm = function() {
    if (Drupal.formBuilder.fieldConfigureForm) {
      $(Drupal.formBuilder.fieldConfigureForm).html(Drupal.settings.formBuilder.fieldLoading);
    }

    $.ajax({
      url: $link.attr('href'),
      type: 'GET',
      dataType: 'json',
      data: 'js=1',
      success: Drupal.formBuilder.displayForm
    });
  };

  Drupal.formBuilder.updatingElement = true;
  Drupal.formBuilder.closeActive(getForm);
  Drupal.formBuilder.setActive($element.get(0), $link.get(0));

  return false;
};

/**
 * Clone a field and insert it in the form immediately after the current field.
 */
Drupal.formBuilder.cloneField = function() {
  var $cloneLink = $(this);
  var name = Drupal.formBuilder.newFieldName();
  var $placeholder = Drupal.formBuilder.ajaxPlaceholder(name);

  // Show loading indicators.
  $cloneLink.addClass('progress');

  $.ajax({
    url: $cloneLink.attr('href'),
    type: 'GET',
    dataType: 'json',
    data: 'js=1&element_id=' + name,
    success: function(response) {
      var $new = Drupal.formBuilder.addElement(response);
      $cloneLink.removeClass('progress');
      $new.hide().fadeIn();
    }
  });

  $placeholder.insertAfter($cloneLink.closest('.form-builder-wrapper'));

  Drupal.formBuilder.updatingElement = true;

  // Scroll the palette into view.
  $(window).triggerHandler('scroll');

  return false;
};

/**
 * Add a field and insert it at the end of the form.
 */
Drupal.formBuilder.addField = function(e) {
  var $link = $(e.target);
  if (!$link.parent().is('.form-builder-palette-element')) {
    return;
  }
  var $palette = $link.parent();
  if ($palette.hasClass('form-builder-unique') || $palette.hasClass('form-builder-wrapper')) {
    $palette.hide();
  }

  var name = Drupal.formBuilder.newFieldName($link.parent());
  var $placeholder = Drupal.formBuilder.ajaxPlaceholder(name);
  var $lastWrapper = $('#form-builder').find('.form-builder-wrapper:last');

  // Show loading indicators.
  $link.addClass('progress');

  $.ajax({
    url: $link.attr('href'),
    type: 'GET',
    dataType: 'json',
    data: 'js=1&element_id=' + name,
    success: function(response) {
      var $new = Drupal.formBuilder.addElement(response);
      var scrollOffset = $new.offset().top;
      $link.removeClass('progress');
      $new.css('visible', 'hidden');
      if ($.fn.scrollTo) {
        $(window).scrollTo(scrollOffset);
      }
      else {
        $(window).scrollTop(scrollOffset);
      }
      $new.fadeIn();
    }
  });

  if ($lastWrapper.is('.form-builder-empty-placeholder')) {
    $lastWrapper.replaceWith($placeholder);
  }
  else {
    $placeholder.insertAfter($lastWrapper);
  }

  Drupal.formBuilder.updatingElement = true;

  return false;
};

/**
 * Click handler for deleting a field.
 */
Drupal.formBuilder.deleteField = function() {
  $(this).parents('div.form-builder-wrapper:first').animate({ height: 'hide', opacity: 'hide' }, 'normal', function() {
    // If this is a unique field, show the field in the palette again.
    var elementId = $(this).find('div.form-builder-element').attr('id');
    $('ul.form-builder-fields').find('li.' + elementId).show('slow');
    // Remove the field from the form.
    $(this).remove();

    // Check for an entirely empty form and for empty fieldsets.
    Drupal.formBuilder.checkForm();
    Drupal.formBuilder.checkFieldsets([], true);
  });
};

Drupal.formBuilder.clickCancel = function() {
  Drupal.formBuilder.closeActive();
  Drupal.formBuilder.unsetActive();
  return false;
};

/**
 * Highlight a particular field, i.e after cloning or adding a new field.
 */
Drupal.formBuilder.highlightField = function(timeout) {
  if (Drupal.formBuilder.highlightedField) {
    $(Drupal.formBuilder.highlightedField).removeClass('highlighted');
  }
  var $wrapper = $(this).closest('.form-builder-wrapper');
  $wrapper.addClass('highlighted');
  Drupal.formBuilder.highlightedField = $wrapper;
  if (timeout) {
    setTimeout(function() {
      $wrapper.removeClass('highlighted');
    }, timeout);
  }
}

/**
 * Display the edit form from the server.
 */
Drupal.formBuilder.displayForm = function(response) {
  // Update Drupal settings.
  if (response.settings) {
    $.extend(true, Drupal.settings, response.settings);
  }

  var $preview = $('#form-builder-element-' + response.elementId);
  var $form = $(response.html);

  if (Drupal.formBuilder.fieldConfigureForm) {
    $(Drupal.formBuilder.fieldConfigureForm).html($form);
    $form.css('display', 'none');
  }
  else {
    $form.insertAfter($preview).css('display', 'none');
  }

  Drupal.attachBehaviors($form.get(0));

  $form
    // Add the ajaxForm behavior to the new form.
    .ajaxForm()
    // Using the 'data' $.ajaxForm property doesn't seem to work.
    // Manually add a hidden element to pass additional data on submit.
    .prepend('<input type="hidden" name="return" value="field" />')
    // Add in any messages from the server.
    .find('fieldset:first').find('.fieldset-wrapper:first').prepend(response.messages);

  $form.slideDown(function() {
    $preview.parents('div.form-builder-wrapper:first').find('a.progress').removeClass('progress');
    $form.find('input:visible:first').focus();
  });

  Drupal.formBuilder.updatingElement = false;
};

/**
 * Upon changing a field, submit via AJAX to the server.
 */
Drupal.formBuilder.elementChange = function() {
  if (!Drupal.formBuilder.updatingElement) {
    $(this).parents('form:first').ajaxSubmit({
      success: Drupal.formBuilder.updateElement,
      dataType: 'json',
      data: {
        '_triggering_element_name': 'op'
      }
    });
  }

  // Clear any pending updates until further changes are made.
  if (Drupal.formBuilder.updateDelay) {
    clearTimeout(Drupal.formBuilder.updateDelay);
  }

  Drupal.formBuilder.updatingElement = true;
};

/**
 * Update a field after a delay.
 *
 * Similar to immediately changing a field, this field as pending changes that
 * will be updated after a delay. This includes textareas and textfields in
 * which updating continuously would be a strain the server and actually slow
 * down responsiveness.
 */
Drupal.formBuilder.elementPendingChange = function(e) {
  // Only operate on "normal" keys, excluding special function keys.
  // http://protocolsofmatrix.blogspot.com/2007/09/javascript-keycode-reference-table-for.html
  if (e.type == 'keyup' && !(
    e.keyCode >= 48 && e.keyCode <= 90 || // 0-9, A-Z.
    e.keyCode >= 93 && e.keyCode <= 111 || // Number pad.
    e.keyCode >= 186 && e.keyCode <= 222 || // Symbols.
    e.keyCode == 8) // Backspace.
    ) {
    return;
  }

  if (Drupal.formBuilder.updateDelay) {
    clearTimeout(Drupal.formBuilder.updateDelay);
  }
  Drupal.formBuilder.updateDelayElement = this;
  Drupal.formBuilder.updateDelay = setTimeout("Drupal.formBuilder.elementChange.apply(Drupal.formBuilder.updateDelayElement, [true])", 500);
};

/**
 * After submitting the change to the server, display the updated element.
 */
Drupal.formBuilder.updateElement = function(response) {
  var $configureForm = $('#form-builder-field-configure');

  // Do not let older requests replace newer updates.
  if (response.time < Drupal.formBuilder.lastUpdateTime) {
    return;
  }
  else {
    Drupal.formBuilder.lastUpdateTime = response.time;
  }

  // Update Drupal.settings.
  if (response.settings) {
    $.extend(true, Drupal.settings, response.settings);
  }

  // Set the error class on fields.
  $configureForm.find('.error').removeClass('error');
  if (response.errors) {
    for (var elementName in response.errors) {
      elementName = elementName.replace(/([a-z0-9_]+)\](.*)/, '$1$2]');
      $configureForm.find('[name="' + elementName + '"]').addClass('error');
    }
  }

  // Display messages, if any.
  $configureForm.find('.messages').remove();
  if (response.messages) {
    $configureForm.find('fieldset:visible:first').find('.fieldset-wrapper:first').prepend(response.messages);
  }

  // Do not update the element if errors were received.
  if (!response.errors) {
    var $exisiting = $('#form-builder-element-' + response.elementId);
    var $new = $(response.html).find('div.form-builder-element:first');
    $exisiting.replaceWith($new);

    // Expand root level fieldsets after updating to prevent them from closing
    // after every update.
    $new.children('fieldset.collapsible').removeClass('collapsed');
    Drupal.attachBehaviors($new.get(0));
  }

  // Set the variable stating we're done updating.
  Drupal.formBuilder.updatingElement = false;
};

/**
 * When adding a new field, remove the placeholder and insert the new element.
 */
Drupal.formBuilder.addElement = function(response) {
  // Update Drupal settings.
  if (response.settings) {
    $.extend(true, Drupal.settings, response.settings);
  }

  // This is very similar to the update element callback, only we replace the
  // entire wrapper instead of just the element.
  var $exisiting = $('#form-builder-element-' + response.elementId).parent();
  var $new = $(response.html).find('div.form-builder-element:first').parent();
  $exisiting.replaceWith($new);
  Drupal.attachBehaviors($new.get(0));

  Drupal.formBuilder.highlightField.apply($new.get(0));

  // Set the variable stating we're done updating.
  Drupal.formBuilder.updatingElement = false;

  // Insert the new position form containing the new element, but maintain
  // the existing form action.
  var positionAction = $('#form-builder-positions').attr('action');
  $('#form-builder-positions').replaceWith(response.positionForm);
  $('#form-builder-positions').attr('action', positionAction);
  Drupal.attachBehaviors($('#form-builder-positions'));

  // Submit the new positions form to save the new element position.
  Drupal.formBuilder.updateElementPosition($new.get(0));

  return $new;
};

/**
 * Given an element, update it's position (weight and parent) on the server.
 */
Drupal.formBuilder.updateElementPosition = function(element) {
  var $element = $(element);

  // Update weights of all children within this element's parent.
  $element.parent().children('div.form-builder-wrapper').each(function(index) {
    var child_id = $(this).children('div.form-builder-element:first').attr('id');
    $('#form-builder-positions input.form-builder-weight').filter('.' + child_id).val(index);
  });

  // Update this element's parent.
  var $parent = $element.parents('div.form-builder-element:first');
  var parent_id = $parent.length ? $parent.attr('id').replace(/form-builder-element-(.*)/, '$1') : 0;
  var child_id = $element.children('div.form-builder-element:first').attr('id');
  $('#form-builder-positions input.form-builder-parent').filter('.' + child_id).val(parent_id);

  // Submit the position form via AJAX to save the new weights and parents.
  $('#form-builder-positions input[type=submit]').triggerHandler('mousedown');
};

/**
 * Called when a field is about to be moved via Sortables.
 *
 * @param e
 *   The event object containing status information about the event.
 * @param ui
 *   The jQuery Sortables object containing information about the sortable.
 */
Drupal.formBuilder.startDrag = function(e, ui) {
  Drupal.formBuilder.activeDragUi = ui;

  var $this = $(this);
  if ($this.hasClass('form-builder-unique') || $this.hasClass('form-builder-wrapper')) {
    $this.hide();
  }

  // Check fieldsets and add placeholder text if needed.
  Drupal.formBuilder.checkFieldsets([this, ui.helper]);

  // Create the drop targets in between the form elements.
  Drupal.formBuilder.createDropTargets(this, ui.helper);
};

/**
 * Creates drop targets for the dragged element to be dropped into.
 */
Drupal.formBuilder.createDropTargets = function(draggable, helper) {
  var $placeholder = $('<div class="form-builder-placeholder"></div>');
  var $elements = $('#form-builder .form-builder-wrapper:not(.form-builder-empty-placeholder)').not(draggable).not(helper);

  if ($elements.length == 0) {
    // There are no form elements, show the placeholder
    $('#form-builder .form-builder-empty-form').show();
  }
  else {
    $elements.each(function(i) {
      $placeholder.clone().insertAfter(this);
      // If the element is the first in its container, add a drop target above it.
      if (this == $(this).parent().children('.form-builder-wrapper:not(.ui-draggable-dragging)').not(draggable)[0]) {
        $placeholder.clone().insertBefore(this);
      }
    });
    // Add a class to the last placeholder so it can be larger.
    $('#form-builder .form-builder-placeholder:last').addClass('form-builder-placeholder-last');
  }

  // Enable the drop targets
  $('#form-builder').find('.form-builder-placeholder, .form-builder-empty-placeholder').droppable({
    greedy: true,
    scope: 'form-builder',
    tolerance: 'pointer',
    deactivate: Drupal.formBuilder.dropElement,
    over: Drupal.formBuilder.dropHover,
    out: Drupal.formBuilder.dropHover
  });
};

/**
 * Handles form elements being dropped onto the form.
 *
 * Existing elements will trigger a reorder, while new elements will be added in
 * place to the form.
 */
Drupal.formBuilder.dropElement = function (event, ui) {
  var $element = ui.draggable;
  var $placeholder = $(this);

  // This callback is triggered for every placeholder, but only one should be
  // active. For all other placeholders, we don't do any processing.
  if (!$placeholder.is('.form-builder-placeholder-hover')) {
    return;
  }

  // If the element is a new field from the palette, update it with a real field.
  if ($element.is('.form-builder-palette-element')) {
    var name = Drupal.formBuilder.newFieldName($element);

    var $ajaxPlaceholder = Drupal.formBuilder.ajaxPlaceholder(name);

    $.ajax({
      url: $element.find('a').attr('href'),
      type: 'GET',
      dataType: 'json',
      data: 'js=1&element_id=' + name,
      success: Drupal.formBuilder.addElement
    });

    $placeholder.replaceWith($ajaxPlaceholder);

    Drupal.formBuilder.updatingElement = true;
  }
  // Update the positions (weights and parents) in the form cache.
  else {
    $element.removeAttr('style');
    $placeholder.replaceWith($element);
    ui.helper.remove();
    Drupal.formBuilder.updateElementPosition($element.get(0));
  }

  Drupal.formBuilder.activeDragUi = false;

  // Scroll the palette into view.
  $(window).triggerHandler('scroll');
};

/**
 * Adjusts the placeholder height for drop targets as they are hovered-over.
 */
Drupal.formBuilder.dropHover = function (event, ui) {
  var $placeholder = $(this);

  if (event.type == 'dropover') {
    // In the event that two droppables overlap, the latest one acts as the drop
    // target. If there is previous active droppable hide it temporarily.
    if (Drupal.formBuilder.activeDropzone) {
      $(Drupal.formBuilder.activeDropzone).css('display', 'none');
      Drupal.formBuilder.previousDropzones.push(Drupal.formBuilder.activeDropzone);
    }
    if (!$placeholder.hasClass('form-builder-empty-placeholder')) {
      $placeholder.css({ height: ui.helper.height() + 'px', display: ''});
    }
    $placeholder.addClass('form-builder-placeholder-hover');
    Drupal.formBuilder.activeDropzone = this;
  }
  else {
    $placeholder.css({ height: '', display: '' }).removeClass('form-builder-placeholder-hover');

    // If this was active drop target, we remove the active state.
    if (Drupal.formBuilder.activeDropzone && Drupal.formBuilder.activeDropzone == this) {
      Drupal.formBuilder.activeDropzone = false;
    }
    // If there is a previous drop target that was hidden, restore it.
    if (Drupal.formBuilder.previousDropzones.length) {
      $(Drupal.formBuilder.previousDropzones).css('display', '');
      Drupal.formBuilder.activeDropzone = Drupal.formBuilder.previousDropzones.pop();
    }
  }
};

/**
 * Called when a field has stopped moving via draggable.
 *
 * @param e
 *   The event object containing status information about the event.
 * @param ui
 *   The jQuery Sortables object containing information about the sortable.
 */
Drupal.formBuilder.stopDrag = function(e, ui) {
  var $this = $(this);
  // If the activeDragUi is still set, we did not drop onto the form.
  if (Drupal.formBuilder.activeDragUi) {
    if ($this.hasClass('form-builder-unique') || $this.hasClass('form-builder-wrapper')) {
      $this.show();
    }
    Drupal.formBuilder.activeDragUi = false;
  }

  // Remove the placeholders and reset the hover state for all for elements
  $('#form-builder .form-builder-placeholder').remove();
  $('#form-builder .form-builder-hover').removeClass('form-builder-hover');

  Drupal.formBuilder.checkFieldsets();

  // Scroll the palette into view.
  $(window).triggerHandler('scroll');
};

/**
 * Insert DIVs into empty fieldsets so that items can be dropped within them.
 *
 * This function is called every time an element changes positions during
 * a drag and drop operation. Fieldsets are considered empty if they have no
 * immediate children or they only contain exclusions.
 *
 * @param exclusions
 *   An array of DOM objects within a fieldset that should not be included when
 *   checking if the fieldset is empty.
 */
Drupal.formBuilder.checkFieldsets = function(exclusions, animate) {
  var $wrappers = $('#form-builder div.form-builder-element > fieldset.form-builder-fieldset > div.fieldset-wrapper');

  // Make sure exclusions is an array and always skip any description.
  exclusions = exclusions ? exclusions : [];
  exclusions.push('.fieldset-description');

  // Insert a placeholder into all empty fieldset wrappers.
  $wrappers.each(function() {
    var children = $(this).children(':visible, :not(.ui-draggable-dragging)');
    for (var i in exclusions) {
      children = children.not(exclusions[i]);
    }

    if (children.length == 0) {
      // No children, add a placeholder.
      if (animate) {
        $(Drupal.settings.formBuilder.emptyFieldset).css('display', 'none').appendTo(this).slideDown();
      }
      else {
        $(Drupal.settings.formBuilder.emptyFieldset).appendTo(this);
      }
    }
    else if (children.length > 1 && children.hasClass('form-builder-empty-placeholder')) {
      // The fieldset has at least one element besides the placeholder, remove
      // the placeholder.
      $(this).find('.form-builder-empty-placeholder').remove();
    }
  });
};

/**
 * Check the root form tag and place explanatory text if the form is empty.
 */
Drupal.formBuilder.checkForm = function() {
  var $formBuilder = $('#form-builder');
  if ($formBuilder.children('div.form-builder-wrapper').length == 0) {
    $formBuilder.append(Drupal.settings.formBuilder.emptyForm);
  }
};

Drupal.formBuilder.setActive = function(element, link) {
  Drupal.formBuilder.unsetActive();
  Drupal.formBuilder.activeElement = element;
  Drupal.formBuilder.activeLink = link;
  $(Drupal.formBuilder.activeElement).addClass('form-builder-active');
};

Drupal.formBuilder.unsetActive = function() {
  if (Drupal.formBuilder.activeElement) {
    $(Drupal.formBuilder.activeElement).removeClass('form-builder-active');
    Drupal.formBuilder.activeElement = false;
    Drupal.formBuilder.activeLink = false;
  }
};

Drupal.formBuilder.closeActive = function(callback) {
  if (Drupal.formBuilder.activeElement) {
    var $activeForm = Drupal.formBuilder.fieldConfigureForm ? $(Drupal.formBuilder.fieldConfigureForm).find('form') : $(Drupal.formBuilder.activeElement).find('form');

    if ($activeForm.length) {
      Drupal.freezeHeight();
      $activeForm.slideUp(function(){
        $(this).remove();
        // Set a message in the custom configure form location if it exists.
        if (Drupal.formBuilder.fieldConfigureForm) {
          $(Drupal.formBuilder.fieldConfigureForm).html(Drupal.settings.formBuilder.noFieldSelected);
        }
        if (callback) {
          callback.call();
        }
      });
    }
  }
  else if (callback && $.isFunction(callback)) {
    callback.call();
  }

  return false;
};

/**
 * Returns a unique machine name that can be used for a new form field.
 */
Drupal.formBuilder.newFieldName = function($element) {
  // If this is a "unique" element, its element ID is hard-coded.
  if ($element && $element.is('.form-builder-unique')) {
    return $element.get(0).className.replace(/^.*?form-builder-element-([a-z0-9_]+).*?$/, '$1');
  }
  else {
    return 'new_' + new Date().getTime();
  }
};

/**
 * Returns HTML to use as an AJAX placeholder when a new field is being added.
 *
 * @param name
 *   The machine name for the new field. See Drupal.formBuilder.newFieldName().
 *
 * @return
 *   The placeholder HTML.
 */
Drupal.formBuilder.ajaxPlaceholder = function(name) {
  return $('<div class="form-builder-wrapper form-builder-new-field"><div id="form-builder-element-' + name + '" class="form-builder-element"><span class="progress">' + Drupal.t('Please wait...') + '</span></div></div>');
};

/**
 * Work around for tabledrags within tabs. On load, if the tab was hidden the
 * offsets cannot be calculated correctly. Recalculate and update the tableDrag.
 */
Drupal.formBuilder.fixTableDragTabs = function(context) {
  if (Drupal.tableDrag && Drupal.tableDrag.length > 1) {
    for (var n in Drupal.tableDrag) {
      if (typeof(Drupal.tableDrag[n]) == 'object') {
        var table = $('#' + n, context).get(0);
        if (table) {
          var indent = Drupal.theme('tableDragIndentation');
          var testCell = $('tr.draggable:first td:first', table).prepend(indent).prepend(indent);
          Drupal.tableDrag[n].indentAmount = $('.indentation', testCell).get(1).offsetLeft - $('.indentation', testCell).get(0).offsetLeft;
          $('.indentation', testCell).slice(0, 2).remove();
        }
      }
    }
  }
};

})(jQuery);
