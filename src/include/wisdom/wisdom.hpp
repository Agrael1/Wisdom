// This file is generated. Do not edit directly.
#ifndef WISDOM_HPP
#define WISDOM_HPP

#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#ifdef WISDOM_WINDOWS
static_assert(WISDOM_WINDOWS && _WIN32, "Platform error");
#endif // WISDOM_WINDOWS

#ifdef WISDOM_LINUX
static_assert(WISDOM_LINUX && __linux__, "Platform error");
#endif // WISDOM_LINUX

#if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#define FORCEVK_SWITCH 1
#else
#define FORCEVK_SWITCH 0
#endif // WISDOM_VULKAN_FOUND

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH
#include "generated/dx12_cpp_api.hpp"

namespace wis {

//==============================================================
// Handles
//==============================================================

using Instance = wis::DX12Instance;

//==============================================================
// Variants
//==============================================================

using DeviceExtensionHeader   = wis::DX12DeviceExtensionHeader;
using InstanceExtensionHeader = wis::DX12InstanceExtensionHeader;

//==============================================================
// Functions
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_layer defines if the instance is to be created with debug mode.
 * @param extensions points to an array of extensions that are to be initialized with pointers to wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success (`wis::Status::Ok`).
 *
 * */
WIS_NODISCARD inline wis::Instance CreateInstance(bool                                     debug_layer,
                                                  wis::span<wis::InstanceExtensionHeader*> extensions,
                                                  wis::Result&                             out_result) noexcept
{
    wis::DX12Instance instance;
    out_result = convert_result(::wisDX12CreateInstance(debug_layer,
                                                        reinterpret_cast<WisDX12InstanceExtensionHeader**>(extensions.data()),
                                                        extensions.size(),
                                                        instance.GetStorage()));
    return instance;
}

} // namespace wis

#elif defined(WISDOM_VULKAN)
#include "generated/vk_cpp_api.hpp"

namespace wis {

//==============================================================
// Handles
//==============================================================

using Instance = wis::VKInstance;

//==============================================================
// Variants
//==============================================================

using DeviceExtensionHeader   = wis::VKDeviceExtensionHeader;
using InstanceExtensionHeader = wis::VKInstanceExtensionHeader;

//==============================================================
// Functions
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_layer defines if the instance is to be created with debug mode.
 * @param extensions points to an array of extensions that are to be initialized with pointers to wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success (`wis::Status::Ok`).
 *
 * */
WIS_NODISCARD inline wis::Instance CreateInstance(bool                                     debug_layer,
                                                  wis::span<wis::InstanceExtensionHeader*> extensions,
                                                  wis::Result&                             out_result) noexcept
{
    wis::VKInstance instance;
    out_result = convert_result(::wisVKCreateInstance(debug_layer,
                                                      reinterpret_cast<WisVKInstanceExtensionHeader**>(extensions.data()),
                                                      extensions.size(),
                                                      instance.GetStorage()));
    return instance;
}

} // namespace wis
#else
#error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection
#endif // WISDOM_HPP
