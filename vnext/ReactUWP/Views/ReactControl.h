// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include <stdint.h>
#include <memory>
#include <set>

#include "IXamlRootView.h"
#include <IReactInstance.h>
#include "TouchEventHandler.h"
#include "Views/KeyboardEventHandler.h"
#include <winrt/Windows.UI.Xaml.Controls.h>

namespace winrt {
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Media;
}

namespace react {
namespace uwp {

class ReactControl : public std::enable_shared_from_this<ReactControl>
{
public:
  ReactControl(IXamlRootView* parent, XamlView rootView)
    : m_pParent(parent),
    m_xamlRootView(rootView)
  {
  }

  virtual ~ReactControl();

  XamlView GetXamlView() const noexcept;
  std::shared_ptr<IReactInstance> GetReactInstance() const noexcept;
  void SetJSComponentName(std::string&& mainComponentName) noexcept;
  void SetInstanceCreator(const ReactInstanceCreator& instanceCreator) noexcept;
  void SetInitialProps(folly::dynamic&& initialProps) noexcept;

  void AttachRoot() noexcept;
  void DetachRoot() noexcept;
  void DetachInstance();
  void Reload(bool shouldRetireCurrentInstance);

  void ResetView() {}
  virtual std::string JSComponentName() const noexcept;
  virtual int64_t GetActualHeight() const;
  virtual int64_t GetActualWidth() const;
  int64_t GetTag() const { return m_rootTag; }
  void SetTag(int64_t tag) { m_rootTag = tag; }

private:
  void HandleInstanceError();
  void HandleInstanceErrorOnUIThread();

  IXamlRootView* m_pParent;

  std::string m_jsComponentName;
  std::shared_ptr<facebook::react::NativeModuleProvider> m_moduleProvider;
  folly::dynamic m_initialProps;
  std::shared_ptr<TouchEventHandler> m_touchEventHandler;
  std::shared_ptr<PreviewKeyboardEventHandlerOnRoot> m_previewKeyboardEventHandlerOnRoot;

  int64_t m_rootTag = -1;
  XamlView m_xamlRootView;
  ReactInstanceCreator m_instanceCreator;
  std::shared_ptr<IReactInstance> m_reactInstance;
  bool m_isAttached { false };
  LiveReloadCallbackCookie m_liveReloadCallbackCookie { 0 };
  ErrorCallbackCookie m_errorCallbackCookie { 0 };

  winrt::Grid m_redBoxGrid { nullptr };
  winrt::TextBlock m_errorTextBlock { nullptr };
};

}
}
