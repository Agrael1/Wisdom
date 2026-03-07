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

inline VkFormat convert_vk(WisDataFormat value) noexcept
{
    switch (value) {
    case WisDataFormatRGBA32Float:
        return VK_FORMAT_R32G32B32A32_SFLOAT;
    case WisDataFormatRGBA32Uint:
        return VK_FORMAT_R32G32B32A32_UINT;
    case WisDataFormatRGBA32Sint:
        return VK_FORMAT_R32G32B32A32_SINT;
    case WisDataFormatRGB32Float:
        return VK_FORMAT_R32G32B32_SFLOAT;
    case WisDataFormatRGB32Uint:
        return VK_FORMAT_R32G32B32_UINT;
    case WisDataFormatRGB32Sint:
        return VK_FORMAT_R32G32B32_SINT;
    case WisDataFormatRGBA16Float:
        return VK_FORMAT_R16G16B16A16_SFLOAT;
    case WisDataFormatRGBA16Unorm:
        return VK_FORMAT_R16G16B16A16_UNORM;
    case WisDataFormatRGBA16Uint:
        return VK_FORMAT_R16G16B16A16_UINT;
    case WisDataFormatRGBA16Snorm:
        return VK_FORMAT_R16G16B16A16_SNORM;
    case WisDataFormatRGBA16Sint:
        return VK_FORMAT_R16G16B16A16_SINT;
    case WisDataFormatRG32Float:
        return VK_FORMAT_R32G32_SFLOAT;
    case WisDataFormatRG32Uint:
        return VK_FORMAT_R32G32_UINT;
    case WisDataFormatRG32Sint:
        return VK_FORMAT_R32G32_SINT;
    case WisDataFormatD32FloatS8Uint:
        return VK_FORMAT_D32_SFLOAT_S8_UINT;
    case WisDataFormatRGB10A2Unorm:
        return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
    case WisDataFormatRGB10A2Uint:
        return VK_FORMAT_A2B10G10R10_UINT_PACK32;
    case WisDataFormatRG11B10Float:
        return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
    case WisDataFormatRGBA8Unorm:
        return VK_FORMAT_R8G8B8A8_UNORM;
    case WisDataFormatRGBA8UnormSrgb:
        return VK_FORMAT_R8G8B8A8_SRGB;
    case WisDataFormatRGBA8Uint:
        return VK_FORMAT_R8G8B8A8_UINT;
    case WisDataFormatRGBA8Snorm:
        return VK_FORMAT_R8G8B8A8_SNORM;
    case WisDataFormatRGBA8Sint:
        return VK_FORMAT_R8G8B8A8_SINT;
    case WisDataFormatRG16Float:
        return VK_FORMAT_R16G16_SFLOAT;
    case WisDataFormatRG16Unorm:
        return VK_FORMAT_R16G16_UNORM;
    case WisDataFormatRG16Uint:
        return VK_FORMAT_R16G16_UINT;
    case WisDataFormatRG16Snorm:
        return VK_FORMAT_R16G16_SNORM;
    case WisDataFormatRG16Sint:
        return VK_FORMAT_R16G16_SINT;
    case WisDataFormatD32Float:
        return VK_FORMAT_D32_SFLOAT;
    case WisDataFormatR32Float:
        return VK_FORMAT_R32_SFLOAT;
    case WisDataFormatR32Uint:
        return VK_FORMAT_R32_UINT;
    case WisDataFormatR32Sint:
        return VK_FORMAT_R32_SINT;
    case WisDataFormatD24UnormS8Uint:
        return VK_FORMAT_D24_UNORM_S8_UINT;
    case WisDataFormatRG8Unorm:
        return VK_FORMAT_R8G8_UNORM;
    case WisDataFormatRG8Uint:
        return VK_FORMAT_R8G8_UINT;
    case WisDataFormatRG8Snorm:
        return VK_FORMAT_R8G8_SNORM;
    case WisDataFormatRG8Sint:
        return VK_FORMAT_R8G8_SINT;
    case WisDataFormatR16Float:
        return VK_FORMAT_R16_SFLOAT;
    case WisDataFormatD16Unorm:
        return VK_FORMAT_D16_UNORM;
    case WisDataFormatR16Unorm:
        return VK_FORMAT_R16_UNORM;
    case WisDataFormatR16Uint:
        return VK_FORMAT_R16_UINT;
    case WisDataFormatR16Snorm:
        return VK_FORMAT_R16_SNORM;
    case WisDataFormatR16Sint:
        return VK_FORMAT_R16_SINT;
    case WisDataFormatR8Unorm:
        return VK_FORMAT_R8_UNORM;
    case WisDataFormatR8Uint:
        return VK_FORMAT_R8_UINT;
    case WisDataFormatR8Snorm:
        return VK_FORMAT_R8_SNORM;
    case WisDataFormatR8Sint:
        return VK_FORMAT_R8_SINT;
    case WisDataFormatRGB9E5UFloat:
        return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
    case WisDataFormatBC1RGBAUnorm:
        return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
    case WisDataFormatBC1RGBAUnormSrgb:
        return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
    case WisDataFormatBC2RGBAUnorm:
        return VK_FORMAT_BC2_UNORM_BLOCK;
    case WisDataFormatBC2RGBAUnormSrgb:
        return VK_FORMAT_BC2_SRGB_BLOCK;
    case WisDataFormatBC3RGBAUnorm:
        return VK_FORMAT_BC3_UNORM_BLOCK;
    case WisDataFormatBC3RGBAUnormSrgb:
        return VK_FORMAT_BC3_SRGB_BLOCK;
    case WisDataFormatBC4RUnorm:
        return VK_FORMAT_BC4_UNORM_BLOCK;
    case WisDataFormatBC4RSnorm:
        return VK_FORMAT_BC4_SNORM_BLOCK;
    case WisDataFormatBC5RGUnorm:
        return VK_FORMAT_BC5_UNORM_BLOCK;
    case WisDataFormatBC5RGSnorm:
        return VK_FORMAT_BC5_SNORM_BLOCK;
    case WisDataFormatB5G6R5Unorm:
        return VK_FORMAT_R5G6B5_UNORM_PACK16;
    case WisDataFormatB5G5R5A1Unorm:
        return VK_FORMAT_A1R5G5B5_UNORM_PACK16;
    case WisDataFormatBGRA8Unorm:
        return VK_FORMAT_B8G8R8A8_UNORM;
    case WisDataFormatBGRA8UnormSrgb:
        return VK_FORMAT_B8G8R8A8_SRGB;
    case WisDataFormatBC6HUfloat16:
        return VK_FORMAT_BC6H_UFLOAT_BLOCK;
    case WisDataFormatBC6HSfloat16:
        return VK_FORMAT_BC6H_SFLOAT_BLOCK;
    case WisDataFormatBC7RGBAUnorm:
        return VK_FORMAT_BC7_UNORM_BLOCK;
    case WisDataFormatBC7RGBAUnormSrgb:
        return VK_FORMAT_BC7_SRGB_BLOCK;
    case WisDataFormatBGRA4Unorm:
        return VK_FORMAT_A4R4G4B4_UNORM_PACK16;
    default:
        return static_cast<VkFormat>(0);
    }
}

