// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using Newtonsoft.Json.Linq;
using ReactNative.Bridge;
using ReactNative.Bridge.Queue;
using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace ReactNative.Tests
{
    class MockReactInstance : IReactInstance
    {
        private readonly Action<int, JArray> _callback;
        private readonly Action<string, string, JArray, string> _function;

        private int _isDisposed;

        public MockReactInstance()
            : this((_, __) => { }, (p0, p1, p2, p3) => { })
        {
        }

        public MockReactInstance(Action<int, JArray> callback)
            : this(callback, (p0, p1, p2, p3) => { })
        {
        }

        public MockReactInstance(Action<string, string, JArray, string> function)
            : this((_, __) => { }, function)
        {
        }

        public MockReactInstance(Action<int, JArray> callback, Action<string, string, JArray, string> function)
        {
            _callback = callback;
            _function = function;
        }

        public bool IsDisposed
        {
            get
            {
                return Volatile.Read(ref _isDisposed) > 0;
            }
        }

        public IEnumerable<INativeModule> NativeModules
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        public IReactQueueConfiguration QueueConfiguration
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        public virtual T GetNativeModule<T>() where T : INativeModule
        {
            throw new NotImplementedException();
        }

        public virtual T GetJavaScriptModule<T>() where T : IJavaScriptModule, new()
        {
            throw new NotImplementedException();
        }

        public Task InitializeAsync()
        {
            throw new NotImplementedException();
        }

        public void InvokeCallback(int callbackId, JArray arguments)
        {
            _callback(callbackId, arguments);
        }

        public void InvokeFunction(string module, string method, JArray arguments, string tracingName)
        {
            _function(module, method, arguments, tracingName);
        }

        public Task DisposeAsync()
        {
            Interlocked.Increment(ref _isDisposed);
            return Task.CompletedTask;
        }
    }
}
