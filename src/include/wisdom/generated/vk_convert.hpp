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

constexpr inline VkFormat convert_vk(WisDataFormat value) noexcept
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

constexpr inline VkSampleCountFlagBits convert_vk(WisSampleCount value) noexcept
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

constexpr inline VkQueueGlobalPriorityEXT convert_vk(WisCommandQueuePriority value) noexcept
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

constexpr inline VkPipelineBindPoint convert_vk(WisPipelineType value) noexcept
{
    switch (value) {
    case WisPipelineTypeGraphics:
        return VK_PIPELINE_BIND_POINT_GRAPHICS;
    case WisPipelineTypeCompute:
        return VK_PIPELINE_BIND_POINT_COMPUTE;
    case WisPipelineTypeRayTracing:
        return VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR;
    default:
        return VK_PIPELINE_BIND_POINT_GRAPHICS;
    }
}

constexpr inline VkShaderStageFlags convert_vk(WisShaderVisibility value) noexcept
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

constexpr inline VkDescriptorType convert_vk(WisDescriptorType value) noexcept
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

constexpr inline VkCompareOp convert_vk(WisCompareOp value) noexcept
{
    switch (value) {
    case WisCompareOpNone:
        return VK_COMPARE_OP_NEVER;
    case WisCompareOpNever:
        return VK_COMPARE_OP_NEVER;
    case WisCompareOpLess:
        return VK_COMPARE_OP_LESS;
    case WisCompareOpEqual:
        return VK_COMPARE_OP_EQUAL;
    case WisCompareOpLessEqual:
        return VK_COMPARE_OP_LESS_OR_EQUAL;
    case WisCompareOpGreater:
        return VK_COMPARE_OP_GREATER;
    case WisCompareOpNotEqual:
        return VK_COMPARE_OP_NOT_EQUAL;
    case WisCompareOpGreaterEqual:
        return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case WisCompareOpAlways:
        return VK_COMPARE_OP_ALWAYS;
    default:
        return VK_COMPARE_OP_NEVER;
    }
}

constexpr inline VkSamplerAddressMode convert_vk(WisAddressMode value) noexcept
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

constexpr inline VkFilter convert_vk(WisFilter value) noexcept
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

constexpr inline VkSamplerReductionModeEXT convert_vk(WisReductionMode value) noexcept
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

constexpr inline VkBorderColor convert_vk(WisStaticBorder value) noexcept
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

constexpr inline VkComponentSwizzle convert_vk(WisComponentSwizzle value) noexcept
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

constexpr inline VkMemoryPropertyFlags convert_vk(WisMemoryType value) noexcept
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

constexpr inline VkImageLayout convert_vk(WisTextureState value) noexcept
{
    switch (value) {
    case WisTextureStateUndefined:
        return VK_IMAGE_LAYOUT_UNDEFINED;
    case WisTextureStateCommon:
        return VK_IMAGE_LAYOUT_GENERAL;
    case WisTextureStateRead:
        return VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
    case WisTextureStateRenderTarget:
        return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    case WisTextureStateUnorderedAccess:
        return VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    case WisTextureStateDepthStencilWrite:
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    case WisTextureStateDepthStencilRead:
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    case WisTextureStateShaderResource:
        return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    case WisTextureStateCopySrc:
        return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    case WisTextureStateCopyDst:
        return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    case WisTextureStatePresent:
        return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    case WisTextureStateShadingRate:
        return VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR;
    case WisTextureStateVideoDecodeRead:
        return VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR;
    case WisTextureStateVideoDecodeWrite:
        return VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR;
    default:
        return static_cast<VkImageLayout>(0);
    }
}

constexpr inline VkStencilOp convert_vk(WisStencilOp value) noexcept
{
    switch (value) {
    case WisStencilOpKeep:
        return VK_STENCIL_OP_KEEP;
    case WisStencilOpZero:
        return VK_STENCIL_OP_ZERO;
    case WisStencilOpReplace:
        return VK_STENCIL_OP_REPLACE;
    case WisStencilOpIncClamp:
        return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
    case WisStencilOpDecClamp:
        return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
    case WisStencilOpInvert:
        return VK_STENCIL_OP_INVERT;
    case WisStencilOpIncWrap:
        return VK_STENCIL_OP_INCREMENT_AND_WRAP;
    case WisStencilOpDecWrap:
        return VK_STENCIL_OP_DECREMENT_AND_WRAP;
    default:
        return static_cast<VkStencilOp>(0);
    }
}

