// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using Microsoft.VisualStudio.TestPlatform.UnitTestFramework;
using Newtonsoft.Json.Linq;
using ReactNative.Bridge;
using ReactNative.Modules.Core;
using ReactNative.UIManager.Events;
using System;
using System.Reactive.Disposables;
using System.Threading;
using System.Threading.Tasks;

namespace ReactNative.Tests.UIManager.Events
{
    [TestClass]
    public class EventDispatcherTests
    {
        private static readonly JArray EmptyResponse = JArray.Parse("[[],[],[]]");

        [TestMethod]
        public async Task EventDispatcher_ArgumentChecks()
        {
            AssertEx.Throws<ArgumentNullException>(() => new EventDispatcher(null), ex => Assert.AreEqual("reactContext", ex.ParamName));

            var context = new ReactContext();
            var dispatcher = new EventDispatcher(context);
            AssertEx.Throws<ArgumentNullException>(() => dispatcher.DispatchEvent(null), ex => Assert.AreEqual("event", ex.ParamName));
            await DispatcherHelpers.CallOnDispatcherAsync(context.DisposeAsync);
        }

        [TestMethod]
        public async Task EventDispatcher_EventDispatches()
        {
            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Initialize);

            // TODO: (#288) Check for non-determinism.
            var waitDispatched = new AutoResetEvent(false);
            var executor = new MockJavaScriptExecutor
            {
                OnCallFunctionReturnFlushedQueue = (p0, p1, p2) =>
                {
                    waitDispatched.Set();
                    return EmptyResponse;
                },
                OnFlushQueue = () => EmptyResponse,
                OnInvokeCallbackAndReturnFlushedQueue = (_, __) => EmptyResponse
            };

            var context = await CreateContextAsync(executor);
            var dispatcher = new EventDispatcher(context);
            await DispatcherHelpers.RunOnDispatcherAsync(dispatcher.OnResume);

            var testEvent = new MockEvent(42, "Foo");
            dispatcher.DispatchEvent(testEvent);

            Assert.IsTrue(waitDispatched.WaitOne());

