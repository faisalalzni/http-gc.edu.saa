// WARNING: Please don't edit this file. It was generated by C++/WinRT v1.0.190111.3

#pragma once

#include "winrt/base.h"


static_assert(winrt::check_version(CPPWINRT_VERSION, "1.0.190111.3"), "Mismatched component and base headers.");
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/impl/Windows.UI.Xaml.2.h"
#include "winrt/impl/Windows.UI.Xaml.Automation.Provider.2.h"
#include "winrt/impl/Windows.UI.Xaml.Automation.Peers.2.h"
#include "winrt/impl/react.uwp.2.h"

namespace winrt::impl {

template <typename D> react::uwp::DynamicAutomationPeer consume_react_uwp_IDynamicAutomationPeerFactory<D>::CreateInstance(Windows::UI::Xaml::FrameworkElement const& owner) const
{
    react::uwp::DynamicAutomationPeer value{ nullptr };
    check_hresult(WINRT_SHIM(react::uwp::IDynamicAutomationPeerFactory)->CreateInstance(get_abi(owner), put_abi(value)));
    return value;
}

template <typename D>
struct produce<D, react::uwp::IDynamicAutomationPeer> : produce_base<D, react::uwp::IDynamicAutomationPeer>
{};

template <typename D>
struct produce<D, react::uwp::IDynamicAutomationPeerFactory> : produce_base<D, react::uwp::IDynamicAutomationPeerFactory>
{
    int32_t WINRT_CALL CreateInstance(void* owner, void** value) noexcept final
    {
        try
        {
            *value = nullptr;
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(CreateInstance, WINRT_WRAP(react::uwp::DynamicAutomationPeer), Windows::UI::Xaml::FrameworkElement const&);
            *value = detach_from<react::uwp::DynamicAutomationPeer>(this->shim().CreateInstance(*reinterpret_cast<Windows::UI::Xaml::FrameworkElement const*>(&owner)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    }
};

}

WINRT_EXPORT namespace winrt::react::uwp {

inline DynamicAutomationPeer::DynamicAutomationPeer(Windows::UI::Xaml::FrameworkElement const& owner) :
    DynamicAutomationPeer(impl::call_factory<DynamicAutomationPeer, react::uwp::IDynamicAutomationPeerFactory>([&](auto&& f) { return f.CreateInstance(owner); }))
{}

}

namespace winrt::impl {

struct property_react_uwp_DynamicAutomationPeer
{ struct named {
    struct IsSelected
    {
        struct name { static constexpr std::wstring_view value{ L"IsSelected"sv }; };
        using property_type = bool;
        using target_type = winrt::react::uwp::DynamicAutomationPeer;

        using is_readable = std::true_type;
        using is_writable = std::false_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.IsSelected();
            }
        };
    };
    struct SelectionContainer
    {
        struct name { static constexpr std::wstring_view value{ L"SelectionContainer"sv }; };
        using property_type = winrt::Windows::UI::Xaml::Automation::Provider::IRawElementProviderSimple;
        using target_type = winrt::react::uwp::DynamicAutomationPeer;

        using is_readable = std::true_type;
        using is_writable = std::false_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.SelectionContainer();
            }
        };
    };
    struct CanSelectMultiple
    {
        struct name { static constexpr std::wstring_view value{ L"CanSelectMultiple"sv }; };
        using property_type = bool;
        using target_type = winrt::react::uwp::DynamicAutomationPeer;

        using is_readable = std::true_type;
        using is_writable = std::false_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.CanSelectMultiple();
            }
        };
    };
    struct IsSelectionRequired
    {
        struct name { static constexpr std::wstring_view value{ L"IsSelectionRequired"sv }; };
        using property_type = bool;
        using target_type = winrt::react::uwp::DynamicAutomationPeer;

        using is_readable = std::true_type;
        using is_writable = std::false_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.IsSelectionRequired();
            }
        };
    };};
    struct list { using type = impl::typelist<named::IsSelected, named::SelectionContainer, named::CanSelectMultiple, named::IsSelectionRequired>; };
};

}

WINRT_EXPORT namespace winrt::experimental::reflect {
template <> struct named_property<react::uwp::DynamicAutomationPeer> : impl::property_react_uwp_DynamicAutomationPeer::named {};
template <> struct properties<react::uwp::DynamicAutomationPeer> : impl::property_react_uwp_DynamicAutomationPeer::list {};

template <>
struct base_type<react::uwp::DynamicAutomationPeer> { using type = Windows::UI::Xaml::Automation::Peers::FrameworkElementAutomationPeer; };
}

WINRT_EXPORT namespace std {

template<> struct hash<winrt::react::uwp::IDynamicAutomationPeer> : winrt::impl::hash_base<winrt::react::uwp::IDynamicAutomationPeer> {};
template<> struct hash<winrt::react::uwp::IDynamicAutomationPeerFactory> : winrt::impl::hash_base<winrt::react::uwp::IDynamicAutomationPeerFactory> {};
template<> struct hash<winrt::react::uwp::DynamicAutomationPeer> : winrt::impl::hash_base<winrt::react::uwp::DynamicAutomationPeer> {};

}
