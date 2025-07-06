/** Wisdom API Version 0.6.11

Copyright (c) 2024 Ilya Doroshenko. All rights reserved.
License: MIT
See also: [repository on GitHub](https://github.com/Agrael1/Wisdom)
*/

module;
#define WISDOM_MODULE_DECL
#define WISDOM_BUILD_BINARIES
#include <wisdom/generated/dx12/dx12.include.h>
export module wisdom.dx12;

export import wisdom.api;

#include <wisdom/dx12/dx12_convert.h>
#include <wisdom/dx12/dx12_info.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/dx12/dx12_memory.h>
#include <wisdom/dx12/dx12_resource.h>
#include <wisdom/dx12/dx12_allocator.h>
#include <wisdom/dx12/dx12_command_list.h>
#include <wisdom/dx12/dx12_descriptor_storage.h>
#include <wisdom/generated/dx12/dx12_structs.hpp>
#include <wisdom/dx12/impl/dx12_command_list.cpp>
#include <wisdom/dx12/dx12_command_queue.h>
#include <wisdom/dx12/dx12_debug.h>
#include <wisdom/dx12/dx12_unique_event.h>
#include <wisdom/dx12/dx12_fence.h>
#include <wisdom/dx12/dx12_pipeline_state.h>
#include <wisdom/dx12/dx12_root_signature.h>
#include <wisdom/dx12/dx12_shader.h>
#include <wisdom/dx12/dx12_device_ext.h>
#include <wisdom/dx12/dx12_device.h>
#include <wisdom/dx12/dx12_factory_ext.h>
#include <wisdom/dx12/dx12_factory.h>
#include <wisdom/dx12/impl/dx12_factory.cpp>
#include <wisdom/dx12/dx12_swapchain.h>
#include <wisdom/dx12/impl/dx12_allocator.cpp>
#include <wisdom/dx12/impl/dx12_device.cpp>
#include <wisdom/dx12/impl/dx12_fence.cpp>
#include <wisdom/dx12/impl/dx12_info.cpp>
#include <wisdom/dx12/impl/dx12_swapchain.cpp>

//-------------------------------------------------------------------------

export namespace wis {

/**
 * @brief Creates the wis::DX12Factory with extensions, specified in extension array.
 * @param debug_layer Enable the debug layer for underlying API.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @return wis::DX12Factory on success (wis::Status::Ok).
 * */
[[nodiscard]] inline wis::DX12Factory DX12CreateFactory(wis::Result& result, bool debug_layer = false, wis::DX12FactoryExtension** extensions = nullptr, uint32_t extension_count = 0)
{
    return wis::ImplDX12CreateFactory(result, debug_layer, extensions, extension_count);
}
/**
 * @brief Creates the wis::DX12Factory with extensions, specified in extension array.
 * @param debug_layer Enable the debug layer for underlying API.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @return wis::DX12Factory on success (wis::Status::Ok).
 * */
[[nodiscard]] inline wis::ResultValue<wis::DX12Factory> DX12CreateFactory(bool debug_layer = false, wis::DX12FactoryExtension** extensions = nullptr, uint32_t extension_count = 0) noexcept
{
    return wis::ResultValue<wis::DX12Factory>{ &wis::ImplDX12CreateFactory, debug_layer, extensions, extension_count };
}
/**
 * @brief Creates the wis::DX12Device with extensions, specified in extension array.
 * @param adapter The adapter to create the logical device on. Must not be NULL.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @param force Create logical device even if some core functionality is absent.
 * The presence of core functionality is checked by the query function.
 * @return wis::DX12Device on success (wis::Status::Ok).
 * */
[[nodiscard]] inline wis::DX12Device DX12CreateDevice(wis::Result& result, wis::DX12Adapter adapter, wis::DX12DeviceExtension** extensions = nullptr, uint32_t extension_count = 0, bool force = false)
{
    return wis::ImplDX12CreateDevice(result, std::move(adapter), extensions, extension_count, force);
}
/**
 * @brief Creates the wis::DX12Device with extensions, specified in extension array.
 * @param adapter The adapter to create the logical device on. Must not be NULL.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @param force Create logical device even if some core functionality is absent.
 * The presence of core functionality is checked by the query function.
 * @return wis::DX12Device on success (wis::Status::Ok).
 * */
[[nodiscard]] inline wis::ResultValue<wis::DX12Device> DX12CreateDevice(wis::DX12Adapter adapter, wis::DX12DeviceExtension** extensions = nullptr, uint32_t extension_count = 0, bool force = false) noexcept
{
    return wis::ResultValue<wis::DX12Device>{ &wis::ImplDX12CreateDevice, std::move(adapter), extensions, extension_count, force };
}

} // namespace wis