constexpr inline VkPrimitiveTopology convert_vk(WisTopologyType value) noexcept
{
    switch (value) {
    case WisTopologyTypePoint:
        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case WisTopologyTypeLine:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case WisTopologyTypeTriangle:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case WisTopologyTypePatch:
        return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
    default:
        return static_cast<VkPrimitiveTopology>(0);
    }
}

constexpr inline VkPolygonMode convert_vk(WisFillMode value) noexcept
{
    switch (value) {
    case WisFillModeLines:
        return VK_POLYGON_MODE_LINE;
    case WisFillModeSolid:
        return VK_POLYGON_MODE_FILL;
    default:
        return static_cast<VkPolygonMode>(0);
    }
}

constexpr inline VkCullModeFlags convert_vk(WisCullMode value) noexcept
{
    switch (value) {
    case WisCullModeNone:
        return VK_CULL_MODE_NONE;
    case WisCullModeFront:
        return VK_CULL_MODE_FRONT_BIT;
    case WisCullModeBack:
        return VK_CULL_MODE_BACK_BIT;
    default:
        return static_cast<VkCullModeFlags>(0);
    }
}

constexpr inline VkFrontFace convert_vk(WisWindingOrder value) noexcept
{
    switch (value) {
    case WisWindingOrderClockwise:
        return VK_FRONT_FACE_CLOCKWISE;
    case WisWindingOrderCounterClockwise:
        return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    default:
        return static_cast<VkFrontFace>(0);
    }
}

constexpr inline VkConservativeRasterizationModeEXT convert_vk(WisConservativeRasterization value) noexcept
{
    return static_cast<VkConservativeRasterizationModeEXT>(value);
}

constexpr inline VkLineRasterizationModeEXT convert_vk(WisLineRasterization value) noexcept
{
    return static_cast<VkLineRasterizationModeEXT>(value);
}

constexpr inline VkBlendFactor convert_vk(WisBlendFactor value) noexcept
{
    switch (value) {
    case WisBlendFactorZero:
        return VK_BLEND_FACTOR_ZERO;
    case WisBlendFactorOne:
        return VK_BLEND_FACTOR_ONE;
    case WisBlendFactorSrcColor:
        return VK_BLEND_FACTOR_SRC_COLOR;
    case WisBlendFactorInvSrcColor:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case WisBlendFactorSrcAlpha:
        return VK_BLEND_FACTOR_SRC_ALPHA;
    case WisBlendFactorInvSrcAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case WisBlendFactorDestAlpha:
        return VK_BLEND_FACTOR_DST_ALPHA;
    case WisBlendFactorInvDestAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case WisBlendFactorDestColor:
        return VK_BLEND_FACTOR_DST_COLOR;
    case WisBlendFactorInvDestColor:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case WisBlendFactorSrcAlphaSat:
        return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
    case WisBlendFactorConstantColor:
        return VK_BLEND_FACTOR_CONSTANT_COLOR;
    case WisBlendFactorInvBlendFactor:
        return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
    case WisBlendFactorSrc1Color:
        return VK_BLEND_FACTOR_SRC1_COLOR;
    case WisBlendFactorInvSrc1Color:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
    case WisBlendFactorSrc1Alpha:
        return VK_BLEND_FACTOR_SRC1_ALPHA;
    case WisBlendFactorInvSrc1Alpha:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
    default:
        return static_cast<VkBlendFactor>(0);
    }
}

constexpr inline VkBlendOp convert_vk(WisBlendOp value) noexcept
{
    switch (value) {
    case WisBlendOpAdd:
        return VK_BLEND_OP_ADD;
    case WisBlendOpSubtract:
        return VK_BLEND_OP_SUBTRACT;
    case WisBlendOpRevSubtract:
        return VK_BLEND_OP_REVERSE_SUBTRACT;
    case WisBlendOpMin:
        return VK_BLEND_OP_MIN;
    case WisBlendOpMax:
        return VK_BLEND_OP_MAX;
    default:
        return static_cast<VkBlendOp>(0);
    }
}

