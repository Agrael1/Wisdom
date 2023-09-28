#pragma once
#include <wisdom/api/consts.h>
#include <wisdom/api/internal.h>
#include <wisdom/dx12/dx12_checks.h>
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

/// @brief Main Factory class
class DX12Factory : public QueryInternal<DX12Factory>
{
public:
    DX12Factory() = default;
    DX12Factory(bool debug_layer, DebugCallback callback) noexcept
    {
        Initialize(debug_layer, callback);
    }
    ~DX12Factory()
    {
        if (callback) {
            DX12Info::RemoveCallback(callback);
        }
    }

    wis::Result Initialize(bool debug_layer = false, DebugCallback callback = nullptr, void* user_data = nullptr) noexcept
    {
        if (debug_layer)
            EnableDebugLayer(callback, user_data);

        auto hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG,
                                     __uuidof(IDXGIFactory4), factory.put_void());

        if (!wis::succeeded(hr))
            return MakeResult<FUNC, "Failed to create DX12 factory">(hr);

        has_preference = bool(factory.as<IDXGIFactory6>());
        return wis::success;
    }

    operator bool() const noexcept
    {
        return bool(factory);
    }

private:
    void EnableDebugLayer(DebugCallback callback, void* user_data) noexcept
    {
        if (callback) {
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
    DebugCallback callback = nullptr;
};
} // namespace wis
