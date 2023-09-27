#pragma once
#include <wisdom/api/api.h>
#include <wisdom/api/internal.h>
#include <wisdom/global/definitions.h>
#include "dx12_checks.h"
#include <wisdom/dx12/xdx12_convert.h>
#include <wisdom/util/error_messages.h>

#include <dxgi1_6.h>
#include <winrt/base.h>

namespace wis {
class DX12Factory;

template<>
struct Internal<DX12Factory> {
    winrt::com_ptr<IDXGIFactory4> factory;
};

/// @brief Main Factory class
class DX12Factory : public QueryInternal<DX12Factory>
{
public:
    DX12Factory() = default;
    DX12Factory(bool debug_layer) noexcept
    {
        Initialize(debug_layer);
    }

    wis::Result Initialize(bool debug_layer = false) noexcept
    {

        if (auto hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG,
                                         __uuidof(IDXGIFactory4), factory.put_void());
            !wis::succeeded(hr))
            return {
                wis::convert(hr), wis::MakeErrorString<__func__>(1, "Failed to create DX12 factory"_sl)
            };
        constexpr auto a = sl_to_sl();
        return {
            wis::Status::Error, wis::MakeErrorString<__FUNCSIG__>(1, "Test the caps of the result"_sl)
        };


        has_preference = bool(factory.try_as<IDXGIFactory6>());
    }

    operator bool() const noexcept
    {
        return bool(factory);
    }

private:
    static inline bool has_preference = true;
};
} // namespace wis
