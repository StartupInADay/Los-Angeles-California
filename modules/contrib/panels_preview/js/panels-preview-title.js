(function ($) {

 Drupal.behaviors.panelsPreviewTitle = {
   attach: function (context, settings) {

     /**
      * Replace the markup of a node title pane with
      * the h1.title page element
      */
     if ($.trim($('.pane-node-title .pane-content').html()) == $.trim($('h1.title').html())) {
       $('.pane-node-title .pane-content').html('');
       $('h1.title').hide().clone().prependTo('.pane-node-title .pane-content');
       $('.pane-node-title h1.title').show();
     }

   }
 }

})(jQuery);
