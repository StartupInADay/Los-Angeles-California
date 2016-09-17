/**
 * @file
 * Drupal Entity plugin.
 */

(function ($) {

  "use strict";

  CKEDITOR.plugins.add('drupalentity', {
    // This plugin requires the Widgets System defined in the 'widget' plugin.
    requires: 'widget',

    // The plugin initialization logic goes inside this method.
    init: function (editor) {
      // Configure CKEditor DTD for custom drupal-entity element.
      // @see https://www.drupal.org/node/2448449#comment-9717735
      var dtd = CKEDITOR.dtd, tagName;
      dtd['drupal-entity'] = {'#': 1};
      // Register drupal-entity element as allowed child, in each tag that can
      // contain a div element.
      for (tagName in dtd) {
        if (dtd[tagName].div) {
          dtd[tagName]['drupal-entity'] = 1;
        }
      }

      // Generic command for adding/editing entities of all types.
      editor.addCommand('editdrupalentity', {
        allowedContent: 'drupal-entity[*]',
        requiredContent: 'drupal-entity[*]',
        modes: { wysiwyg : 1 },
        canUndo: true,
        exec: function (editor, data) {
          data = data || {};

          var existingElement = getSelectedEntity(editor);

          var existingValues = {};
          if (existingElement && existingElement.$ && existingElement.$.firstChild) {
            var entityDOMElement = existingElement.$.firstChild;
            // Populate array with the entity's current attributes.
            var attribute = null, attributeName;
            for (var key = 0; key < entityDOMElement.attributes.length; key++) {
              attribute = entityDOMElement.attributes.item(key);
              attributeName = attribute.nodeName.toLowerCase();
              if (attributeName.substring(0, 15) === 'data-cke-saved-') {
                continue;
              }
              existingValues[attributeName] = existingElement.data('cke-saved-' + attributeName) || attribute.nodeValue;
            }
          }

          var entity_label = data.label ? data.label : existingValues['data-entity-label'];
          var embed_button_id = data.id ? data.id : existingValues['data-embed-button'];

          var dialogSettings = {
            title: existingElement ? 'Edit ' + entity_label : 'Insert ' + entity_label,
            dialogClass: 'entity-select-dialog',
            resizable: false,
            minWidth: 800
          };

          var saveCallback = function (values) {
            var entityElement = editor.document.createElement('drupal-entity');
            var attributes = values.attributes;
            for (var key in attributes) {
              entityElement.setAttribute(key, attributes[key]);
            }
            editor.insertHtml(entityElement.getOuterHtml());
            if (existingElement) {
              // Detach the behaviors that were attached when the entity content
              // was inserted.
              runEmbedBehaviors('detach', existingElement.$);
              existingElement.remove();
            }
          };

          // Open the entity embed dialog for corresponding EmbedButton.
          Drupal.entityEmbed.openDialog(editor, Drupal.settings.basePath + 'entity-embed/dialog/entity-embed/' + editor.config.drupal.format + '/' + embed_button_id + '?_format=drupal_dialog', existingValues, saveCallback, dialogSettings);
        }
      });

      // Register the entity embed widget.
      editor.widgets.add('drupalentity', {
        // Minimum HTML which is required by this widget to work.
        allowedContent: 'drupal-entity[*]',
        requiredContent: 'drupal-entity[*]',

        // Simply recognize the element as our own. The inner markup if fetched
        // and inserted the init() callback, since it requires the actual DOM
        // element.
        upcast: function (element) {
          var attributes = element.attributes;
            if (attributes['data-entity-type'] === undefined || (attributes['data-entity-id'] === undefined && attributes['data-entity-uuid'] === undefined) || (attributes['data-view-mode'] === undefined && attributes['data-entity-embed-display'] === undefined)) {
            return;
          }
          // Generate an ID for the element, so that we can use the Ajax
          // framework.
          element.attributes.id = generateEmbedId();
          return element;
        },

        // Fetch the rendered entity.
        init: function () {
          var element = this.element;
          var $element = $(element.$);
          // Use the Ajax framework to fetch the HTML, so that we can retrieve
          // out-of-band assets (JS, CSS...).
          new Drupal.ajax($element.attr('id'), $element, {
            url: Drupal.settings.basePath + 'entity-embed/preview/' + editor.config.drupal.format + '?' + $.param({
              value: element.getOuterHtml()
            }),
            progress: {type: 'none'},
            // Use a custom event to trigger the call.
            event: 'entity_embed_dummy_event'
          });
          // Trigger the call manually. The actual HTML is inserted in our
          // 'entityEmbedInsertEditor' Ajax command on success.
          $element.trigger('entity_embed_dummy_event');
        },

        // Downcast the element.
        downcast: function (element) {
          // Only keep the wrapping element.
          element.setHtml('');
          // Remove the auto-generated ID.
          delete element.attributes.id;
          return element;
        }
      });

      // Register the toolbar buttons.
      if (editor.ui.addButton) {
        for (var key in Drupal.settings.editor.formats[editor.config.drupal.format].filterSettings.DrupalEntity_buttons) {
          var button = Drupal.settings.editor.formats[editor.config.drupal.format].filterSettings.DrupalEntity_buttons[key];
          editor.ui.addButton(button.id, {
            label: button.label,
            data: button,
            click: function(editor) {
              editor.execCommand('editdrupalentity', this.data);
            },
            icon: button.image
          });
        }
      }

      // Register context menu option for editing widget.
      if (editor.contextMenu) {
        editor.addMenuGroup('drupalentity');
        editor.addMenuItem('drupalentity', {
          label: Drupal.t('Edit Entity'),
          icon: this.path + 'entity.png',
          command: 'editdrupalentity',
          group: 'drupalentity'
        });

        editor.contextMenu.addListener(function(element) {
          if (isEntityWidget(editor, element)) {
            return { drupalentity: CKEDITOR.TRISTATE_OFF };
          }
        });
      }

      // Execute widget editing action on double click.
      editor.on('doubleclick', function (evt) {
        var element = getSelectedEntity(editor) || evt.data.element;

        if (isEntityWidget(editor, element)) {
          editor.execCommand('editdrupalentity');
        }
      });
    }
  });

  /**
   * Get the surrounding drupalentity widget element.
   *
   * @param {CKEDITOR.editor} editor
   */
  function getSelectedEntity(editor) {
    var selection = editor.getSelection();
    var selectedElement = selection.getSelectedElement();
    if (isEntityWidget(editor, selectedElement)) {
      return selectedElement;
    }

    return null;
  }

  /**
   * Returns whether or not the given element is a drupalentity widget.
   *
   * @param {CKEDITOR.editor} editor
   * @param {CKEDITOR.htmlParser.element} element
   */
  function isEntityWidget (editor, element) {
    var widget = editor.widgets.getByElement(element, true);
    return widget && widget.name === 'drupalentity';
  }

  /**
   * Generates unique HTML IDs for the widgets.
   *
   * @returns {string}
   */
  function generateEmbedId() {
    if (typeof generateEmbedId.counter == 'undefined') {
      generateEmbedId.counter = 0;
    }
    return 'entity-embed-' + generateEmbedId.counter++;
  }

  /**
   * Attaches or detaches behaviors, except the ones we do not want.
   *
   * @param {string} action
   *   Either 'attach' or 'detach'.
   * @param context
   *   The context argument for Drupal.attachBehaviors()/detachBehaviors().
   * @param settings
   *   The settings argument for Drupal.attachBehaviors()/detachBehaviors().
   */
  function runEmbedBehaviors(action, context, settings) {
    // Do not run the excluded behaviors.
    var stashed = {};
    $.each(Drupal.entityEmbed.excludedBehaviors, function (i, behavior) {
      stashed[behavior] = Drupal.behaviors[behavior];
      delete Drupal.behaviors[behavior];
    });
    // Run the remaining behaviors.
    (action == 'attach' ? Drupal.attachBehaviors : Drupal.detachBehaviors)(context, settings);
    // Put the stashed behaviors back in.
    $.extend(Drupal.behaviors, stashed);
  }

  /**
   * Ajax 'entityEmbedInsertEditor' command: insert the rendered entity.
   *
   * The regular Drupal.ajax.commands.insert() command cannot target elements
   * within iFrames. This is a skimmed down equivalent that works whether the
   * CKEditor is in iframe or divarea mode.
   */
  Drupal.ajax.prototype.commands.entityEmbedInsertEditor = function(ajax, response, status) {
    var $target = ajax.element;
    // No need to detach behaviors here, the widget is created fresh each time.
    $target.html(response.html);
    runEmbedBehaviors('attach', $target.get(0), response.settings || ajax.settings);
  };

  /**
   * Stores settings specific to Entity Embed module.
   */
  Drupal.entityEmbed = {
    /**
     * A list of behaviors which are to be excluded while attaching/detaching.
     *
     * - Drupal.behaviors.editor, to avoid CK inception.
     * - Drupal.behaviors.contextual, to keep contextual links hidden.
     */
    excludedBehaviors: ['editor', 'contextual'],

    /**
     * Variable storing the current dialog's save callback.
     */
    saveCallback: null,

    /**
     * Open a dialog for a Drupal-based plugin.
     *
     * This dynamically loads jQuery UI (if necessary) using the Drupal AJAX
     * framework, then opens a dialog at the specified Drupal path.
     *
     * @param editor
     *   The CKEditor instance that is opening the dialog.
     * @param string url
     *   The URL that contains the contents of the dialog.
     * @param Object existingValues
     *   Existing values that will be sent via POST to the url for the dialog
     *   contents.
     * @param Function saveCallback
     *   A function to be called upon saving the dialog.
     * @param Object dialogSettings
     *   An object containing settings to be passed to the jQuery UI.
     */
    openDialog: function (editor, url, existingValues, saveCallback, dialogSettings) {
      // Locate a suitable place to display our loading indicator.
      var $target = $(editor.container.$);
      if (editor.elementMode === CKEDITOR.ELEMENT_MODE_REPLACE) {
        $target = $target.find('.cke_contents');
      }

      // Remove any previous loading indicator.
      $target.css('position', 'relative').find('.ckeditor-dialog-loading').remove();

      // Add a consistent dialog class.
      var classes = dialogSettings.dialogClass ? dialogSettings.dialogClass.split(' ') : [];
      classes.push('editor-dialog');
      dialogSettings.dialogClass = classes.join(' ');
      dialogSettings.autoResize = Drupal.entityEmbed.checkWidthBreakpoint(600);

      // Add a "Loading…" message, hide it underneath the CKEditor toolbar, create
      // a Drupal.ajax instance to load the dialog and trigger it.
      var $content = $('<div class="ckeditor-dialog-loading"><span style="top: -40px;" class="ckeditor-dialog-loading-link"><a>' + Drupal.t('Loading...') + '</a></span></div>');
      $content.appendTo($target);
      new Drupal.ajax('ckeditor-dialog', $content.find('a').get(0), {
        dialog: dialogSettings,
        selector: '.ckeditor-dialog-loading-link',
        url: url,
        event: 'ckeditor-internal.ckeditor',
        progress: {'type': 'throbber'},
        submit: {
          editor_object: existingValues,
          dialog: dialogSettings
        }
      });
      $content.find('a')
          .on('click', function () { return false; })
          .trigger('ckeditor-internal.ckeditor');

      // After a short delay, show "Loading…" message.
      window.setTimeout(function () {
        $content.find('span').animate({top: '0px'});
      }, 1000);

      // Store the save callback to be executed when this dialog is closed.
      Drupal.entityEmbed.saveCallback = saveCallback;
    }
  };

  /**
   * Helper to test document width for mobile configurations.
   * @todo Temporary solution for the mobile initiative.
   */
  Drupal.entityEmbed.checkWidthBreakpoint = function (width) {
    width = width || drupalSettings.widthBreakpoint || 640;
    return (document.documentElement.clientWidth > width);
  };

  // Respond to new dialogs that are opened by CKEditor, closing the AJAX loader.
  $(window).on('dialog:beforecreate', function (e, dialog, $element, settings) {
    $('.ckeditor-dialog-loading').animate({top: '-40px'}, function () {
      $(this).remove();
    });
  });

  // Respond to dialogs that are saved, sending data back to CKEditor.
  $(window).on('editor:dialogsave', function (e, values) {
    if (Drupal.entityEmbed.saveCallback) {
      Drupal.entityEmbed.saveCallback(values);
    }
  });

  // Respond to dialogs that are closed, removing the current save handler.
  $(window).on('dialog:afterclose', function (e, dialog, $element) {
    if (Drupal.entityEmbed.saveCallback) {
      Drupal.entityEmbed.saveCallback = null;
    }
  });

})(jQuery);
