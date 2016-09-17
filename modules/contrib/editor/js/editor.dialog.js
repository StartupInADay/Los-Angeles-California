/**
 * AJAX commands used by Editor module.
 */

(function ($) {

  "use strict";

  /**
   * Command to save the contents of an editor-provided modal.
   *
   * This command does not close the open modal. It should be followed by a call
   * to Drupal.AjaxCommands.prototype.closeDialog. Editors that are integrated
   * with dialogs must independently listen for an editor:dialogsave event to save
   * the changes into the contents of their interface.
   */
  Drupal.ajax.prototype.commands.editorDialogSave = function (ajax, response, status) {
    $(window).trigger('editor:dialogsave', [response.values]);
  };

})(jQuery);