inline VkSampleCountFlagBits convert_vk(WisSampleCount value) noexcept
{
    switch (value) {
    case WisSampleCountS1:
        return VK_SAMPLE_COUNT_1_BIT;
    case WisSampleCountS2:
        return VK_SAMPLE_COUNT_2_BIT;
    case WisSampleCountS4:
        return VK_SAMPLE_COUNT_4_BIT;
    case WisSampleCountS8:
        return VK_SAMPLE_COUNT_8_BIT;
    case WisSampleCountS16:
        return VK_SAMPLE_COUNT_16_BIT;
    default:
        return static_cast<VkSampleCountFlagBits>(0);
    }
}

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

inline VkShaderStageFlags convert_vk(WisShaderVisibility value) noexcept
{
    switch (value) {
    case WisShaderVisibilityAll:
        return VK_SHADER_STAGE_ALL;
    case WisShaderVisibilityVertex:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case WisShaderVisibilityHull:
        return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    case WisShaderVisibilityDomain:
        return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    case WisShaderVisibilityGeometry:
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    case WisShaderVisibilityPixel:
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    case WisShaderVisibilityAmplification:
        return VK_SHADER_STAGE_TASK_BIT_EXT;
    case WisShaderVisibilityMesh:
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

inline VkSamplerReductionModeEXT convert_vk(WisReductionMode value) noexcept
{
    switch (value) {
    case WisReductionModeStandard:
        return VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT;
    case WisReductionModeMin:
        return VK_SAMPLER_REDUCTION_MODE_MIN_EXT;
    case WisReductionModeMax:
        return VK_SAMPLER_REDUCTION_MODE_MAX_EXT;
    default:
        return VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT;
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
        return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    }
}

inline VkComponentSwizzle convert_vk(WisComponentSwizzle value) noexcept
{
    switch (value) {
    case WisComponentSwizzleIdentity:
        return VK_COMPONENT_SWIZZLE_IDENTITY;
    case WisComponentSwizzleRed:
        return VK_COMPONENT_SWIZZLE_R;
    case WisComponentSwizzleGreen:
        return VK_COMPONENT_SWIZZLE_G;
    case WisComponentSwizzleBlue:
        return VK_COMPONENT_SWIZZLE_B;
    case WisComponentSwizzleAlpha:
        return VK_COMPONENT_SWIZZLE_A;
    case WisComponentSwizzleZero:
        return VK_COMPONENT_SWIZZLE_ZERO;
    case WisComponentSwizzleOne:
        return VK_COMPONENT_SWIZZLE_ONE;
    default:
        return static_cast<VkComponentSwizzle>(0);
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
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
    }
    if (value & WisBufferUsageFlagsCopyDst) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_BUFFER_USAGE_TRANSFER_DST_BIT));
    }
    if (value & WisBufferUsageFlagsConstantBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT));
    }
    if (value & WisBufferUsageFlagsIndexBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_BUFFER_USAGE_INDEX_BUFFER_BIT));
    }
    if (value & WisBufferUsageFlagsVertexBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT));
    }
    if (value & WisBufferUsageFlagsIndirectBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT));
    }
    if (value & WisBufferUsageFlagsStorageBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT));
    }
    if (value & WisBufferUsageFlagsAccelerationStructureBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT));
    }
    if (value & WisBufferUsageFlagsAccelerationStructureInput) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR));
    }
    if (value & WisBufferUsageFlagsShaderBindingTable) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR));
    }
    return result;
}

