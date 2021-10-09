// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"

#include "SwitchViewManager.h"
#include <Views/ShadowNodeBase.h>

#include <IReactInstance.h>

#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>

namespace winrt {
  using namespace Windows::UI::Xaml;
  using namespace Windows::UI::Xaml::Controls;
}

namespace react { namespace uwp {

class SwitchShadowNode : public ShadowNodeBase
{
  using Super = ShadowNodeBase;
public:
  SwitchShadowNode() = default;
  void createView() override;
  void updateProperties(const folly::dynamic&& props) override;

private:
  static void OnToggled(IReactInstance& instance, int64_t tag, bool newValue);
};

void SwitchShadowNode::createView()
{
  Super::createView();

  auto toggleSwitch = GetView().as<winrt::ToggleSwitch>();
  auto wkinstance = GetViewManager()->GetReactInstance();
  toggleSwitch.Toggled([=](auto&&, auto&&)
  {
    auto instance = wkinstance.lock();
    if (!m_updating && instance != nullptr)
      OnToggled(*instance, m_tag, toggleSwitch.IsOn());
  });
}

void SwitchShadowNode::updateProperties(const folly::dynamic&& props)
{
  m_updating = true;
  Super::updateProperties(std::move(props));
  m_updating = false;
}

/*static*/ void SwitchShadowNode::OnToggled(IReactInstance& instance, int64_t tag, bool newValue)
{
  folly::dynamic eventData = folly::dynamic::object("target", tag)("value", newValue);
  instance.DispatchEvent(tag, "topChange", std::move(eventData));
}

SwitchViewManager::SwitchViewManager(const std::shared_ptr<IReactInstance>& reactInstance)
  : Super(reactInstance)
{
}

const char* SwitchViewManager::GetName() const
{
  return "RCTSwitch";
}

folly::dynamic SwitchViewManager::GetNativeProps() const
{
  auto props = Super::GetNativeProps();

  props.update(folly::dynamic::object
    ("value", "boolean")
    ("disabled", "boolean")
  );

  return props;
}

facebook::react::ShadowNode* SwitchViewManager::createShadow() const
{
  return new SwitchShadowNode();
}

XamlView SwitchViewManager::CreateViewCore(int64_t tag)
{
  auto toggleSwitch = winrt::ToggleSwitch();
  toggleSwitch.OnContent(nullptr);
  toggleSwitch.OffContent(nullptr);

  return toggleSwitch;
}

void SwitchViewManager::UpdateProperties(ShadowNodeBase* nodeToUpdate, const folly::dynamic& reactDiffMap)
{
  auto toggleSwitch = nodeToUpdate->GetView().as<winrt::ToggleSwitch>();
  if (toggleSwitch == nullptr)
    return;

  for (const auto& pair : reactDiffMap.items())
  {
    const std::string& propertyName = pair.first.getString();
    const folly::dynamic& propertyValue = pair.second;

   if (propertyName == "disabled")
   {
      if (propertyValue.isBool())
        toggleSwitch.IsEnabled(!propertyValue.asBool());
      else if (pair.second.isNull())
        toggleSwitch.ClearValue(winrt::Control::IsEnabledProperty());
   }
   else if (propertyName == "value")
   {
     if (propertyValue.isBool())
       toggleSwitch.IsOn(propertyValue.asBool());
     else if (pair.second.isNull())
       toggleSwitch.ClearValue(winrt::ToggleSwitch::IsOnProperty());
   }
  }

  Super::UpdateProperties(nodeToUpdate, reactDiffMap);
}

}}
