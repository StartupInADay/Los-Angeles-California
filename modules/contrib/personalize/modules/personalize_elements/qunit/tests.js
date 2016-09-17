var $ = jQuery;
QUnit.module("Personalize Elements", {
  // Set up a base agent and the personalize element types.
  setup: function() {
    Drupal.settings.personalize = {
      'cacheExpiration': {
        'decisions': 'session'
      },
      'agent_map': {
        'my-agent': {
          'active': 1,
          'cache_decisions': true,
          'enabled_contexts': [],
          'type': 'test_agent'
        }
      }
    };
    Drupal.settings.personalize_elements.elements = {};
    Drupal.settings.personalize.option_sets = {};
  },
  teardown: function() {
    Drupal.personalize.resetAll();
    sessionStorage.clear();
    delete Drupal.attachBehaviors;
  }
});

QUnit.test("Personalize page control text", function(assert) {
  addOptionSet('osid-1', '#personalize-option-set-edit-text p', 'editText', [{option_id: 'variation-1', option_label: 'Variation #1', personalize_elements_content: 'The Rainbow Connection'}], 'variation-1');
  addOptionSet('osid-2', '#personalize-option-set-edit-html', 'editHtml', [{option_id: 'variation-1', option_label: 'Variation #1', personalize_elements_content: '<div id="when">Someday we\'ll find it</div>'}], 'variation-1');
  addOptionSet('osid-3', '#personalize-option-set-replace-html', 'replaceHtml', [{option_id: 'variation-1', option_label: 'Variation #1', personalize_elements_content: '<div id="who">The lovers the dreamers and me</div>'}], 'variation-1');
  // Preselect variation 1s
  Drupal.settings.personalize.preselected = Drupal.settings.personalize.preselected || {};
  Drupal.settings.personalize.preselected['osid-1'] = 'variation-1';
  Drupal.settings.personalize.preselected['osid-2'] = 'variation-1';
  Drupal.settings.personalize.preselected['osid-3'] = 'variation-1';

  Drupal.personalize.personalizePage(Drupal.settings);
  // Check personalizations correctly made.
  assert.ok($('#personalize-option-set-edit-text p').data('personalize') == 'osid-1', 'Personalized element was assigned a data identifier.');
  assert.equal($('#personalize-option-set-edit-text p').text(), 'The Rainbow Connection', 'Text was replaced.');

  assert.equal($('#personalize-option-set-edit-html').length, 0, 'Original unpersonalized HTML has been removed.');
  assert.ok($('#when').data('personalize') == 'osid-2', 'Personalized element was assigned a data identifier.');
  assert.equal($('#when').html(), "Someday we'll find it", 'HTML was replaced.');

  assert.ok($('#personalize-option-set-replace-html').data('personalize') == 'osid-3', 'Personalized element was assigned a data identifier.');
  assert.equal($('#personalize-option-set-replace-html').html(), '<div id="who">The lovers the dreamers and me</div>', 'HTML was set within replacement container.');

  var expectedControl = {};
  expectedControl.editText = 'Unpersonalized content.';
  expectedControl.editHtml = "<p>Unpersonalized content.</p>";
  expectedControl.replaceHtml = '<p>Unpersonalized content.</p>';
  assert.equal(Drupal.personalizeElements.editText.controlContent['osid-1'], expectedControl.editText, 'Control value was saved for edit text operation');
  assert.equal($(Drupal.personalizeElements.editHtml.controlContent['osid-2']).html().trim(), expectedControl.editHtml, 'Control value was saved for edit html operation');
  assert.equal(Drupal.personalizeElements.replaceHtml.controlContent['osid-3'].trim(), expectedControl.replaceHtml, 'Control value was saved for replace html operation');
});

QUnit.test("Personalize page editText", function( assert ) {
  expect(2);

  Drupal.attachBehaviors = function (context, settings) {
    assert.ok(false, 'Attach behaviors was called.');
  }

  addOptionSet('osid-1', '#personalize-option-set-edit-text p', 'editText', [{option_id: 'variation-1', option_label: 'Variation #1', personalize_elements_content: 'The Rainbow Connection'}], 'variation-1');
  Drupal.personalize.personalizePage(Drupal.settings);
  assert.ok($('#personalize-option-set-edit-text p').data('personalize') == 'osid-1', 'Personalized element was assigned a data identifier.');
  assert.equal($('#personalize-option-set-edit-text p').text(), 'The Rainbow Connection', 'Text was replaced.');
});

