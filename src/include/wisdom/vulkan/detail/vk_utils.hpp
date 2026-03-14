#ifndef WIS_VK_UTILS_HPP
#define WIS_VK_UTILS_HPP

#include <vulkan/vulkan.h>
#include <wisdom/generated/c_api.h>
#include <wisdom/util/error_messages.hpp>
#include <wisdom/vulkan/vk_loader.hpp>

namespace wis::detail {
static constexpr WisResult vk_success{ WisStatusOk, VK_SUCCESS, "Operation succeeded." };

//-----------------------------------------------------------------------------
/**
 * @brief Converts a Vulkan VkResult to a WisStatus. This function maps specific Vulkan result codes to corresponding WisStatus values, allowing for consistent error handling across the Wisdom library when dealing with Vulkan API calls.
 * @param hr The VkResult value returned by a Vulkan function call that indicates the result of the operation.
 * @return A WisStatus value that corresponds to the given VkResult, providing a standardized status code for use within the Wisdom library.
 */
constexpr WisStatus convert_vk(const VkResult hr) noexcept
{
    switch (hr) {
    case VK_SUCCESS:
        return WisStatus::WisStatusOk;
    case VK_TIMEOUT:
        return WisStatus::WisStatusTimeout;
    case VK_ERROR_UNKNOWN:
        return WisStatus::WisStatusInvalidArgument;
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return WisStatus::WisStatusOutOfHostMemory;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return WisStatus::WisStatusOutOfDeviceMemory;
    case VK_ERROR_DEVICE_LOST:
        return WisStatus::WisStatusDeviceLost;
    case VK_ERROR_OUT_OF_DATE_KHR:
        return WisStatus::WisStatusOccluded;
    case VK_ERROR_VALIDATION_FAILED_EXT:
        return WisStatus::WisStatusValidationFailed;
    default:
        return WisStatus::WisStatusError;
    }
}

//-----------------------------------------------------------------------------
/**
 * @brief Creates a WisResult from a Vulkan VkResult, including an error message that incorporates the function name and a custom message. This function uses compile-time string manipulation to generate a descriptive error message based on the source location of the call and the provided custom message.
 * @tparam func A compile-time pair of indices representing the start and end positions of the function name in the source location's function name string. This is used to extract the function name for the error message.
 * @tparam message A compile-time fixed string that provides additional context about the error. This message is included in the generated error string to give more information about the failure.
 * @param hr The VkResult value returned by a Vulkan function call that indicates the result of the operation.
 * @param sl The source location information, which defaults to the current location where wis::detail::make_result is called. This is used to extract the function name for the error message.
 * @return A WisResult object that contains the converted WisStatus, original VkResult, and a descriptive error message that includes both the function name and the custom message.
 */
template<func_pair func, wis::fixed_string message>
WIS_CONSTEXPR23 WisResult make_result(VkResult hr, wis::source_location sl = wis::source_location::current()) noexcept
{
    static const auto str = wis::detail::make_error_string<message, func>(sl);
    return { wis::detail::convert_vk(hr), hr, str.c_str() };
}

//-----------------------------------------------------------------------------
/**
 * @brief Creates a WisResult from a Vulkan VkResult, including an error message that incorporates the function name and a custom message. This function uses compile-time string manipulation to generate a descriptive error message based on the source location of the call and the provided custom message.
 * @tparam func A compile-time pair of indices representing the start and end positions of the function name in the source location's function name string. This is used to extract the function name for the error message.
 * @tparam message A compile-time fixed string that provides additional context about the error. This message is included in the generated error string to give more information about the failure.
 * @param status The WisStatus value that is custom, allowing the caller to specify a more specific status code than the default conversion from VkResult. This can be used to provide more granular error information based on the context of the failure.
 * @param hr The VkResult value returned by a Vulkan function call that indicates the result of the operation.
 * @param sl The source location information, which defaults to the current location where wis::detail::make_result is called. This is used to extract the function name for the error message.
 * @return A WisResult object that contains the converted WisStatus, original VkResult, and a descriptive error message that includes both the function name and the custom message.
 */
template<func_pair func, wis::fixed_string message>
WIS_CONSTEXPR23 WisResult make_result(WisStatus status, VkResult hr, wis::source_location sl = wis::source_location::current()) noexcept
{
    static const auto str = wis::detail::make_error_string<message, func>(sl);
    return { status, hr, str.c_str() };
}

//-----------------------------------------------------------------------------
/**
 * @brief Check if the given VkResult indicates a successful operation.
 * @param hr The VkResult value to check for success.
 * @return True if the VkResult indicates success (non-negative), false otherwise.
 */
constexpr bool succeeded(const VkResult hr) noexcept
{
    return hr >= 0;
}
} // namespace wis::detail

#endif // WIS_VK_UTILS_HPP
