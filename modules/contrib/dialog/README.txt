CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Sub-modules
 * Technical details

INTRODUCTION
------------

Current Maintainers:

 * Devin Carlson <http://drupal.org/user/290182>

Dialog provides an API for opening content in an interactive overlay.

Dialog is a backport of the Drupal 8 Dialog API for JavaScript
'https://www.drupal.org/node/1852224'.

REQUIREMENTS
------------

Dialog has one dependency.

Contributed modules
 * jQuery Update - Configured to use jQuery 1.7 or higher.

INSTALLATION
------------

* Install Dialog via the standard Drupal installation process:
  'http://drupal.org/node/895232'.
* Configure the jQuery Update module to use jQuery 1.7 or higher:
  '/admin/config/development/jquery_update'.

SUB-MODULES
------------

Dialog is packaged with a number of sub-modules, each of which provides dialog
integration with a specific core or contrib module. Integration is generally in
the form of moving various actions, such as confirmation forms, into overlays.

TECHNICAL DETAILS
-----------------

Dialog provides an API for opening and manipulating interactive overlays via
AJAX requests.

Any page can be opened in a dialog by adding an HTML attribute to the link that
should trigger the dialog.

Make sure that the Drupal AJAX library is available:
 * $build['#attached']['library'][] = array('system', 'drupal.ajax');

Create a link with the class 'use-ajax' and the data-attribute
'data-dialog="true"' signaling that the response should be loaded in a dialog:
 * <a href="/node/1" class="use-ajax" data-dialog="true">Example Link</a>

You can specify any jQuery UI dialog options using a 'data-dialog-options'
data-attribute:

 * <a href="/node/1" class="use-ajax" data-dialog="true" data-dialog-options='{"dialogClass":"my-class"}'>Example Link</a>

A full list of options can be found at 'http://api.jqueryui.com/dialog/'.

Dialogs can also be manipulated via server-side requests. You can change a
dialog's title, configure its options or close it. To do this, specify an
'#ajax' property on a form element or the 'use-ajax' class with a callback path
from another link. The callback path can then return a set of commands for
manipulating the dialog via AJAX.

An example of a form button within a dialog which saves the form and then closes
the dialog:

function my_module_form($form, &$form_state) {
  $form['option'] = array(
    '#type' => 'textfield',
    '#title' => t('Option to save'),
  );
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Close and save'),
    '#ajax' => array(
      'callback' => 'my_module_dialog_close',
    ),
  );
}

function my_module_form_submit($form, &$form_state) {
  // Save the $form_state['values']['option'] value here.
}

function my_module_dialog_close($form, &$form_state) {
  $commands = array(
    '#type' => 'ajax',
    '#commands' => array(),
  );
  $commands['#commands'][] = dialog_command_close_modal_dialog();
  return $commands;
}

See includes/dialog.commands.inc for a full list of dialog commands.
