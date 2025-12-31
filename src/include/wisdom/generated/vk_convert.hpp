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
        return static_cast<VkShaderStageFlags>(value);
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
        return static_cast<VkDescriptorType>(value);
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
        return static_cast<VkCompareOp>(value);
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
        return static_cast<VkSamplerAddressMode>(value);
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
        return static_cast<VkFilter>(value);
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
    default:
        return static_cast<VkBorderColor>(value);
    }
}

} // namespace detail
} // namespace wis
#endif // WISDOM_VK_CONVERT_HPP
