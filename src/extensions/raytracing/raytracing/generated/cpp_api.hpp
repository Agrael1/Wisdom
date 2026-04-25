// This file is generated. Do not edit directly.
#ifndef WISDOM_RAYTRACING_CPP_API_HPP
#define WISDOM_RAYTRACING_CPP_API_HPP
#ifndef __cplusplus
#    error C++ is required to include this header.
#endif // __cplusplus

#include <wisdom/generated/cpp_api.hpp>
#include "c_api.h"
#include "wisdom_exports.h"

namespace wis {} // namespace wis

#ifdef WISDOM_DX12
#    include <raytracing/dx12/dx12_types.hpp>

namespace wis {
struct DX12RaytracingExtensionDeleter {
    void operator()(WisDX12RaytracingExtension* handle) noexcept { ::wisDX12DestroyRaytracingExtension(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Extension handle for raytracing.
 *
 * */
class DX12RaytracingExtension : public wis::impl::Implements<
                                    wis::impl::DX12RaytracingExtensionImpl,
                                    WisDX12RaytracingExtension,
                                    wis::DX12RaytracingExtensionDeleter>
{
public:
    DX12RaytracingExtension() noexcept
        : ImplType(wis::in_place)
    {
        ::wisDX12InitRaytracingExtension(GetStorage());
    }
    // Operator & overload
    wis::DX12DeviceExtensionHeader* operator&() noexcept { return &GetMutableInternal().header; }

public:
    /**
     * @brief Provided by Wisdom 0.7.1. Checks if raytracing is supported on the current device.
     * @return bool true if raytracing is supported, false otherwise.
     *
     * */
    WIS_NODISCARD inline bool Supported() noexcept { return (::wisDX12RaytracingExtensionSupported(&_impl_storage)); }
};

} // namespace wis
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
#    include <raytracing/vulkan/vk_types.hpp>

namespace wis {
struct VKRaytracingExtensionDeleter {
    void operator()(WisVKRaytracingExtension* handle) noexcept { ::wisVKDestroyRaytracingExtension(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Extension handle for raytracing.
 *
 * */
class VKRaytracingExtension
    : public wis::impl::
          Implements<wis::impl::VKRaytracingExtensionImpl, WisVKRaytracingExtension, wis::VKRaytracingExtensionDeleter>
{
public:
    VKRaytracingExtension() noexcept
        : ImplType(wis::in_place)
    {
        ::wisVKInitRaytracingExtension(GetStorage());
    }
    // Operator & overload
    wis::VKDeviceExtensionHeader* operator&() noexcept { return &GetMutableInternal().header; }

public:
    /**
     * @brief Provided by Wisdom 0.7.1. Checks if raytracing is supported on the current device.
     * @return bool true if raytracing is supported, false otherwise.
     *
     * */
    WIS_NODISCARD inline bool Supported() noexcept { return (::wisVKRaytracingExtensionSupported(&_impl_storage)); }
};

} // namespace wis
#endif // WISDOM_VULKAN

#endif // WISDOM_RAYTRACING_CPP_API_HPP
