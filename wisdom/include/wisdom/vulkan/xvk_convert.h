#pragma once
#include <wisdom/api/api.h>
#include <vulkan/vulkan.h>

namespace wis {
inline constexpr wis::Status convert(VkResult hr) noexcept
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
    default:
        return wis::Status::Error;
    }
}
inline constexpr wis::Severity convert(VkDebugUtilsMessageSeverityFlagBitsEXT sev) noexcept
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
inline constexpr VkShaderStageFlagBits convert(wis::ShaderStages stage) noexcept
{
    switch (stage) {
    case wis::ShaderStages::Vertex:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case wis::ShaderStages::Pixel:
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    case wis::ShaderStages::Geometry:
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    case wis::ShaderStages::Hull:
        return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    case wis::ShaderStages::Domain:
        return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    case wis::ShaderStages::Amplification:
        return VK_SHADER_STAGE_TASK_BIT_NV;
    case wis::ShaderStages::Mesh:
        return VK_SHADER_STAGE_MESH_BIT_NV;
    default:
        return VK_SHADER_STAGE_ALL;
    }
}
inline constexpr VkDescriptorType convert(wis::DescriptorType type) noexcept
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

} // namespace wis
