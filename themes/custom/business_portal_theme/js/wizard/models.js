  // Model constructor Screen.

var wiz = wiz ||  {};
wiz.models = {};

(function($) {

////////////
// Screen //
////////////

wiz.models.Screen = Backbone.Model.extend({
  defaults: {
    chosen: false,
    title: "",
    description: "",
    buttons: [],
    Color: "#EEEEEE",
    next: undefined,
    chosenResultText: "",
    selected: false
  },

  initialize: function() {
    this.setNextScreen();
  },

  controlSelected: function (bidString) {
    var storedBid = this.get("chosenBid");
    var bid = bidString.charAt(bidString.length -1);
    if (bid !== storedBid) {
      this.set({
        selected: true,
        storedBid: bid
      });
    }
    if (bid === storedBid) {
      this.set({selected: !this.get("selected")});
    }
  },

  // This is called when a button is clicked on.
  setNext: function(bidString) {
    var bid = bidString.charAt(bidString.length -1);
    if (this.get("buttons")[bid]["Destination Screen"] !== undefined) {
      var nid = this.get("buttons")[bid]["Destination Screen"]["target_id"];
    } else {
      //console.log("APP: Destination screen not defined: ", this.get("buttons"));
      return;
    }
    this.set({
      next: nid,
      chosenBid: bid,
    });
  },

  // This is called on model initialising.
  setNextScreen: function() {
    // Set the next screen, when we have that information available:
    switch (this.get("buttons").length) {
    case 1:
    case 2:
      if (this.get("buttons")[0]["Destination Screen"] !== undefined) {
      var nid = this.get("buttons")[0]["Destination Screen"]["target_id"];
      this.set({next: nid});
      }
      break;
    }
  },

});

})(jQuery);