inline VkImageUsageFlags convert_vk(WisTextureUsageFlags value) noexcept
{
    VkImageUsageFlags result = static_cast<VkImageUsageFlags>(0);
    if (value & WisTextureUsageFlagsRenderTarget) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT));
    }
    if (value & WisTextureUsageFlagsDepthStencil) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT));
    }
    if (value & WisTextureUsageFlagsCopySrc) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_IMAGE_USAGE_TRANSFER_SRC_BIT));
    }
    if (value & WisTextureUsageFlagsCopyDst) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_IMAGE_USAGE_TRANSFER_DST_BIT));
    }
    if (value & WisTextureUsageFlagsShaderResource) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_IMAGE_USAGE_SAMPLED_BIT));
    }
    if (value & WisTextureUsageFlagsUnorderedAccess) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_IMAGE_USAGE_STORAGE_BIT));
    }
    if (value & WisTextureUsageFlagsHostCopy) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT));
    }
    return result;
}

inline VmaAllocationCreateFlags convert_vk(WisMemoryFlags value) noexcept
{
    VmaAllocationCreateFlags result = static_cast<VmaAllocationCreateFlags>(0);
    if (value & WisMemoryFlagsDedicatedAllocation) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT));
    }
    if (value & WisMemoryFlagsMapped) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VMA_ALLOCATION_CREATE_MAPPED_BIT));
    }
    return result;
}

