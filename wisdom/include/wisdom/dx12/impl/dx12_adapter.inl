//#include "../dx12_adapter.h"

void wis::DX12Adapter::GetDesc(wis::AdapterDesc& out_desc) const noexcept
{
    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);
    auto description = winrt::to_string(desc.Description);
    std::strncpy(out_desc.description, description.c_str(), sizeof(out_desc.description) - 1);
    out_desc.vendor_id = desc.VendorId;
    out_desc.device_id = desc.DeviceId;
    out_desc.subsys_id = desc.SubSysId;
    out_desc.revision = desc.Revision;

    out_desc.dedicated_video_memory = desc.DedicatedVideoMemory;
    out_desc.dedicated_system_memory = desc.DedicatedSystemMemory;
    out_desc.shared_system_memory = desc.SharedSystemMemory;
    out_desc.adapter_id = reinterpret_cast<uint64_t&>(desc.AdapterLuid);
    out_desc.flags = wisAdapterFlags(desc.Flags);
}
