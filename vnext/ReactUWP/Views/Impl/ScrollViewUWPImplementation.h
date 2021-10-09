#pragma once

#include "pch.h"

#include "SnapPointManagingContentControl.h"

#include <Utils/PropertyUtils.h>
#include <Utils/ValueUtils.h>

#include <IReactInstance.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Text.h>
#include <winrt/Windows.UI.Xaml.Controls.h>

namespace winrt {
  using namespace Windows::Foundation;
  using namespace Windows::Foundation::Collections;
  using namespace Windows::UI::Xaml;
  using namespace Windows::UI::Xaml::Controls;
  using namespace Windows::UI::Xaml::Controls::Primitives;
}

namespace react {
namespace uwp {

class ScrollViewUWPImplementation
{
public:
  ScrollViewUWPImplementation(const winrt::ScrollViewer& scrollViewer);

  void SetHorizontal(bool isHorizontal);
  void SnapToInterval(float interval);
  void SnapToOffsets(const winrt::IVectorView<float>& offsets);

  void UpdateScrollableSize() const;

  winrt::ScrollViewer ScrollViewer() const;
  winrt::com_ptr<SnapPointManagingContentControl> ScrollViewerSnapPointManager() const;

private:
  winrt::weak_ref<winrt::ScrollViewer> m_scrollViewer{};
};

} }
