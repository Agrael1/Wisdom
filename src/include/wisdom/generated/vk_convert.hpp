// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_VK_CONVERT_HPP
#define WISDOM_CPP_VK_CONVERT_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include <wisdom/generated/c_api.h>
#include <vulkan/vulkan.h>

namespace wis {
namespace detail {

inline VkQueueGlobalPriorityEXT convert_vk(WisCommandQueuePriority value) noexcept
{
    switch (value) {
    case WisCommandQueuePriorityNormal:
        return VK_QUEUE_GLOBAL_PRIORITY_MEDIUM_EXT;
    case WisCommandQueuePriorityHigh:
        return VK_QUEUE_GLOBAL_PRIORITY_HIGH_EXT;
    case WisCommandQueuePriorityRealtime:
        return VK_QUEUE_GLOBAL_PRIORITY_REALTIME_EXT;
    default:
        return VK_QUEUE_GLOBAL_PRIORITY_MEDIUM_EXT;
    }
}

inline VkShaderStageFlags convert_vk(WisShaderStages value) noexcept
{
    switch (value) {
    case WisShaderStagesAll:
        return VK_SHADER_STAGE_ALL;
    case WisShaderStagesVertex:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case WisShaderStagesHull:
        return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    case WisShaderStagesDomain:
        return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    case WisShaderStagesGeometry:
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    case WisShaderStagesPixel:
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    case WisShaderStagesAmplification:
        return VK_SHADER_STAGE_TASK_BIT_EXT;
    case WisShaderStagesMesh:
        return VK_SHADER_STAGE_MESH_BIT_EXT;
    default:
        return VK_SHADER_STAGE_ALL;
    }
}

inline VkDescriptorType convert_vk(WisDescriptorType value) noexcept
{
    switch (value) {
    case WisDescriptorTypeSampler:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    case WisDescriptorTypeConstantBuffer:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case WisDescriptorTypeTexture:
        return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    case WisDescriptorTypeRWTexture:
        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    case WisDescriptorTypeRWBuffer:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    case WisDescriptorTypeBuffer:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    case WisDescriptorTypeAccelerationStructure:
        return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    default:
        return VK_DESCRIPTOR_TYPE_MAX_ENUM;
    }
}

inline VkCompareOp convert_vk(WisCompareOperation value) noexcept
{
    switch (value) {
    case WisCompareOperationNone:
        return VK_COMPARE_OP_NEVER;
    case WisCompareOperationNever:
        return VK_COMPARE_OP_NEVER;
    case WisCompareOperationLess:
        return VK_COMPARE_OP_LESS;
    case WisCompareOperationEqual:
        return VK_COMPARE_OP_EQUAL;
    case WisCompareOperationLessEqual:
        return VK_COMPARE_OP_LESS_OR_EQUAL;
    case WisCompareOperationGreater:
        return VK_COMPARE_OP_GREATER;
    case WisCompareOperationNotEqual:
        return VK_COMPARE_OP_NOT_EQUAL;
    case WisCompareOperationGreaterEqual:
        return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case WisCompareOperationAlways:
        return VK_COMPARE_OP_ALWAYS;
    default:
        return VK_COMPARE_OP_NEVER;
    }
}

inline VkSamplerAddressMode convert_vk(WisAddressMode value) noexcept
{
    switch (value) {
    case WisAddressModeRepeat:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case WisAddressModeMirroredRepeat:
        return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case WisAddressModeClampToEdge:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case WisAddressModeClampToBorder:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    case WisAddressModeMirrorClampToEdge:
        return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
    default:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    }
}

inline VkFilter convert_vk(WisFilter value) noexcept
{
    switch (value) {
    case WisFilterPoint:
        return VK_FILTER_NEAREST;
    case WisFilterLinear:
        return VK_FILTER_LINEAR;
    default:
        return VK_FILTER_NEAREST;
    }
}

inline VkBorderColor convert_vk(WisStaticBorder value) noexcept
{
    switch (value) {
    case WisStaticBorderTransparentBlack:
        return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    case WisStaticBorderOpaqueBlack:
        return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
    case WisStaticBorderOpaqueWhite:
        return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    case WisStaticBorderCustom:
        return VK_BORDER_COLOR_FLOAT_CUSTOM_EXT;
    default:
        return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    }
}

inline VkMemoryPropertyFlags convert_vk(WisMemoryType value) noexcept
{
    switch (value) {
    case WisMemoryTypeDeviceLocal:
        return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    case WisMemoryTypeUpload:
        return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    case WisMemoryTypeReadback:
        return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
    case WisMemoryTypeGPUUpload:
        return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    default:
        return static_cast<VkMemoryPropertyFlags>(0);
    }
}

inline VkBufferUsageFlags convert_vk(WisBufferUsageFlags value) noexcept
{
    VkBufferUsageFlags result = static_cast<VkBufferUsageFlags>(0);
    if (value & WisBufferUsageFlagsCopySrc) {
        result |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    }
    if (value & WisBufferUsageFlagsCopyDst) {
        result |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    if (value & WisBufferUsageFlagsConstantBuffer) {
        result |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    }
    if (value & WisBufferUsageFlagsIndexBuffer) {
        result |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }
    if (value & WisBufferUsageFlagsVertexBuffer) {
        result |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    }
    if (value & WisBufferUsageFlagsIndirectBuffer) {
        result |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
    }
    if (value & WisBufferUsageFlagsStorageBuffer) {
        result |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    }
    if (value & WisBufferUsageFlagsAccelerationStructureBuffer) {
        result |= VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    }
    if (value & WisBufferUsageFlagsAccelerationStructureInput) {
        result |= VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;
    }
    if (value & WisBufferUsageFlagsShaderBindingTable) {
        result |= VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR;
    }
    return result;
}

inline VmaAllocationCreateFlags convert_vk(WisMemoryFlags value) noexcept
{
    VmaAllocationCreateFlags result = static_cast<VmaAllocationCreateFlags>(0);
    if (value & WisMemoryFlagsDedicatedAllocation) {
        result |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    }
    if (value & WisMemoryFlagsMapped) {
        result |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
    }
    return result;
}

} // namespace detail
} // namespace wis
#endif // WISDOM_VK_CONVERT_HPP
