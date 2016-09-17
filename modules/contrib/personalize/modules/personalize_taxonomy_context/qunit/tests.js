QUnit.module("Personalize Taxonomy Context", {
  setup: function() {
    Drupal.settings.personalize_taxonomy_context = {
      vocabularies: {
        country: "Canada",
        people: "Canadian",
        subject: "Beaver,Hockey,Toronto"
      }
    }
  }
});

QUnit.test("Retrieve taxonomy by getContext", function( assert ) {
  expect(3);
  var actualResults = {};
  var enabledList = {
    empty: {},
    partial: {
      subject: "subject"
    },
    over: {
      country: "country",
      subject: "subject",
      gender: "gender"
    }
  };
  var expectedResults = {
    empty: {},
    partial: {
      subject: "Beaver,Hockey,Toronto"
    },
    over: {
      country: "Canada",
      subject: "Beaver,Hockey,Toronto"
    }
  };
  actualResults.empty = Drupal.personalize.visitor_context.taxonomy_context.getContext(enabledList.empty);
  actualResults.partial = Drupal.personalize.visitor_context.taxonomy_context.getContext(enabledList.partial);
  actualResults.over = Drupal.personalize.visitor_context.taxonomy_context.getContext(enabledList.over);
  assert.deepEqual(actualResults.empty, expectedResults.empty, 'Empty context results in no terms');
  assert.deepEqual(actualResults.partial, expectedResults.partial, 'Correctly cherry-pick terms');
  assert.deepEqual(actualResults.over, expectedResults.over, 'Correctly handle when a vocabulary is asked of, but not defined');
});
