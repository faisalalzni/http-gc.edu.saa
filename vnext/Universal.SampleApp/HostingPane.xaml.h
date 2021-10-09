﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// HostingPane.xaml.h
// Declaration of the HostingPane class
//

#pragma once

#include "HostingPane.g.h"

#include <ReactUWP/IXamlRootView.h>

#include <memory>
#include <string>

namespace WindowsSampleApp
{

[Windows::Foundation::Metadata::WebHostHidden]
public ref class HostingPane sealed
{
public:
  HostingPane();

  property Windows::UI::Xaml::Input::ICommand^ AddPaneCommand
  {
    void set(Windows::UI::Xaml::Input::ICommand^ value);
    Windows::UI::Xaml::Input::ICommand^ get();
  }

  property Windows::UI::Xaml::Input::ICommand^ RemovePaneCommand
  {
    void set(Windows::UI::Xaml::Input::ICommand^ value);
    Windows::UI::Xaml::Input::ICommand^ get();
  }

private:
  void OnTextChanged_JavaScriptFilename(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs args);
  void OnLoadClicked(Platform::Object^ sender, Platform::Object^ args);
  void OnUnloadClicked(Platform::Object^ sender, Platform::Object^ args);
  void OnReloadClicked(Platform::Object^ sender, Platform::Object^ args);

  void DetachAndClearRoot();
  void LoadReactNative();

  void LoadFilenameSettings();
  void StoreFilenameSettings();

  void UpdateUI();

internal:
  std::shared_ptr<react::uwp::IReactInstance> getInstance();
  void markAsNeedsReload();

private:
  std::wstring m_loadedJSComponentName;
  std::wstring m_loadedBundleFileName;

  react::uwp::ReactInstanceCreator m_instanceCreator;

  std::shared_ptr<react::uwp::IXamlRootView> m_rootView;
  std::shared_ptr<react::uwp::IReactInstance> m_instance;
};

}
