// This file is generated. Do not edit directly.
#ifndef WISDOM_RAYTRACING_C_API_H
#define WISDOM_RAYTRACING_C_API_H
#include <wisdom/generated/c_api.h>
#include "wisdom_exports.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//==============================================================
// Enums
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.1. Enumeration for the level of an acceleration structure in raytracing.
 *
 * */
typedef enum WisAccelerationStructureLevel {
    /**
     * @brief Top-level acceleration structure, which contains instances of bottom-level structures.
     * */
    WisAccelerationStructureLevelTopLevel = 0,
    /**
     * @brief Bottom-level acceleration structure, which contains geometry data such as triangles or AABBs.
     * */
    WisAccelerationStructureLevelBottomLevel = 1,
} WisAccelerationStructureLevel;

/**
 * @brief Provided by Wisdom 0.7.1. Enumeration for the type of geometry in a bottom-level acceleration structure.
 *
 * */
typedef enum WisGeometryType {
    WisGeometryTypeTriangles = 0, ///< Triangles geometry type. Used for triangle meshes.
    WisGeometryTypeAABBs = 1, ///< Axis Aligned Bounding Boxes geometry type. Used for bounding volume hierarchies.
} WisGeometryType;

/**
 * @brief Provided by Wisdom 0.7.1. Bitmask for geometry flags in raytracing.
 *
 * */
typedef enum WisGeometryFlags {
    WisGeometryFlagsNone = 0, ///< No flags set. Geometry is regular.
    WisGeometryFlagsOpaque = (1u << 0), ///< Geometry is opaque. Used for opaque geometry.
    WisGeometryFlagsNoDuplicateAnyHitInvocation = (1u << 1), ///< Geometry has no duplicate any hit invocation.
} WisGeometryFlags;

/**
 * @brief Provided by Wisdom 0.7.1. Acceleration structure flags for additional acceleration structure features
 *
 * */
typedef enum WisAccelerationStructureFlags {
    WisAccelerationStructureFlagsNone = 0, ///< No flags set. Acceleration structure is regular.
    WisAccelerationStructureFlagsAllowUpdate = (1u << 0), ///< Acceleration structure is allowed to be updated.
    WisAccelerationStructureFlagsAllowCompaction = (1u << 1), ///< Acceleration structure is allowed to be compacted.
    /**
     * @brief Acceleration structure is preferred to be fast traced.
     * */
    WisAccelerationStructureFlagsPreferFastTrace = (1u << 2),
    WisAccelerationStructureFlagsPreferFastBuild = (1u << 3), ///< Acceleration structure is preferred to be fast built.
    WisAccelerationStructureFlagsMinimizeMemory = (1u << 4), ///< Acceleration structure is minimized for memory usage.
    /**
     * @brief Acceleration structure build is performed as an update. Only used for update builds.
     * */
    WisAccelerationStructureFlagsPerformUpdate = (1u << 5),
} WisAccelerationStructureFlags;

//==============================================================
// Structs
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.1. Structure describing the single geometry instance.
 *
 * */
typedef struct WisAcceleratedGeometryDesc {
    WisGeometryType type; ///< The type of geometry (triangles or AABBs).
    WisGeometryFlags flags; ///< The geometry flags for this geometry instance.
    /**
     * @brief The GPU address of the vertex buffer for this geometry instance.
     * */
    uint64_t vertex_or_aabb_buffer_address;
    /**
     * @brief The GPU address of the index buffer for this geometry instance. Only used for triangles geometry type.
     * */
    uint64_t index_buffer_address;
    /**
     * @brief The GPU address of the transform matrix (float [3][4]) for this geometry instance. Only used for triangles
     * geometry type.
     * */
    uint64_t transform_matrix_address;
    uint32_t vertex_or_aabb_stride; ///< The stride in bytes between vertices or AABBs in the buffer.
    /**
     * @brief The number of vertices in the vertex buffer. Only used for triangles geometry type.
     * */
    uint32_t vertex_count;
    uint32_t triangle_or_aabb_count; ///< For triangles it is equal to (index_count/3) and count for AABBs.
    /**
     * @brief The format of the vertex data in the vertex buffer. Only used for triangles geometry type.
     * */
    WisDataFormat vertex_format;
    /**
     * @brief The format of the index data in the index buffer. Only used for triangles geometry type.
     * */
    WisIndexType index_format;
} WisAcceleratedGeometryDesc;

/**
 * @brief Provided by Wisdom 0.7.1. Structure describing the allocation information for an acceleration structure.
 *
 * */