inline VkPipelineStageFlags2 convert_vk(WisBarrierSync value) noexcept
{
    VkPipelineStageFlags2 result = static_cast<VkPipelineStageFlags2>(0);
    if (value & WisBarrierSyncAll) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT));
    }
    if (value & WisBarrierSyncDraw) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_VERTEX_ATTRIBUTE_INPUT_BIT | VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT | VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT));
    }
    if (value & WisBarrierSyncIndexInput) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT));
    }
    if (value & WisBarrierSyncVertexShading) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT));
    }
    if (value & WisBarrierSyncPixelShading) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT));
    }
    if (value & WisBarrierSyncDepthStencil) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT));
    }
    if (value & WisBarrierSyncRenderTarget) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT));
    }
    if (value & WisBarrierSyncCompute) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT));
    }
    if (value & WisBarrierSyncRaytracing) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_RAY_TRACING_SHADER_BIT_KHR));
    }
    if (value & WisBarrierSyncCopy) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_COPY_BIT));
    }
    if (value & WisBarrierSyncResolve) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_COPY_BIT | VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_2_RESOLVE_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT));
    }
    if (value & WisBarrierSyncExecuteIndirect) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT));
    }
    if (value & WisBarrierSyncAllShading) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT));
    }
    if (value & WisBarrierSyncNonPixelShading) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT));
    }
    if (value & WisBarrierSyncClearUAV) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT));
    }
    if (value & WisBarrierSyncVideoDecode) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_VIDEO_DECODE_BIT_KHR));
    }
    if (value & WisBarrierSyncVideoEncode) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_VIDEO_ENCODE_BIT_KHR));
    }
    if (value & WisBarrierSyncBuildRTAS) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_BUILD_BIT_KHR));
    }
    if (value & WisBarrierSyncCopyRTAS) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_COPY_BIT_KHR));
    }
    return result;
}

inline VkAccessFlags2 convert_vk(WisResourceAccess value) noexcept
{
    VkAccessFlags2 result = static_cast<VkAccessFlags2>(0);
    if (value & WisResourceAccessVertexBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT));
    }
    if (value & WisResourceAccessConstantBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_UNIFORM_READ_BIT));
    }
    if (value & WisResourceAccessIndexBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_INDEX_READ_BIT));
    }
    if (value & WisResourceAccessRenderTarget) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT));
    }
    if (value & WisResourceAccessUnorderedAccess) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_SHADER_WRITE_BIT | VK_ACCESS_2_SHADER_READ_BIT));
    }
    if (value & WisResourceAccessDepthWrite) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT));
    }
    if (value & WisResourceAccessDepthRead) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT));
    }
    if (value & WisResourceAccessShaderResource) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_SHADER_READ_BIT));
    }
    if (value & WisResourceAccessStreamOutput) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_TRANSFORM_FEEDBACK_WRITE_BIT_EXT));
    }
    if (value & WisResourceAccessIndirectArgument) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT | VK_ACCESS_2_SHADER_READ_BIT));
    }
    if (value & WisResourceAccessCopyDest) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_TRANSFER_WRITE_BIT));
    }
    if (value & WisResourceAccessCopySource) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_TRANSFER_READ_BIT));
    }
    if (value & WisResourceAccessConditionalRendering) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_CONDITIONAL_RENDERING_READ_BIT_EXT));
    }
    if (value & WisResourceAccessAccelerationStructureRead) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_ACCELERATION_STRUCTURE_READ_BIT_KHR));
    }
    if (value & WisResourceAccessAccelerationStructureWrite) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_ACCELERATION_STRUCTURE_WRITE_BIT_KHR));
    }
    if (value & WisResourceAccessShadingRate) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR));
    }
    if (value & WisResourceAccessVideoDecodeRead) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_VIDEO_DECODE_READ_BIT_KHR));
    }
    if (value & WisResourceAccessVideoDecodeWrite) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_VIDEO_DECODE_WRITE_BIT_KHR));
    }
    if (value & WisResourceAccessResolveDest) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_TRANSFER_WRITE_BIT | VK_ACCESS_2_SHADER_WRITE_BIT));
    }
    if (value & WisResourceAccessResolveSource) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_TRANSFER_READ_BIT | VK_ACCESS_2_SHADER_READ_BIT));
    }
    if (value & WisResourceAccessNoAccess) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(VK_ACCESS_2_NONE));
    }
    return result;
}

} // namespace detail
} // namespace wis
#endif // WISDOM_VK_CONVERT_HPP
