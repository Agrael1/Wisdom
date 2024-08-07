#ifndef DX12_FACTORY_H
#define DX12_FACTORY_H
#include <d3d12.h>
#include <wisdom/generated/dx12/dx12_structs.hpp>
#include <wisdom/global/definitions.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_info.h>
#include <wisdom/dx12/dx12_debug.h>
#include <wisdom/dx12/dx12_factory_ext.h>

namespace wis {
class DX12Factory;

template<>
struct Internal<DX12Factory> {
    wis::com_ptr<IDXGIFactory6> factory;
};

/// @brief Main Factory class
class DX12Factory : public QueryInternal<DX12Factory>
{
    friend wis::ResultValue<wis::DX12Factory> DX12CreateFactory(bool) noexcept;

public:
    DX12Factory() noexcept = default;
    WIS_INLINE explicit DX12Factory(wis::com_ptr<IDXGIFactory6> factory) noexcept;
    DX12Factory(DX12Factory&& other) noexcept = default;
    DX12Factory& operator=(DX12Factory&& other) noexcept = default;
    DX12Factory(const DX12Factory&) = delete;
    DX12Factory& operator=(const DX12Factory&) = delete;

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

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Factory>
DX12CreateFactory(bool enable_debug) noexcept;

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::DX12Factory>
DX12CreateFactoryWithExtensions(bool enable_debug, DX12FactoryExtension** extensions, size_t extension_count) noexcept;

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_factory.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // DX12_FACTORY_H
