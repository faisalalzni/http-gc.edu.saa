// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <Views/ViewPanel.h>
#include <Views/ShadowNodeBase.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Automation.h>
#include <winrt/Windows.UI.Xaml.Automation.Peers.h>
#include <winrt/Windows.UI.Xaml.Automation.Provider.h>
#include <winrt/Windows.UI.Xaml.Controls.h>

namespace react {
namespace uwp {

//
// ViewControl is ViewViewManager's ContentControl but with a custom AutomationPeer
// using DynamicAutomationPeer (See below)
//
struct ViewControl : winrt::Windows::UI::Xaml::Controls::ContentControlT<ViewControl>
{
  using Super = winrt::Windows::UI::Xaml::Controls::ContentControlT<ViewControl>;
private:
  // Constructors
  ViewControl();

public:
  static winrt::com_ptr<ViewControl> Create();
  template <typename D, typename... Args> friend auto winrt::make_self(Args&&... args);

  winrt::Windows::UI::Xaml::Automation::Peers::AutomationPeer OnCreateAutomationPeer();

  // Public Methods
  AccessibilityRoles AccessibilityRole() { return m_accessibilityRole; }
  void AccessibilityRole(AccessibilityRoles role) { m_accessibilityRole = role; }

  bool AccessibilityState(AccessibilityStates state) { return m_accessibilityStates[state]; }
  void AccessibilityState(AccessibilityStates state, bool value)
  {
    m_accessibilityStates[state] = value;
  }

  using AccessibilityInvokeEventHandler = std::function<void(void)>;
  const AccessibilityInvokeEventHandler& AccessibilityInvoke() { return m_accessibilityInvokeHandler; }
  void AccessibilityInvoke(AccessibilityInvokeEventHandler&& handler)
  {
    m_accessibilityInvokeHandler = std::move(handler);
  }

private:
  AccessibilityRoles m_accessibilityRole = AccessibilityRoles::None;
  bool m_accessibilityStates[AccessibilityStates::CountStates] = { };
  AccessibilityInvokeEventHandler m_accessibilityInvokeHandler = nullptr;
};

}} // namespace react::uwp


#include "cppwinrt/DynamicAutomationPeer.g.h"
namespace winrt::react::uwp::implementation
{
//
// DynamicAutomationPeer refers to the owner ViewControl to determine what type control
// it appears to be for accessibility tools
//
struct DynamicAutomationPeer : DynamicAutomationPeerT<DynamicAutomationPeer>
{
  using Super = DynamicAutomationPeerT<DynamicAutomationPeer>;

  DynamicAutomationPeer() = delete;
  DynamicAutomationPeer(winrt::FrameworkElement const& owner);

  winrt::hstring GetClassNameCore() const;

  winrt::Windows::UI::Xaml::Automation::Peers::AutomationControlType GetAutomationControlTypeCore() const;
  winrt::IInspectable GetPatternCore(winrt::Windows::UI::Xaml::Automation::Peers::PatternInterface const& patternInterface) const;

  bool IsEnabledCore() const;

  // IInvokeProvider
  void Invoke() const;

  // ISelectionProvider
  bool CanSelectMultiple() const { return true; }
  bool IsSelectionRequired() const { return false; }
  winrt::com_array<winrt::Windows::UI::Xaml::Automation::Provider::IRawElementProviderSimple> GetSelection() const;

  // ISelectionItemProvider
  bool IsSelected() const;
  winrt::Windows::UI::Xaml::Automation::Provider::IRawElementProviderSimple SelectionContainer() const;
  void AddToSelection() const;
  void RemoveFromSelection() const;
  void Select() const;

private:
  winrt::com_ptr<::react::uwp::ViewControl> GetViewControl() const;
  winrt::com_ptr<::react::uwp::ViewPanel> GetViewPanel() const;
  winrt::com_ptr<::react::uwp::ViewControl> GetParentViewControl() const;
};
} // namespace winrt::react::uwp::implementation

