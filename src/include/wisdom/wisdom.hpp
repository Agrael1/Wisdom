// This file is generated. Do not edit directly.
#ifndef WISDOM_HPP
#define WISDOM_HPP

#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

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

using Buffer            = wis::DX12Buffer;
using DescriptorHeap    = wis::DX12DescriptorHeap;
using PipelineLayout    = wis::DX12PipelineLayout;
using ResourceAllocator = wis::DX12ResourceAllocator;
using Fence             = wis::DX12Fence;
using CommandList       = wis::DX12CommandList;
using CommandQueue      = wis::DX12CommandQueue;
using Device            = wis::DX12Device;
using AdapterQuery      = wis::DX12AdapterQuery;
using Instance          = wis::DX12Instance;
using FenceView         = wis::DX12FenceView;
using CommandListView   = wis::DX12CommandListView;

//==============================================================
// Variants
//==============================================================

using DeviceExtensionHeader   = wis::DX12DeviceExtensionHeader;
using InstanceExtensionHeader = wis::DX12InstanceExtensionHeader;
using DeviceRequirements      = wis::DX12DeviceRequirements;

//==============================================================
// Functions
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_desc points to wis::DebugDesc, which defines debug callback and debug layer usage. If `nullptr`, debug layer is disabled.
 * @param extensions points to an array of extensions that are to be initialized with pointers to wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success.
 *
 * */
WIS_NODISCARD inline wis::Instance CreateInstance(const wis::DebugDesc*                    debug_desc,
                                                  wis::span<wis::InstanceExtensionHeader*> extensions,
                                                  wis::Result&                             out_result) noexcept
{
    wis::DX12Instance instance;
    out_result = convert_result(::wisDX12CreateInstance(reinterpret_cast<const WisDebugDesc*>(debug_desc),
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

using Buffer            = wis::VKBuffer;
using DescriptorHeap    = wis::VKDescriptorHeap;
using PipelineLayout    = wis::VKPipelineLayout;
using ResourceAllocator = wis::VKResourceAllocator;
using Fence             = wis::VKFence;
using CommandList       = wis::VKCommandList;
using CommandQueue      = wis::VKCommandQueue;
using Device            = wis::VKDevice;
using AdapterQuery      = wis::VKAdapterQuery;
using Instance          = wis::VKInstance;
using FenceView         = wis::VKFenceView;
using CommandListView   = wis::VKCommandListView;

//==============================================================
// Variants
//==============================================================

using DeviceExtensionHeader   = wis::VKDeviceExtensionHeader;
using InstanceExtensionHeader = wis::VKInstanceExtensionHeader;
using DeviceRequirements      = wis::VKDeviceRequirements;

//==============================================================
// Functions
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_desc points to wis::DebugDesc, which defines debug callback and debug layer usage. If `nullptr`, debug layer is disabled.
 * @param extensions points to an array of extensions that are to be initialized with pointers to wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success.
 *
 * */
WIS_NODISCARD inline wis::Instance CreateInstance(const wis::DebugDesc*                    debug_desc,
                                                  wis::span<wis::InstanceExtensionHeader*> extensions,
                                                  wis::Result&                             out_result) noexcept
{
    wis::VKInstance instance;
    out_result = convert_result(::wisVKCreateInstance(reinterpret_cast<const WisDebugDesc*>(debug_desc),
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
