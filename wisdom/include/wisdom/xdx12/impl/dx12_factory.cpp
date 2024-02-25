#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xdx12/dx12_factory.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/xdx12/dx12_checks.h>

wis::DX12Factory::DX12Factory(wis::com_ptr<IDXGIFactory6> factory, bool debug_layer) noexcept
    : QueryInternal(std::move(factory), debug_layer)
{
    if (debug_layer)
        EnableDebugLayer();
}

[[nodiscard]] wis::ResultValue<wis::DX12Factory>
wis::DX12CreateFactory(bool debug_layer, wis::DebugCallback callback, void* user_data) noexcept
{
    wis::com_ptr<IDXGIFactory6> factory;
    auto hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG, __uuidof(*factory),
                                 factory.put_void());

    if (!wis::succeeded(hr)) {
        hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG, __uuidof(IDXGIFactory4),
                                factory.put_void());
        DX12Factory::has_preference = false;
        if (!wis::succeeded(hr)) {
            return wis::make_result<FUNC, "Failed to create DXGI factory">(hr);
        }
    }
    return DX12Factory(std::move(factory), debug_layer);
}

wis::ResultValue<wis::DX12Adapter>
wis::DX12Factory::GetAdapter(uint32_t index, AdapterPreference preference) const noexcept
{
    auto gen = has_preference ? GetAdapterByGPUPreference(index, convert_dx(preference))
                              : GetAdapter1(index);

    if (!gen)
        return wis::make_result<FUNC, "Failed to get adapter">(gen.result);

    return wis::DX12Adapter(std::move(gen.ptr));
}

wis::ResultValue<wis::DX12DebugMessenger>
wis::DX12Factory::CreateDebugMessenger(wis::DebugCallback callback, void* user_data) const noexcept
{
    if (!debug_layer)
        return wis::make_result<FUNC, "Debug layer is not enabled">(E_INVALIDARG);

    return wis::DX12DebugMessenger{
        DX12InfoToken{ true }, callback, user_data
    };
}

void wis::DX12Factory::EnableDebugLayer() noexcept
{
    wis::com_ptr<ID3D12Debug> debugController;
    if (wis::succeeded(
                D3D12GetDebugInterface(__uuidof(*debugController), debugController.put_void())))
        debugController->EnableDebugLayer();

    if (auto dc = debugController.as<ID3D12Debug1>())
        dc->SetEnableGPUBasedValidation(true);
}

wis::com_with_result<IDXGIAdapter1>
wis::DX12Factory::GetAdapterByGPUPreference(uint32_t index,
                                            DXGI_GPU_PREFERENCE preference) const noexcept
{
    wis::com_ptr<IDXGIAdapter1> adapter;
    auto hr = factory->EnumAdapterByGpuPreference(index, preference, __uuidof(*adapter),
                                                  adapter.put_void());
    return { hr, std::move(adapter) };
}

wis::com_with_result<IDXGIAdapter1>
wis::DX12Factory::GetAdapter1(uint32_t index) const noexcept
{
    wis::com_ptr<IDXGIAdapter1> adapter;
    auto hr = factory->EnumAdapters1(index, adapter.put());
    return { hr, std::move(adapter) };
}
