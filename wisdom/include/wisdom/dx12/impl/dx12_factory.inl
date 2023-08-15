#ifndef WISDOM_MODULES
#include <wisdom/dx12/dx12_checks.h>
#endif

inline constexpr DXGI_GPU_PREFERENCE to_dxgi(wis::AdapterPreference pref) noexcept
{
    switch (pref) {
    default:
    case wis::AdapterPreference::None:
        return DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_UNSPECIFIED;
    case wis::AdapterPreference::MinConsumption:
        return DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_MINIMUM_POWER;
    case wis::AdapterPreference::Performance:
        return DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;
    }
}

wis::DX12Factory::DX12Factory(const ApplicationInfo& app_info) noexcept
{
    if (factory)
        return;
    EnableDebugLayer();
    wis::succeded(CreateDXGIFactory2(debug_flag,
                                     __uuidof(IDXGIFactory4), factory.put_void()));

    // TODO: consider constexpr
    has_preference = bool(factory.try_as<IDXGIFactory6>());
}

wis::generator<wis::DX12Adapter> wis::DX12Factory::EnumerateAdapters(wis::AdapterPreference preference) const noexcept
{
    auto gen = has_preference ? AdaptersByGPUPreference(to_dxgi(preference)) : Adapters();
    for (auto&& i : gen)
        co_yield DX12Adapter(i);
}

void wis::DX12Factory::EnableDebugLayer() noexcept
{
    if constexpr (wis::debug_mode) {
        winrt::com_ptr<ID3D12Debug> debugController;
        if (wis::succeded(D3D12GetDebugInterface(__uuidof(*debugController), debugController.put_void())))
            debugController->EnableDebugLayer();

        if (auto d1 = debugController.try_as<ID3D12Debug1>())
            d1->SetEnableGPUBasedValidation(true);
    }
}

wis::generator<winrt::com_ptr<IDXGIAdapter1>>
wis::DX12Factory::AdaptersByGPUPreference(DXGI_GPU_PREFERENCE preference) const noexcept
{
    winrt::com_ptr<IDXGIAdapter1> adapter;
    auto factory6 = factory.try_as<IDXGIFactory6>();
    uint32_t index = 0;

    while (wis::succeded(factory6->EnumAdapterByGpuPreference(index++,
                                                                   preference,
                                                                   __uuidof(IDXGIAdapter1), adapter.put_void()))) {
        co_yield std::move(adapter);
    }
}

wis::generator<winrt::com_ptr<IDXGIAdapter1>>
wis::DX12Factory::Adapters() const noexcept
{
    winrt::com_ptr<IDXGIAdapter1> adapter;
    uint32_t index = 0;

    while (wis::succeded(factory->EnumAdapters1(index++, adapter.put())))
        co_yield std::move(adapter);
}