            await DispatcherHelpers.CallOnDispatcherAsync(context.DisposeAsync);

            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Dispose);
        }

        [TestMethod]
        public async Task EventDispatcher_NonCoalesced()
        {
            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Initialize);

            var waitDispatched = new AutoResetEvent(false);
            var executor = new MockJavaScriptExecutor
            {
                OnCallFunctionReturnFlushedQueue = (p0, p1, p2) =>
                {
                    waitDispatched.Set();
                    return EmptyResponse;
                },
                OnFlushQueue = () => EmptyResponse,
                OnInvokeCallbackAndReturnFlushedQueue = (_, __) => EmptyResponse
            };

            var context = await CreateContextAsync(executor);
            var dispatcher = new EventDispatcher(context);
            await DispatcherHelpers.RunOnDispatcherAsync(dispatcher.OnResume);

            var e1 = new NonCoalescedEvent(42, "Foo");
            var e2 = new NonCoalescedEvent(42, "Foo");

            using (BlockJavaScriptThread(context))
            {
                dispatcher.DispatchEvent(e1);
                dispatcher.DispatchEvent(e2);
            }

            Assert.IsTrue(waitDispatched.WaitOne());
            Assert.IsTrue(waitDispatched.WaitOne());

            await DispatcherHelpers.CallOnDispatcherAsync(context.DisposeAsync);

            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Dispose);
        }

        [TestMethod]
        public async Task EventDispatcher_MultipleDispatches()
        {
            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Initialize);

            var waitDispatched = new AutoResetEvent(false);
            var executor = new MockJavaScriptExecutor
            {
                OnCallFunctionReturnFlushedQueue = (p0, p1, p2) =>
                {
                    waitDispatched.Set();
                    return EmptyResponse;
                },
                OnFlushQueue = () => EmptyResponse,
                OnInvokeCallbackAndReturnFlushedQueue = (_, __) => EmptyResponse
            };

            var context = await CreateContextAsync(executor);
            var dispatcher = new EventDispatcher(context);
            await DispatcherHelpers.RunOnDispatcherAsync(dispatcher.OnResume);

            var count = 100;
            for (var i = 0; i < count; ++i)
            {
                var testEvent = new MockEvent(42, "Foo");
                dispatcher.DispatchEvent(testEvent);
                Assert.IsTrue(waitDispatched.WaitOne());
            }

            await DispatcherHelpers.CallOnDispatcherAsync(context.DisposeAsync);

            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Dispose);
        }

        [TestMethod]
        public async Task EventDispatcher_EventsCoalesced1()
        {
            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Initialize);

            var waitDispatched = new AutoResetEvent(false);
            var executor = new MockJavaScriptExecutor
            {
                OnCallFunctionReturnFlushedQueue = (p0, p1, p2) =>
                {
                    waitDispatched.Set();
                    return EmptyResponse;
                },
                OnFlushQueue = () => EmptyResponse,
                OnInvokeCallbackAndReturnFlushedQueue = (_, __) => EmptyResponse
            };

            var context = await CreateContextAsync(executor);
            var dispatcher = new EventDispatcher(context);
            await DispatcherHelpers.RunOnDispatcherAsync(dispatcher.OnResume);

            var winner = default(int);
            var disposed = new AutoResetEvent(false);

            var firstEvent = new TestEvent(42, "foo", 1, () => winner = 1, () => disposed.Set());
            await Task.Delay(1); // Ensure second event has higher timstamp
            var secondEvent = new TestEvent(42, "foo", 1, () => winner = 2, () => disposed.Set());

            using (BlockJavaScriptThread(context))
            {
                dispatcher.DispatchEvent(firstEvent);
                dispatcher.DispatchEvent(secondEvent);

                // First event is disposed after coalesce
                Assert.IsTrue(disposed.WaitOne());
            }

            Assert.IsTrue(waitDispatched.WaitOne());
            Assert.AreEqual(2, winner);
            Assert.IsFalse(waitDispatched.WaitOne(500));

            // Second event is disposed after dispatch
            Assert.IsTrue(disposed.WaitOne());

            await DispatcherHelpers.CallOnDispatcherAsync(context.DisposeAsync);

            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Dispose);
        }

        [TestMethod]
        public async Task EventDispatcher_EventsNotCoalesced()
        {
            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Initialize);

            var waitDispatched = new AutoResetEvent(false);
            var executor = new MockJavaScriptExecutor
            {
                OnCallFunctionReturnFlushedQueue = (p0, p1, p2) =>
                {
                    waitDispatched.Set();
                    return EmptyResponse;
                },
                OnFlushQueue = () => EmptyResponse,
                OnInvokeCallbackAndReturnFlushedQueue = (_, __) => EmptyResponse
            };

            var context = await CreateContextAsync(executor);
            var dispatcher = new EventDispatcher(context);
            await DispatcherHelpers.RunOnDispatcherAsync(dispatcher.OnResume);

            var disposed = new AutoResetEvent(false);

            var diffTag1 = new TestEvent(42, "foo", 1);
            var diffTag2 = new TestEvent(43, "foo", 1);

            var diffName1 = new TestEvent(42, "foo", 1);
            var diffName2 = new TestEvent(42, "bar", 1);

            var diffKey1 = new TestEvent(42, "foo", 1);
            var diffKey2 = new TestEvent(42, "foo", 2);

            var pairs = new[]
            {
                new[] { diffTag1, diffTag2 },
                new[] { diffName1, diffName2 },
                new[] { diffKey1, diffKey2 },
            };

            foreach (var pair in pairs)
            {
                using (BlockJavaScriptThread(context))
                {
                    dispatcher.DispatchEvent(pair[0]);
                    dispatcher.DispatchEvent(pair[1]);
                }

                Assert.IsTrue(waitDispatched.WaitOne());
                Assert.IsTrue(waitDispatched.WaitOne());
            }

            await DispatcherHelpers.CallOnDispatcherAsync(context.DisposeAsync);

            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Dispose);
        }

        [TestMethod]
        public async Task EventDispatcher_OnSuspend_EventDoesNotDispatch()
        {
            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Initialize);

            var waitDispatched = new AutoResetEvent(false);
            var executor = new MockJavaScriptExecutor
            {
                OnCallFunctionReturnFlushedQueue = (p0, p1, p2) =>
                {
                    waitDispatched.Set();
                    return EmptyResponse;
                },
                OnFlushQueue = () => EmptyResponse,
                OnInvokeCallbackAndReturnFlushedQueue = (_, __) => EmptyResponse
            };

            var context = await CreateContextAsync(executor);
            var dispatcher = new EventDispatcher(context);
            await DispatcherHelpers.RunOnDispatcherAsync(dispatcher.OnResume);

            var testEvent = new MockEvent(42, "Foo");

            using (BlockJavaScriptThread(context))
            {
                dispatcher.DispatchEvent(testEvent);
                await DispatcherHelpers.RunOnDispatcherAsync(dispatcher.OnSuspend);
            }

            Assert.IsFalse(waitDispatched.WaitOne(500));

            await DispatcherHelpers.CallOnDispatcherAsync(context.DisposeAsync);

            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Dispose);
        }

        [TestMethod]
        public async Task EventDispatcher_OnReactInstanceDispose_EventDoesNotDispatch()
        {
            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Initialize);

            var waitDispatched = new AutoResetEvent(false);
            var executor = new MockJavaScriptExecutor
            {
                OnCallFunctionReturnFlushedQueue = (p0, p1, p2) =>
                {
                    waitDispatched.Set();
                    return EmptyResponse;
                },
                OnFlushQueue = () => EmptyResponse,
                OnInvokeCallbackAndReturnFlushedQueue = (_, __) => EmptyResponse
            };

            var context = await CreateContextAsync(executor);
            var dispatcher = new EventDispatcher(context);
            await DispatcherHelpers.RunOnDispatcherAsync(dispatcher.OnResume);

            var testEvent = new MockEvent(42, "Foo");

            using (BlockJavaScriptThread(context))
            {
                dispatcher.DispatchEvent(testEvent);
                await DispatcherHelpers.RunOnDispatcherAsync(dispatcher.OnReactInstanceDispose);
            }

            Assert.IsFalse(waitDispatched.WaitOne(500));

            await DispatcherHelpers.CallOnDispatcherAsync(context.DisposeAsync);

            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Dispose);
        }

        [TestMethod]
        public async Task EventDispatcher_DispatchedAfterSuspend_ThenResume()
        {
            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Initialize);

            var waitDispatched = new AutoResetEvent(false);
            var executor = new MockJavaScriptExecutor
            {
                OnCallFunctionReturnFlushedQueue = (p0, p1, p2) =>
                {
                    waitDispatched.Set();
                    return EmptyResponse;
                },
                OnFlushQueue = () => EmptyResponse,
                OnInvokeCallbackAndReturnFlushedQueue = (_, __) => EmptyResponse
            };

            var context = await CreateContextAsync(executor);
            var dispatcher = new EventDispatcher(context);
            await DispatcherHelpers.RunOnDispatcherAsync(dispatcher.OnResume);

            var testEvent = new MockEvent(42, "Foo");

            await DispatcherHelpers.RunOnDispatcherAsync(dispatcher.OnSuspend);
            dispatcher.DispatchEvent(testEvent);

            Assert.IsFalse(waitDispatched.WaitOne(500));

            await DispatcherHelpers.RunOnDispatcherAsync(dispatcher.OnResume);
            Assert.IsTrue(waitDispatched.WaitOne());

            await DispatcherHelpers.CallOnDispatcherAsync(context.DisposeAsync);

            await DispatcherHelpers.RunOnDispatcherAsync(ReactChoreographer.Dispose);
        }

        private static async Task<ReactContext> CreateContextAsync(IJavaScriptExecutor executor)
        {
            var context = new ReactContext();

            var reactInstance = await DispatcherHelpers.CallOnDispatcherAsync(async () =>
            {
                var instance = CreateReactInstance(context, executor);
                context.InitializeWithInstance(instance);
                await instance.InitializeBridgeAsync(CancellationToken.None);
                await instance.InitializeAsync();
                return instance;
            });

            return context;
        }

        private static ReactInstance CreateReactInstance(ReactContext reactContext, IJavaScriptExecutor executor)
        {
            var registry = new NativeModuleRegistry.Builder(reactContext).Build();

            var instance = new ReactInstance.Builder
            {
                QueueConfiguration = TestReactQueueConfiguration.Create(ex => Assert.Fail(ex.ToString())),
                BundleLoader = JavaScriptBundleLoader.CreateFileLoader("ms-appx:///Resources/test.js"),
                Registry = registry,
                JavaScriptExecutorFactory = () => executor,
            }.Build();

            return instance;
        }

        private static IDisposable BlockJavaScriptThread(ReactContext reactContext)
        {
            var enter = new AutoResetEvent(false);
            var exit = new AutoResetEvent(false);

            reactContext.RunOnJavaScriptQueueThread(() =>
            {
                enter.Set();
                exit.WaitOne();
            });

            enter.WaitOne();
            return Disposable.Create(() => exit.Set());
        }

        class NonCoalescedEvent : MockEvent
        {
            public NonCoalescedEvent(int viewTag, string eventName)
                : base(viewTag, eventName)
            {
            }

            public override bool CanCoalesce
            {
                get
                {
                    return false;
                }
            }
        }

        class TestEvent : MockEvent
        {
            private readonly Action _onDispatched;

            public TestEvent(
                int viewTag,
                string eventName,
                short coalescingKey)
                : this(viewTag, eventName, coalescingKey, () => { }, () => { })
            {
            }

            public TestEvent(
                int viewTag, 
                string eventName, 
                short coalescingKey,
                Action onDispatched,
                Action onDispose)
                : base(viewTag, eventName, new JObject(), onDispose)
            {
                _onDispatched = onDispatched;

                CoalescingKey = coalescingKey;
            }

            public override short CoalescingKey { get; }

            public override void Dispatch(RCTEventEmitter eventEmitter)
            {
                _onDispatched();

                base.Dispatch(eventEmitter);
            }
        }
    }
}
