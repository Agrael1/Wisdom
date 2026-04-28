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

/**
 * @brief Provided by Wisdom 0.7.1. Enumeration for the type of geometry in a bottom-level acceleration structure.
 *
 * */
enum class GeometryType {
    Triangles = 0, ///< Triangles geometry type. Used for triangle meshes.
    AABBs = 1, ///< Axis Aligned Bounding Boxes geometry type. Used for bounding volume hierarchies.
};

/**
 * @brief Provided by Wisdom 0.7.1. Bitmask for geometry flags in raytracing.
 *
 * */
enum class GeometryFlags : uint32_t {
    None = 0, ///< No flags set. Geometry is regular.
    Opaque = (1u << 0), ///< Geometry is opaque. Used for opaque geometry.
    NoDuplicateAnyHitInvocation = (1u << 1), ///< Geometry has no duplicate any hit invocation.
};
WISDOM_DEFINE_ENUM_OPERATORS(GeometryFlags)

/**
 * @brief Provided by Wisdom 0.7.1. Acceleration structure flags for additional acceleration structure features
 *
 * */
enum class AccelerationStructureFlags : uint32_t {
    None = 0, ///< No flags set. Acceleration structure is regular.
    AllowUpdate = (1u << 0), ///< Acceleration structure is allowed to be updated.
    AllowCompaction = (1u << 1), ///< Acceleration structure is allowed to be compacted.
    PreferFastTrace = (1u << 2), ///< Acceleration structure is preferred to be fast traced.
    PreferFastBuild = (1u << 3), ///< Acceleration structure is preferred to be fast built.
    MinimizeMemory = (1u << 4), ///< Acceleration structure is minimized for memory usage.
    PerformUpdate = (1u << 5), ///< Acceleration structure build is performed as an update. Only used for update builds.
    IndirectInput = (1u << 6), ///< Acceleration structure build uses indirect input.
};
WISDOM_DEFINE_ENUM_OPERATORS(AccelerationStructureFlags)

//==============================================================
// Structs
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.1. Structure describing the single geometry instance.
 *
 * */
struct AcceleratedGeometryDesc {
    wis::GeometryType type; ///< The type of geometry (triangles or AABBs).
    wis::GeometryFlags flags; ///< The geometry flags for this geometry instance.
    /**
     * @brief The GPU address of the vertex buffer for this geometry instance.
     * */
    std::uint64_t vertex_or_aabb_buffer_address;
    /**
     * @brief The GPU address of the index buffer for this geometry instance. Only used for triangles geometry type.
     * */
    std::uint64_t index_buffer_address;
    /**
     * @brief The GPU address of the transform matrix (float [3][4]) for this geometry instance. Only used for triangles
     * geometry type.
     * */
    std::uint64_t transform_matrix_address;
    std::uint32_t vertex_or_aabb_stride; ///< The stride in bytes between vertices or AABBs in the buffer.
    /**
     * @brief The number of vertices in the vertex buffer. Only used for triangles geometry type.
     * */
    std::uint32_t vertex_count;
    std::uint32_t triangle_or_aabb_count; ///< For triangles it is equal to (index_count/3) and count for AABBs.
    /**
     * @brief The format of the vertex data in the vertex buffer. Only used for triangles geometry type.
     * */
    wis::DataFormat vertex_format;
    /**
     * @brief The format of the index data in the index buffer. Only used for triangles geometry type.
     * */
    wis::IndexType index_format;
};

/**
 * @brief Provided by Wisdom 0.7.1. Structure describing the allocation information for an acceleration structure.
 *
 * */
struct StructureAllocationInfo {
    std::uint64_t structure_size; ///< The size of the acceleration structure in bytes.
    std::uint64_t scratch_size; ///< The size of the scratch buffer needed to build the acceleration structure in bytes.
    std::uint64_t update_size; ///< The size of the scratch buffer needed to update the acceleration structure in bytes.
};

/**
 * @brief Provided by Wisdom 0.7.1. Structure describing the build description for a bottom-level acceleration
 * structure.
 *
 * */
