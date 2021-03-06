// WARNING: Please don't edit this file. It was generated by C++/WinRT v1.0.190111.3

#pragma once

#include "winrt/base.h"


static_assert(winrt::check_version(CPPWINRT_VERSION, "1.0.190111.3"), "Mismatched component and base headers.");
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/impl/Windows.Graphics.Effects.2.h"
#include "winrt/impl/Microsoft.UI.Composition.Effects.2.h"

namespace winrt::impl {

template <typename D> Microsoft::UI::Composition::Effects::CanvasEdgeBehavior consume_Microsoft_UI_Composition_Effects_IBorderEffect<D>::ExtendX() const
{
    Microsoft::UI::Composition::Effects::CanvasEdgeBehavior value{};
    check_hresult(WINRT_SHIM(Microsoft::UI::Composition::Effects::IBorderEffect)->get_ExtendX(put_abi(value)));
    return value;
}

template <typename D> void consume_Microsoft_UI_Composition_Effects_IBorderEffect<D>::ExtendX(Microsoft::UI::Composition::Effects::CanvasEdgeBehavior const& value) const
{
    check_hresult(WINRT_SHIM(Microsoft::UI::Composition::Effects::IBorderEffect)->put_ExtendX(get_abi(value)));
}

template <typename D> Microsoft::UI::Composition::Effects::CanvasEdgeBehavior consume_Microsoft_UI_Composition_Effects_IBorderEffect<D>::ExtendY() const
{
    Microsoft::UI::Composition::Effects::CanvasEdgeBehavior value{};
    check_hresult(WINRT_SHIM(Microsoft::UI::Composition::Effects::IBorderEffect)->get_ExtendY(put_abi(value)));
    return value;
}

template <typename D> void consume_Microsoft_UI_Composition_Effects_IBorderEffect<D>::ExtendY(Microsoft::UI::Composition::Effects::CanvasEdgeBehavior const& value) const
{
    check_hresult(WINRT_SHIM(Microsoft::UI::Composition::Effects::IBorderEffect)->put_ExtendY(get_abi(value)));
}

template <typename D> Windows::Graphics::Effects::IGraphicsEffectSource consume_Microsoft_UI_Composition_Effects_IBorderEffect<D>::Source() const
{
    Windows::Graphics::Effects::IGraphicsEffectSource source{ nullptr };
    check_hresult(WINRT_SHIM(Microsoft::UI::Composition::Effects::IBorderEffect)->get_Source(put_abi(source)));
    return source;
}

template <typename D> void consume_Microsoft_UI_Composition_Effects_IBorderEffect<D>::Source(Windows::Graphics::Effects::IGraphicsEffectSource const& source) const
{
    check_hresult(WINRT_SHIM(Microsoft::UI::Composition::Effects::IBorderEffect)->put_Source(get_abi(source)));
}

template <typename D>
struct produce<D, Microsoft::UI::Composition::Effects::IBorderEffect> : produce_base<D, Microsoft::UI::Composition::Effects::IBorderEffect>
{
    int32_t WINRT_CALL get_ExtendX(Microsoft::UI::Composition::Effects::CanvasEdgeBehavior* value) noexcept final
    {
        try
        {
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(ExtendX, WINRT_WRAP(Microsoft::UI::Composition::Effects::CanvasEdgeBehavior));
            *value = detach_from<Microsoft::UI::Composition::Effects::CanvasEdgeBehavior>(this->shim().ExtendX());
            return 0;
        }
        catch (...) { return to_hresult(); }
    }

