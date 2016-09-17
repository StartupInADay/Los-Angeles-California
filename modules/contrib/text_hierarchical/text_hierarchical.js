(function($) {
  Drupal.tableDrag.prototype.row.prototype.findSiblings = function(rowSettings) {
    var siblings = []
      , directions = [ 'prev', 'next' ];
    for (var d = 0; d < directions.length; ++d) {
      var checkRow = $(this.element)[directions[d]]();
      while (checkRow.length) {
        if ($('.' + rowSettings.target, checkRow)) {
          siblings.push(checkRow[0]);
        }
        else {
          break;
        }
        checkRow = $(checkRow)[directions[d]]();
      }
      if ('prev' == directions[d]) {
        siblings.reverse();
        siblings.push(this.element);
      }
    }
    return siblings;
  };
})(jQuery);