typedef struct WisStructureAllocationInfo {
    uint64_t structure_size; ///< The size of the acceleration structure in bytes.
    uint64_t scratch_size; ///< The size of the scratch buffer needed to build the acceleration structure in bytes.
    uint64_t update_size; ///< The size of the scratch buffer needed to update the acceleration structure in bytes.
} WisStructureAllocationInfo;

/**
 * @brief Provided by Wisdom 0.7.1. Structure describing the build description for a bottom-level acceleration
 * structure.
 *
 * */
typedef struct WisBottomLevelStructureBuildDesc {
    WisAccelerationStructureFlags flags; ///< The build flags for the acceleration structure build.
    /**
     * @brief The number of geometry instances in the bottom-level acceleration structure.
     * */
    uint32_t geometry_count;
    /**
     * @brief The array of geometry descriptions for the bottom-level acceleration structure. Has higher precedence over
     * `WisBottomLevelStructureBuildDesc::indirect_geometries`.
     * */
    const WisAcceleratedGeometryDesc* geometries;
    /**
     * @brief The array of geometry descriptions for indirect build of the bottom-level acceleration structure.
     * */
    const WisAcceleratedGeometryDesc** indirect_geometries;
} WisBottomLevelStructureBuildDesc;

//==============================================================
// Constants
//==============================================================

/// @brief Provided by Wisdom 0.7.1. Alignment in bytes for acceleration structure buffers. Acceleration structures must
/// be allocated with this alignment and offset of the acceleration structure within the buffer must also be aligned to
/// this value.
#define WIS_ACCELERATION_STRUCTURE_ALIGNMENT ((uint32_t)256)

#ifdef WISDOM_DX12
/**
 * @brief Provided by Wisdom 0.7.1. Handle for an acceleration structure used in raytracing.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12AccelerationStructure, 2);

/**
 * @brief Provided by Wisdom 0.7.1. Extension handle for raytracing.
 *
 * */
WIS_DEFINE_DX12_DEVICE_EXT_HANDLE(WisDX12RaytracingExtension, 2);

/**
 * @brief Provided by Wisdom 0.7.1. Structure describing the properties of an acceleration structure.
 *
 * */
typedef struct WisDX12AccelerationStructureDesc {
    WisAccelerationStructureLevel level; ///< The level of the acceleration structure (top-level or bottom-level).
    /**
     * @brief The offset in bytes from the start of the buffer where the acceleration structure is located.
     * */
    uint64_t offset;
    uint64_t size; ///< The size of the acceleration structure in bytes.
} WisDX12AccelerationStructureDesc;

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisAccelerationStructure handle.
 * @param self is a pointer to the valid WisAccelerationStructure instance.
 *
 * */
WISDOM_RAYTRACING_API void wisDX12DestroyAccelerationStructure(WisDX12AccelerationStructure* self);

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisRaytracingExtension handle.
 * @param self is a pointer to the valid WisRaytracingExtension instance.
 *
 * */
WISDOM_RAYTRACING_API void wisDX12DestroyRaytracingExtension(WisDX12RaytracingExtension* self);

/**
 * @brief Provided by Wisdom 0.7.1. Initializes a WisRaytracingExtension handle.
 * @param self is a pointer to the valid WisRaytracingExtension instance.
 *
 * */
WISDOM_RAYTRACING_API void wisDX12InitRaytracingExtension(WisDX12RaytracingExtension* self);

/**
 * @brief Provided by Wisdom 0.7.1. Checks if raytracing is supported on the current device.
 * @param self is a pointer to the valid WisRaytracingExtension instance.
 * @return bool true if raytracing is supported, false otherwise.
 *
 * */
WISDOM_RAYTRACING_API bool wisDX12RaytracingExtensionSupported(WisDX12RaytracingExtension* self);

/**
 * @brief Provided by Wisdom 0.7.1. Retrieves the allocation information for a bottom-level acceleration structure based
 * on the provided build description.
 * @param self is a pointer to the valid WisRaytracingExtension instance.
 * @param build_desc The build description for the bottom-level acceleration structure.
 * @param info The allocation information for the bottom-level acceleration structure.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_RAYTRACING_API WisResult wisDX12RaytracingExtensionGetBottomLevelStructureInfo(
    WisDX12RaytracingExtension* self,
    const WisBottomLevelStructureBuildDesc* build_desc,
    WisStructureAllocationInfo* info
);

/**
 * @brief Provided by Wisdom 0.7.1. Creates an acceleration structure based on the provided description.
 * @param self is a pointer to the valid WisRaytracingExtension instance.
 * @param buffer The buffer to write the acceleration structure data to.
 * @param desc The description of the acceleration structure to create.
 * @param acceleration_structure The created acceleration structure handle.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_RAYTRACING_API WisResult wisDX12RaytracingExtensionCreateAccelerationStructure(
    WisDX12RaytracingExtension* self,
    WisDX12Buffer* buffer,
    const WisDX12AccelerationStructureDesc* desc,
    WisDX12AccelerationStructure* acceleration_structure
);

/**
 * @brief Provided by Wisdom 0.7.1. Retrieves the GPU address of the acceleration structure.
 * @param self is a pointer to the valid WisAccelerationStructure instance.
 * @return u64 The GPU address of the acceleration structure.
 *
 * */