struct BottomLevelStructureBuildDesc {
    wis::AccelerationStructureFlags flags; ///< The build flags for the acceleration structure build.
    /**
     * @brief The number of geometry instances in the bottom-level acceleration structure.
     * */
    std::uint32_t geometry_count;
    /**
     * @brief The array of geometry descriptions for the bottom-level acceleration structure.
     * */
    const wis::AcceleratedGeometryDesc* geometries;
    /**
     * @brief The array of geometry descriptions for indirect build of the bottom-level acceleration structure. This
     * input is ignored unless `wis::AccelerationStructureFlags::IndirectInput` is specified.
     * */
    const wis::AcceleratedGeometryDesc** indirect_geometries;
};

/**
 * @brief Provided by Wisdom 0.7.1. Structure describing the build description for a top-level acceleration structure.
 *
 * */
struct TopLevelStructureBuildDesc {
    wis::AccelerationStructureFlags flags; ///< The build flags for the acceleration structure build.
    /**
     * @brief The number of instances in the top-level acceleration structure.
     * */
    std::uint32_t instance_count;
    /**
     * @brief The GPU address of the instance buffer for the top-level acceleration structure.
     * */
    std::uint64_t instance_buffer_address;
};

//==============================================================
// Constants
//==============================================================

/// @brief Provided by Wisdom 0.7.1. Alignment in bytes for acceleration structure buffers. Acceleration structures must
/// be allocated with this alignment and offset of the acceleration structure within the buffer must also be aligned to
/// this value.
static constexpr std::uint32_t AccelerationStructureAlignment = 256;

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
     * @brief Provided by Wisdom 0.7.1. Retrieves the allocation information for a bottom-level acceleration structure
     * based on the provided build description.
     * @param build_desc The build description for the bottom-level acceleration structure.
     * @param out_result denoting the outcome of operation.
     * @return info The allocation information for the bottom-level acceleration structure.
     *
     * */
    WIS_NODISCARD inline wis::StructureAllocationInfo GetBottomLevelStructureInfo(
        const wis::BottomLevelStructureBuildDesc& build_desc,
        wis::Result& out_result
    ) noexcept
    {
        wis::StructureAllocationInfo info;
        const WisResult wis_result = ::wisDX12RaytracingExtensionGetBottomLevelStructureInfo(
            &_impl_storage,
            reinterpret_cast<const WisBottomLevelStructureBuildDesc*>(&build_desc),
            reinterpret_cast<WisStructureAllocationInfo*>(&info)
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return info;
    }
    /**
     * @brief Provided by Wisdom 0.7.1. Retrieves the allocation information for a top-level acceleration structure
     * based on the provided build description.
     * @param build_desc The build description for the bottom-level acceleration structure.
     * @param out_result denoting the outcome of operation.
     * @return info The allocation information for the bottom-level acceleration structure.
     *
     * */
    WIS_NODISCARD inline wis::StructureAllocationInfo GetTopLevelStructureInfo(
        const wis::TopLevelStructureBuildDesc& build_desc,
        wis::Result& out_result
    ) noexcept
    {
        wis::StructureAllocationInfo info;
        const WisResult wis_result = ::wisDX12RaytracingExtensionGetTopLevelStructureInfo(
            &_impl_storage,
            reinterpret_cast<const WisTopLevelStructureBuildDesc*>(&build_desc),
            reinterpret_cast<WisStructureAllocationInfo*>(&info)
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return info;
    }
    /**
     * @brief Provided by Wisdom 0.7.1. Creates a batch of acceleration structures based on the provided descriptions.
     * @param buffer The buffer to write the acceleration structure data to.
     * @param structures The descriptions of the acceleration structures to create.
     * @param acceleration_structures The created acceleration structure handle.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result CreateAccelerationStructures(
        wis::DX12Buffer& buffer,
        wis::span<const wis::DX12AccelerationStructureDesc> structures,
        wis::DX12AccelerationStructure* acceleration_structures
    ) noexcept
    {
        const WisResult wis_result = ::wisDX12RaytracingExtensionCreateAccelerationStructures(
            &_impl_storage,
            reinterpret_cast<WisDX12Buffer*>(&buffer),
            reinterpret_cast<const WisDX12AccelerationStructureDesc*>(structures.data()),
            structures.size(),
            reinterpret_cast<WisDX12AccelerationStructure*>(acceleration_structures)
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.1. Destroys a batch of acceleration structures.
     * @param acceleration_structures The acceleration structures to destroy.
     *
     * */
    inline void DestroyAccelerationStructures(
        wis::span<wis::DX12AccelerationStructure> acceleration_structures
    ) noexcept
    {
        ::wisDX12RaytracingExtensionDestroyAccelerationStructures(
            &_impl_storage,
            reinterpret_cast<WisDX12AccelerationStructure*>(acceleration_structures.data()),
            acceleration_structures.size()
        );
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
     * @brief Provided by Wisdom 0.7.1. Retrieves the allocation information for a bottom-level acceleration structure
     * based on the provided build description.
     * @param build_desc The build description for the bottom-level acceleration structure.
     * @param out_result denoting the outcome of operation.
     * @return info The allocation information for the bottom-level acceleration structure.
     *
     * */
    WIS_NODISCARD inline wis::StructureAllocationInfo GetBottomLevelStructureInfo(
        const wis::BottomLevelStructureBuildDesc& build_desc,
        wis::Result& out_result
    ) noexcept
    {
        wis::StructureAllocationInfo info;
        const WisResult wis_result = ::wisVKRaytracingExtensionGetBottomLevelStructureInfo(
            &_impl_storage,
            reinterpret_cast<const WisBottomLevelStructureBuildDesc*>(&build_desc),
            reinterpret_cast<WisStructureAllocationInfo*>(&info)
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return info;
    }
    /**
     * @brief Provided by Wisdom 0.7.1. Retrieves the allocation information for a top-level acceleration structure
     * based on the provided build description.
     * @param build_desc The build description for the bottom-level acceleration structure.
     * @param out_result denoting the outcome of operation.
     * @return info The allocation information for the bottom-level acceleration structure.
     *
     * */
    WIS_NODISCARD inline wis::StructureAllocationInfo GetTopLevelStructureInfo(
        const wis::TopLevelStructureBuildDesc& build_desc,
        wis::Result& out_result
    ) noexcept
    {
        wis::StructureAllocationInfo info;
        const WisResult wis_result = ::wisVKRaytracingExtensionGetTopLevelStructureInfo(
            &_impl_storage,
            reinterpret_cast<const WisTopLevelStructureBuildDesc*>(&build_desc),
            reinterpret_cast<WisStructureAllocationInfo*>(&info)
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return info;
    }
    /**
     * @brief Provided by Wisdom 0.7.1. Creates a batch of acceleration structures based on the provided descriptions.
     * @param buffer The buffer to write the acceleration structure data to.
     * @param structures The descriptions of the acceleration structures to create.
     * @param acceleration_structures The created acceleration structure handle.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result CreateAccelerationStructures(
        wis::VKBuffer& buffer,
        wis::span<const wis::VKAccelerationStructureDesc> structures,
        wis::VKAccelerationStructure* acceleration_structures
    ) noexcept
    {
        const WisResult wis_result = ::wisVKRaytracingExtensionCreateAccelerationStructures(
            &_impl_storage,
            reinterpret_cast<WisVKBuffer*>(&buffer),
            reinterpret_cast<const WisVKAccelerationStructureDesc*>(structures.data()),
            structures.size(),
            reinterpret_cast<WisVKAccelerationStructure*>(acceleration_structures)
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.1. Destroys a batch of acceleration structures.
     * @param acceleration_structures The acceleration structures to destroy.
     *
     * */
    inline void DestroyAccelerationStructures(wis::span<wis::VKAccelerationStructure> acceleration_structures) noexcept
    {
        ::wisVKRaytracingExtensionDestroyAccelerationStructures(
            &_impl_storage,
            reinterpret_cast<WisVKAccelerationStructure*>(acceleration_structures.data()),
            acceleration_structures.size()
        );
    }
};

} // namespace wis
#endif // WISDOM_VULKAN

#endif // WISDOM_RAYTRACING_CPP_API_HPP
