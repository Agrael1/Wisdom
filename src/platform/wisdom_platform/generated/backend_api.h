// This file is generated. Do not edit directly.
#ifndef WISDOM_PLATFORM_C_BACKEND_API_H
#define WISDOM_PLATFORM_C_BACKEND_API_H

#include <wisdom/generated/backend_api.h>
#include "c_api.h"
#include "wisdom_exports.h"

#ifdef WISDOM_DX12
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/**
 * @brief Provided by Wisdom 0.7.0. Extension for Win32 surface creation functions.
 *
 * */
WIS_DEFINE_DX12_INSTANCE_EXT_HANDLE(WisDX12Win32Extension, 1);

/**
 * @brief Provided by Wisdom 0.7.0. Extension for UWP surface creation functions.
 *
 * */
WIS_DEFINE_DX12_INSTANCE_EXT_HANDLE(WisDX12UWPExtension, 1);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisWin32Extension handle.
 * @param self is a pointer to the valid WisWin32Extension instance.
 *
 * */
WISDOM_PLATFORM_API void wisDX12DestroyWin32Extension(WisDX12Win32Extension* self);

/**
 * @brief Provided by Wisdom 0.7.0. Initializes a WisWin32Extension handle.
 * @param self is a pointer to the valid WisWin32Extension instance.
 *
 * */
WISDOM_PLATFORM_API void wisDX12InitWin32Extension(WisDX12Win32Extension* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisUWPExtension handle.
 * @param self is a pointer to the valid WisUWPExtension instance.
 *
 * */
WISDOM_PLATFORM_API void wisDX12DestroyUWPExtension(WisDX12UWPExtension* self);

/**
 * @brief Provided by Wisdom 0.7.0. Initializes a WisUWPExtension handle.
 * @param self is a pointer to the valid WisUWPExtension instance.
 *
 * */
WISDOM_PLATFORM_API void wisDX12InitUWPExtension(WisDX12UWPExtension* self);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a surface using Win32.
 * @param self is a pointer to the valid WisWin32Extension instance.
 * @param info Win32 windowing data.
 * @param surface points to WisSurface, initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_PLATFORM_API WisResult wisDX12Win32ExtensionCreateSurface(WisDX12Win32Extension*    self,
                                                                 const WisWin32WindowDesc* info,
                                                                 WisDX12Surface*           surface);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a surface using UWP.
 * @param self is a pointer to the valid WisUWPExtension instance.
 * @param info UWP windowing data.
 * @param surface points to WisSurface, initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_PLATFORM_API WisResult wisDX12UWPExtensionCreateSurface(WisDX12UWPExtension*    self,
                                                               const WisUWPWindowDesc* info,
                                                               WisDX12Surface*         surface);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/**
 * @brief Provided by Wisdom 0.7.0. Extension for Xlib surface creation functions.
 *
 * */
WIS_DEFINE_VK_INSTANCE_EXT_HANDLE(WisVKXlibExtension, 2);

/**
 * @brief Provided by Wisdom 0.7.0. Extension for Xlib surface creation functions.
 *
 * */
WIS_DEFINE_VK_INSTANCE_EXT_HANDLE(WisVKXCBExtension, 2);

/**
 * @brief Provided by Wisdom 0.7.0. Extension for Xlib surface creation functions.
 *
 * */
WIS_DEFINE_VK_INSTANCE_EXT_HANDLE(WisVKWaylandExtension, 2);

/**
 * @brief Provided by Wisdom 0.7.0. Extension for Win32 surface creation functions.
 *
 * */
WIS_DEFINE_VK_INSTANCE_EXT_HANDLE(WisVKWin32Extension, 2);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisXlibExtension handle.
 * @param self is a pointer to the valid WisXlibExtension instance.
 *
 * */
WISDOM_PLATFORM_API void wisVKDestroyXlibExtension(WisVKXlibExtension* self);

/**
 * @brief Provided by Wisdom 0.7.0. Initializes a WisXlibExtension handle.
 * @param self is a pointer to the valid WisXlibExtension instance.
 *
 * */
WISDOM_PLATFORM_API void wisVKInitXlibExtension(WisVKXlibExtension* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisXCBExtension handle.
 * @param self is a pointer to the valid WisXCBExtension instance.
 *
 * */
WISDOM_PLATFORM_API void wisVKDestroyXCBExtension(WisVKXCBExtension* self);

/**
 * @brief Provided by Wisdom 0.7.0. Initializes a WisXCBExtension handle.
 * @param self is a pointer to the valid WisXCBExtension instance.
 *
 * */
WISDOM_PLATFORM_API void wisVKInitXCBExtension(WisVKXCBExtension* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisWaylandExtension handle.
 * @param self is a pointer to the valid WisWaylandExtension instance.
 *
 * */
WISDOM_PLATFORM_API void wisVKDestroyWaylandExtension(WisVKWaylandExtension* self);

/**
 * @brief Provided by Wisdom 0.7.0. Initializes a WisWaylandExtension handle.
 * @param self is a pointer to the valid WisWaylandExtension instance.
 *
 * */
WISDOM_PLATFORM_API void wisVKInitWaylandExtension(WisVKWaylandExtension* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisWin32Extension handle.
 * @param self is a pointer to the valid WisWin32Extension instance.
 *
 * */
WISDOM_PLATFORM_API void wisVKDestroyWin32Extension(WisVKWin32Extension* self);

/**
 * @brief Provided by Wisdom 0.7.0. Initializes a WisWin32Extension handle.
 * @param self is a pointer to the valid WisWin32Extension instance.
 *
 * */
WISDOM_PLATFORM_API void wisVKInitWin32Extension(WisVKWin32Extension* self);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a Vulkan surface using Xlib.
 * @param self is a pointer to the valid WisXlibExtension instance.
 * @param info Xlib windowing data.
 * @param surface points to WisSurface, initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_PLATFORM_API WisResult wisVKXlibExtensionCreateSurface(WisVKXlibExtension*      self,
                                                              const WisXlibWindowDesc* info,
                                                              WisVKSurface*            surface);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a surface using Win32.
 * @param self is a pointer to the valid WisXCBExtension instance.
 * @param info XCB windowing data.
 * @param surface points to WisSurface, initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_PLATFORM_API WisResult wisVKXCBExtensionCreateSurface(WisVKXCBExtension*      self,
                                                             const WisXCBWindowDesc* info,
                                                             WisVKSurface*           surface);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a surface using Wayland.
 * @param self is a pointer to the valid WisWaylandExtension instance.
 * @param info Wayland windowing data.
 * @param surface points to WisSurface, initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_PLATFORM_API WisResult wisVKWaylandExtensionCreateSurface(WisVKWaylandExtension*      self,
                                                                 const WisWaylandWindowDesc* info,
                                                                 WisVKSurface*               surface);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a surface using Win32.
 * @param self is a pointer to the valid WisWin32Extension instance.
 * @param info Win32 windowing data.
 * @param surface points to WisSurface, initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_PLATFORM_API WisResult wisVKWin32ExtensionCreateSurface(WisVKWin32Extension*      self,
                                                               const WisWin32WindowDesc* info,
                                                               WisVKSurface*             surface);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_VULKAN

#endif // WISDOM_PLATFORM_C_BACKEND_API_H
