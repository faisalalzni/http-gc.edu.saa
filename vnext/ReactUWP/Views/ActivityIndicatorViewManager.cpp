// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"

#include "ActivityIndicatorViewManager.h"

#include <Utils/PropertyUtils.h>
#include <Utils/ValueUtils.h>

#include <winrt/Windows.UI.Xaml.Controls.h>

namespace winrt {
  using namespace Windows::UI::Xaml::Controls;
}

namespace react { namespace uwp {

ActivityIndicatorViewManager::ActivityIndicatorViewManager(const std::shared_ptr<IReactInstance>& reactInstance)
  : Super(reactInstance)
{
}

const char* ActivityIndicatorViewManager::GetName() const
{
  return "RCTActivityIndicatorView";
}

folly::dynamic ActivityIndicatorViewManager::GetNativeProps() const
{
  auto props = Super::GetNativeProps();

  props.update(folly::dynamic::object
    ("animating", "boolean")
    ("color", "Color")
  );

  return props;
}

XamlView ActivityIndicatorViewManager::CreateViewCore(int64_t tag)
{
  auto progressRing = winrt::ProgressRing();
  return progressRing;
}

void ActivityIndicatorViewManager::UpdateProperties(ShadowNodeBase* nodeToUpdate, const folly::dynamic& reactDiffMap)
{
  auto progressRing = nodeToUpdate->GetView().as<winrt::ProgressRing>();
  if (progressRing == nullptr)
    return;

  for (const auto& pair : reactDiffMap.items())
  {
    const std::string& propertyName = pair.first.getString();
    const folly::dynamic& propertyValue = pair.second;

    if (propertyName == "animating")
    {
      if (propertyValue.isBool())
        progressRing.IsActive(propertyValue.asBool());
      else if (pair.second.isNull())
        progressRing.ClearValue(winrt::ProgressRing::IsActiveProperty());
    }
  }

  Super::UpdateProperties(nodeToUpdate, reactDiffMap);
}

}}
