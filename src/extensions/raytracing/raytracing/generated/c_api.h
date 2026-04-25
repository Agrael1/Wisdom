// This file is generated. Do not edit directly.
#ifndef WISDOM_RAYTRACING_C_API_H
#define WISDOM_RAYTRACING_C_API_H
#include <wisdom/generated/c_api.h>
#include "wisdom_exports.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef WISDOM_DX12
/**
 * @brief Provided by Wisdom 0.7.1. Extension handle for raytracing.
 *
 * */
WIS_DEFINE_DX12_DEVICE_EXT_HANDLE(WisDX12RaytracingExtension, 2);

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

#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
/**
 * @brief Provided by Wisdom 0.7.1. Extension handle for raytracing.
 *
 * */
WIS_DEFINE_VK_DEVICE_EXT_HANDLE(WisVKRaytracingExtension, 5);

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

#endif // WISDOM_VULKAN

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_RAYTRACING_C_API_H