QUnit.test("Personalize page editHtml", function( assert ) {
  expect(6);
  Drupal.attachBehaviors = function (context, settings) {
    assert.ok(true, 'Attach behaviors was called.');
    assert.equal(context.attr('id'), 'someday', 'Context passed to attachBehaviors is the new HTML.');
  }

  addOptionSet('osid-1', '#personalize-option-set-edit-html', 'editHtml', [{option_id: 'variation-1', option_label: 'Variation #1', personalize_elements_content: '<div id="someday">The lovers the dreamers and me</div>'}], 'variation-1');
  Drupal.personalize.personalizePage(Drupal.settings);
  // The HTML will have been replaced so the ID is no longer there.
  assert.equal($('#someday').length, 1, 'Personalized div has been added to the page.');
  assert.equal($('#personalize-option-set-edit-html').length, 0, 'Original unpersonalized HTML has been removed.');
  assert.ok($('#someday').data('personalize') == 'osid-1', 'Personalized element was assigned a data identifier.');
  assert.equal($('#someday').html(), "The lovers the dreamers and me", 'HTML was replaced.');
});

QUnit.test("Personalize page replaceHtml", function( assert ) {
  expect(4);
  Drupal.attachBehaviors = function (context, settings) {
    assert.ok(true, 'Attach behaviors was called.');
    assert.equal(context.attr('id'), 'personalize-option-set-replace-html', 'Context passed to attachBehaviors is the container with replaced content.');
  }

  addOptionSet('osid-1', '#personalize-option-set-replace-html', 'replaceHtml', [{option_id: 'variation-1', option_label: 'Variation #1', personalize_elements_content: '<div id="someday">The lovers the dreamers and me</div>'}], 'variation-1');
  Drupal.personalize.personalizePage(Drupal.settings);
  assert.ok($('#personalize-option-set-replace-html').data('personalize') == 'osid-1', 'Personalized element was assigned a data identifier.');
  assert.equal($('#personalize-option-set-replace-html').html(), '<div id="someday">The lovers the dreamers and me</div>', 'HTML was set within replacement container.');
});

QUnit.test("Personalize page addClass", function( assert ) {
  expect(2);

  Drupal.attachBehaviors = function (context, settings) {
    assert.ok(false, 'Attach behaviors was called.');
  }

  addOptionSet('osid-1', '#personalize-option-set-add-class', 'addClass', [{option_id: 'variation-1', option_label: 'Variation #1', personalize_elements_content: 'my-test-class'}], 'variation-1');
  Drupal.personalize.personalizePage(Drupal.settings);
  assert.ok($('#personalize-option-set-add-class').data('personalize') == 'osid-1', 'Personalized element was assigned a data identifier.');
  assert.ok($('#personalize-option-set-add-class').hasClass('my-test-class'), 'Class was added.');
});

QUnit.test("Personalize page prependHtml", function( assert ) {
  expect(4);
  Drupal.attachBehaviors = function (context, settings) {
    assert.ok(true, 'Attach behaviors was called.');
    assert.equal(context.attr('id'), 'personalize-option-set-prepend-html', 'Context passed to attachBehaviors is the updated container.');
  }

  var originalHtml = $('#personalize-option-set-prepend-html').html();

  addOptionSet('osid-1', '#personalize-option-set-prepend-html', 'prependHtml', [{option_id: 'variation-1', option_label: 'Variation #1', personalize_elements_content: '<div id="someday">The lovers the dreamers and me</div>'}], 'variation-1');
  Drupal.personalize.personalizePage(Drupal.settings);
  assert.ok($('#personalize-option-set-prepend-html').data('personalize') == 'osid-1', 'Personalized element was assigned a data identifier.');
  var expected = '<span id="personalize-elements-prepend-osid-1"><div id="someday">The lovers the dreamers and me</div></span>' + originalHtml;
  assert.equal($('#personalize-option-set-prepend-html').html(), expected, 'HTML was set prepended to existing HTML.');
});

