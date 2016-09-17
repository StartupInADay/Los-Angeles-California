/**
 * @file Plugin to support responsive images with the Picture module and 
 * the CKEditor module.
 */
( function(){
  CKEDITOR.plugins.add('picture_ckeditor',
  {
      init : function(editor)
      {
                
          // This disables the browser resize handles since the width will now be set
          // in the image dialog. This also prevents CKEditor from automatically adding
          // pesky inline width and height styles, although these inline styles are
          // still added when an image is dragged and dropped.
          // Resize handles are also removed from tables as an unintended consequence.
        CKEDITOR.config.disableObjectResizing = true; 


        // When opening a dialog, a 'definition' is created for it. For
        // each editor instance the 'dialogDefinition' event is then
        // fired. We can use this event to make customizations to the
        // definition of existing dialogs.
        CKEDITOR.on('dialogDefinition', function(event) {
          // Take the dialog name.
          if ((event.editor != editor)) return;
          var dialogName = event.data.name;
          // The definition holds the structured data that is used to eventually
          // build the dialog and we can use it to customize just about anything.
          // In Drupal terms, it's sort of like CKEditor's version of a Forms API and
          // what we're doing here is a bit like a hook_form_alter.
          var dialogDefinition = event.data.definition;


          // Resources for the following:
          // Download: https://github.com/ckeditor/ckeditor-dev
          // See /plugins/image/dialogs/image.js
          // and refer to http://docs.ckeditor.com/#!/api/CKEDITOR.dialog.definition
          // Visit: file:///[path_to_ckeditor-dev]/plugins/devtools/samples/devtools.html
          // for an excellent way to find machine names for dialog elements.
          if (dialogName == 'image') {
            dialogDefinition.removeContents('advanced');
            dialogDefinition.removeContents('Link');
            var infoTab = dialogDefinition.getContents('info');
            var altText = infoTab.get('txtAlt');
            var IMAGE = 1,
                LINK = 2,
                PREVIEW = 4,
                CLEANUP = 8;
            // UpdatePreview is copied from ckeditor image plugin.
            var updatePreview = function(dialog) {
              // Don't load before onShow.
              if (!dialog.originalElement || !dialog.preview) {
                return 1;
              }

              // Read attributes and update imagePreview.
              dialog.commitContent(PREVIEW, dialog.preview);
              return 0;
            };
            // Add the select list for choosing the image width.
            infoTab.add({
              type: 'select',
              id: 'imageSize',
              label: Drupal.settings.picture.label,
              items: Drupal.settings.picture.groups,
              'default': 'not_set',
              onChange: function() {
                var dialog = this.getDialog();
                var element = dialog.originalElement;
                element.setAttribute('data-picture-group', this.getValue());
                updatePreview(this.getDialog());
              },
              setup: function(type, element) {
                if (type == IMAGE) {
                  var value = element.getAttribute('data-picture-group');
                  this.setValue(value);
                }
              },
              // Create a custom data-picture-group attribute.
              commit: function(type, element) {
                if (type == IMAGE) {
                  if (this.getValue() || this.isChanged()) {
                    element.setAttribute('data-picture-group', this.getValue());
                  }
                } else if (type == PREVIEW) {
                  element.setAttribute('data-picture-group', this.getValue());
                } else if (type == CLEANUP) {
                  element.setAttribute('data-picture-group', '');
                }
              },
              validate: function() {
                if (this.getValue() == 'not_set') {
                  var message = 'Please make a selection from ' + Drupal.settings.picture.label;
                  alert(message);
                  return false;
                } else {
                  return true;
                }
              }
            },
              // Position before preview.
              'htmlPreview'
            );

            // Put a title attribute field on the main 'info' tab.
            infoTab.add( {
              type: 'text',
              id: 'txtTitle',
              label: 'The title attribute is used as a tooltip when the mouse hovers over the image.',
              onChange: function() {
                updatePreview(this.getDialog());
              },
              setup: function(type, element) {
                if (type == IMAGE)
                  this.setValue(element.getAttribute('title'));
              },
              commit: function(type, element) {
                if (type == IMAGE) {
                  if (this.getValue() || this.isChanged())
                    element.setAttribute('title', this.getValue());
                } else if (type == PREVIEW) {
                  element.setAttribute('title', this.getValue());
                } else if (type == CLEANUP) {
                  element.removeAttribute('title');
                }
              }
            },
              // Position before the imageSize select box.
              'htmlPreview'
            );

            // Add a select widget to choose image alignment.
            infoTab.add({
              type: 'select',
              id: 'imageAlign',
              label: 'Image Alignment',
              items: [ [ 'Not Set', '' ], [ 'Left', 'left'],
                       [ 'Right', 'right' ], [ 'Center', 'center'] ],
              'default': '',
              onChange: function() {
                updatePreview(this.getDialog());
              },
              setup: function(type, element) {
                if (type == IMAGE) {
                  var value = element.getAttribute('data-picture-align');
                  this.setValue(value);
                }
              },
              // Creates a custom data-picture-align attribute since working with classes
              // is more difficult. If we used classes, then we'd have to search for
              // exisiting alignment classes and remove them before adding a new one.
              // With the custom attribute we can always just overwrite it's value.
              commit: function(type, element) {
                if (type == IMAGE) {
                  if (this.getValue() || this.isChanged()) {
                    element.setAttribute('data-picture-align', this.getValue());
                  }
                } else if (type == PREVIEW) {
                  element.setAttribute('data-picture-align', this.getValue());
                } else if (type == CLEANUP) {
                  element.setAttribute('data-picture-align', '');
                }
              }

            },
              // Position before imageSize.
              'imageSize'
            );

            // Improve the alt field label. Copied from Drupal's image field.
            altText.label = 'The alt attribute may be used by search engines, and screen readers.';

            // Remove a bunch of extraneous fields. These properties will be set in
            // the theme or module CSS.
            infoTab.remove('basic');
          }
        });
      }
  });
})();