constexpr inline VkLogicOp convert_vk(WisLogicOp value) noexcept
{
    switch (value) {
    case WisLogicOpClear:
        return VK_LOGIC_OP_CLEAR;
    case WisLogicOpSet:
        return VK_LOGIC_OP_SET;
    case WisLogicOpCopy:
        return VK_LOGIC_OP_COPY;
    case WisLogicOpCopyInverted:
        return VK_LOGIC_OP_COPY_INVERTED;
    case WisLogicOpNoop:
        return VK_LOGIC_OP_NO_OP;
    case WisLogicOpInvert:
        return VK_LOGIC_OP_INVERT;
    case WisLogicOpAnd:
        return VK_LOGIC_OP_AND;
    case WisLogicOpNand:
        return VK_LOGIC_OP_NAND;
    case WisLogicOpOr:
        return VK_LOGIC_OP_OR;
    case WisLogicOpNor:
        return VK_LOGIC_OP_NOR;
    case WisLogicOpXor:
        return VK_LOGIC_OP_XOR;
    case WisLogicOpEquiv:
        return VK_LOGIC_OP_EQUIVALENT;
    case WisLogicOpAndReverse:
        return VK_LOGIC_OP_AND_REVERSE;
    case WisLogicOpAndInverted:
        return VK_LOGIC_OP_AND_INVERTED;
    case WisLogicOpOrReverse:
        return VK_LOGIC_OP_OR_REVERSE;
    case WisLogicOpOrInverted:
        return VK_LOGIC_OP_OR_INVERTED;
    default:
        return static_cast<VkLogicOp>(0);
    }
}

constexpr inline VkBufferUsageFlags convert_vk(WisBufferUsageFlags value) noexcept
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

