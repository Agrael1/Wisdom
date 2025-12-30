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

inline VkShaderStageFlagBits convert_vk(WisShaderStages value) noexcept
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
        return static_cast<VkShaderStageFlagBits>(value);
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

} // namespace detail
} // namespace wis
#endif // WISDOM_VK_CONVERT_HPP