    int32_t WINRT_CALL put_ExtendX(Microsoft::UI::Composition::Effects::CanvasEdgeBehavior value) noexcept final
    {
        try
        {
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(ExtendX, WINRT_WRAP(void), Microsoft::UI::Composition::Effects::CanvasEdgeBehavior const&);
            this->shim().ExtendX(*reinterpret_cast<Microsoft::UI::Composition::Effects::CanvasEdgeBehavior const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
    }

    int32_t WINRT_CALL get_ExtendY(Microsoft::UI::Composition::Effects::CanvasEdgeBehavior* value) noexcept final
    {
        try
        {
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(ExtendY, WINRT_WRAP(Microsoft::UI::Composition::Effects::CanvasEdgeBehavior));
            *value = detach_from<Microsoft::UI::Composition::Effects::CanvasEdgeBehavior>(this->shim().ExtendY());
            return 0;
        }
        catch (...) { return to_hresult(); }
    }

    int32_t WINRT_CALL put_ExtendY(Microsoft::UI::Composition::Effects::CanvasEdgeBehavior value) noexcept final
    {
        try
        {
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(ExtendY, WINRT_WRAP(void), Microsoft::UI::Composition::Effects::CanvasEdgeBehavior const&);
            this->shim().ExtendY(*reinterpret_cast<Microsoft::UI::Composition::Effects::CanvasEdgeBehavior const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
    }

    int32_t WINRT_CALL get_Source(void** source) noexcept final
    {
        try
        {
            *source = nullptr;
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(Source, WINRT_WRAP(Windows::Graphics::Effects::IGraphicsEffectSource));
            *source = detach_from<Windows::Graphics::Effects::IGraphicsEffectSource>(this->shim().Source());
            return 0;
        }
        catch (...) { return to_hresult(); }
    }

    int32_t WINRT_CALL put_Source(void* source) noexcept final
    {
        try
        {
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(Source, WINRT_WRAP(void), Windows::Graphics::Effects::IGraphicsEffectSource const&);
            this->shim().Source(*reinterpret_cast<Windows::Graphics::Effects::IGraphicsEffectSource const*>(&source));
            return 0;
        }
        catch (...) { return to_hresult(); }
    }
};

}

WINRT_EXPORT namespace winrt::Microsoft::UI::Composition::Effects {

}

namespace winrt::impl {

struct property_Microsoft_UI_Composition_Effects_IBorderEffect
{ struct named {
    struct ExtendX
    {
        struct name { static constexpr std::wstring_view value{ L"ExtendX"sv }; };
        using property_type = winrt::Microsoft::UI::Composition::Effects::CanvasEdgeBehavior;
        using target_type = winrt::Microsoft::UI::Composition::Effects::IBorderEffect;

        using is_readable = std::true_type;
        using is_writable = std::true_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.ExtendX();
            }
        };
        struct setter
        {
            template <typename Value>
            void operator()(target_type const& target, Value&& value) const
            {
                target.ExtendX(std::forward<Value>(value));
            }
        };
    };
    struct ExtendY
    {
        struct name { static constexpr std::wstring_view value{ L"ExtendY"sv }; };
        using property_type = winrt::Microsoft::UI::Composition::Effects::CanvasEdgeBehavior;
        using target_type = winrt::Microsoft::UI::Composition::Effects::IBorderEffect;

        using is_readable = std::true_type;
        using is_writable = std::true_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.ExtendY();
            }
        };
        struct setter
        {
            template <typename Value>
            void operator()(target_type const& target, Value&& value) const
            {
                target.ExtendY(std::forward<Value>(value));
            }
        };
    };
    struct Source
    {
        struct name { static constexpr std::wstring_view value{ L"Source"sv }; };
        using property_type = winrt::Windows::Graphics::Effects::IGraphicsEffectSource;
        using target_type = winrt::Microsoft::UI::Composition::Effects::IBorderEffect;

        using is_readable = std::true_type;
        using is_writable = std::true_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.Source();
            }
        };
        struct setter
        {
            template <typename Value>
            void operator()(target_type const& target, Value&& value) const
            {
                target.Source(std::forward<Value>(value));
            }
        };
    };};
    struct list { using type = impl::typelist<named::ExtendX, named::ExtendY, named::Source>; };
};

