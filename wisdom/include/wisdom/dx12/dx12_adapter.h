#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/global/definitions.h>
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_adapter.h>
#include <wisdom/dx12/dx12_views.h>

#include <winrt/base.h>
#include <dxgi.h>
#endif // !WISDOM_MODULES

namespace wis {
class DX12Adapter;

template<>
class Internal<DX12Adapter>
{
public:
    winrt::com_ptr<IDXGIAdapter1> adapter;
};

/// @brief DX12 physical adapter
WIS_EXPORT class DX12Adapter : public QueryInternal<DX12Adapter>
{
public:
    DX12Adapter() = default;
    explicit DX12Adapter(winrt::com_ptr<IDXGIAdapter1> adapter) noexcept
        : QueryInternal(std::move(adapter))
    {
    }
    operator DX12AdapterView() const noexcept
    {
        return adapter.get();
    }
    operator bool() const noexcept
    {
        return adapter != nullptr;
    }

public:
    /// @brief Get the adapter description
    /// @return Adapter Description
    [[nodiscard]] AdapterDesc
    GetDesc() const noexcept
    {
        AdapterDesc desc{};
        GetDesc(desc);
        return desc;
    }

    /// @brief Get the adapter description
    /// @return Adapter Description
    void GetDesc(AdapterDesc& out_desc) const noexcept
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
        out_desc.flags = wis::AdapterFlags(desc.Flags);
    }
};
} // namespace wis
