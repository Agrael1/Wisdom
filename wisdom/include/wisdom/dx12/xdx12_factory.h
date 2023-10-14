#pragma once
#include <wisdom/api/consts.h>
#include <wisdom/dx12/xdx12_checks.h>
#include <wisdom/dx12/xdx12_convert.h>
#include <wisdom/dx12/xdx12_info.h>
#include <wisdom/dx12/xdx12_adapter.h>
#include <wisdom/global/definitions.h>
#include <wisdom/util/error_messages.h>

#include <d3d12.h>

namespace wis {
class DX12Factory;

template<>
struct Internal<DX12Factory> {
    wis::com_ptr<IDXGIFactory6> factory;
};

inline std::pair<wis::Result, wis::DX12Factory> DX12CreateFactory(bool debug_layer = false, wis::DebugCallback callback = nullptr, void* user_data = nullptr) noexcept;

/// @brief Main Factory class
class DX12Factory : public QueryInternal<DX12Factory>
{
    friend std::pair<wis::Result, wis::DX12Factory> DX12CreateFactory(bool, wis::DebugCallback, void*) noexcept;

public:
    DX12Factory() noexcept = default;
    explicit DX12Factory(wis::com_ptr<IDXGIFactory6> factory, bool debug_layer = false, wis::DebugCallback callback = nullptr, void* user_data = nullptr) noexcept
        : QueryInternal(std::move(factory))
    {
        if constexpr (wis::debug_layer) {
            if (debug_layer && callback)
                EnableDebugLayer(callback, user_data);
        }
    }
    ~DX12Factory() noexcept
    {
        if constexpr (wis::debug_layer) {
            DX12Info::RemoveCallback(this);
            token.Release();
        }
    }
    DX12Factory(const DX12Factory&) = delete;
    DX12Factory& operator=(const DX12Factory&) = delete;
    DX12Factory(DX12Factory&& other) noexcept
        : QueryInternal<DX12Factory>(std::move(other))
    {
        if constexpr (wis::debug_layer) {
            if (DX12Info::RebindCallback(this, &other))
                token.Acquire();
        }
    }
    DX12Factory& operator=(DX12Factory&& other) noexcept
    {
        QueryInternal<DX12Factory>::operator=(std::move(other));
        if constexpr (wis::debug_layer) {
            if (DX12Info::RebindCallback(this, &other))
                token.Acquire();
        }
        return *this;
    }

public:
    operator bool() const noexcept
    {
        return bool(factory);
    }

    [[nodiscard]] std::pair<wis::Result, wis::DX12Adapter>
    GetAdapter(uint32_t index, AdapterPreference preference = AdapterPreference::Performance) const noexcept
    {
        auto gen = has_preference ? GetAdapterByGPUPreference(index, convert_dx(preference)) : GetAdapter1(index);
        return !gen
                ? std::pair{ wis::make_result<FUNC, "Failed to get adapter">(gen.result), wis::DX12Adapter{} }
                : std::pair{ wis::success, wis::DX12Adapter(std::move(gen.ptr)) };
    }

private:
    void
    EnableDebugLayer(DebugCallback callback, void* user_data) noexcept
    {
        if (callback) {
            token.Acquire();
            DX12Info::AddCallback(this, callback, user_data);
        }

        wis::com_ptr<ID3D12Debug> debugController;
        if (wis::succeeded(D3D12GetDebugInterface(__uuidof(*debugController), debugController.put_void())))
            debugController->EnableDebugLayer();

        if (auto dc = debugController.as<ID3D12Debug1>())
            dc->SetEnableGPUBasedValidation(true);
    }

    wis::com_with_result<IDXGIAdapter1>
    GetAdapterByGPUPreference(uint32_t index, DXGI_GPU_PREFERENCE preference = DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE) const noexcept
    {
        wis::com_ptr<IDXGIAdapter1> adapter;
        auto hr = factory->EnumAdapterByGpuPreference(index,
                                                      preference,
                                                      __uuidof(*adapter), adapter.put_void());
        return { hr, std::move(adapter) };
    }

    wis::com_with_result<IDXGIAdapter1>
    GetAdapter1(uint32_t index) const noexcept
    {
        wis::com_ptr<IDXGIAdapter1> adapter;
        auto hr = factory->EnumAdapters1(index, adapter.put());
        return { hr, std::move(adapter) };
    }

private:
    static inline bool has_preference = true;
    [[no_unique_address]] wis::DX12InfoToken token;
};

std::pair<wis::Result, wis::DX12Factory> wis::DX12CreateFactory(bool debug_layer, wis::DebugCallback callback, void* user_data) noexcept
{
    wis::com_ptr<IDXGIFactory6> factory;
    auto hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG,
                                 __uuidof(*factory), factory.put_void());

    if (!wis::succeeded(hr)) {
        hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG,
                                __uuidof(IDXGIFactory4), factory.put_void());
        DX12Factory::has_preference = false;
        if (!wis::succeeded(hr)) {
            return { wis::make_result<FUNC, "Failed to create DXGI factory">(hr), wis::DX12Factory{} };
        }
    }
    return { wis::success, DX12Factory(std::move(factory), debug_layer, callback, user_data) };
}
} // namespace wis
