#ifndef WIS_DX12_EXTENSIONS_HPP
#define WIS_DX12_EXTENSIONS_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

#include <wisdom/dx12/dx12_types.hpp>
#include <wisdom/generated/c_api.h>

#include <cassert>

namespace wis {
namespace impl {
struct DX12InstanceImpl;
struct DX12DeviceImpl;
} // namespace impl

template <typename T>
struct DX12InstanceExtensionImpl : public DX12InstanceExtensionHeader {
    DX12InstanceExtensionImpl() noexcept
        : DX12InstanceExtensionHeader{&DX12InstanceExtensionImpl<T>::InitThunk}
    {
        assert(
            static_cast<std::uintptr_t>(static_cast<T*>(this)) ==
                reinterpret_cast<std::uintptr_t>(static_cast<DX12InstanceExtensionHeader*>(this)) &&
            "DX12InstanceExtensionImpl must be the first base class!"
        );
    }

private:
    static WisResult InitThunk(DX12InstanceExtensionHeader* self, const impl::DX12InstanceImpl& instance_impl) noexcept
    {
        return reinterpret_cast<T*>(self)->Init(instance_impl);
    }

public:
    WisResult Init(const impl::DX12InstanceImpl& instance_impl) noexcept
    {
        (void)instance_impl;
        return {};
    }
};

template <typename T>
struct DX12DeviceExtensionImpl : public DX12DeviceExtensionHeader {
    DX12DeviceExtensionImpl() noexcept
        : DX12DeviceExtensionHeader{&DX12DeviceExtensionImpl<T>::InitThunk}
    {
        assert(
            static_cast<std::uintptr_t>(static_cast<T*>(this)) ==
                reinterpret_cast<std::uintptr_t>(static_cast<DX12DeviceExtensionHeader*>(this)) &&
            "DX12DeviceExtensionImpl must be the first base class!"
        );
    }

private:
    static WisResult InitThunk(DX12DeviceExtensionHeader* self, const impl::DX12DeviceImpl& device_impl) noexcept
    {
        return reinterpret_cast<T*>(self)->Init(device_impl);
    }

public:
    WisResult Init(const impl::DX12DeviceImpl& device_impl) noexcept
    {
        (void)device_impl;
        return {};
    }
};

} // namespace wis

#endif // !WIS_DX12_EXTENSIONS_HPP
