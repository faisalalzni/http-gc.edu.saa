// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"

#include "FlyoutViewManager.h"
#include <Views/ShadowNodeBase.h>
#include "TouchEventHandler.h"
#include "ViewPanel.h"

#include <Modules/NativeUIManager.h>
#include <Utils/PropertyHandlerUtils.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>

namespace winrt {
  using namespace Windows::UI::Xaml::Controls::Primitives;
  using namespace Windows::UI::Xaml::Interop;
}

template<>
struct json_type_traits<winrt::FlyoutPlacementMode>
{
  static winrt::FlyoutPlacementMode parseJson(const folly::dynamic& json)
  {
    winrt::FlyoutPlacementMode placement;
    if (json == "top")
    {
      placement = winrt::FlyoutPlacementMode::Top;
    }
    else if (json == "bottom")
    {
      placement = winrt::FlyoutPlacementMode::Bottom;
    }
    else if (json == "left")
    {
      placement = winrt::FlyoutPlacementMode::Left;
    }
    else if (json == "right")
    {
      placement = winrt::FlyoutPlacementMode::Right;
    }
    else if (json == "top-edge-aligned-left")
    {
      placement = winrt::FlyoutPlacementMode::TopEdgeAlignedLeft;
    }
    else if (json == "top-edge-aligned-right")
    {
      placement = winrt::FlyoutPlacementMode::TopEdgeAlignedRight;
    }
    else if (json == "bottom-edge-aligned-left")
    {
      placement = winrt::FlyoutPlacementMode::BottomEdgeAlignedLeft;
    }
    else if (json == "bottom-edge-aligned-right")
    {
      placement = winrt::FlyoutPlacementMode::BottomEdgeAlignedRight;
    }
    else if (json == "left-edge-aligned-top")
    {
      placement = winrt::FlyoutPlacementMode::LeftEdgeAlignedTop;
    }
    else if (json == "right-edge-aligned-top")
    {
      placement = winrt::FlyoutPlacementMode::RightEdgeAlignedTop;
    }
    else if (json == "left-bottom")
    {
      placement = winrt::FlyoutPlacementMode::LeftEdgeAlignedBottom;
    }
    else if (json == "right-edge-aligned-bottom")
    {
      placement = winrt::FlyoutPlacementMode::RightEdgeAlignedBottom;
    }
    else if (json == "full")
    {
      placement = winrt::FlyoutPlacementMode::Full;
    }
    else
    {
    placement = winrt::FlyoutPlacementMode::Top;
    }

    return placement;
  }
};

namespace react { namespace uwp {

class FlyoutShadowNode : public ShadowNodeBase
{
  using Super = ShadowNodeBase;
public:
  FlyoutShadowNode() = default;
  virtual ~FlyoutShadowNode();

  void AddView(ShadowNode& child, int64_t index) override;
  void createView() override;
  static void OnFlyoutClosed(IReactInstance& instance, int64_t tag, bool newValue);
  void onDropViewInstance() override;
  void removeAllChildren() override;
  void updateProperties(const folly::dynamic&& props) override;
  
private:
  void SetTargetFrameworkElement();
  void AdjustDefaultFlyoutStyle();
  winrt::Popup GetFlyoutParentPopup() const;

  winrt::FrameworkElement m_targetElement = nullptr;
  winrt::Flyout m_flyout = nullptr;
  bool m_isLightDismissEnabled = true;
  bool m_isOpen = false;
  int64_t m_targetTag = -1;
  float m_horizontalOffset = 0;
  float m_verticalOffset = 0;
  bool m_isFlyoutShowOptionsSupported = false;
  winrt::FlyoutShowOptions m_showOptions = nullptr;

