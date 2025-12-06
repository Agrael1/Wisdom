// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_API_H
#define WISDOM_CPP_API_H
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

} // namespace wis
#endif // __cplusplus
#endif // WISDOM_CPP_API_H
