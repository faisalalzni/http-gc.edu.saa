// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.


'use strict';

// TODO Hx: Implement.

type BackPressEventName = $Enum<{
  backPress: string,
}>;

var _backPressSubscriptions = new Set();

function emptyFunction() { }

/**
 * Detect hardware button presses for back navigation.
 *
 * Android: Detect hardware back button presses, and programmatically invoke the default back button
 * functionality to exit the app if there are no listeners or if none of the listeners return true.
 *
 * tvOS: Detect presses of the menu button on the TV remote.  (Still to be implemented:
 * programmatically disable menu button handling
 * functionality to exit the app if there are no listeners or if none of the listeners return true.)
 *
 * iOS: Not applicable.
 *
 * The event subscriptions are called in reverse order (i.e. last registered subscription first),
 * and if one subscription returns true then subscriptions registered earlier will not be called.
 *
 * Example:
 *
 * ```javascript
 * BackHandler.addEventListener('hardwareBackPress', function() {
 *  // this.onMainScreen and this.goBack are just examples, you need to use your own implementation here
 *  // Typically you would use the navigator here to go to the last state.
 *
 *  if (!this.onMainScreen()) {
 *    this.goBack();
 *    return true;
 *  }
 *  return false;
 * });
 * ```
 */

var BackHandler = {
    exitApp: emptyFunction,
    addEventListener() {
        return {
            remove: emptyFunction,
        };
    },
    removeEventListener: emptyFunction,
};

module.exports = BackHandler;
