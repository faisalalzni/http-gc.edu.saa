// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"

#include "CheckboxViewManager.h"
#include <Views/ShadowNodeBase.h>

#include <Utils/ValueUtils.h>

#include <IReactInstance.h>

#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>


namespace winrt {
  using namespace Windows::UI::Xaml;
  using namespace Windows::UI::Xaml::Controls;
}

namespace react { namespace uwp {

enum class CheckboxCommands
{
  SetFocus = 1,
};

class CheckBoxShadowNode : public ShadowNodeBase
{
  using Super = ShadowNodeBase;
public:
  CheckBoxShadowNode() = default;
  void createView() override;
  void updateProperties(const folly::dynamic&& props) override;

private:
  static void OnCheckedChanged(IReactInstance& instance, int64_t tag, bool newValue);
};

void CheckBoxShadowNode::createView()
{
  Super::createView();

  auto checkbox = GetView().as<winrt::CheckBox>();
  auto wkinstance = GetViewManager()->GetReactInstance();
  checkbox.Checked([=](auto&&, auto&&)
  {
    auto instance = wkinstance.lock();
    if (!m_updating && instance != nullptr)
      OnCheckedChanged(*instance, m_tag, true);
  });
  checkbox.Unchecked([=](auto&&, auto&&)
  {
    auto instance = wkinstance.lock();
    if (!m_updating && instance != nullptr)
      OnCheckedChanged(*instance, m_tag, false);
  });
}

void CheckBoxShadowNode::updateProperties(const folly::dynamic&& props)
{
  m_updating = true;
  Super::updateProperties(std::move(props));
  m_updating = false;
}

/*static*/ void CheckBoxShadowNode::OnCheckedChanged(IReactInstance& instance, int64_t tag, bool newValue)
{
  folly::dynamic eventData = folly::dynamic::object("target", tag)("value", newValue);
  instance.DispatchEvent(tag, "topChange", std::move(eventData));
}


CheckBoxViewManager::CheckBoxViewManager(const std::shared_ptr<IReactInstance>& reactInstance)
  : Super(reactInstance)
{
}

const char* CheckBoxViewManager::GetName() const
{
  return "RCTCheckBox";
}

folly::dynamic CheckBoxViewManager::GetCommands() const
{
  auto commands = Super::GetCommands();
  commands.update(folly::dynamic::object
    ("SetFocus", static_cast<std::underlying_type_t<CheckboxCommands>>(CheckboxCommands::SetFocus))
  );
  return commands;
}

folly::dynamic CheckBoxViewManager::GetNativeProps() const
{
  auto props = Super::GetNativeProps();

  props.update(folly::dynamic::object
    ("checked", "boolean")
    ("disabled", "boolean")
  );

  return props;
}

facebook::react::ShadowNode* CheckBoxViewManager::createShadow() const
{
  return new CheckBoxShadowNode();
}

XamlView CheckBoxViewManager::CreateViewCore(int64_t tag)
{
  auto checkbox = winrt::CheckBox();
  return checkbox;
}

void CheckBoxViewManager::UpdateProperties(ShadowNodeBase* nodeToUpdate, const folly::dynamic& reactDiffMap)
{
  auto checkbox = nodeToUpdate->GetView().as<winrt::CheckBox>();
  if (checkbox == nullptr)
    return;

  for (const auto& pair : reactDiffMap.items())
  {
    const std::string& propertyName = pair.first.getString();
    const folly::dynamic& propertyValue = pair.second;

   if (propertyName == "disabled")
   {
     if (propertyValue.isBool())
       checkbox.IsEnabled(!propertyValue.asBool());
     else if (pair.second.isNull())
       checkbox.ClearValue(winrt::Control::IsEnabledProperty());
   }
   else if (propertyName == "checked")
   {
     if (propertyValue.isBool())
       checkbox.IsChecked(propertyValue.asBool());
     else if (pair.second.isNull())
       checkbox.ClearValue(winrt::Primitives::ToggleButton::IsCheckedProperty());
   }
  }

  Super::UpdateProperties(nodeToUpdate, reactDiffMap);
}

void CheckBoxViewManager::DispatchCommand(XamlView viewToUpdate, int64_t commandId, const folly::dynamic& commandArgs)
{
  auto checkbox = viewToUpdate.as<winrt::CheckBox>();
  if (checkbox == nullptr)
    return;

  switch (commandId)
  {
    case static_cast<int64_t>(CheckboxCommands::SetFocus):
    {
      checkbox.Focus(winrt::FocusState::Programmatic);
      break;
    }
  }
}

}}
