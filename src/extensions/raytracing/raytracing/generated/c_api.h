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
