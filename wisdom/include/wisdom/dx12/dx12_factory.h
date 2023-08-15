#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_factory.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_info.h>
#include <wisdom/bridge/generator.h>

#include <dxgi1_6.h>
#endif

namespace wis {
class DX12Factory;

template<>
struct Internal<DX12Factory> {
    winrt::com_ptr<IDXGIFactory4> factory{};
};

/// @brief Main Factory class, since we don't need more than one factory it is a static resource
/// @note Not thread safe on creation
WIS_EXPORT class DX12Factory : public QueryInternal<DX12Factory>
{
    static inline constexpr uint32_t debug_flag = wis::debug_mode & DXGI_CREATE_FACTORY_DEBUG;

public:
    /// @brief Creates a new factory
    /// @param app_info Application info, not used
    ///	@param use_preference Use the preference when enumerating adapters
    WIS_INLINE DX12Factory([[maybe_unused]] const ApplicationInfo& app_info = {}) noexcept;
    operator bool() const noexcept
    {
        return bool(factory);
    }
    operator DX12FactoryHandle() const noexcept
	{
		return factory.get();
	}

public:
    /// @brief Enumerates all adapters on the system
    /// @param preference Preference to use when enumerating adapters, changes the order of the adapters
    /// @return coroutine that yields DX12Adapter
    WIS_INLINE [[nodiscard]] wis::generator<DX12Adapter>
    EnumerateAdapters(AdapterPreference preference = AdapterPreference::Performance) const noexcept;

private:
    /// @brief enables the debug layer if debug mode is enabled by compiler
    WIS_INLINE void EnableDebugLayer() noexcept;

    /// @brief Enumerates all adapters on the system by GPU preference
    /// @param preference Specifies the GPU preference
    /// @return Coroutine that yields IDXGIAdapter1
    WIS_INLINE wis::generator<winrt::com_ptr<IDXGIAdapter1>>
    AdaptersByGPUPreference(DXGI_GPU_PREFERENCE preference = DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE) const noexcept;

    /// @brief Enumerates all adapters on the system
    /// @return Coroutine that yields IDXGIAdapter1
    WIS_INLINE wis::generator<winrt::com_ptr<IDXGIAdapter1>>
    Adapters() const noexcept;

private:
    DX12Info info; //< infromation queue for debugging and error messages
    static inline bool has_preference = true;
};
} // namespace wis

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_factory.inl"
#endif
