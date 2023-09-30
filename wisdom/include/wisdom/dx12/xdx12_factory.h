#pragma once
#include <wisdom/api/consts.h>
#include <wisdom/api/internal.h>
#include <wisdom/dx12/xdx12_checks.h>
#include <wisdom/dx12/xdx12_convert.h>
#include <wisdom/dx12/xdx12_info.h>
#include <wisdom/global/definitions.h>
#include <wisdom/util/error_messages.h>

#include <d3d12.h>
#include <dxgi1_6.h>

namespace wis {
class DX12Factory;

template<>
struct Internal<DX12Factory> {
    wis::com_ptr<IDXGIFactory4> factory;
};

inline std::pair<wis::Result, wis::DX12Factory> DX12CreateFactory(bool debug_layer = false, wis::DebugCallback callback = nullptr, void* user_data = nullptr) noexcept;

/// @brief Main Factory class
class DX12Factory : public QueryInternal<DX12Factory>
{
    friend std::pair<wis::Result, wis::DX12Factory> DX12CreateFactory(bool, wis::DebugCallback, void*) noexcept;

public:
    DX12Factory() noexcept = default;
    DX12Factory(wis::com_ptr<IDXGIFactory4> factory, bool debug_layer = false, wis::DebugCallback callback = nullptr, void* user_data = nullptr) noexcept
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
            if (callback) {
                DX12Info::RemoveCallback(callback);
                token.Release();
            }
        }
    }
    DX12Factory(const DX12Factory&) = delete;
    DX12Factory& operator=(const DX12Factory&) = delete;
    DX12Factory(DX12Factory&& other) noexcept
        : QueryInternal<DX12Factory>(std::move(other))
        , callback(std::exchange(other.callback, nullptr))
    {
    }
    DX12Factory& operator=(DX12Factory&& other) noexcept
    {
        QueryInternal<DX12Factory>::operator=(std::move(other));
        callback = std::exchange(other.callback, nullptr);
        return *this;
    }

public:
    void SetName(const char* name) noexcept
    {
        if constexpr (wis::debug_layer)
            factory->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
    }

    operator bool() const noexcept
    {
        return bool(factory);
    }

private:
    void EnableDebugLayer(DebugCallback callback, void* user_data) noexcept
    {
        if (callback) {
            token.Acquire();
            this->callback = callback;
            DX12Info::AddCallback(callback, user_data);
        }

        wis::com_ptr<ID3D12Debug> debugController;
        if (wis::succeeded(D3D12GetDebugInterface(__uuidof(*debugController), debugController.put_void())))
            debugController->EnableDebugLayer();

        if (auto dc = debugController.as<ID3D12Debug1>())
            dc->SetEnableGPUBasedValidation(true);
    }

private:
    static inline bool has_preference = true;
    wis::DebugCallback callback = nullptr;
    [[no_unique_address]] wis::DX12InfoToken token;
};

std::pair<wis::Result, wis::DX12Factory> DX12CreateFactory(bool debug_layer, wis::DebugCallback callback, void* user_data) noexcept
{
    wis::com_ptr<IDXGIFactory4> factory;
    auto hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG,
                                 __uuidof(*factory), factory.put_void());

    if (!wis::succeeded(hr)) {
        return { wis::make_result<FUNC, "Failed to create DXGI factory">(hr), wis::DX12Factory{} };
    }

    DX12Factory::has_preference = bool(factory.as<IDXGIFactory6>());
    return { wis::success, DX12Factory(std::move(factory), debug_layer, callback, user_data) };
}
} // namespace wis
