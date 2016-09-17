/**
 * @file
 * Drupal Foundation Accordion plugin.
 *
 * @ignore
 */

(function ($, Drupal, CKEDITOR) {

  function foundationAccordion_in_array(foundationAccordion_needle, foundationAccordion_haystack) {
    for (var i in foundationAccordion_haystack) {
      if (foundationAccordion_haystack[i] == foundationAccordion_needle) {
        return true;
      }
    }

    return false;
  }

  function foundationAccordion_escapeHtml(foundationAccordion_text) {
    var map = {
      "&": "&amp;",
      "<": "&lt;",
      ">": "&gt;",
      '"': '&quot;',
      "'": '&#39;',
      "/": '&#x2F;'
    };

    return foundationAccordion_text.replace(/[&"']/g, function(m) {
      return map[m]
    })
  }

  CKEDITOR.plugins.add('foundationAccordion', {
    lang: ['en', 'af', 'sq', 'ar', 'eu', 'bn', 'bs', 'bg', 'ca', 'zh-cn', 'zh', 'hr', 'cs', 'da', 'nl', 'en-au', 'en-ca', 'en-gb', 'eo', 'et', 'fo', 'fi', 'fr', 'fr-ca', 'gl', 'ka', 'de', 'el', 'gu', 'he', 'hi', 'hu', 'is', 'id', 'it', 'ja', 'km', 'ko', 'ku', 'lv', 'lt', 'mk', 'ms', 'mn', 'no', 'nb', 'fa', 'pl', 'pt-br', 'pt', 'ro', 'ru', 'sr', 'sr-latn', 'si', 'sk', 'sl', 'es', 'sv', 'tt', 'th', 'tr', 'ug', 'uk', 'vi', 'cy'],
    requires: 'widget',
    icons: 'foundationAccordion',
    init: function(editor) {
      editor.widgets.add('FoundationAccordion', {
        button: editor.lang.foundationAccordion.plugin,
        template: '<ul class="accordion" data-accordion>' + '<li class="accordion-navigation">' + '<a href=""></a>' + '<div id="" class="content active"></div>' + '</li>' + '</ul>',
        mask: true,
        allowedContent: 'ul(*)[*];li(*)[*];div(*)[*];a(*)[*]',
        dialog: 'foundationAccordionDialog',

        upcast: function(element) {
          return element.name == 'ul' && element.hasClass('accordion')
        },

        init: function() {
          var foundationAccordion_total = 0;

          for (var foundationAccordion_i = 0; foundationAccordion_i < this.element.$.children.length; foundationAccordion_i++) {
            eval("this.setData('foundationAccordion_item" + (foundationAccordion_i + 1) + "', '" + foundationAccordion_escapeHtml(this.element.$.children[foundationAccordion_i].firstChild.innerHTML) + "')");
            eval("this.setData('foundationAccordion_content" + (foundationAccordion_i + 1) + "', '" + foundationAccordion_escapeHtml(this.element.$.children[foundationAccordion_i].lastChild.innerHTML) + "')");

            var foundationAccordion_contentClass = foundationAccordion_in_array('active', this.element.$.children[foundationAccordion_i].lastChild.className.split(' ')) ? 'content active' : 'content';
            eval("this.setData('foundationAccordion_contentClass" + (foundationAccordion_i + 1) + "', '" + foundationAccordion_contentClass + "')");
            foundationAccordion_total++
          }

          this.setData('foundationAccordion_total', foundationAccordion_total)
        },

        data: function() {
          var foundationAccordion_d = new Date();
          var foundationAccordion_id = foundationAccordion_d.getTime();
          var foundationAccordion_item = '';

          for (var foundationAccordion_i = 0; foundationAccordion_i <= this.data.foundationAccordion_total; foundationAccordion_i++) {
            eval("foundationAccordion_title = this.data.foundationAccordion_item" + foundationAccordion_i);
            foundationAccordion_title = foundationAccordion_title != undefined ? foundationAccordion_title : '';
            eval("foundationAccordion_content = this.data.foundationAccordion_content" + foundationAccordion_i);
            foundationAccordion_content = foundationAccordion_content != undefined ? foundationAccordion_content : '';
            eval("foundationAccordion_contentClass = this.data.foundationAccordion_contentClass" + foundationAccordion_i);
            foundationAccordion_contentClass = foundationAccordion_contentClass != undefined ? foundationAccordion_contentClass : '';

            if (foundationAccordion_title) {
              foundationAccordion_item += '<li class="accordion-navigation"><a href="#accordion' + foundationAccordion_id + '_' + foundationAccordion_i + '">' + foundationAccordion_title + '</a><div id="accordion' + foundationAccordion_id + '_' + foundationAccordion_i + '" class="' + foundationAccordion_contentClass + '">' + foundationAccordion_content + '</div></li> 						'
            }
          }

          this.element.setAttribute('id', 'accordion' + foundationAccordion_id);
          this.element.$.innerHTML = foundationAccordion_item
        }
      });

      if (editor.ui.addButton) {
        editor.ui.addButton('foundationAccordion', {
          label: Drupal.t('Foundation Accordion'),
          command: 'FoundationAccordion',
          icon: this.path + '/icons/foundationAccordion.png'
        });
      }

      CKEDITOR.dialog.add('foundationAccordionDialog', CKEDITOR.plugins.getPath('foundationAccordion') + 'dialogs/foundationAccordion.js');
      CKEDITOR.document.appendStyleSheet(CKEDITOR.plugins.getPath('foundationAccordion') + 'css/style.css')
    }
  });

})(jQuery, Drupal, CKEDITOR);
