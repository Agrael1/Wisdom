/** \mainpage Wisdom API Documentation

<b>Version 0.6.6</b>

Copyright (c) 2024 Ilya Doroshenko. All rights reserved.
License: MIT
See also: [repository on GitHub](https://github.com/Agrael1/Wisdom)
*/

module;
#define WISDOM_MODULE_DECL
#define WISDOM_BUILD_BINARIES
#include <wisdom/generated/vulkan/vk.include.h>
export module wisdom.vk;

export import wisdom.api;

#include <wisdom/vulkan/vk_convert.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/generated/vulkan/vk_functions.hpp>
#include <wisdom/vulkan/vk_handles.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_adapter.h>
#include <wisdom/vulkan/impl/vk_adapter.cpp>
#include <wisdom/vulkan/vk_memory.h>
#include <wisdom/vulkan/vk_resource.h>
#include <wisdom/vulkan/vk_allocator.h>
#include <wisdom/vulkan/vk_command_queue.h>
#include <wisdom/vulkan/impl/vk_command_queue.cpp>
#include <wisdom/generated/vulkan/vk_structs.hpp>
#include <wisdom/vulkan/vk_external.h>
#include <wisdom/vulkan/impl/vk_allocator.cpp>
#include <wisdom/vulkan/vk_queue_residency.h>
#include <wisdom/vulkan/vk_command_list.h>
#include <wisdom/vulkan/impl/vk_command_list.cpp>
#include <wisdom/vulkan/vk_fence.h>
#include <wisdom/vulkan/impl/vk_fence.cpp>
#include <wisdom/vulkan/vk_debug.h>
#include <wisdom/vulkan/vk_factory_ext.h>
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/impl/vk_create_factory.cpp>
#include <wisdom/vulkan/vk_pipeline_state.h>
#include <wisdom/vulkan/vk_root_signature.h>
#include <wisdom/vulkan/vk_shader.h>
#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_descriptor_storage.h>
#include <wisdom/vulkan/vk_device_ext.h>
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vulkan/impl/vk_device.cpp>
#include <wisdom/vulkan/impl/vk_factory.cpp>
#include <wisdom/vulkan/impl/vk_swapchain.cpp>

//-------------------------------------------------------------------------

export namespace wis {

/**
 * @brief Creates the wis::VKFactory with extensions, specified in extension array.
 * @param debug_layer Enable the debug layer for underlying API.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @return wis::VKFactory on success (wis::Status::Ok).
 * */
[[nodiscard]] inline wis::VKFactory VKCreateFactory(wis::Result& result, bool debug_layer = false, wis::VKFactoryExtension** extensions = nullptr, uint32_t extension_count = 0)
{
    return wis::ImplVKCreateFactory(result, debug_layer, extensions, extension_count);
}
/**
 * @brief Creates the wis::VKFactory with extensions, specified in extension array.
 * @param debug_layer Enable the debug layer for underlying API.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @return wis::VKFactory on success (wis::Status::Ok).
 * */
[[nodiscard]] inline wis::ResultValue<wis::VKFactory> VKCreateFactory(bool debug_layer = false, wis::VKFactoryExtension** extensions = nullptr, uint32_t extension_count = 0) noexcept
{
    return wis::ResultValue<wis::VKFactory>{ &wis::ImplVKCreateFactory, debug_layer, extensions, extension_count };
}
/**
 * @brief Creates the wis::VKDevice with extensions, specified in extension array.
 * @param adapter The adapter to create the logical device on. Must not be NULL.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @param force Create logical device even if some core functionality is absent.
 * The presence of core functionality is checked by the query function.
 * @return wis::VKDevice on success (wis::Status::Ok).
 * */
[[nodiscard]] inline wis::VKDevice VKCreateDevice(wis::Result& result, wis::VKAdapter adapter, wis::VKDeviceExtension** extensions = nullptr, uint32_t extension_count = 0, bool force = false)
{
    return wis::ImplVKCreateDevice(result, std::move(adapter), extensions, extension_count, force);
}
/**
 * @brief Creates the wis::VKDevice with extensions, specified in extension array.
 * @param adapter The adapter to create the logical device on. Must not be NULL.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @param force Create logical device even if some core functionality is absent.
 * The presence of core functionality is checked by the query function.
 * @return wis::VKDevice on success (wis::Status::Ok).
 * */
[[nodiscard]] inline wis::ResultValue<wis::VKDevice> VKCreateDevice(wis::VKAdapter adapter, wis::VKDeviceExtension** extensions = nullptr, uint32_t extension_count = 0, bool force = false) noexcept
{
    return wis::ResultValue<wis::VKDevice>{ &wis::ImplVKCreateDevice, std::move(adapter), extensions, extension_count, force };
}

} // namespace wis