WISDOM_RAYTRACING_API uint64_t wisDX12AccelerationStructureGetGPUAddress(WisDX12AccelerationStructure* self);

#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
/**
 * @brief Provided by Wisdom 0.7.1. Handle for an acceleration structure used in raytracing.
 *
 * */
WIS_DEFINE_HANDLE(WisVKAccelerationStructure, 4);

/**
 * @brief Provided by Wisdom 0.7.1. Extension handle for raytracing.
 *
 * */
WIS_DEFINE_VK_DEVICE_EXT_HANDLE(WisVKRaytracingExtension, 5);

/**
 * @brief Provided by Wisdom 0.7.1. Structure describing the properties of an acceleration structure.
 *
 * */
typedef struct WisVKAccelerationStructureDesc {
    WisAccelerationStructureLevel level; ///< The level of the acceleration structure (top-level or bottom-level).
    /**
     * @brief The offset in bytes from the start of the buffer where the acceleration structure is located.
     * */
    uint64_t offset;
    uint64_t size; ///< The size of the acceleration structure in bytes.
} WisVKAccelerationStructureDesc;

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisAccelerationStructure handle.
 * @param self is a pointer to the valid WisAccelerationStructure instance.
 *
 * */
WISDOM_RAYTRACING_API void wisVKDestroyAccelerationStructure(WisVKAccelerationStructure* self);

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisRaytracingExtension handle.
 * @param self is a pointer to the valid WisRaytracingExtension instance.
 *
 * */
WISDOM_RAYTRACING_API void wisVKDestroyRaytracingExtension(WisVKRaytracingExtension* self);

/**
 * @brief Provided by Wisdom 0.7.1. Initializes a WisRaytracingExtension handle.
 * @param self is a pointer to the valid WisRaytracingExtension instance.
 *
 * */
WISDOM_RAYTRACING_API void wisVKInitRaytracingExtension(WisVKRaytracingExtension* self);

/**
 * @brief Provided by Wisdom 0.7.1. Checks if raytracing is supported on the current device.
 * @param self is a pointer to the valid WisRaytracingExtension instance.
 * @return bool true if raytracing is supported, false otherwise.
 *
 * */
WISDOM_RAYTRACING_API bool wisVKRaytracingExtensionSupported(WisVKRaytracingExtension* self);

/**
 * @brief Provided by Wisdom 0.7.1. Retrieves the allocation information for a bottom-level acceleration structure based
 * on the provided build description.
 * @param self is a pointer to the valid WisRaytracingExtension instance.
 * @param build_desc The build description for the bottom-level acceleration structure.
 * @param info The allocation information for the bottom-level acceleration structure.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_RAYTRACING_API WisResult wisVKRaytracingExtensionGetBottomLevelStructureInfo(
    WisVKRaytracingExtension* self,
    const WisBottomLevelStructureBuildDesc* build_desc,
    WisStructureAllocationInfo* info
);

/**
 * @brief Provided by Wisdom 0.7.1. Creates an acceleration structure based on the provided description.
 * @param self is a pointer to the valid WisRaytracingExtension instance.
 * @param buffer The buffer to write the acceleration structure data to.
 * @param desc The description of the acceleration structure to create.
 * @param acceleration_structure The created acceleration structure handle.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_RAYTRACING_API WisResult wisVKRaytracingExtensionCreateAccelerationStructure(
    WisVKRaytracingExtension* self,
    WisVKBuffer* buffer,
    const WisVKAccelerationStructureDesc* desc,
    WisVKAccelerationStructure* acceleration_structure
);

/**
 * @brief Provided by Wisdom 0.7.1. Retrieves the GPU address of the acceleration structure.
 * @param self is a pointer to the valid WisAccelerationStructure instance.
 * @return u64 The GPU address of the acceleration structure.
 *
 * */
WISDOM_RAYTRACING_API uint64_t wisVKAccelerationStructureGetGPUAddress(WisVKAccelerationStructure* self);

#endif // WISDOM_VULKAN

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_RAYTRACING_C_API_H