  std::unique_ptr<TouchEventHandler> m_touchEventHanadler;
  std::unique_ptr<PreviewKeyboardEventHandlerOnRoot> m_previewKeyboardEventHandlerOnRoot;
};

FlyoutShadowNode::~FlyoutShadowNode()
{
  m_touchEventHanadler->RemoveTouchHandlers();
  m_previewKeyboardEventHandlerOnRoot->unhook();
}

void FlyoutShadowNode::AddView(ShadowNode& child, int64_t index)
{
  auto childView = static_cast<ShadowNodeBase&>(child).GetView();
  m_touchEventHanadler->AddTouchHandlers(childView);
  m_previewKeyboardEventHandlerOnRoot->hook(childView);

  if (m_flyout != nullptr)
    m_flyout.Content(childView.as<winrt::UIElement>());
}

void FlyoutShadowNode::createView()
{
  Super::createView();

  m_flyout = winrt::Flyout();
  m_isFlyoutShowOptionsSupported = !!(m_flyout.try_as<winrt::IFlyoutBase5>());

  if (m_isFlyoutShowOptionsSupported)
    m_showOptions = winrt::FlyoutShowOptions();

  auto wkinstance = GetViewManager()->GetReactInstance();
  m_touchEventHanadler = std::make_unique<TouchEventHandler>(wkinstance);
  m_previewKeyboardEventHandlerOnRoot = std::make_unique<PreviewKeyboardEventHandlerOnRoot>(wkinstance);

  m_flyout.Closing([=](winrt::FlyoutBase /*flyoutbase*/, winrt::FlyoutBaseClosingEventArgs args)
  {
    auto instance = wkinstance.lock();
    if (!m_updating && instance != nullptr && !m_isLightDismissEnabled && m_isOpen)
    {
      args.Cancel(true);
    }
  });

  m_flyout.Closed([=](auto&&, auto&&)
  {
    auto instance = wkinstance.lock();
    if (!m_updating && instance != nullptr)
      OnFlyoutClosed(*instance, m_tag, false);
  });

  // Set XamlRoot on the Flyout to handle XamlIsland/AppWindow scenarios.
  if (auto flyoutBase6 = m_flyout.try_as<winrt::IFlyoutBase6>())
  {
    if (auto instance = wkinstance.lock())
    {
      if (auto xamlRoot = static_cast<NativeUIManager*>(instance->NativeUIManager())->tryGetXamlRoot())
      {
        flyoutBase6.XamlRoot(xamlRoot);
      }
    }
  }
}

/*static*/ void FlyoutShadowNode::OnFlyoutClosed(IReactInstance& instance, int64_t tag, bool newValue)
{
  folly::dynamic eventData = folly::dynamic::object("target", tag)("isOpen", newValue);
  instance.DispatchEvent(tag, "topDismiss", std::move(eventData));
}

void FlyoutShadowNode::onDropViewInstance() {
  m_isOpen = false;
  m_flyout.Hide();
}

void FlyoutShadowNode::removeAllChildren()
{
  m_flyout.ClearValue(winrt::Flyout::ContentProperty());
}

void FlyoutShadowNode::updateProperties(const folly::dynamic&& props)
{
  m_updating = true;
  bool updateTargetElement = false;
  bool updateIsOpen = false;
  bool updateOffset = false;

  if (m_flyout == nullptr)
    return;

  for (auto& pair : props.items())
  {
    const std::string& propertyName = pair.first.getString();
    const folly::dynamic& propertyValue = pair.second;

    if (propertyName == "horizontalOffset")
    {
      if (propertyValue.isNumber())
        m_horizontalOffset = static_cast<float>(propertyValue.asDouble());
      else
        m_horizontalOffset = 0;

      updateOffset = true;
    }
    else if (propertyName == "isLightDismissEnabled")
    {
      if (propertyValue.isBool())
        m_isLightDismissEnabled = propertyValue.asBool();
      else if (propertyValue.isNull())
        m_isLightDismissEnabled = true;
      if (m_isOpen)
      {
        auto popup = GetFlyoutParentPopup();
        if (popup != nullptr)
          popup.IsLightDismissEnabled(m_isLightDismissEnabled);
      }
    }
    else if (propertyName == "isOpen")
    {
      if (propertyValue.isBool())
      {
        m_isOpen = propertyValue.asBool();
        updateIsOpen = true;
      }
    }
    else if (propertyName == "placement")
    {
      auto placement = json_type_traits<winrt::FlyoutPlacementMode>::parseJson(propertyValue);
      m_flyout.Placement(placement);
    }
    else if (propertyName == "target")
    {
      if (propertyValue.isNumber())
      {
        m_targetTag = static_cast<int64_t>(propertyValue.asDouble());
        updateTargetElement = true;
      }
      else
        m_targetTag = -1;
    }
    else if (propertyName == "verticalOffset")
    {
      if (propertyValue.isNumber())
        m_verticalOffset = static_cast<float>(propertyValue.asDouble());
      else
        m_verticalOffset = 0;

      updateOffset = true;
    }
  }

  if (updateTargetElement || m_targetElement == nullptr)
  {
    SetTargetFrameworkElement();
    winrt::FlyoutBase::SetAttachedFlyout(m_targetElement, m_flyout);
  }

  if (updateOffset && m_isFlyoutShowOptionsSupported)
  {
    winrt::Point newPoint(m_horizontalOffset, m_verticalOffset);
    m_showOptions.Position(newPoint);
  }

  if (m_isFlyoutShowOptionsSupported)
  {
    winrt::Rect exclusionRect = winrt::Rect(100, 100, 20, 20);
    m_showOptions.ExclusionRect(exclusionRect);
  }

  if (updateIsOpen)
  {
    if (m_isOpen)
    {
      AdjustDefaultFlyoutStyle();
      if (m_isFlyoutShowOptionsSupported)
      {
        m_flyout.ShowAt(m_targetElement, m_showOptions);
      }
      else
      {
        winrt::FlyoutBase::ShowAttachedFlyout(m_targetElement);
      }

      auto popup = GetFlyoutParentPopup();
      if (popup != nullptr)
        popup.IsLightDismissEnabled(m_isLightDismissEnabled);
    }
    else
    {
      m_flyout.Hide();
    }
  }

  // TODO: hook up view props to the flyout (m_flyout) instead of setting them on the dummy view.
  //Super::updateProperties(std::move(props));
  m_updating = false;
}

void FlyoutShadowNode::SetTargetFrameworkElement()
{
  auto wkinstance = GetViewManager()->GetReactInstance();
  auto instance = wkinstance.lock();

  if (instance == nullptr)
    return;

  if (m_targetTag > 0)
  {
    auto pNativeUIManagerHost = static_cast<NativeUIManager*>(instance->NativeUIManager())->getHost();
    ShadowNodeBase* pShadowNodeChild = static_cast<ShadowNodeBase*>(pNativeUIManagerHost->FindShadowNodeForTag(m_targetTag));

    if (pShadowNodeChild != nullptr)
    {
      auto targetView = pShadowNodeChild->GetView();
      m_targetElement = targetView.as<winrt::FrameworkElement>();
    }
  }
  else
  {
    m_targetElement = winrt::Window::Current().Content().as<winrt::FrameworkElement>();
  }
}

void FlyoutShadowNode::AdjustDefaultFlyoutStyle()
{
  winrt::Style flyoutStyle({ L"Windows.UI.Xaml.Controls.FlyoutPresenter", winrt::TypeKind::Metadata });
  flyoutStyle.Setters().Append(winrt::Setter(winrt::FrameworkElement::MaxWidthProperty(), winrt::box_value(50000)));
  flyoutStyle.Setters().Append(winrt::Setter(winrt::FrameworkElement::MaxHeightProperty(), winrt::box_value(50000)));
  flyoutStyle.Setters().Append(winrt::Setter(winrt::Control::PaddingProperty(), winrt::box_value(0)));
  m_flyout.FlyoutPresenterStyle(flyoutStyle);
}

winrt::Popup FlyoutShadowNode::GetFlyoutParentPopup() const
{
  // TODO: Use VisualTreeHelper::GetOpenPopupsFromXamlRoot when running against RS6
  winrt::Windows::Foundation::Collections::IVectorView<winrt::Popup> popups = winrt::VisualTreeHelper::GetOpenPopups(winrt::Window::Current());
  if (popups.Size() > 0)
    return popups.GetAt(0);
  return nullptr;
}

FlyoutViewManager::FlyoutViewManager(const std::shared_ptr<IReactInstance>& reactInstance)
  : Super(reactInstance)
{
}

const char* FlyoutViewManager::GetName() const
{
  return "RCTFlyout";
}

XamlView FlyoutViewManager::CreateViewCore(int64_t tag)
{
  return ViewPanel::Create().as<XamlView>();
}

facebook::react::ShadowNode* FlyoutViewManager::createShadow() const
{
  return new FlyoutShadowNode();
}

folly::dynamic FlyoutViewManager::GetNativeProps() const
{
  auto props = Super::GetNativeProps();

  props.update(folly::dynamic::object
    ("horizontalOffset", "number")
    ("isLightDismissEnabled", "boolean")
    ("isOpen", "boolean")
    ("placement", "number")
    ("target", "number")
    ("verticalOffset", "number")
  );

  return props;
}

folly::dynamic FlyoutViewManager::GetExportedCustomDirectEventTypeConstants() const
{
  auto directEvents = Super::GetExportedCustomDirectEventTypeConstants();
  directEvents["topDismiss"] = folly::dynamic::object("registrationName", "onDismiss");

  return directEvents;
}

void FlyoutViewManager::SetLayoutProps(ShadowNodeBase& nodeToUpdate, XamlView viewToUpdate, float left, float top, float width, float height)
{
}

}}
