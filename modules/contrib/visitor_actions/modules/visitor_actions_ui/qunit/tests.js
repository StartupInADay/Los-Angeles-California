/**
 * @file tests.js
 */
QUnit.module("Visitor Actions UI element selector tests");
QUnit.test("selector basics", function( assert ) {
  expect(6);
  var $elem = jQuery('#page');
  var selector = Drupal.utilities.getSelector($elem[0]);
  assert.equal(selector, '#page', 'ID selector matched');
  $elem = jQuery('.myclass');
  selector = Drupal.utilities.getSelector($elem[0]);
  assert.equal(selector, '#page > .ignoreme', 'Class match produces a unique selector.');
  $elem = jQuery('#ignoremyid');
  selector = Drupal.utilities.getSelector($elem[0], 'ignoremyid');
  assert.equal(selector, '#page > p:eq(2)', 'Ignored id not included in selector.');
  $elem = jQuery('#ignoremyid');
  assert.equal($elem.length, 1, 'Ignore id restored');
  $elem = jQuery('.ignoreme');
  selector = Drupal.utilities.getSelector($elem[0], 'ignoremyid', 'ignoreme');
  assert.equal(selector, '#page > p:eq(0)', 'Ignored class name not included in selector.');
  $elem = jQuery('.ignoreme');
  assert.equal($elem.length, 2, 'Ignore class restored');
});

QUnit.module("Visitor Actions UI dialog model tests");
QUnit.test("dialog model tests", function (assert) {
  expect(6);

  var model = new Drupal.visitorActions.ui.dialog.models.DialogModel();
  assert.equal(model.get('active'), false, 'Default value for active is correct.');
  assert.equal(model.get('formPath'), '', 'Default value for formPath is correct.');
  assert.equal(model.get('selector'), null, 'Default value for selector is correct.');

  var valueModel = new Drupal.visitorActions.ui.dialog.models.DialogModel({
    active: true,
    formPath: 'test',
    selector: '#selector'
  });
  assert.equal(valueModel.get('active'), true, 'Model value for active set correctly.');
  assert.equal(valueModel.get('formPath'), 'test', 'Model value for formPath set correctly.');
  assert.equal(valueModel.get('selector'), '#selector', 'Model value for selector set correctly.');
});

QUnit.module("Visitor Actions UI dialog view tests");
QUnit.test("dialog view tests", function (assert) {
  var selector = '#ignoremyid';
  var $elem = jQuery(selector);
  expect(3);

  // Create the dialog backbone objects.
  var model = new Drupal.visitorActions.ui.dialog.models.DialogModel({
    active: false,
    selector: selector
  });
  // @todo: Write test with form loading via mock back-end server call.
  var view = new Drupal.visitorActions.ui.dialog.views.ElementDialogView({
    model: model,
    el: $elem[0]
  });

  // Make sure the dialog isn't rendered until the model is active.
  var $dialog = jQuery('.visitor-actions-ui-dialog');
  assert.equal($dialog.length, 0, 'View is not rendered when model is inactive.');
  model.set('active', true);
  $dialog = jQuery('.visitor-actions-ui-dialog');
  assert.equal($dialog.length, 1, 'View is rendered after model is activated.');
  $dialog.remove();
  $dialog = jQuery('.visitor-actions-ui-dialog');
  assert.equal($dialog.length, 0, 'DOM element is removed after view is removed.');
});
