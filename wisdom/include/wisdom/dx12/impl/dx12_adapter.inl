

wis::AdapterDesc wis::DX12Adapter::GetDesc() const noexcept
{
    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);

    return AdapterDesc{
        .description{ winrt::to_string(desc.Description) },
        .vendor_id = desc.VendorId,
        .device_id = desc.DeviceId,
        .subsys_id = desc.SubSysId,
        .revision = desc.Revision,

        .dedicated_video_memory = desc.DedicatedVideoMemory,
        .dedicated_system_memory = desc.DedicatedSystemMemory,
        .shared_system_memory = desc.SharedSystemMemory,
        .adapter_id{ reinterpret_cast<uint64_t &>(desc.AdapterLuid) },
        .flags = AdapterFlags(desc.Flags)
    };
}
