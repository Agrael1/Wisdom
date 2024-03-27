#pragma once
#include <vulkan/vulkan.h>
#include <wisdom/generated/api/api.h>

namespace wis {
inline constexpr wis::Status convert_vk(VkResult hr) noexcept
{
    switch (hr) {
    case VK_SUCCESS:
        return wis::Status::Ok;
    case VK_TIMEOUT:
        return wis::Status::Timeout;
    case VK_ERROR_UNKNOWN:
        return wis::Status::InvalidArgument;
    case VK_ERROR_OUT_OF_HOST_MEMORY:
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return wis::Status::OutOfMemory;
    case VK_ERROR_DEVICE_LOST:
        return wis::Status::DeviceLost;
    case VK_ERROR_OUT_OF_DATE_KHR:
        return wis::Status::Occluded;
    default:
        return wis::Status::Error;
    }
}
inline constexpr wis::Severity convert_vk(VkDebugUtilsMessageSeverityFlagBitsEXT sev) noexcept
{
    switch (sev) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        return wis::Severity::Trace;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        return wis::Severity::Info;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        return wis::Severity::Warning;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        return wis::Severity::Error;
    default:
        return wis::Severity::Critical;
    }
}

// TODO: move to generated
inline constexpr VkDescriptorType convert_vk(wis::DescriptorType type) noexcept
{
    switch (type) {
    default:
    case DescriptorType::ConstantBuffer:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case DescriptorType::ShaderResource:
        return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    case DescriptorType::UnorderedAccess:
        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    }
}
inline constexpr VkImageAspectFlags aspect_flags(VkFormat format)
{
    switch (format) {
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D16_UNORM_S8_UINT:
        return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_D32_SFLOAT:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
        return VK_IMAGE_ASPECT_DEPTH_BIT;
    case VK_FORMAT_S8_UINT:
        return VK_IMAGE_ASPECT_STENCIL_BIT;
    default:
        return VK_IMAGE_ASPECT_COLOR_BIT;
    }
}
} // namespace wis
