#ifndef WIS_DX12_DEVICE_EXT_H
#define WIS_DX12_DEVICE_EXT_H

#include <wisdom/generated/api/api.hpp>

namespace wis {
class DX12Device;
}

namespace wis {
struct DX12DeviceExtension {
    virtual ~DX12DeviceExtension() = default;
    virtual wis::Result Init(const wis::DX12Device& instance) noexcept
    {
        return {};
    }
    virtual bool Supported() const noexcept
    {
        return true; // most of the time, it is supported and embedded in device. Needs to be queried after creation.
    }
};
} // namespace wis

#endif // DX12_DEVICE_EXT_H
