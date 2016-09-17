(function ($) {
    Drupal.behaviors.emberSupport = {
        attach: function (context, settings) {
            // Every time the window is re-sized we should check if the tablesaw stack is visible
            $(window).resize(function(){
                refreshWorkbenchTable();
            });

            // If a user loads the page at an initially small size, we need to add our fixes in
            $(window).load(function(){
                refreshWorkbenchTable();
            });
        }
    };
    function refreshWorkbenchTable() {
        // Check to see if the workbench tablesaw is stacked
        var table = $('.view-workbench-moderation .tablesaw tbody');
        // Do a quick check to see if the tablesaw is stacked
        if (table.find('.tablesaw-cell-label').is(':visible')){
            // If we haven't already processed the table, do so now
            if (!table.hasClass('ember-support-workbench-processed')) {
                table.find('tr').each(function() {
                    var row = $(this);
                    // Clone the title and prepend it to the row
                    var title = row.find('.views-field-title');
                    var clone = title.children('.tablesaw-cell-content').clone().addClass('ember-support-workbench-clone').wrapInner('<h3>');
                    title.closest('tr').before(clone);
                    title.hide();

                    // Clone the workbench tasks and append them to the row
                    var actions = row.find('.views-field-moderation-actions');
                    clone = actions.clone().addClass('ember-support-workbench-clone');
                    clone.children('.tablesaw-cell-label').css('width', '100%');
                    clone.find('a').addClass('button');
                    actions.closest('tr').append(clone);
                    actions.hide();
                });

                // Mark the table as processed
                table.addClass('ember-support-workbench-processed');
            }
        }
        // If we're moving out of tablesaw range, un-do our processing
        else if (table.hasClass('ember-support-workbench-processed')) {
            table.find('.ember-support-workbench-clone').remove();
            table.find('td').css('display', 'table-cell');
            table.removeClass('ember-support-workbench-processed');
        }
    }
})(jQuery);
