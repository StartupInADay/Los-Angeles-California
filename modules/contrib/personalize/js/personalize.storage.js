'use strict';

(function (Drupal) {

  Drupal.personalizeStorage = (function() {

    var keyListKey = 'personalize::storage::keys';

    /**
     * Determine the kind of storage to use based on session type requested.
     *
     * @param session
     *   True if session storage, false if local storage.  Defaults true.
     */
    function _getStore(session) {
      session = session === undefined ? true : session;
      return session ? sessionStorage : localStorage;
    }

    /**
     * Gets the listing of keys and the order in which they were added.
     *
     * @param key
     *   The key of the item saved to storage
     * @param session
     *   True if session storage, false if local storage.  Defaults true.
     */
    function _getTrackedKeys(session) {
      var store = _getStore(session);
      var keys = store.getItem(keyListKey);
      if (keys) {
        keys = JSON.parse(keys);
      } else {
        keys = [];
      }
      return keys;
    }

    /**
     * Add key to the end of the key list.
     *
     * @param key
     *   The key of the item add to storage
     * @param session
     *   True if session storage, false if local storage.  Defaults true.
     */
    function _addToKeyList(key, session) {
      var store = _getStore(session);
      var keys = _getTrackedKeys(session);
      keys.push(key);
      store.setItem(keyListKey, JSON.stringify(keys));
    }

    /**
     * Remove key from the key list.
     *
     * @param key
     *   The key of the item saved to storage
     * @param session
     *   True if session storage, false if local storage. Defaults true.
     */
    function _removeFromKeyList(key, session) {
      var store = _getStore(session);
      var keys = _getTrackedKeys(session);
      var index = keys.indexOf(key);
      if (index > -1) {
        keys.splice(index, 1);
      }
      store.setItem(keyListKey, JSON.stringify(keys));
    }

    /**
     * Prunes the oldest key(s) from storage.
     *
     * @param session
     *   True if session storage, false if local storage.  Defaults true.
     * @param numEntries
     *   The number of entries to remove.  Default = 10.
     */
    function _pruneOldest(session, numEntries) {
      numEntries = numEntries || 10;
      var keys = _getTrackedKeys(session);
      var totalKeys = keys.length;
      var until = totalKeys > numEntries ? totalKeys - numEntries : 0;
      var key, i;

      for (i = totalKeys; i >= until; i--) {
        key = keys.pop();
        _remove(key, session);
      }
    }

    /**
     * Writes an item to storage.
     *
     * @param key
     *   The bucket-specific key to use to store the item.
     * @param value
     *   The value to store (in any format that JSON.stringify can handle).
     * @param session
     *   True if session storage, false if local storage.  Defaults true.
     */
    function _write(key, value, session) {
      var store = _getStore(session);
      store.setItem(key, JSON.stringify(value));
      _addToKeyList(key, session);
    }

    /**
     * Removes an item from storage.
     *
     * @param key
     *   The bucket-specific key to use to remove the item.
     * @param session
     *   True if session storage, false if local storage.  Defaults true.
     */
    function _remove(key, session) {
      var store = _getStore(session);
      store.removeItem(key);
      _removeFromKeyList(key, session);
    }

    return {
      keyListKey: keyListKey,

      /**
       * Determine if the current browser supports web storage.
       * @return
       *   True if the current browser supports local storage, false otherwise.
       */
      supportsLocalStorage: function() {
        if (this.supportsHtmlLocalStorage !== undefined) {
          return this.supportsHtmlLocalStorage;
        }
        try {
          this.supportsHtmlLocalStorage = window.hasOwnProperty('localStorage') && window.localStorage !== null;
        } catch (e) {
          this.supportsHtmlLocalStorage = false;
        }
        return this.supportsHtmlLocalStorage;
      },

      /**
       * Reads an item from storage.
       *
       * @param key
       *   The bucket-specific key to use to lookup the item.
       * @param session
       *   True if session storage, false if local storage.  Defaults true.
       * @return
       *   The value set for the key or null if not available.
       */
      read: function (key, session) {
        if (!this.supportsLocalStorage()) { return null; }

        var store = _getStore(session),
            stored = store.getItem(key),
            record;
        if (stored) {
          record = JSON.parse(stored);
          if (record !== undefined) {
            return record;
          }
        }
        return null;
      },

      /**
       * Writes an item to the bucket.
       *
       * @param key
       *   The bucket-specific key to use to store the item.
       * @param value
       *   The value to store (in any format that JSON.stringify can handle).
       * @param session
       *   True if session storage, false if local storage.  Defaults true.
       */
      write: function (key, value, session) {
        if (!this.supportsLocalStorage()) { return; }

        // Fix for iPad issue - sometimes throws QUOTA_EXCEEDED_ERR on setItem.
        _remove(key, session);
        try {
          _write(key, value, session);
        } catch (e) {
          if (e.name === 'QUOTA_EXCEEDED_ERR' || e.name === 'NS_ERROR_DOM_QUOTA_REACHED') {
            // Prune off the oldest entries and try again.
            _pruneOldest(session);
            try {
              _write(key, value, session);
            } catch (e2) {
              console.error('Failed to write to storage, unhandled exception: ', e2);
            }
            return;
          }
          console.error('Failed to write to storage, unhandled exception: ', e);
        }
      },

      /**
       * Removes an item from a bucket.
       *
       * @param key
       *   The bucket-specific key to use to remove the item.
       * @param session
       *   True if session storage, false if local storage.  Defaults true.
       */
      remove: function (key, session) {
        if (!this.supportsLocalStorage()) { return; }

        _remove(key, session);
      },

      /**
       * Clears all items key containing prefix in storage
       *
       * @param prefix
       *    The bucket-specific key to use to remove the item.
       * @param session
       *    True if session storage, false if local storage. Defaults true.
       */
      clearStorage: function(prefix, session){
        if (!this.supportsLocalStorage()) { return; }

        var store = _getStore(session),
            i = store.length,
            key;
        while(i--) {
          key = store.key(i);
          if(key.indexOf(prefix) === 0) {
            _remove(key, session);
          }
        }
      }
    };
  })();

})(Drupal);
