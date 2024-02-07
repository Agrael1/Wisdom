#pragma once
#include <wisdom/generated/api/api.h>
#include <wisdom/global/internal.h>
#include <wisdom/util/com_ptr.h>
#include <wisdom/xdx12/dx12_convert.h>
#include <wisdom/xdx12/dx12_views.h>

namespace wis {
class DX12Adapter;

template <> struct Internal<DX12Adapter> {
  wis::com_ptr<IDXGIAdapter1> adapter;
};

class DX12Adapter : public QueryInternal<DX12Adapter> {
public:
  DX12Adapter() noexcept = default;
  explicit DX12Adapter(wis::com_ptr<IDXGIAdapter1> adapter) noexcept
      : QueryInternal(std::move(adapter)) {}
  operator bool() const noexcept { return bool(adapter); }
  operator DX12AdapterHandle() const noexcept { return {adapter.get()}; }

public:
  void GetDesc(AdapterDesc *pout_desc) const noexcept {
    auto &out_desc = *pout_desc;

    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);
    auto description = wis::to_string(desc.Description);
    std::strncpy(const_cast<char *>(out_desc.description.data()), description.c_str(),
                 sizeof(out_desc.description) - 1);
    out_desc.vendor_id = desc.VendorId;
    out_desc.device_id = desc.DeviceId;
    out_desc.subsys_id = desc.SubSysId;
    out_desc.revision = desc.Revision;

    out_desc.dedicated_video_memory = desc.DedicatedVideoMemory;
    out_desc.dedicated_system_memory = desc.DedicatedSystemMemory;
    out_desc.shared_system_memory = desc.SharedSystemMemory;
    out_desc.adapter_id = reinterpret_cast<uint64_t &>(desc.AdapterLuid);
    out_desc.flags = wis::AdapterFlags(desc.Flags);
  }
};
} // namespace wis