/**
 * @file libraries.tests.js
 */
var personalizeStorage = Drupal.personalizeStorage,
    personalizeStorageKeyPrefix = "Personalize::qunit::";

QUnit.module("Personalize storage tests", {
  'setup': function() {
    // Create fixtures.
    personalizeStorage.write(personalizeStorageKeyPrefix + '1', 'my_session_value_1', true);
    personalizeStorage.write(personalizeStorageKeyPrefix + '2', {field: 'my_session_value_2'}, true);
    personalizeStorage.write(personalizeStorageKeyPrefix + '1', 'my_local_value_1', false);
    personalizeStorage.write(personalizeStorageKeyPrefix + '2', {field: 'my_local_value_2'}, false);
  },
  'teardown': function() {
    // Remove all fixtures.
    personalizeStorage.clearStorage(personalizeStorageKeyPrefix, true);
    personalizeStorage.clearStorage(personalizeStorageKeyPrefix, false);
  }
});

QUnit.test("Supports local storage", function(assert) {
  expect(1);
  assert.ok(personalizeStorage.supportsLocalStorage(), 'Supports local storage.');
});

QUnit.test("Read", function(assert) {
  expect(9);
  // Read existing values.
  var sessionData1 = personalizeStorage.read(personalizeStorageKeyPrefix + '1', true),
      sessionData1a = personalizeStorage.read(personalizeStorageKeyPrefix + '1'),
      sessionData2 = personalizeStorage.read(personalizeStorageKeyPrefix + '2', true),
      localData1 = personalizeStorage.read(personalizeStorageKeyPrefix + '1', false),
      localData2 = personalizeStorage.read(personalizeStorageKeyPrefix + '2', false);
  assert.equal(sessionData1, 'my_session_value_1', 'Read session storage string data.');
  assert.equal(sessionData1a, 'my_session_value_1', 'Read session storage data, as defaulted.');
  assert.deepEqual(sessionData2, {field: 'my_session_value_2'}, 'Read session storage object data.');
  assert.equal(localData1, 'my_local_value_1', 'Read local storage string data.');
  assert.deepEqual(localData2, {field: 'my_local_value_2'}, 'Read local storage object data.');
  assert.deepEqual(_getStorageKeyList(true), ["Personalize::qunit::1", "Personalize::qunit::2"], 'Session storage key list should match the fixture.');
  assert.deepEqual(_getStorageKeyList(false), ["Personalize::qunit::1", "Personalize::qunit::2"], 'Local storage key list should match the fixture.');

  // Read non-existing values.
  var sessionDataNoSuchKey = personalizeStorage.read(personalizeStorageKeyPrefix + 'no_such_key', true),
      localDataNoSuchKey = personalizeStorage.read(personalizeStorageKeyPrefix + 'no_such_key', false);
  assert.equal(sessionDataNoSuchKey, null, 'Read session storage data of non-existing key returns null.');
  assert.equal(localDataNoSuchKey, null, 'Read local storage storage data of non-existing key returns null.');
});

QUnit.test("Write", function(assert) {
  expect(6);
  // Overwrite existing values.
  personalizeStorage.write(personalizeStorageKeyPrefix + '1', 'my_session_value_1_new', true);
  personalizeStorage.write(personalizeStorageKeyPrefix + '2', {field: 'my_session_value_2_new'}, true);
  personalizeStorage.write(personalizeStorageKeyPrefix + '1', 'my_local_value_1_new', false);
  personalizeStorage.write(personalizeStorageKeyPrefix + '2', {field: 'my_local_value_2_new'}, false);

  var sessionData1 = personalizeStorage.read(personalizeStorageKeyPrefix + '1', true),
      sessionData2 = personalizeStorage.read(personalizeStorageKeyPrefix + '2', true),
      localData1 = personalizeStorage.read(personalizeStorageKeyPrefix + '1', false),
      localData2 = personalizeStorage.read(personalizeStorageKeyPrefix + '2', false);
  assert.equal(sessionData1, 'my_session_value_1_new', 'Overwritten session storage string data.');
  assert.deepEqual(sessionData2, {field: 'my_session_value_2_new'}, 'Overwritten session storage object data.');
  assert.equal(localData1, 'my_local_value_1_new', 'Overwritten local storage string data.');
  assert.deepEqual(localData2, {field: 'my_local_value_2_new'}, 'Overwritten local storage object data.');
  assert.deepEqual(_getStorageKeyList(true), ["Personalize::qunit::1", "Personalize::qunit::2"], 'Session storage key list should remain the same after replacement.');
  assert.deepEqual(_getStorageKeyList(false), ["Personalize::qunit::1", "Personalize::qunit::2"], 'Local storage key list should remain the same after replacement.');
});

