(function($) {
  Drupal.behaviors.entityreferenceViewWidget = {
    attach: function(context, settings) {
      var checkboxes = '#modal-content input.entity-reference-view-widget-select';
      var selectAllSelector = '#entityreference-view-widget-select-all';
      $(selectAllSelector).unbind('click').data('unselect', 0).click(function() {
        if ($(this).data('unselect')) {
          $(checkboxes).removeAttr('checked');
          $(this).data('unselect', 0).text(Drupal.t('Select all'));
        }
        else {
          $(checkboxes).attr('checked', 'checked');
          $(this).data('unselect', 1).text(Drupal.t('Unselect all'));
        }
        return false;
      });

      if (settings.entityReferenceViewWidget) {
        var ervwSetttings = settings.entityReferenceViewWidget.settings;
        if (ervwSetttings.cardinality != -1 || $(checkboxes).length === 0) {
          $(selectAllSelector).remove();
        }

        var selector = '#' + ervwSetttings.table_id + ' input[type=checkbox]:checked';
        var selected_ids = '';
        $(selector).each(function() {
          selected_ids += $(this).val() + ';';
        });
        if (selected_ids.length > 0) {
          $('input[name="selected_entity_ids"]').val(selected_ids.substring(0, selected_ids.length - 1)).trigger('change');
        }

        // We need to pass the settings via an hidden field because Views doesn't
        // allow us to pass data between ajax requests.
        if (settings.entityReferenceViewWidget.serialized) {
          $('input[name="ervw_settings"]').val(settings.entityReferenceViewWidget.serialized);
        }
      }
    }
  }

  // Create a new ajax command, ervw_draggable that is called to make the rows
  // of the widget draggable.
  Drupal.ajax.prototype.commands.ervw_draggable = function(ajax, response, status) {
    $('#' + response.selector + ' tr').each(function(){
      var el = $(this);
      Drupal.tableDrag[response.selector].makeDraggable(el.get(0));
      el.find('td:last').addClass('tabledrag-hide');
      if ($.cookie('Drupal.tableDrag.showWeight') == 1) {
        el.find('.tabledrag-handle').hide();
      }
      else {
        el.find('td:last').hide();
      }
    });
  };
})(jQuery);
