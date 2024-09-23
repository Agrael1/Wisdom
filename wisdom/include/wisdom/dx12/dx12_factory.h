#ifndef DX12_FACTORY_H
#define DX12_FACTORY_H
#include <wisdom/generated/dx12/dx12_structs.hpp>
#include <wisdom/global/definitions.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_debug.h>
#include <wisdom/dx12/dx12_factory_ext.h>

namespace wis {
class DX12Factory;

template<>
struct Internal<DX12Factory> {
    wis::com_ptr<IDXGIFactory6> factory;
};

/// @brief Main Factory class
class ImplDX12Factory : public QueryInternal<DX12Factory>
{
    friend wis::ResultValue<wis::DX12Factory>
    ImplDX12CreateFactory(bool enable_debug, DX12FactoryExtension** extensions, size_t extension_count) noexcept;

public:
    ImplDX12Factory() noexcept = default;
    WIS_INLINE explicit ImplDX12Factory(wis::com_ptr<IDXGIFactory6> factory) noexcept;
    ImplDX12Factory(ImplDX12Factory&& other) noexcept = default;
    ImplDX12Factory& operator=(ImplDX12Factory&& other) noexcept = default;
    ImplDX12Factory(const ImplDX12Factory&) = delete;
    ImplDX12Factory& operator=(const ImplDX12Factory&) = delete;

    operator bool() const noexcept
    {
        return bool(factory);
    }
    operator DX12FactoryHandle() const noexcept
    {
        return factory.get();
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Adapter>
    GetAdapter(uint32_t index,
               AdapterPreference preference = AdapterPreference::Performance) const noexcept;

private:
    WIS_INLINE wis::com_with_result<IDXGIAdapter1> GetAdapterByGPUPreference(
        uint32_t index, DXGI_GPU_PREFERENCE preference = DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE) const noexcept;

    WIS_INLINE wis::com_with_result<IDXGIAdapter1> GetAdapter1(uint32_t index) const noexcept;

private:
    static inline bool has_preference = true;
};

#pragma region DX12Factory
/**
 * @brief Class for creating adapters.
 * Can be safely destroyed after adapter has been chosen.
 * */
struct DX12Factory : public wis::ImplDX12Factory {
public:
    using wis::ImplDX12Factory::ImplDX12Factory;

public:
    /**
     * @brief Creates the wis::DX12Adapter for the factory with provided index.
     * @param index The index of the adapter to get.
     * @param preference The preference of the adapter to get.
     * Default is wis::AdapterPreference::Performance.
     * @return wis::DX12Adapter on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12Adapter> GetAdapter(uint32_t index, wis::AdapterPreference preference = wis::AdapterPreference::Performance)
    {
        return wis::ImplDX12Factory::GetAdapter(index, preference);
    }
};
#pragma endregion DX12Factory

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Factory>
ImplDX12CreateFactory(bool enable_debug, DX12FactoryExtension** extensions, size_t extension_count) noexcept;

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_factory.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // DX12_FACTORY_H
