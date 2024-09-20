// GENERATED
#pragma once
#include "dx12/dx12_factory.h"
#include "dx12/dx12_swapchain.h"
#include "dx12/dx12_device.h"

namespace wis {

/**
 * @brief Creates the wis::DX12Factory with extensions, specified in extension array.
 * @param debug_layer Enable the debug layer for underlying API.
 * @param extensions The extensions to enable.
 * The extensions are initialized through this array.
 * @param extension_count The number of extensions to enable.
 * @return wis::DX12Factory on success (wis::Status::Ok).
 * */
inline wis::ResultValue<wis::DX12Factory> DX12CreateFactory(bool debug_layer = false, wis::DX12FactoryExtension** extensions = nullptr, uint32_t extension_count = 0)
{
    return wis::ImplDX12CreateFactory(debug_layer, extensions, extension_count);
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
inline wis::ResultValue<wis::DX12Device> DX12CreateDevice(wis::DX12Adapter adapter, wis::DX12DeviceExtension** extensions = nullptr, uint32_t extension_count = 0, bool force = false)
{
    return wis::ImplDX12CreateDevice(adapter, extensions, extension_count, force);
}

//-------------------------------------------------------------------------

} // namespace wis
