/**
 * @file
 * Attaches behavior for the Editor module.
 */

(function ($, Drupal) {

  "use strict";

  /**
   * Initialize an empty object for editors to place their attachment code.
   *
   * @namespace
   */
  Drupal.editors = {};

  /**
   * Enables an editor (if any) when the matching format is selected.
   */
  Drupal.behaviors.editors = {
    attach: function (context, settings) {
      // If there are no editor settings, there are no editors to enable.
      if (!settings.editor) {
        return;
      }

      var $context = $(context);
      $context.find('.filter-list:input').once('editors', function () {
        var $this = $(this);
        var activeEditor = $this.val();
        var field = $this.closest('.text-format-wrapper').find('textarea').get(-1);

        // No textarea found. This may happen on long text elements that use a
        // single-line text field widget.
        if (!field) {
          return;
        }

        // Directly attach this editor, if the input format is enabled or there is
        // only one input format at all.
        if ($this.is(':input')) {
          if (Drupal.settings.editor.formats[activeEditor]) {
            Drupal.editorAttach(field, Drupal.settings.editor.formats[activeEditor]);
          }
        }
        // Attach onChange handlers to input format selector elements.
        if ($this.is('select')) {
          $this.change(function() {
            // Detach the current editor (if any) and attach a new editor.
            if (Drupal.settings.editor.formats[activeEditor]) {
              Drupal.editorDetach(field, Drupal.settings.editor.formats[activeEditor]);
            }
            activeEditor = $this.val();
            if (Drupal.settings.editor.formats[activeEditor]) {
              Drupal.editorAttach(field, Drupal.settings.editor.formats[activeEditor]);
            }
          });
        }
        // Detach any editor when the containing form is submitted.
        $this.parents('form').submit(function (event) {
          // Do not detach if the event was canceled.
          if (event.isDefaultPrevented()) {
            return;
          }
          // Detach the current editor (if any).
          if (Drupal.settings.editor.formats[activeEditor]) {
            Drupal.editorDetach(field, Drupal.settings.editor.formats[activeEditor], 'serialize');
          }
        });
      });
    },
    detach: function (context, settings) {
      var $context = $(context);
      $context.find('.filter-list:input').each(function () {
        var $this = $(this);
        var activeEditor = $this.val();
        var field = $this.closest('.text-format-wrapper').find('textarea').get(-1);
        if (field && Drupal.settings.editor.formats[activeEditor]) {
          Drupal.editorDetach(field, Drupal.settings.editor.formats[activeEditor]);
        }
      });
    }
  };

  Drupal.editorAttach = function(field, format) {
    if (format.editor && Drupal.editors[format.editor]) {
      Drupal.editors[format.editor].attach(field, format);
    }
  };

  Drupal.editorDetach = function(field, format) {
    if (format.editor && Drupal.editors[format.editor]) {
      Drupal.editors[format.editor].detach(field, format);
    }
  };

})(jQuery, Drupal);
