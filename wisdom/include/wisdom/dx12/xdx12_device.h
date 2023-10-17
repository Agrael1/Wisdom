#pragma once
#include <wisdom/dx12/xdx12_fence.h>
#include <wisdom/dx12/xdx12_allocator.h>

namespace wis {
class DX12Device;

template<>
struct Internal<DX12Device> {
    wis::com_ptr<ID3D12Device10> device;
    wis::com_ptr<IDXGIAdapter1> adapter;
    wis::com_ptr<IDXGIFactory4> factory;
};

class DX12Device : public QueryInternal<DX12Device>
{
public:
    DX12Device() noexcept = default;
    explicit DX12Device(wis::com_ptr<ID3D12Device10> device, wis::com_ptr<IDXGIAdapter1> adapter, wis::com_ptr<IDXGIFactory4> factory) noexcept
        : QueryInternal(std::move(device), std::move(adapter), std::move(factory))
    {
    }
    operator bool() const noexcept { return bool(device); }

public:
    [[nodicard]] WIS_INLINE wis::Result
    WaitForMultipleFences(const DX12FenceView* fences,
                          const uint64_t* values,
                          size_t count,
                          MutiWaitFlags wait_all = MutiWaitFlags::All,
                          uint64_t timeout = std::numeric_limits<uint64_t>::max()) const noexcept;

    [[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::DX12Fence> 
    CreateFence(uint64_t initial_value = 0ull) const noexcept;

    [[nodiscard]] WIS_INLINE std::pair<wis::Result, wis::DX12ResourceAllocator>
    CreateAllocator() const noexcept;
};

WIS_INLINE [[nodiscard]] std::pair<wis::Result, wis::DX12Device> 
DX12CreateDevice(wis::DX12FactoryHandle factory, wis::DX12AdapterHandle adapter) noexcept;
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include <wisdom/dx12/impl/dx12_device.h>
#endif // !WISDOM_HEADER_ONLY