QUnit.test("Remove", function(assert) {
  expect(6);
  // Remove existing values.
  personalizeStorage.remove(personalizeStorageKeyPrefix + '1', true);
  personalizeStorage.remove(personalizeStorageKeyPrefix + '2', false);

  var sessionData1 = personalizeStorage.read(personalizeStorageKeyPrefix + '1', true),
      sessionData2 = personalizeStorage.read(personalizeStorageKeyPrefix + '2', true),
      localData1 = personalizeStorage.read(personalizeStorageKeyPrefix + '1', false),
      localData2 = personalizeStorage.read(personalizeStorageKeyPrefix + '2', false);
  assert.equal(sessionData1, null, 'Read removed session storage string data should return null.');
  assert.deepEqual(sessionData2, {field: 'my_session_value_2'}, 'Read other session storage object data should return intact data.');
  assert.equal(localData1, 'my_local_value_1', 'Read other local storage string data should return intact data.');
  assert.deepEqual(localData2, null, 'Read removed local storage object data should return null.');
  assert.deepEqual(_getStorageKeyList(true), ["Personalize::qunit::2"], 'Session storage key list should exclude the removed items.');
  assert.deepEqual(_getStorageKeyList(false), ["Personalize::qunit::1"], 'Local storage key list should exclude the removed items.');
});

QUnit.test("Clean storage", function(assert) {
  expect(8);
  // Create items using a new prefix.
  var personalizeStorageTestClearKeyPrefix = "Personalize::qunit::test_clear::";
  personalizeStorage.write(personalizeStorageTestClearKeyPrefix + '1', 'my_session_value_1', true);
  personalizeStorage.write(personalizeStorageTestClearKeyPrefix + '1', 'my_local_value_1', false);

  // Assert the keys are added accordingly.
  assert.deepEqual(_getStorageKeyList(true), ["Personalize::qunit::1","Personalize::qunit::2","Personalize::qunit::test_clear::1"], 'Session storage key list are added new keys, accordingly.');
  assert.deepEqual(_getStorageKeyList(false), ["Personalize::qunit::1","Personalize::qunit::2","Personalize::qunit::test_clear::1"], 'Local storage key list are added new keys, accordingly.');

  // Clear everything associated with this new prefix.
  personalizeStorage.clearStorage(personalizeStorageTestClearKeyPrefix, true);
  personalizeStorage.clearStorage(personalizeStorageTestClearKeyPrefix, false);

  // Now verify the cleared storages are indeed cleared; and the remaining, intact.
  var sessionDataCleared1 = personalizeStorage.read(personalizeStorageTestClearKeyPrefix + '1', true),
      localDataCleared1 = personalizeStorage.read(personalizeStorageTestClearKeyPrefix + '1', false),
      sessionData1 = personalizeStorage.read(personalizeStorageKeyPrefix + '1', true),
      localData1 = personalizeStorage.read(personalizeStorageKeyPrefix + '1', false);
  assert.equal(sessionDataCleared1, null, 'New session storage should be cleared.');
  assert.equal(localDataCleared1, null, 'New local storage should be cleared.');
  assert.equal(sessionData1, 'my_session_value_1', 'Other session storage data are still intact.');
  assert.equal(localData1, 'my_local_value_1', 'Other local storage data are still intact.');
  assert.deepEqual(_getStorageKeyList(true), ["Personalize::qunit::1","Personalize::qunit::2"], 'Session storage key list should exclude the removed items.');
  assert.deepEqual(_getStorageKeyList(false), ["Personalize::qunit::1","Personalize::qunit::2"], 'Local storage key list should exclude the removed items.');
});

function _getStorageKeyList(session) {
  session = session === undefined ? true : session;
  var storage = session ? sessionStorage : localStorage,
      keyListString = storage.getItem(personalizeStorage.keyListKey);

  return JSON.parse(keyListString);
}
