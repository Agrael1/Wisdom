// This file is generated. Do not edit directly.
#ifndef WISDOM_RAYTRACING_CPP_API_HPP
#define WISDOM_RAYTRACING_CPP_API_HPP
#ifndef __cplusplus
#    error C++ is required to include this header.
#endif // __cplusplus

#include <wisdom/generated/cpp_api.hpp>
#include "c_api.h"
#include "wisdom_exports.h"

namespace wis {

//==============================================================
// Enums
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.1. Enumeration for the level of an acceleration structure in raytracing.
 *
 * */
enum class AccelerationStructureLevel {
    TopLevel = 0, ///< Top-level acceleration structure, which contains instances of bottom-level structures.
    BottomLevel = 1, ///< Bottom-level acceleration structure, which contains geometry data such as triangles or AABBs.
};

} // namespace wis

#ifdef WISDOM_DX12
#    include <raytracing/dx12/dx12_types.hpp>

namespace wis {
/**
 * @brief Provided by Wisdom 0.7.1. Structure describing the properties of an acceleration structure.
 *
 * */
struct DX12AccelerationStructureDesc {
    wis::AccelerationStructureLevel level; ///< The level of the acceleration structure (top-level or bottom-level).
    /**
     * @brief The offset in bytes from the start of the buffer where the acceleration structure is located.
     * */
    std::uint64_t offset;
    std::uint64_t size; ///< The size of the acceleration structure in bytes.
};

struct DX12AccelerationStructureDeleter {
    void operator()(WisDX12AccelerationStructure* handle) noexcept { ::wisDX12DestroyAccelerationStructure(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Handle for an acceleration structure used in raytracing.
 *
 * */
class DX12AccelerationStructure : public wis::impl::Implements<
                                      wis::impl::DX12AccelerationStructureImpl,
                                      WisDX12AccelerationStructure,
                                      wis::DX12AccelerationStructureDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.1. Retrieves the GPU address of the acceleration structure.
     * @return u64 The GPU address of the acceleration structure.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetGPUAddress() noexcept
    {
        return (::wisDX12AccelerationStructureGetGPUAddress(&_impl_storage));
    }
};

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
    /**
     * @brief Provided by Wisdom 0.7.1. Creates an acceleration structure based on the provided description.
     * @param buffer The buffer to write the acceleration structure data to.
     * @param desc The description of the acceleration structure to create.
     * @param out_result denoting the outcome of operation.
     * @return acceleration_structure The created acceleration structure handle.
     *
     * */
    WIS_NODISCARD inline wis::DX12AccelerationStructure CreateAccelerationStructure(
        wis::DX12Buffer& buffer,
        const wis::DX12AccelerationStructureDesc& desc,
        wis::Result& out_result
    ) noexcept
    {
        wis::DX12AccelerationStructure acceleration_structure;
        const WisResult wis_result = ::wisDX12RaytracingExtensionCreateAccelerationStructure(
            &_impl_storage,
            reinterpret_cast<WisDX12Buffer*>(&buffer),
            reinterpret_cast<const WisDX12AccelerationStructureDesc*>(&desc),
            acceleration_structure.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return acceleration_structure;
    }
};

} // namespace wis
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
#    include <raytracing/vulkan/vk_types.hpp>

namespace wis {
/**
 * @brief Provided by Wisdom 0.7.1. Structure describing the properties of an acceleration structure.
 *
 * */
struct VKAccelerationStructureDesc {
    wis::AccelerationStructureLevel level; ///< The level of the acceleration structure (top-level or bottom-level).
    /**
     * @brief The offset in bytes from the start of the buffer where the acceleration structure is located.
     * */
    std::uint64_t offset;
    std::uint64_t size; ///< The size of the acceleration structure in bytes.
};

struct VKAccelerationStructureDeleter {
    void operator()(WisVKAccelerationStructure* handle) noexcept { ::wisVKDestroyAccelerationStructure(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Handle for an acceleration structure used in raytracing.
 *
 * */
class VKAccelerationStructure : public wis::impl::Implements<
                                    wis::impl::VKAccelerationStructureImpl,
                                    WisVKAccelerationStructure,
                                    wis::VKAccelerationStructureDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.1. Retrieves the GPU address of the acceleration structure.
     * @return u64 The GPU address of the acceleration structure.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetGPUAddress() noexcept
    {
        return (::wisVKAccelerationStructureGetGPUAddress(&_impl_storage));
    }
};

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
    /**
     * @brief Provided by Wisdom 0.7.1. Creates an acceleration structure based on the provided description.
     * @param buffer The buffer to write the acceleration structure data to.
     * @param desc The description of the acceleration structure to create.
     * @param out_result denoting the outcome of operation.
     * @return acceleration_structure The created acceleration structure handle.
     *
     * */
    WIS_NODISCARD inline wis::VKAccelerationStructure CreateAccelerationStructure(
        wis::VKBuffer& buffer,
        const wis::VKAccelerationStructureDesc& desc,
        wis::Result& out_result
    ) noexcept
    {
        wis::VKAccelerationStructure acceleration_structure;
        const WisResult wis_result = ::wisVKRaytracingExtensionCreateAccelerationStructure(
            &_impl_storage,
            reinterpret_cast<WisVKBuffer*>(&buffer),
            reinterpret_cast<const WisVKAccelerationStructureDesc*>(&desc),
            acceleration_structure.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return acceleration_structure;
    }
};

} // namespace wis
#endif // WISDOM_VULKAN

#endif // WISDOM_RAYTRACING_CPP_API_HPP
