#pragma once
#include <d3d12.h>
#include <wisdom/generated/dx12/dx12_structs.hpp>
#include <wisdom/global/definitions.h>
#include <wisdom/xdx12/dx12_adapter.h>
#include <wisdom/xdx12/dx12_info.h>

namespace wis {
class DX12Factory;

template<>
struct Internal<DX12Factory> {
    wis::com_ptr<IDXGIFactory6> factory;
};

WIS_INLINE [[nodiscard]] std::pair<wis::Result, wis::DX12Factory>
DX12CreateFactory(bool debug_layer = false, wis::DebugCallback callback = nullptr,
                  void* user_data = nullptr) noexcept;

/// @brief Main Factory class
class DX12Factory : public QueryInternal<DX12Factory>
{
    friend std::pair<wis::Result, wis::DX12Factory> DX12CreateFactory(bool, wis::DebugCallback,
                                                                      void*) noexcept;

public:
    DX12Factory() noexcept = default;
    WIS_INLINE explicit DX12Factory(wis::com_ptr<IDXGIFactory6> factory, bool debug_layer = false,
                                    wis::DebugCallback callback = nullptr,
                                    void* user_data = nullptr) noexcept;
    WIS_INLINE ~DX12Factory() noexcept;
    WIS_INLINE DX12Factory(DX12Factory&& other) noexcept;
    WIS_INLINE DX12Factory& operator=(DX12Factory&& other) noexcept;
    DX12Factory(const DX12Factory&) = delete;
    DX12Factory& operator=(const DX12Factory&) = delete;

    operator bool() const noexcept { return bool(factory); }
    operator DX12FactoryHandle() const noexcept { return factory.get(); }

public:
    WIS_INLINE [[nodiscard]] std::pair<wis::Result, wis::DX12Adapter>
    GetAdapter(uint32_t index,
               AdapterPreference preference = AdapterPreference::Performance) const noexcept;

private:
    WIS_INLINE void EnableDebugLayer(DebugCallback callback, void* user_data) noexcept;

    WIS_INLINE wis::com_with_result<IDXGIAdapter1> GetAdapterByGPUPreference(
            uint32_t index, DXGI_GPU_PREFERENCE preference = DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE) const noexcept;

    WIS_INLINE wis::com_with_result<IDXGIAdapter1> GetAdapter1(uint32_t index) const noexcept;

private:
    static inline bool has_preference = true;
    [[no_unique_address]] wis::DX12InfoToken token;
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/dx12_factory.cpp"
#endif // !WISDOM_HEADER_ONLY