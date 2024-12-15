#ifndef WIS_DX12_ADAPTER_H
#define WIS_DX12_ADAPTER_H
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/global/internal.h>

namespace wis {
class DX12Adapter;

template<>
struct Internal<DX12Adapter> {
    wis::com_ptr<IDXGIAdapter1> adapter;
};

class ImplDX12Adapter : public QueryInternal<DX12Adapter>
{
public:
    ImplDX12Adapter() noexcept = default;
    operator bool() const noexcept
    {
        return bool(adapter);
    }

public:
    [[nodiscard]] wis::Result GetDesc(AdapterDesc* pout_desc) const noexcept
    {
        if (!pout_desc) {
            return wis::make_result<FUNC, "AdapterDesc was nullptr">(E_INVALIDARG);
        }

        auto& out_desc = *pout_desc;

        DXGI_ADAPTER_DESC1 desc;
        auto hr = adapter->GetDesc1(&desc);
        if (!wis::succeeded(hr)) {
            return wis::make_result<FUNC, "IDXGIAdapter1::GetDesc1 failed">(hr);
        }

        auto description = wis::to_string(desc.Description);
        std::strncpy(const_cast<char*>(out_desc.description.data()), description.c_str(),
                     sizeof(out_desc.description) - 1);
        out_desc.vendor_id = desc.VendorId;
        out_desc.device_id = desc.DeviceId;
        out_desc.subsys_id = desc.SubSysId;
        out_desc.revision = desc.Revision;

        out_desc.dedicated_video_memory = desc.DedicatedVideoMemory;
        out_desc.dedicated_system_memory = desc.DedicatedSystemMemory;
        out_desc.shared_system_memory = desc.SharedSystemMemory;
        out_desc.adapter_id = reinterpret_cast<uint64_t&>(desc.AdapterLuid);
        out_desc.flags = wis::AdapterFlags(desc.Flags);
        return wis::success;
    }
};
#pragma region DX12Adapter
/**
 * @brief Represents physical device.
 * Can safely be deleted once logical device has been created.
 * */
class DX12Adapter : public wis::ImplDX12Adapter
{
public:
    using wis::ImplDX12Adapter::ImplDX12Adapter;

public:
    /**
     * @brief Fills wis::AdapterDesc with physical adapter's data.
     * @param inout_desc The wis::AdapterDesc to fill.
     * Must not be NULL.
     * */
    [[nodiscard]] inline wis::Result GetDesc(wis::AdapterDesc* inout_desc) const noexcept
    {
        return wis::ImplDX12Adapter::GetDesc(inout_desc);
    }
};
#pragma endregion DX12Adapter
} // namespace wis

#endif // DX12_ADAPTER_H
