#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_factory.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_info.h>
#include <wisdom/dx12/dx12_format.h>
#include <wisdom/bridge/generator.h>

#include <dxgi1_6.h>
#endif

namespace wis {
class DX12Factory;

template<>
class Internal<DX12Factory>
{
public:
    winrt::com_ptr<IDXGIFactory4> factory{};
};

/// @brief Main Factory class
WIS_EXPORT class DX12Factory : public QueryInternal<DX12Factory>
{
    static inline constexpr uint32_t debug_flag = wis::debug_mode * DXGI_CREATE_FACTORY_DEBUG;

public:
    /// @brief Creates a new factory
    /// @param app_info Application info, not used
    ///	@param use_preference Use the preference when enumerating adapters
    explicit DX12Factory([[maybe_unused]] const ApplicationInfo& app_info = {}) noexcept
    {
        EnableDebugLayer();
        wis::succeeded(CreateDXGIFactory2(debug_flag,
                                          __uuidof(IDXGIFactory4), factory.put_void()));

        // TODO: consider constexpr
        has_preference = bool(factory.try_as<IDXGIFactory6>());
    }
    operator DX12FactoryView() const noexcept
    {
        return factory.get();
    }
    operator bool() const noexcept
    {
        return bool(factory);
    }

public:
    /// @brief Enumerates all adapters on the system
    /// @param preference Preference to use when enumerating adapters, changes the order of the adapters
    /// @return coroutine that yields DX12Adapter
    [[nodiscard]] wis::generator<DX12Adapter>
    EnumerateAdapters(AdapterPreference preference = AdapterPreference::Performance) const noexcept
    {
        auto gen = has_preference ? AdaptersByGPUPreference(to_dxgi(preference)) : Adapters();
        for (auto&& i : gen)
            co_yield DX12Adapter(i);
    }

private:
    /// @brief enables the debug layer if debug mode is enabled by compiler
    void EnableDebugLayer() noexcept
    {
        if constexpr (wis::debug_mode) {
            winrt::com_ptr<ID3D12Debug> debugController;
            if (wis::succeeded(D3D12GetDebugInterface(__uuidof(*debugController), debugController.put_void())))
                debugController->EnableDebugLayer();

            if (auto d1 = debugController.try_as<ID3D12Debug1>())
                d1->SetEnableGPUBasedValidation(true);
        }
    }

    /// @brief Enumerates all adapters on the system by GPU preference
    /// @param preference Specifies the GPU preference
    /// @return Coroutine that yields IDXGIAdapter1
    wis::generator<winrt::com_ptr<IDXGIAdapter1>>
    AdaptersByGPUPreference(DXGI_GPU_PREFERENCE preference = DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE) const noexcept
    {
        winrt::com_ptr<IDXGIAdapter1> adapter;
        auto factory6 = factory.try_as<IDXGIFactory6>();
        uint32_t index = 0;

        while (wis::succeeded(factory6->EnumAdapterByGpuPreference(index++,
                                                                   preference,
                                                                   __uuidof(IDXGIAdapter1), adapter.put_void()))) {
            co_yield std::move(adapter);
        }
    }

    /// @brief Enumerates all adapters on the system
    /// @return Coroutine that yields IDXGIAdapter1
    wis::generator<winrt::com_ptr<IDXGIAdapter1>>
    Adapters() const noexcept
    {
        winrt::com_ptr<IDXGIAdapter1> adapter;
        uint32_t index = 0;

        while (wis::succeeded(factory->EnumAdapters1(index++, adapter.put())))
            co_yield std::move(adapter);
    }

private:
    DX12Info info; //< infromation queue for debugging and error messages
    static inline bool has_preference = true;
};
} // namespace wis
