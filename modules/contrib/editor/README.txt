CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Technical details

INTRODUCTION
------------

Current Maintainers:

 * Devin Carlson <http://drupal.org/user/290182>

Editor allows rich text fields to be edited using WYSIWYG client-side editors.

REQUIREMENTS
------------

Editor has one dependency.

Drupal core modules
 * Filter

The included Editor CKEditor module has one additional dependency.

Contributed modules
 * Dialog

INSTALLATION
------------

 * Install Editor via the standard Drupal installation process:
   'http://drupal.org/node/895232'.
 * Enable and configure an editor for your desired text format at
   '/admin/config/content/formats'. Editor provides an API for integrating
   client-side WYSIWYG editors but does provide support for any editors out of
   the box. You must enable enable one or more editor provider modules in order
   to select an editor.

The included Editor CKEditor submodule module adds support for the popular
CKEditor WYSIWYG editor which is bundled with Drupal 8.