struct property_Microsoft_UI_Composition_Effects_BorderEffect
{ struct named {
    struct Source
    {
        struct name { static constexpr std::wstring_view value{ L"Source"sv }; };
        using property_type = winrt::Windows::Graphics::Effects::IGraphicsEffectSource;
        using target_type = winrt::Microsoft::UI::Composition::Effects::BorderEffect;

        using is_readable = std::true_type;
        using is_writable = std::true_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.Source();
            }
        };
        struct setter
        {
            template <typename Value>
            void operator()(target_type const& target, Value&& value) const
            {
                target.Source(std::forward<Value>(value));
            }
        };
    };
    struct ExtendY
    {
        struct name { static constexpr std::wstring_view value{ L"ExtendY"sv }; };
        using property_type = winrt::Microsoft::UI::Composition::Effects::CanvasEdgeBehavior;
        using target_type = winrt::Microsoft::UI::Composition::Effects::BorderEffect;

        using is_readable = std::true_type;
        using is_writable = std::true_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.ExtendY();
            }
        };
        struct setter
        {
            template <typename Value>
            void operator()(target_type const& target, Value&& value) const
            {
                target.ExtendY(std::forward<Value>(value));
            }
        };
    };
    struct ExtendX
    {
        struct name { static constexpr std::wstring_view value{ L"ExtendX"sv }; };
        using property_type = winrt::Microsoft::UI::Composition::Effects::CanvasEdgeBehavior;
        using target_type = winrt::Microsoft::UI::Composition::Effects::BorderEffect;

        using is_readable = std::true_type;
        using is_writable = std::true_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.ExtendX();
            }
        };
        struct setter
        {
            template <typename Value>
            void operator()(target_type const& target, Value&& value) const
            {
                target.ExtendX(std::forward<Value>(value));
            }
        };
    };
    struct Name
    {
        struct name { static constexpr std::wstring_view value{ L"Name"sv }; };
        using property_type = winrt::hstring;
        using target_type = winrt::Microsoft::UI::Composition::Effects::BorderEffect;

        using is_readable = std::true_type;
        using is_writable = std::true_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.Name();
            }
        };
        struct setter
        {
            template <typename Value>
            void operator()(target_type const& target, Value&& value) const
            {
                target.Name(std::forward<Value>(value));
            }
        };
    };};
    struct list { using type = impl::typelist<named::Source, named::ExtendY, named::ExtendX, named::Name>; };
};

}

WINRT_EXPORT namespace winrt::experimental::reflect {
template <> struct named_property<Microsoft::UI::Composition::Effects::IBorderEffect> : impl::property_Microsoft_UI_Composition_Effects_IBorderEffect::named {};
template <> struct properties<Microsoft::UI::Composition::Effects::IBorderEffect> : impl::property_Microsoft_UI_Composition_Effects_IBorderEffect::list {};
template <> struct named_property<Microsoft::UI::Composition::Effects::BorderEffect> : impl::property_Microsoft_UI_Composition_Effects_BorderEffect::named {};
template <> struct properties<Microsoft::UI::Composition::Effects::BorderEffect> : impl::property_Microsoft_UI_Composition_Effects_BorderEffect::list {};
template <> struct get_enumerator_names<Microsoft::UI::Composition::Effects::CanvasEdgeBehavior>
{
    static constexpr std::array<std::wstring_view, 3> value{{ 
        {L"Clamp", 5},
        {L"Wrap", 4},
        {L"Mirror", 6}, }};
};
template <> struct get_enumerator_values<Microsoft::UI::Composition::Effects::CanvasEdgeBehavior>
{
    static constexpr std::array<Microsoft::UI::Composition::Effects::CanvasEdgeBehavior, 3> value{{ 
        Microsoft::UI::Composition::Effects::CanvasEdgeBehavior::Clamp,
        Microsoft::UI::Composition::Effects::CanvasEdgeBehavior::Wrap,
        Microsoft::UI::Composition::Effects::CanvasEdgeBehavior::Mirror, }};
};

}

WINRT_EXPORT namespace std {

template<> struct hash<winrt::Microsoft::UI::Composition::Effects::IBorderEffect> : winrt::impl::hash_base<winrt::Microsoft::UI::Composition::Effects::IBorderEffect> {};
template<> struct hash<winrt::Microsoft::UI::Composition::Effects::BorderEffect> : winrt::impl::hash_base<winrt::Microsoft::UI::Composition::Effects::BorderEffect> {};

}
