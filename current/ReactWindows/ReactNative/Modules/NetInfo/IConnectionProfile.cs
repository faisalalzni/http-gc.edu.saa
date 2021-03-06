// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using Windows.Networking.Connectivity;

namespace ReactNative.Modules.NetInfo
{
    /// <summary>
    /// An interface for network connection profiles.
    /// </summary>
    public interface IConnectionProfile
    {
        /// <summary>
        /// A value that indicates the network connectivity level.
        /// </summary>
        NetworkConnectivityLevel ConnectivityLevel { get; }
    }
}
