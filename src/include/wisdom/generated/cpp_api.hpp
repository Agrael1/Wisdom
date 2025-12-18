// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_API_HPP
#define WISDOM_CPP_API_HPP
#ifdef __cplusplus
#include <wisdom/global/definitions.h>
#include <wisdom/bridge/span.hpp>

namespace wis {

//==============================================================
// Enums
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Defines common return status codes. Compare against `wis::Status::Ok` for success.
 *
 * */
enum class Status {
    Ok                = 0, ///< Operation succeded.
    Timeout           = 1, ///< Operation timed out.
    Partial           = 2, ///< Operation partially succeeded.
    InvalidArgument   = -1, ///< One or more arguments, or parts of arguments passed to the function were incorrect.
    OutOfHostMemory   = -2, ///< There is no more host memory available.
    OutOfDeviceMemory = -3, ///< There is no more device memory available.
    DeviceLost        = -4, ///< Device driver was forcefully stopped.
    Occluded          = -5, ///< Swapchain presentation was not visible to the user. Rendering is too fast.
    ValidationFailed  = -6, ///< A validation layer found an error.
    Error             = -10000, ///< Operation failed.
};

/**
 * @brief Provided by Wisdom 0.7.0. Orders the adapters according to preference using builtin heuristics of underlying APIs. For DirectX 12, this translates directly to `DXGI_GPU_PREFERENCE`. For Vulkan, sorting is based on `VkPhysicalDeviceType` heuristics.
 *
 * */
enum class AdapterPreference {
    None           = 0, ///< No particular preference, list adapters in system devised order.
    MinConsumption = 1, ///< List the adapters from low power consumption to high. DirectX 12: Integrated, Discrete, External, Software. Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU.
    Performance    = 2, ///< List the adapters from high performance to low. DirectX 12: External, Discrete, Integrated, Software. Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
};

/**
 * @brief Provided by Wisdom 0.7.0. Flags that describe adapter.
 *
 * */
enum class AdapterFlags : uint32_t {
    None     = 0, ///< No flags set. Adapter @wis_may be descrete or embedded.
    Remote   = (1 << 0), ///< Adapter is remote. Used for remote rendering.
    Software = (1 << 1), ///< Adapter is software. Uses CPU for software rendering.
};

//==============================================================
// Structs
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Main source of communication of operation success.
 *
 * */
struct WIS_NODISCARD Result {
    wis::Status  status; ///< defines operation status. Compare with `wis::Status::Ok`.
    std::int32_t platform_code; ///< defines platfrom code from underlying implementation. Is an `HRESULT` for DX12 and a `VkResult` for Vulkan.
    const char*  error; ///< contains a human readable error message.
};

/**
 * @brief Provided by Wisdom 0.7.0. Adapter description. Describes hardware driver identificators as well as memory limits.
 *
 * */
struct AdapterDesc {
    std::array<char, 256>        description; ///< Adapter description. Contains name of the graphics adapter.
    std::uint32_t                vendor_id; ///< denotes Vendor ID. Can be used to find the correct adapter.
    std::uint32_t                device_id; ///< denotes Device ID. Together with `wis::AdapterDesc::vendor_id` uniquely identifies the device.
    std::uint64_t                dedicated_video_memory; ///< measures dedicated video memory in bytes. Used for device local memory type.
    std::uint64_t                shared_system_memory; ///< measures memory that is shared with CPU in bytes. Used for upload and readback.
    std::uint64_t                adapter_id; ///< denotes adapter unique ID (LUID). Can be used to find the correct adapter.
    std::array<std::uint8_t, 16> adapter_uuid; ///< stores UUID of the adapter, used only with Vulkan API on systems with no LUID.
    wis::AdapterFlags            flags; ///< Adapter flags. Describe the adapter kind.
};

} // namespace wis
#endif // __cplusplus
#endif // WISDOM_CPP_API_HPP
