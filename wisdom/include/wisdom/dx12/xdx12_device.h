#pragma once
#include <wisdom/api/consts.h>
#include <wisdom/api/internal.h>
#include <wisdom/dx12/xdx12_checks.h>
#include <wisdom/util/com_ptr.h>
#include <wisdom/dx12/xdx12_views.h>

#include <d3d12.h>

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
};

WIS_INLINE [[nodiscard]] std::pair<wis::Result, wis::DX12Device> 
DX12CreateDevice(wis::DX12FactoryHandle factory, wis::DX12AdapterHandle adapter) noexcept;
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include <wisdom/dx12/impl/dx12_device.h>
#endif // !WISDOM_HEADER_ONLY