constexpr inline VkImageUsageFlags convert_vk(WisTextureUsageFlags value) noexcept
{
    VkImageUsageFlags result = static_cast<VkImageUsageFlags>(0);
    if (value & WisTextureUsageFlagsRenderTarget) {
        result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    if (value & WisTextureUsageFlagsDepthStencil) {
        result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    if (value & WisTextureUsageFlagsCopySrc) {
        result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    if (value & WisTextureUsageFlagsCopyDst) {
        result |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    if (value & WisTextureUsageFlagsShaderResource) {
        result |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }
    if (value & WisTextureUsageFlagsUnorderedAccess) {
        result |= VK_IMAGE_USAGE_STORAGE_BIT;
    }
    if (value & WisTextureUsageFlagsHostCopy) {
        result |= VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT;
    }
    return result;
}

constexpr inline VmaAllocationCreateFlags convert_vk(WisMemoryFlags value) noexcept
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

constexpr inline VkPipelineStageFlags2 convert_vk(WisBarrierSync value) noexcept
{
    VkPipelineStageFlags2 result = static_cast<VkPipelineStageFlags2>(0);
    if (value & WisBarrierSyncAll) {
        result |= VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
    }
    if (value & WisBarrierSyncDraw) {
        result |= VK_PIPELINE_STAGE_2_VERTEX_ATTRIBUTE_INPUT_BIT | VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT | VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    if (value & WisBarrierSyncIndexInput) {
        result |= VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT;
    }
    if (value & WisBarrierSyncVertexShading) {
        result |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT;
    }
    if (value & WisBarrierSyncPixelShading) {
        result |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
    }
    if (value & WisBarrierSyncDepthStencil) {
        result |= VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
    }
    if (value & WisBarrierSyncRenderTarget) {
        result |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    if (value & WisBarrierSyncCompute) {
        result |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (value & WisBarrierSyncRaytracing) {
        result |= VK_PIPELINE_STAGE_2_RAY_TRACING_SHADER_BIT_KHR;
    }
    if (value & WisBarrierSyncCopy) {
        result |= VK_PIPELINE_STAGE_2_COPY_BIT;
    }
    if (value & WisBarrierSyncResolve) {
        result |= VK_PIPELINE_STAGE_2_COPY_BIT | VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_2_RESOLVE_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (value & WisBarrierSyncExecuteIndirect) {
        result |= VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (value & WisBarrierSyncAllShading) {
        result |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (value & WisBarrierSyncNonPixelShading) {
        result |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (value & WisBarrierSyncClearUAV) {
        result |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if (value & WisBarrierSyncVideoDecode) {
        result |= VK_PIPELINE_STAGE_2_VIDEO_DECODE_BIT_KHR;
    }
    if (value & WisBarrierSyncVideoEncode) {
        result |= VK_PIPELINE_STAGE_2_VIDEO_ENCODE_BIT_KHR;
    }
    if (value & WisBarrierSyncBuildRTAS) {
        result |= VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_BUILD_BIT_KHR;
    }
    if (value & WisBarrierSyncCopyRTAS) {
        result |= VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_COPY_BIT_KHR;
    }
    return result;
}

constexpr inline VkAccessFlags2 convert_vk(WisResourceAccess value) noexcept
{
    VkAccessFlags2 result = static_cast<VkAccessFlags2>(0);
    if (value & WisResourceAccessVertexBuffer) {
        result |= VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT;
    }
    if (value & WisResourceAccessConstantBuffer) {
        result |= VK_ACCESS_2_UNIFORM_READ_BIT;
    }
    if (value & WisResourceAccessIndexBuffer) {
        result |= VK_ACCESS_2_INDEX_READ_BIT;
    }
    if (value & WisResourceAccessRenderTarget) {
        result |= VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
    }
    if (value & WisResourceAccessUnorderedAccess) {
        result |= VK_ACCESS_2_SHADER_WRITE_BIT | VK_ACCESS_2_SHADER_READ_BIT;
    }
    if (value & WisResourceAccessDepthWrite) {
        result |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }
    if (value & WisResourceAccessDepthRead) {
        result |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    }
    if (value & WisResourceAccessShaderResource) {
        result |= VK_ACCESS_2_SHADER_READ_BIT;
    }
    if (value & WisResourceAccessStreamOutput) {
        result |= VK_ACCESS_2_TRANSFORM_FEEDBACK_WRITE_BIT_EXT;
    }
    if (value & WisResourceAccessIndirectArgument) {
        result |= VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT | VK_ACCESS_2_SHADER_READ_BIT;
    }
    if (value & WisResourceAccessCopyDst) {
        result |= VK_ACCESS_2_TRANSFER_WRITE_BIT;
    }
    if (value & WisResourceAccessCopySrc) {
        result |= VK_ACCESS_2_TRANSFER_READ_BIT;
    }
    if (value & WisResourceAccessConditionalRendering) {
        result |= VK_ACCESS_2_CONDITIONAL_RENDERING_READ_BIT_EXT;
    }
    if (value & WisResourceAccessAccelerationStructureRead) {
        result |= VK_ACCESS_2_ACCELERATION_STRUCTURE_READ_BIT_KHR;
    }
    if (value & WisResourceAccessAccelerationStructureWrite) {
        result |= VK_ACCESS_2_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
    }
    if (value & WisResourceAccessShadingRate) {
        result |= VK_ACCESS_2_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR;
    }
    if (value & WisResourceAccessVideoDecodeRead) {
        result |= VK_ACCESS_2_VIDEO_DECODE_READ_BIT_KHR;
    }
    if (value & WisResourceAccessVideoDecodeWrite) {
        result |= VK_ACCESS_2_VIDEO_DECODE_WRITE_BIT_KHR;
    }
    if (value & WisResourceAccessResolveDst) {
        result |= VK_ACCESS_2_TRANSFER_WRITE_BIT | VK_ACCESS_2_SHADER_WRITE_BIT;
    }
    if (value & WisResourceAccessResolveSrc) {
        result |= VK_ACCESS_2_TRANSFER_READ_BIT | VK_ACCESS_2_SHADER_READ_BIT;
    }
    if (value & WisResourceAccessNone) {
        result |= VK_ACCESS_2_NONE;
    }
    return result;
}

} // namespace detail
} // namespace wis
#endif // WISDOM_VK_CONVERT_HPP
