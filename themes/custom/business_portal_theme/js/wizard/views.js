var wiz = wiz || {};
wiz.views = {};
wiz.extensions = {};

// User moustachs style templates.
_.templateSettings = {
  interpolate: /\{\{(.+?)\}\}/g

};


(function($) {
  $( document ).ready(function() {

    ////////////////
    // Extensions //
    ////////////////

    wiz.extensions.View = Backbone.View.extend({

      render: function(options) {

        options = options || {};

        if (options.page === true) {
          this.$el.addClass('page');
        }

        return this;

      },

      transitionIn: function (callback) {

        var view = this, delay

        var transitionIn = function () {
          view.$el.addClass('is-visible');
          view.$el.one('transitionend', function () {
            if (_.isFunction(callback)) {
              callback();
            }
          })
        };

        _.delay(transitionIn, 20);

      },

      transitionOut: function (callback) {

        var view = this;

        view.$el.removeClass('is-visible');
        view.$el.one('transitionend', function () {
          if (_.isFunction(callback)) {
            callback();
          };
        });

      }

    });


/////////
// APP //
/////////

wiz.views.App = wiz.extensions.View.extend({
  el: "#wizard",

  goto: function (view) {

    var previous = this.currentPage || null;
    var next = view;

    if (previous) {
      previous.transitionOut(function () {
        previous.remove();
      });
    }

    next.render({ page: true });
    this.$el.html( next.$el );
    next.transitionIn();
    this.currentPage = next;
  }
});


    ////////////
    // Wizard //
    ////////////

    wiz.views.Wizard = wiz.extensions.View.extend({

      className: "wiz",

      render: function() {

        // Styles for all sections:
       $("body").css("background", "#" + this.model.get("Primary Color"));

        // Clear all the listening events we might have set up on backbone.
        Backbone.off();

       /*
        * Array of views that we have loaded, that will be removed each
        * time a new wizard is instantiated.
        */
        views = ["start",
                 "nav",
                 "header",
                 "intro",
                 "headerForQuestion",
                 "question",
                 "buttons",
                 "tip",
                 "linkButton",
                 "results",
                 "addressForm",
                 "bar",
                 "button",
                 "buttonLink",
                 "selectableButton",
                 "wizard",
                 "arrows",
                 "section",
                 "sectionSteps",
                 "sectionStepsItem",
                 "result",
                 "email",
                ];

        _.each(views, function(v) {
          if (wiz[v] !== undefined) {
            wiz[v].remove();
          }
        });


        switch (this.model.get("screen-type")) {

        case "start":
          //console.log("APP: Start");
          wiz.start = new wiz.views.Start({model: this.model});
          this.$el.append(wiz.start.render().el);

          wiz.nav = new wiz.views.Nav({model: this.model});
          this.$el.append(wiz.nav.render().el);
          break;

        case "section":
          //console.log("APP: Section");
          wiz.header = new wiz.views.Header({model: this.model});
          this.$el.append(wiz.header.render().el);

          wiz.intro = new wiz.views.IntroWithIllustration({ model: this.model });
          this.$el.append(wiz.intro.render().el);

          wiz.nav = new wiz.views.Nav({model: this.model});
          this.$el.append(wiz.nav.render().el);
          break;

        case "question":
          //console.log("APP: question");
          wiz.headerForQuestion = new wiz.views.HeaderForQuestion({model: this.model});
          this.$el.append(wiz.headerForQuestion.render().el);

          wiz.question = new wiz.views.Question({model: this.model});
          this.$el.append(wiz.question.render().el);

          wiz.buttons = new wiz.views.Buttons({ model: this.model });
          this.$el.append(wiz.buttons.render().el);

          wiz.tip = new wiz.views.Tip({model: this.model});
          this.$el.append(wiz.tip.render().el);

          wiz.linkButton = new wiz.views.ButtonLink({ model: this.model });
          this.$el.append(wiz.linkButton.render().el);

          wiz.nav = new wiz.views.Nav({model: this.model});
          this.$el.append(wiz.nav.render().el);
          break;

        case "contextual help":
          //console.log("APP: contextual help");
          wiz.header = new wiz.views.HeaderForContextual({model: this.model});
          this.$el.append(wiz.header.render().el);

          wiz.desc = new wiz.views.DescriptionForContextual({model: this.model});
          this.$el.append(wiz.desc.render().el);

           wiz.nav = new wiz.views.NavForContext({model: this.model});
          this.$el.append(wiz.nav.render().el);
          break;

        case "confirmation":
          //console.log("APP: confirmation");

          wiz.header = new wiz.views.HeaderForConfirm({model: this.model});
          this.$el.append(wiz.header.render().el);

          wiz.question = new wiz.views.Question({model: this.model});
          this.$el.append(wiz.question.render().el);

          wiz.intro = new wiz.views.IntroWithIllustration({ model: this.model });
          this.$el.append(wiz.intro.render().el);

          wiz.cta = new wiz.views.ResultsCTA({model: this.model});
          this.$el.append(wiz.cta.render().el);

          wiz.email = new wiz.views.Email({model: this.model});
          this.$el.append(wiz.email.render().el);

          wiz.results = new wiz.views.ResultsView({model: this.model});
          this.$el.append(wiz.results.render().el);

          wiz.nav = new wiz.views.NavStartOver({model: this.model});
          this.$el.append(wiz.nav.render().el);
          break;

        case "address lookup":
          //console.log("APP: address lookup");
          //console.log(this.model);

          wiz.headerForQuestion = new wiz.views.HeaderForQuestion({model: this.model});
          this.$el.append(wiz.headerForQuestion.render().el);

          wiz.question = new wiz.views.Question({model: this.model});
          this.$el.append(wiz.question.render().el);

          wiz.addressForm = new wiz.views.AddressForm({ model: this.model });
          this.$el.append(wiz.addressForm.render().el);

          wiz.nav = new wiz.views.NavForAddress({model: this.model});
          this.$el.append(wiz.nav.render().el);
          break;

        default:
          //console.log("APP: No screen type defined", this.model.get("screen-type") );
          break;
        }

        wiz.bar = new wiz.views.ProgressBar({model: this.model});
        if (this.model.get("screen-type") != "contextual help") {
          this.$el.append(wiz.bar.render().el);
        }

        return wiz.extensions.View.prototype.render.apply(this, arguments);
      }
    });

////////////
// Header //
////////////

wiz.views.Header = Backbone.View.extend({
  className: "wizard__header constrained",
  template: _.template($('#header-template').html()),
  render: function() {
    this.$el.html(this.template(this.model.toJSON()));
    return this;
  }
});

/////////////////////////
// Header for question //
/////////////////////////

wiz.views.HeaderForQuestion = Backbone.View.extend({
  className: "wizard__header constrained",
  template: _.template($('#header-for-question-template').html()),
  render: function() {
    this.$el.html(this.template(this.model.toJSON()));
    return this;
  }
});

/////////////////////////
// Header for context  //
/////////////////////////

wiz.views.HeaderForContextual = Backbone.View.extend({
  className: "wizard__header wizard__header-contextual",
  template: _.template($('#header-for-contextual-template').html()),
  render: function() {
    this.$el.html(this.template(this.model.toJSON()));
    return this;
  }
});

//////////////////////////////
// Description for context  //
//////////////////////////////

wiz.views.DescriptionForContextual = Backbone.View.extend({
  template: _.template($('#description-for-contextual-template').html()),
  render: function() {
    this.$el.html(this.template(this.model.toJSON()));
    return this;
  }
});

/////////////////////////
// Header for confirm //
/////////////////////////

wiz.views.HeaderForConfirm = Backbone.View.extend({
  className: "wizard__header constrained",
  template: _.template($('#header-for-confirm-template').html()),
  render: function() {
    this.$el.html(this.template(this.model.toJSON()));
    return this;
  }
});

///////////
// Intro //
///////////

wiz.views.Intro = Backbone.View.extend({
  className: "wizard__intro-block constrained",
  template: _.template($('#intro-template').html()),
  render: function() {
    this.$el.html(this.template(this.model.toJSON()));
    return this;
  }
});

///////////
// Start //
///////////

wiz.views.Start = Backbone.View.extend({
  className: "wizard__intro-block constrained",
  template: _.template($('#start-template').html()),
  initialize: function() {

  },
  render: function() {
    this.$el.html(this.template(this.model.toJSON()));
    return this;
  }
});

///////////
// Email //
///////////


wiz.views.Email = Backbone.View.extend({
  template: _.template($('#wizard-results-email-template').html()),
  events:  {
    "click #wizard-email-placeholder": "emailSubmit",
    "submit" : "onSubmit"
  },
  onSubmit : function(e) {
    e.preventDefault();
    var email = 'email=' + $( "input[name='emailResults']" ).val();
    var emailValidate = '&emailvalidate=' + $( "input[name='emailCheck']" ).val();
    var emailToken = '&emailtoken=' + $( "input[name='form_token']" ).val();
    var message = '';
    //var message = $(".wizard__content--results-list").html();
    var wizard_steps = $(".wizard__content--results-list").children('div').each(function(ind) {
      var stepcontent =  $(this).html();
      //console.log('SECTION');
      //console.log(stepcontent);
      message = message + encodeURIComponent('<div>'+stepcontent+'</div>');
    });


    //message = encodeURIComponent(message);
    //console.log('MESSAGE');
    //console.log(message);
    var strip = ['%5Cn', '%5C%22'];
    var replacement = ['', '%22'];
    for (var ind = 0; ind < strip.length; ind++) {
      message = message.split(strip[ind]).join(replacement[ind]);
    }
    //message = message.replace(/(\r\n|\n|\r)/gm, '');

    //console.log(message);
    message = '&message=' + message;
    $.ajax({
        url: '/labp/wizard-email',
        dataType: 'text',
        type: 'post',
        contentType: 'application/x-www-form-urlencoded',
        data: email + emailValidate + emailToken + message,
        success: function( data, textStatus, jQxhr ){
          //console.log(data);
          $("#wizard-email").addClass("element-invisible");
          $( "#message-response" ).html( "Your message has been sent" );
        },
        error: function( jqXhr, textStatus, errorThrown ){
          //console.log( errorThrown );
          $("#wizard-email").addClass("element-invisible");
          $( "#message-response" ).html( "There was an error sending your message. If you continue to experience problems, please contact the site administrator." );
        }
    });

  },
  render: function() {
    this.$el.html(this.template());
    return this;
  }
});

/////////////////////////////
// Intro with Illustration //
/////////////////////////////

wiz.views.IntroWithIllustration = Backbone.View.extend({
  className: "wizard__intro-block constrained",
  template: _.template($('#intro-with-illustration-template').html()),
  render: function() {
    this.$el.html(this.template(this.model.toJSON()));
    return this;
  }
});

//////////////
// Question //
//////////////

wiz.views.Question = Backbone.View.extend({
  template: _.template($('#question-template').html()),
  render: function() {
    this.$el.html(this.template(this.model.toJSON()));
    return this;
  }
});


/////////////
// Buttons //
/////////////

wiz.views.Buttons = Backbone.View.extend({
  className: "wizard__buttons constrained",
  template: _.template($('#buttons-template').html()),

  events: {
    "click .wizard__button": "markSelected"
  },

  markSelected: function(e) {
    e.preventDefault();
    var bidString =  $(e.currentTarget).attr("id");
    this.model.controlSelected(bidString);
    if (this.model.get("selected")) {
      $(".wizard__button").removeClass("wizard__button--selected");
      $(e.currentTarget).addClass("wizard__button--selected");
      Backbone.trigger("button:selected");
    } else {
      $(".wizard__button").removeClass("wizard__button--selected");
      Backbone.trigger("button:deselected");
    }
    var m = wiz.collections.chosen.last();
    m.setNext(bidString);
  },

  render: function() {
    var buttons = this.model.get("buttons");
      if (buttons.length > 0) {
        _.each(buttons, function(b, index) {
          if (b.Style["#markup"] === "Button") {
            wiz.button =  new wiz.views.SelectableButton({
              button: b,
              model: this.model,
              index: index
            });
            this.$el.append(wiz.button.render().el);
          }
        }, this);
      }
    return this;
  }
});

/////////////////
// Button Link //
/////////////////

wiz.views.ButtonLink = Backbone.View.extend({
  className: "wizard__buttons constrained",
  template: _.template($('#buttons-template').html()),
  events: {"click .wizard__tip_button": "goToScreen" },

  goToScreen: function(event) {
    var bidString =  $(event.currentTarget).attr("id");
    this.model.setNext(bidString);
    var nm = wiz.collections.screens.find({
      Nid: this.model.get("next")
    });
    wiz.collections.chosen.add(nm);
    event.preventDefault();
  },

  render: function() {
    var buttons = this.model.get("buttons");
      if (buttons.length > 0) {
        _.each(buttons, function(b, index) {
          if (b.Style["#markup"] === "Link") {
            wiz.buttonLink =  new wiz.views.Button({
              button: b,
              model: this.model,
              index: index,
              className: "wizard__tip_button"
            });
            this.$el.append(wiz.buttonLink.render().el);
          }
        }, this);
      }
    return this;
  }
});



///////////////////////
// Selectable Button //
///////////////////////

wiz.views.SelectableButton = Backbone.View.extend({

  className: "wizard__button",
  tagName: "a",

  initialize: function(options) {
    this.options = options || {};
    this.button = this.options.button;
    this.index = this.options.index;
  },

  render: function() {
    this.$el.attr("href", "#");
    this.$el.attr("id", "button-id-" + this.index);
    this.$el.text(this.button["Button Title"]["#markup"]);
    this.updateSelectedClass();
    return this;
  },

  updateSelectedClass: function() {
    if (this.model.get("selected")) {
      if (this.$el.attr("id") === "button-id-" + this.model.get("chosenBid")) {
        this.$el.addClass("wizard__button--selected");
      } else {
        this.$el.removeClass("wizard__button--selected");
      }
    }
  },

});


//////////////
// A Button //
//////////////

wiz.views.Button = Backbone.View.extend({

  tagName: "a",

  initialize: function(options) {
    this.options = options || {};
    this.button = this.options.button;
    this.index = this.options.index;
  },

  render: function() {
    this.$el.attr("href", "#");
    this.$el.attr("id", "button-id-" + this.index);
    this.$el.text(this.button["Button Title"]["#markup"]);
    return this;
  },

});


/////////
// TIP //
/////////

wiz.views.Tip = Backbone.View.extend({
  className: "wizard__tip",
  template: _.template($('#tip-template').html()),
  render: function() {
    if(parseInt(this.model.toJSON().tip.length) > 0) {
        this.$el.html(this.template(this.model.toJSON()));
    }
    return this;
  }
});



/////////
// Nav //
/////////

wiz.views.Nav = Backbone.View.extend({

  className: "wizard__nav",

  initialize: function() {
    Backbone.on("button:selected", this.forwardEnabled, this);
    Backbone.on("button:deselected", this.forwardDisabled, this);
  },

  forwardEnabled: function() {
    this.model.set({"selected": true});
    this.$el.find(".wizard__arrow-down").addClass("enabled");
  },

  forwardDisabled: function() {
   this.$el.find(".wizard__arrow-down").removeClass("enabled");
  },

  events:  {
    "click .wizard__arrow-up": "backArrowClick",
    "click .wizard__arrow-down": "forwardArrowClick"
  },

  backArrowClick: function() {

    if (wiz.collections.chosen.length > 1) {
      var last = wiz.collections.chosen.last();
      wiz.collections.chosen.remove(last);

      wiz.wizard = new wiz.views.Wizard({
        model:  wiz.collections.chosen.last()
      });
      wiz.instance.goto(wiz.wizard);
    }

    event.preventDefault();
  },

  forwardArrowClick: function(event) {
    var m =  wiz.collections.screens.find({
      "Nid": this.model.get("next")
    });
    wiz.collections.chosen.add(m);
    event.preventDefault();
  },

  render: function() {
    switch (this.model.get("screen-type")) {
    case "start":
      wiz.arrows = new wiz.views.NavStart();
      this.$el.append(wiz.arrows.render().el);
      break;
    case "section":
      wiz.arrows = new wiz.views.NavSection();
      this.$el.append(wiz.arrows.render().el);
      break;
    case "question":
      wiz.arrows = new wiz.views.NavQuestion();
      this.$el.append(wiz.arrows.render().el);
      break;
    case "contextual help":
      wiz.arrows = new wiz.views.NavContextualHelp();
      this.$el.append(wiz.arrows.render().el);
      break;
    }

    if (this.model.get("selected")) {
      this.forwardEnabled();
    }
    return this;
  }

});


///////////////
// NAV START //
///////////////

wiz.views.NavStart = Backbone.View.extend({
  template: _.template($('#wizard-nav-start-template').html()),
  render: function() {
    this.$el.html(this.template());
    return this;
  }
});

/////////////////
// ADDRESS FORM //
/////////////////
wiz.views.AddressForm = Backbone.View.extend({
  template: _.template($('#wizard-address-form-template').html()),
  events:  {
    "click #form-address-lookup": "addressSubmit"
  },

  addressSubmit: function(event) {
    var that = this;
    event.preventDefault();
    var address = $( "input[name='streetAddress']" ).val();
      if (address === '') {
        alert('Please provide an address or intersection.');
        $("#streetAddress").focus();
        return false;
      }
      // Check the address to make sure it follows the rules for addresses or intersections
      // Setup the regular expression.
      addressRegex = /^(\d{1,6}\s+\w{1,30}.{0,10})|(\w{1,30}\s{0,5}(\b[Aa][Nn][Dd]\b|\b[Aa][Tt]\b|\/|&|\\|\|)\s{0,5}\w{1,30})/;
      // Trim whitespace.
      trimmedAddress = address.replace(/^\s+|\s+$/g, '') ;
      // Strip out fractional addresses such as 1/2 and 1/4, etc.
      trimmedAddress = trimmedAddress.replace(/\b\d{1}\/\d{1}\b/g,'');
      // Strip out all characters other than digits, alpha, or intersection delimiters
      // The acceptable delimiters are: / \ | AND AT and &
      trimmedAddress = trimmedAddress.replace(/[^\w\s&\|\/\\]/g,'');
      // Strip out zip codes and state codes - these are not needed and can cause mismatches.
      trimmedAddress = trimmedAddress.replace(/(\d{5}$|\b[Cc][Aa]\b)/g,'');
      $("#streetAddress").val(trimmedAddress); // Update address field with formatted address.
      if (trimmedAddress.match(addressRegex) === null) {
        alert('Please provide a house number and street name or an intersection.');
        $("#streetAddress").focus();
        return false;
      }
      // Now check the address to see if they mixed an address with an intersection which is invalid.
      if (trimmedAddress.match(/^(\d{1,6}\s+\w{1,30}.{0,10})/) !== null) {
      // Check for intersection delimiters.
        if (trimmedAddress.match(/(\b[Aa][Nn][Dd]\b|\b[Aa][Tt]\b|\/|&|\\|\|)/) !==null) {
          alert('Please enter either an address or an intersection, not both.');
          $("#streetAddress").focus();
          return false;
        }
      }

      var laAddressAPI = "/labp/address-lookup/" + address;
      $.getJSON( laAddressAPI, function( json ) {
        if(json.inLA == '1'){
          that.hasAddressRender();
        }
        if(json.inLA == '0'){
          that.noAddressRender();
        }
      });
  },

  hasAddressRender: function() {
    var text = Drupal.t("<p>Your business is in the city of Los Angeles.</p><br />");
    this.$el.find(".address-result").html(text);
    Backbone.trigger("button:selected");
  },

  noAddressRender: function() {
    var text = Drupal.t("<p>Your business is not in the city of Los Angeles.</p><br/><p>You can continue to get your guide to register as a business with the County, State and Federal government. Be sure to check with your local municipality to complete your business registration there.</p><br />");
    this.$el.find(".address-result").html(text);
    Backbone.trigger("button:selected");
  },

  render: function() {
    this.$el.html(this.template());
    return this;
  }
});

/////////////////
// NAV ADDRESS //
/////////////////

wiz.views.NavForAddress = Backbone.View.extend({
  template: _.template($('#wizard-nav-address-template').html()),

  initialize: function() {
    Backbone.on("button:selected", this.forwardEnabled, this);
    Backbone.on("button:deselected", this.forwardDisabled, this);
  },

  forwardEnabled: function() {
    this.model.set({selected: true});
    this.$el.find(".wizard__arrow-down").addClass("enabled");
  },

  forwardDisabled: function() {
    this.model.set({selected: true});
    this.$el.find(".wizard__arrow-down").removeClass("enabled");
  },

  events:  {
    "click .wizard__address_back_button": "backArrowClick",
    "click .wizard__arrow-up": "backArrowClick",
    "click .wizard__arrow-down": "forwardArrowClick"
  },

  backArrowClick: function() {
    if (wiz.collections.chosen.length > 1) {
      var last = wiz.collections.chosen.last();
      wiz.collections.chosen.remove(last);

      wiz.wizard = new wiz.views.Wizard({
        model:  wiz.collections.chosen.last()
      });
      wiz.instance.goto(wiz.wizard);
    }
    event.preventDefault();
  },

  forwardArrowClick: function(event) {
    var m =  wiz.collections.screens.find({
      "Nid": this.getNextScreen()
    });
    wiz.collections.chosen.add(m);
    event.preventDefault();
  },

  getNextScreen: function() {
    var bid = 0, nid = undefined; // next screen button id. could check for Yes text. this method could live in the model for DRY purposes.
    if (this.model.get("buttons")[bid]["Destination Screen"] !== undefined) {
      nid = this.model.get("buttons")[bid]["Destination Screen"]["target_id"];
    } else {
      //console.log("APP: Destination screen not defined: ", m.get("buttons"));
      return;
    }
    return nid;
  },
  render: function() {
    this.$el.html(this.template());
    return this;
  }
});

/////////////////
// NAV CONTEXT //
/////////////////

wiz.views.NavForContext = Backbone.View.extend({
  className: "wizard__nav_context",
  template: _.template($('#wizard-nav-contextual-help-template').html()),
  events:  {
    "click .wizard__arrow-back": "backArrowClick"
  },
  backArrowClick: function() {

    if (wiz.collections.chosen.length > 1) {
      var last = wiz.collections.chosen.last();
      wiz.collections.chosen.remove(last);

      wiz.wizard = new wiz.views.Wizard({
        model:  wiz.collections.chosen.last()
      });
      wiz.instance.goto(wiz.wizard);
    }

    event.preventDefault();
  },
  render: function() {
    this.$el.html(this.template());
    return this;
  }
});

/////////////////
// NAV START   //
/////////////////

wiz.views.NavStart = Backbone.View.extend({
  template: _.template($('#wizard-nav-start-template').html()),
  render: function() {
    this.$el.html(this.template());
    return this;
  }
});

/////////////////
// NAV SECTION //
/////////////////

wiz.views.NavSection = Backbone.View.extend({
  template: _.template($('#wizard-nav-section-template').html()),
  render: function() {
    this.$el.html(this.template());
    return this;
  }
});


//////////////////
// NAV Question //
//////////////////

wiz.views.NavQuestion = Backbone.View.extend({
  template: _.template($('#wizard-nav-question-template').html()),
  render: function() {
    this.$el.html(this.template());
    return this;
  }
});

/////////////////////////
// Nav Contextual Help //
/////////////////////////

wiz.views.NavContextualHelp = Backbone.View.extend({
  template: _.template($('#wizard-nav-contextual-help-template').html()),
  render: function() {
    this.$el.html(this.template());
    return this;
  }
});


////////////////////
// Nav Start Over //
////////////////////

wiz.views.NavStartOver = Backbone.View.extend({
  className: "wizard__content--results-cta",
  events:  {
    "click .wizard__arrow-up": "startOverClick"
  },
  startOverClick: function() {

    /*if (wiz.collections.chosen.length > 1) {
      var first = wiz.collections.chosen.first();
      wiz.collections.chosen.remove(first);

      wiz.wizard = new wiz.views.Wizard({
        model:  wiz.collections.chosen.first()
      });
      wiz.instance.goto(wiz.wizard);
    }*/
    window.location.href = "/start";

    event.preventDefault();
  },
  template: _.template($('#wizard-nav-start-over-template').html()),
  render: function() {
    this.$el.html(this.template());
    return this;
  }
});

//////////////////////////
// // Results CTA /////
//////////////////////////

wiz.views.ResultsCTA = Backbone.View.extend({
  className: "wizard__content--results-cta",
  events: {
    "click .wizard__button.print": "callPrint",
    "click .wizard__button.email": "callEmail"
  },
  template: _.template($('#wizard-results-cta-template').html()),
  render: function() {
    this.$el.html(this.template());
    return this;
  },
  callPrint: function() {
    window.print();
  },
  callEmail: function() {
    $("#wizard-email").removeClass("element-invisible");
  }
});

////////////////////////
// // Results View /////
////////////////////////

wiz.views.ResultsView = Backbone.View.extend({
  className: "wizard__content--results-list",
  render: function() {
    var nodes = [];
    var results = [];
    var unsorted_results = [];
    unsorted_results = wiz.collections.chosen.getResults();
    var results = _.sortBy(unsorted_results, function(res){
      if (res["Button Result Order"] !== undefined) {
        return res["Button Result Order"]["#markup"];
      }
    })
    _.each(results, function(r, index) {
      //console.log(r);
      var resulttext = r["Button Result Text"];
      if (resulttext !== undefined) {
        resulttext = resulttext["#markup"];
      }
      var link1 = r["CTA Link 1"];
      var link2 = r["CTA Link 2"];
      var link3 = r["CTA Link 3"];
      var buttontext = '';
      if (link1 !== undefined) {
        buttontext = '<a target="_blank" href="' + link1['#element']['url'] + '">' + link1['#element']['title'] + '</a>';
      }
      if (link2 !== undefined) {
        buttontext = buttontext + '<a target="_blank" href="' + link2['#element']['url'] + '">' + link2['#element']['title'] + '</a>';
      }
      if (link3 !== undefined) {
        buttontext = buttontext + '<a target="_blank" href="' + link3['#element']['url'] + '">' + link3['#element']['title'] + '</a>';
      }

      if (resulttext !== undefined) {
        wiz.result = new wiz.views.Result({result: resulttext, index: index, buttons: buttontext});
      }
      this.$el.append(wiz.result.render().el);
    }, this);
    return this;
  }
});

////////////
// Result //
////////////

wiz.views.Result = Backbone.View.extend({

  tagName: "div",
  //className: "wizard__step",
  template: _.template($('#results-template').html()),

  initialize: function(options) {
    this.result = options.result;
    this.index = options.index + 1;
    this.buttons = options.buttons;
  },

  render: function() {
    this.$el.append(this.template({result: this.result, index: this.index, buttons: this.buttons}));
    return this;
  }

});


//////////////////
// Progress Bar //
//////////////////

wiz.views.ProgressBar = Backbone.View.extend({

  template: _.template($('#progress-bar-section-template').html()),
  templateWithIcon: _.template($('#progress-bar-section-with-icon-template').html()),
  className: "wizard__progress-bar--simple",

  render: function() {
    var currentSection = this.model.get("section").tid;
    var sections = wiz.collections.screens.getSections();
    _.each(sections, function(section) {
      if (currentSection === section.tid) {
        this.$el.append(this.templateWithIcon({icon: section.icon}));
      } else {
        this.$el.append(this.template());
      }
    }, this);
    return this;
  }

});



/////////////////////
// Progress Drawer //
/////////////////////

wiz.views.ProgressDrawer = Backbone.View.extend({

  className: "wizard__progress-drawer",

  initialize: function() {
    Backbone.on("screen:add", this.render, this);
  },

  render: function() {
    this.$el.find("ul").remove();
    this.$el.find("li").remove();
    this.$el.css("background", "#" + this.model.get("drawer-color"));

    var progress = wiz.collections.sections.models;
    _.each(progress, function(section) {
      wiz.section = new wiz.views.Section({model: section}).render().el;
      this.$el.append(wiz.section);
    }, this);

    return this;
  }

});


wiz.views.Section = Backbone.View.extend({

  tagName: "li",

  template: _.template("<h5>{{name}}</h5>"),

  render: function() {
    this.$el.append(this.template({name: this.model.get("title")}));
    wiz.sectionSteps = new wiz.views.SectionSteps({
      sectionId: this.model.get("id"),
      model: this.model
    }).render().el;
    this.$el.append(wiz.sectionSteps);
    return this;
  }
});




wiz.views.SectionSteps = Backbone.View.extend({
  tagName: "ul",

  initialize: function(options) {
    this.options = options || {};
    this.sectionId = this.options.sectionId;
  },

  render:function() {

    // @TODO move this method into the collection.
    // @TODO Sort screen by section ID.
    var screens = _.filter(wiz.collections.screens.models, function(s){
      return s.attributes.section.tid === this.sectionId;
    }, this);

    var graphic;
    _.each(screens, function(s) {
      if (s.get("Nid") === this.model.get("Nid")) {
        graphic = " O ";
      } else {
        graphic = " | ";
      }
      wiz.sectionStepItem = new wiz.views.SectionStepItem({graphic: graphic}).render().el;
      this.$el.append(wiz.sectionStepItem);
    }, this );
    return this;
  }
});



wiz.views.SectionStepItem = Backbone.View.extend({
  tagName: "li",

  initialize: function(options) {
    this.options = options || {};
    this.graphic = this.options.graphic;
  },

  render: function() {
    this.$el.html(this.graphic);
    return this;
  }
});

  });

})(jQuery);