QUnit.test("Personalize page appendHtml", function( assert ) {
  expect(4);
  Drupal.attachBehaviors = function (context, settings) {
    assert.ok(true, 'Attach behaviors was called.');
    assert.equal(context.attr('id'), 'personalize-option-set-append-html', 'Context passed to attachBehaviors is the updated container.');
  }

  var originalHtml = $('#personalize-option-set-append-html').html();

  addOptionSet('osid-1', '#personalize-option-set-append-html', 'appendHtml', [{option_id: 'variation-1', option_label: 'Variation #1', personalize_elements_content: '<div id="someday">The lovers the dreamers and me</div>'}], 'variation-1');
  Drupal.personalize.personalizePage(Drupal.settings);
  assert.ok($('#personalize-option-set-append-html').data('personalize') == 'osid-1', 'Personalized element was assigned a data identifier.');
  var expected = originalHtml + '<span id="personalize-elements-append-osid-1"><div id="someday">The lovers the dreamers and me</div></span>';
  assert.equal($('#personalize-option-set-append-html').html(), expected, 'HTML was set appended to existing HTML.');
});

QUnit.test("Personalize page runJS", function( assert ) {
  expect(3);
  Drupal.runJSCallback = function (message) {
    assert.equal(message, 'Mahna mahna', 'RunJS callback was executed with parameters.');
  }

  Drupal.attachBehaviors = function (context, settings) {
    assert.ok(true, 'Attach behaviors was called.');
    assert.ok(typeof context === 'undefined', 'Behaviors called with the default document context.');
  }

  addOptionSet('osid-1', '', 'runJS', [{option_id: 'variation-1', option_label: 'Variation #1', personalize_elements_content: "Drupal.runJSCallback('Mahna mahna');"}], 'variation-1');
  Drupal.personalize.personalizePage(Drupal.settings);
});


/**
 * Adds a personalize elements option set to the settings.
 *
 * @param osid
 *   The option set id to use.
 * @param selector
 *   The selector that should be affected.
 * @param variationType
 *   The type of variation, e.g., editText, addClass, etc.
 * @param options
 *   An array of options to add not including the control.  Each object in the
 *   array should have the following keys:
 *   - option_id: the id of the option
 *   - option_label: the label of the option
 *   - personalize_elements_content: The content that should be used for
 *     personalization.
 * @param selectedOptionId
 *   (Optional) the option id to treat as the option to return.
 */
function addOptionSet(osid, selector, variationType, options, selectedOptionId) {
  Drupal.settings.personalize.option_sets[osid] = {
    agent: "my-agent",
    agent_info: {
      active: false,
      cache_decisions: false,
      enabled_contexts: [],
      label: "My agent",
      type: "test_agent"
    },
    data: {
      pages: "node",
      personalize_elements_selector: selector,
      personalize_elements_type: variationType
    },
    decision_name: osid,
    decision_point: osid,
    entity_type: "personalize_option_set",
    executor: "personalizeElements",
    label: "Option set " + osid,
    mvt: "",
    option_names: ["control-variation"],
    options: [
      {
        option_id: "control-variation",
        option_label: "Control variation",
        original_index: 0,
        personalize_elements_content: ""
      }
    ],
    osid: osid,
    plugin: "elements",
    preview_link: "",
    rdf_mapping: [],
    selector: selector,
    stateful: "0",
    targeting: null,
    winner: null
  }

  Drupal.settings.personalize_elements.elements[osid] = {
    previewable: true,
    selector: selector,
    variation_type: variationType
  }

  for (var i = 0; i < options.length; i++) {
    options[i].original_index = i + 1;
    Drupal.settings.personalize.option_sets[osid]['option_names'].push(options[i].option_id);
    Drupal.settings.personalize.option_sets[osid]['options'].push(options[i]);
  }

  if (selectedOptionId) {
    var bucket = Drupal.personalize.storage.utilities.getBucket('decisions');
    var decisions = {};
    decisions[osid] = selectedOptionId;
    bucket.write('my-agent:' + osid, decisions);
  }
}
