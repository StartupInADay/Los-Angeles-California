/**
 * @file
 * Drupal Foundation Accordion plugin dialogs.
 *
 * @ignore
 */

(function ($, Drupal, CKEDITOR) {

  var foundationAccordion_popup = 'toolbar=no,status=no,resizable=yes,dependent=yes,width=' + (screen.width * 0.7) + ',height=' + (screen.height * 0.7) + ',left=' + ((screen.width - (screen.width * 0.7)) / 2) + ',top=' + ((screen.height - (screen.height * 0.7)) / 2);
  var foundationAccordion_wrapper = foundationAccordion_deleteMessage = foundationAccordion_minimum = foundationAccordion_managePopupTitle = foundationAccordion_managePopupContent = '';

  function foundationAccordion_template() {
    return '<tr><td><input type="button" class="foundationAccordion_up" value="&and;" onclick="foundationAccordion_up(this)" /><input type="button" class="foundationAccordion_down" value="&or;" onclick="foundationAccordion_down(this)" /></td><td><input type="text" class="foundationAccordion_title" /></td><td><textarea class="foundationAccordion_content"' + (foundationAccordion_managePopupContent ? ' onclick="window.open(\'' + CKEDITOR.plugins.getPath('foundationAccordion') + 'ckeditor.html?edit=foundationAccordion_content&content=\'+foundationAccordion_index(this.parentNode.parentNode), \'Foundation Accordion\', \'' + foundationAccordion_popup + '\')"' : '') + '></textarea></td> 			<td><input type="button" class="foundationAccordion_remove" value="x" onclick="foundationAccordion_remove(this)" /></td> 		</tr> 		<tr> 			<td> 				<input type="button" class="foundationAccordion_up" value="&and;" onclick="foundationAccordion_up(this)" /> 				<input type="button" class="foundationAccordion_down" value="&or;" onclick="foundationAccordion_down(this)" /> 			</td> 			<td><input type="text" class="foundationAccordion_title" /></td> 			<td><textarea class="foundationAccordion_content"' + (foundationAccordion_managePopupContent ? ' onclick="window.open(\'' + CKEDITOR.plugins.getPath('foundationAccordion') + 'ckeditor.html?edit=foundationAccordion_content&content=\'+foundationAccordion_index(this.parentNode.parentNode), \'Foundation Accordion\', \'' + foundationAccordion_popup + '\')"' : '') + '></textarea></td> 			<td><input type="button" class="foundationAccordion_remove" value="x" onclick="foundationAccordion_remove(this)" /></td> 		</tr> 		<tr> 			<td> 				<input type="button" class="foundationAccordion_up" value="&and;" onclick="foundationAccordion_up(this)" /> 				<input type="button" class="foundationAccordion_down" value="&or;" onclick="foundationAccordion_down(this)" /> 			</td> 			<td><input type="text" class="foundationAccordion_title" /></td> 			<td><textarea class="foundationAccordion_content"' + (foundationAccordion_managePopupContent ? ' onclick="window.open(\'' + CKEDITOR.plugins.getPath('foundationAccordion') + 'ckeditor.html?edit=foundationAccordion_content&content=\'+foundationAccordion_index(this.parentNode.parentNode), \'Foundation Accordion\', \'' + foundationAccordion_popup + '\')"' : '') + '></textarea></td> 			<td><input type="button" class="foundationAccordion_remove" value="x" onclick="foundationAccordion_remove(this)" /></td> 		</tr> 	'
  }

  function foundationAccordion_up(foundationAccordion_element) {
    var foundationAccordion_elementIndex = foundationAccordion_index(foundationAccordion_element.parentNode.parentNode);

    if (foundationAccordion_elementIndex > 0) {
      var foundationAccordion_titleSwap1 = foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[foundationAccordion_elementIndex].children[1].firstChild.value;
      var foundationAccordion_titleSwap2 = foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[parseInt(foundationAccordion_elementIndex) - 1].children[1].firstChild.value;

      foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[parseInt(foundationAccordion_elementIndex) - 1].children[1].firstChild.value = foundationAccordion_titleSwap1;
      foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[foundationAccordion_elementIndex].children[1].firstChild.value = foundationAccordion_titleSwap2;

      var foundationAccordion_contentSwap1 = foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[foundationAccordion_elementIndex].children[2].firstChild.value;
      var foundationAccordion_contentSwap2 = foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[parseInt(foundationAccordion_elementIndex) - 1].children[2].firstChild.value;

      foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[parseInt(foundationAccordion_elementIndex) - 1].children[2].firstChild.value = foundationAccordion_contentSwap1;
      foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[foundationAccordion_elementIndex].children[2].firstChild.value = foundationAccordion_contentSwap2
    }

    return false;
  }

  function foundationAccordion_down(foundationAccordion_element) {
    var foundationAccordion_elementIndex = foundationAccordion_index(foundationAccordion_element.parentNode.parentNode);

    if (foundationAccordion_elementIndex < foundationAccordion_wrapper.getElementsByClassName('foundationAccordion_title').length - 1) {
      var foundationAccordion_titleSwap1 = foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[foundationAccordion_elementIndex].children[1].firstChild.value;
      var foundationAccordion_titleSwap2 = foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[parseInt(foundationAccordion_elementIndex) + 1].children[1].firstChild.value;

      foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[parseInt(foundationAccordion_elementIndex) + 1].children[1].firstChild.value = foundationAccordion_titleSwap1;
      foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[foundationAccordion_elementIndex].children[1].firstChild.value = foundationAccordion_titleSwap2;

      var foundationAccordion_contentSwap1 = foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[foundationAccordion_elementIndex].children[2].firstChild.value;
      var foundationAccordion_contentSwap2 = foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[parseInt(foundationAccordion_elementIndex) + 1].children[2].firstChild.value;

      foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[parseInt(foundationAccordion_elementIndex) + 1].children[2].firstChild.value = foundationAccordion_contentSwap1;
      foundationAccordion_wrapper.getElementsByTagName('tbody')[0].getElementsByTagName('tr')[foundationAccordion_elementIndex].children[2].firstChild.value = foundationAccordion_contentSwap2
    }

    return false;
  }

  function foundationAccordion_addNewItem() {
    var foundationAccordion_newItem = document.createElement('tr');

    foundationAccordion_newItem.innerHTML = '<td><input type="button" class="foundationAccordion_up" value="&and;" onclick="foundationAccordion_up(this)" /><input type="button" class="foundationAccordion_down" value="&or;" onclick="foundationAccordion_down(this)" /></td><td><input type="text" class="foundationAccordion_title" /></td><td><textarea class="foundationAccordion_content"' + (foundationAccordion_managePopupContent ? ' onclick="window.open(\'' + CKEDITOR.plugins.getPath('foundationAccordion') + 'ckeditor.html?edit=foundationAccordion_content&content=\'+foundationAccordion_index(this.parentNode.parentNode), \'Foundation Accordion\', \'' + foundationAccordion_popup + '\')"' : '') + '></textarea></td><td><input type="button" class="foundationAccordion_remove" value="x" onclick="foundationAccordion_remove(this)" /></td>';
    foundationAccordion_wrapper.getElementsByTagName('tbody')[0].appendChild(foundationAccordion_newItem);

    return false;
  }

  function foundationAccordion_remove(foundationAccordion_element) {
    if (foundationAccordion_wrapper.getElementsByClassName('foundationAccordion_title').length > 1) {
      if (confirm(foundationAccordion_deleteMessage)) {
        foundationAccordion_element.parentNode.parentNode.parentNode.removeChild(foundationAccordion_element.parentNode.parentNode)
      }
    }
    else {
      alert(foundationAccordion_minimum)
    }

    return false;
  }

  function foundationAccordion_in_array(foundationAccordion_needle, foundationAccordion_haystack) {
    for (var i in foundationAccordion_haystack) {
      if (foundationAccordion_haystack[i] == foundationAccordion_needle) return true
    }

    return false;
  }

  function foundationAccordion_index(node) {
    var children = node.parentNode.childNodes;
    var num = 0;

    for (var i = 0; i < children.length; i++) {
      if (children[i] == node) return num;
      if (children[i].nodeType == 1) num++
    }

    return -1
  }

  CKEDITOR.dialog.add('foundationAccordionDialog', function(editor) {
    return {
      title: editor.lang.foundationAccordion.plugin,
      minWidth: 600,
      minHeight: 400,
      resizable: false,
      contents: [{
        id: 'FoundationAccordionTab',
        label: 'FoundationAccordionTab',
        elements: [{
          type: 'html',
          html: '<div id="foundationAccordion"><div id="foundationAccordion_Container"><table><thead><tr><th></th><th>' + editor.lang.foundationAccordion.title + '</th><th>' + editor.lang.foundationAccordion.content + ' (' + editor.lang.foundationAccordion.htmlIsAllowed + ')</th><th></th></tr></thead><tbody></tbody></table></div><input type="button" value="+ ' + editor.lang.foundationAccordion.addNewItem + '" class="foundationAccordion_addNew" onclick="foundationAccordion_addNewItem()" /></div>',
          commit: function(widget) {
            var foundationAccordion_total = 0;
            var foundationAccordion_items = foundationAccordion_wrapper.getElementsByClassName('foundationAccordion_title');
            var foundationAccordion_contents = foundationAccordion_wrapper.getElementsByClassName('foundationAccordion_content');

            for (var foundationAccordion_i = 0; foundationAccordion_i <= foundationAccordion_items.length; foundationAccordion_i++) {
              if (foundationAccordion_items[foundationAccordion_i]) {
                if (foundationAccordion_i == 0) {
                  widget.setData('foundationAccordion_contentClass' + foundationAccordion_i, 'content active')
                }
                else {
                  widget.setData('foundationAccordion_contentClass' + foundationAccordion_i, 'content')
                }

                widget.setData('foundationAccordion_item' + foundationAccordion_i, foundationAccordion_items[foundationAccordion_i].value);
                widget.setData('foundationAccordion_content' + foundationAccordion_i, foundationAccordion_contents[foundationAccordion_i].value)
              }
              else {
                widget.setData('foundationAccordion_item' + foundationAccordion_i, '');
                widget.setData('foundationAccordion_content' + foundationAccordion_i, '')
              }
            }

            widget.setData('foundationAccordion_total', foundationAccordion_items.length)
          },
          setup: function(widget) {
            var selection = editor.getSelection();
            element = selection.getStartElement();

            var foundationAccordion_classes = element.$.children[0].className.split(' ');
            var foundationAccordion_item = '';

            if ((element.$.children[0].tagName).toLowerCase() == 'ul' && foundationAccordion_in_array('accordion', foundationAccordion_classes)) {
              for (var foundationAccordion_i = 0; foundationAccordion_i <= element.$.children[0].children.length; foundationAccordion_i++) {
                eval("foundationAccordion_title = widget.data.foundationAccordion_item" + foundationAccordion_i);
                foundationAccordion_title = foundationAccordion_title != undefined ? foundationAccordion_title : '';

                eval("foundationAccordion_content = widget.data.foundationAccordion_content" + foundationAccordion_i);
                foundationAccordion_content = foundationAccordion_content != undefined ? foundationAccordion_content : '';

                if (foundationAccordion_title) {
                  foundationAccordion_item += '<tr><td><input type="button" class="foundationAccordion_up" value="&and;" onclick="foundationAccordion_up(this)" /><input type="button" class="foundationAccordion_down" value="&or;" onclick="foundationAccordion_down(this)" /></td><td><input type="text" class="foundationAccordion_title" value="' + foundationAccordion_title + '" /></td><td><textarea class="foundationAccordion_content"' + (foundationAccordion_managePopupContent ? ' onclick="window.open(\'' + CKEDITOR.plugins.getPath('foundationAccordion') + 'ckeditor.html?edit=foundationAccordion_content&content=\'+foundationAccordion_index(this.parentNode.parentNode), \'Foundation Accordion\', \'' + foundationAccordion_popup + '\')"' : '') + '>' + foundationAccordion_content + '</textarea></td><td><input type="button" class="foundationAccordion_remove" value="x" onclick="foundationAccordion_remove(this)" /></td></tr>'
                }
              }

              foundationAccordion_wrapper.getElementsByTagName('tbody')[0].innerHTML = foundationAccordion_item
            }
            else {
              foundationAccordion_wrapper.getElementsByTagName('tbody')[0].innerHTML = foundationAccordion_template()
            }
          }
        }]
      }],

      onLoad: function() {
        for (var foundationAccordion_a = 0; foundationAccordion_a < CKEDITOR.dialog.getCurrent()._.element.$.getElementsByTagName('div').length; foundationAccordion_a++) {
          if (CKEDITOR.dialog.getCurrent()._.element.$.getElementsByTagName('div')[foundationAccordion_a].id == 'foundationAccordion_Container') {
            foundationAccordion_wrapper = CKEDITOR.dialog.getCurrent()._.element.$.getElementsByTagName('div')[foundationAccordion_a]
          }
        }

        foundationAccordion_managePopupTitle = editor.config.foundationAccordion_managePopupTitle;
        foundationAccordion_managePopupContent = editor.config.foundationAccordion_managePopupContent;
        foundationAccordion_deleteMessage = editor.lang.foundationAccordion.delete;
        foundationAccordion_minimum = editor.lang.foundationAccordion.minimum
      }
    }
  });

})(jQuery, Drupal, CKEDITOR);
