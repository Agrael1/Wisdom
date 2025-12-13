#ifndef WIS_VK_UTILS_HPP
#define WIS_VK_UTILS_HPP
#include <wisdom/generated/c_api.h>
#include <wisdom/util/error_messages.hpp>
#include <wisdom/impl/vulkan/vk_loader.hpp>
#include <vulkan/vulkan.h>

namespace wis::detail {
static constexpr inline WisResult vk_success{ WisStatusOk, VK_SUCCESS, "Operation succeeded." };


//-----------------------------------------------------------------------------
inline constexpr WisStatus convert_vk(VkResult hr) noexcept
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
template<func_pair func, wis::fixed_string message>
WIS_CONSTEXPR23 inline WisResult make_result(VkResult hr) noexcept
{
    static WIS_CONSTEXPR23 const auto str = wis::detail::make_error_string<message, func>();
    return { convert_vk(hr), hr, str.c_str() };
}

//-----------------------------------------------------------------------------
/// @brief Check if the given VkResult a success code
/// @param hr VkResult to check
/// @return True if the VkResult is a success code
constexpr inline bool succeeded(VkResult hr) noexcept
{
    return hr >= 0;
}
} // namespace wis::detail

#endif // WIS_VK_UTILS_HPP