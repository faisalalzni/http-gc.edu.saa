// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using NUnit.Framework;
using ReactNative.Modules.Core;

namespace ReactNative.Tests.Modules.Core
{
    [TestFixture]
    public class RCTDeviceEventEmitterTests
    {
        public void RCTDeviceEventEmitter_Invoke()
        {
            var module = new RCTDeviceEventEmitter();

            var name = default(string);
            var args = default(object[]);
            module.InvocationHandler = new MockInvocationHandler((n, a) =>
            {
                name = n;
                args = a;
            });

            var eventName = "foo";
            var data = new object();
            module.emit(eventName, data);
            Assert.AreEqual(nameof(RCTDeviceEventEmitter.emit), name);
            Assert.AreEqual(2, args.Length);
            Assert.AreSame(eventName, args[0]);
            Assert.AreSame(data, args[1]);
        }
    }
}
