// This file is generated. Do not edit directly.
#ifndef WISDOM_CORE_CPP_DX12_CONVERT_HPP
#define WISDOM_CORE_CPP_DX12_CONVERT_HPP
#ifndef __cplusplus
#    error "This is a C++ only header"
#endif // __cplusplus

#include <D3D12MemAlloc.h>
#include <dxgi1_6.h>
#include "c_api.h"

namespace wis {
namespace detail {

constexpr inline DXGI_FORMAT DX12Convert(WisDataFormat value) noexcept
{
    switch (value) {
    case WisDataFormatRGBA32Float:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case WisDataFormatRGBA32Uint:
        return DXGI_FORMAT_R32G32B32A32_UINT;
    case WisDataFormatRGBA32Sint:
        return DXGI_FORMAT_R32G32B32A32_SINT;
    case WisDataFormatRGB32Float:
        return DXGI_FORMAT_R32G32B32_FLOAT;
    case WisDataFormatRGB32Uint:
        return DXGI_FORMAT_R32G32B32_UINT;
    case WisDataFormatRGB32Sint:
        return DXGI_FORMAT_R32G32B32_SINT;
    case WisDataFormatRGBA16Float:
        return DXGI_FORMAT_R16G16B16A16_FLOAT;
    case WisDataFormatRGBA16Unorm:
        return DXGI_FORMAT_R16G16B16A16_UNORM;
    case WisDataFormatRGBA16Uint:
        return DXGI_FORMAT_R16G16B16A16_UINT;
    case WisDataFormatRGBA16Snorm:
        return DXGI_FORMAT_R16G16B16A16_SNORM;
    case WisDataFormatRGBA16Sint:
        return DXGI_FORMAT_R16G16B16A16_SINT;
    case WisDataFormatRG32Float:
        return DXGI_FORMAT_R32G32_FLOAT;
    case WisDataFormatRG32Uint:
        return DXGI_FORMAT_R32G32_UINT;
    case WisDataFormatRG32Sint:
        return DXGI_FORMAT_R32G32_SINT;
    case WisDataFormatD32FloatS8Uint:
        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    case WisDataFormatRGB10A2Unorm:
        return DXGI_FORMAT_R10G10B10A2_UNORM;
    case WisDataFormatRGB10A2Uint:
        return DXGI_FORMAT_R10G10B10A2_UINT;
    case WisDataFormatRG11B10Float:
        return DXGI_FORMAT_R11G11B10_FLOAT;
    case WisDataFormatRGBA8Unorm:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    case WisDataFormatRGBA8UnormSrgb:
        return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    case WisDataFormatRGBA8Uint:
        return DXGI_FORMAT_R8G8B8A8_UINT;
    case WisDataFormatRGBA8Snorm:
        return DXGI_FORMAT_R8G8B8A8_SNORM;
    case WisDataFormatRGBA8Sint:
        return DXGI_FORMAT_R8G8B8A8_SINT;
    case WisDataFormatRG16Float:
        return DXGI_FORMAT_R16G16_FLOAT;
    case WisDataFormatRG16Unorm:
        return DXGI_FORMAT_R16G16_UNORM;
    case WisDataFormatRG16Uint:
        return DXGI_FORMAT_R16G16_UINT;
    case WisDataFormatRG16Snorm:
        return DXGI_FORMAT_R16G16_SNORM;
    case WisDataFormatRG16Sint:
        return DXGI_FORMAT_R16G16_SINT;
    case WisDataFormatD32Float:
        return DXGI_FORMAT_D32_FLOAT;
    case WisDataFormatR32Float:
        return DXGI_FORMAT_R32_FLOAT;
    case WisDataFormatR32Uint:
        return DXGI_FORMAT_R32_UINT;
    case WisDataFormatR32Sint:
        return DXGI_FORMAT_R32_SINT;
    case WisDataFormatD24UnormS8Uint:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case WisDataFormatRG8Unorm:
        return DXGI_FORMAT_R8G8_UNORM;
    case WisDataFormatRG8Uint:
        return DXGI_FORMAT_R8G8_UINT;
    case WisDataFormatRG8Snorm:
        return DXGI_FORMAT_R8G8_SNORM;
    case WisDataFormatRG8Sint:
        return DXGI_FORMAT_R8G8_SINT;
    case WisDataFormatR16Float:
        return DXGI_FORMAT_R16_FLOAT;
    case WisDataFormatR16Unorm:
        return DXGI_FORMAT_R16_UNORM;
    case WisDataFormatR16Uint:
        return DXGI_FORMAT_R16_UINT;
    case WisDataFormatR16Snorm:
        return DXGI_FORMAT_R16_SNORM;
    case WisDataFormatR16Sint:
        return DXGI_FORMAT_R16_SINT;
    case WisDataFormatR8Unorm:
        return DXGI_FORMAT_R8_UNORM;
    case WisDataFormatR8Uint:
        return DXGI_FORMAT_R8_UINT;
    case WisDataFormatR8Snorm:
        return DXGI_FORMAT_R8_SNORM;
    case WisDataFormatR8Sint:
        return DXGI_FORMAT_R8_SINT;
    case WisDataFormatRGB9E5UFloat:
        return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
    case WisDataFormatBC1RGBAUnorm:
        return DXGI_FORMAT_BC1_UNORM;
    case WisDataFormatBC1RGBAUnormSrgb:
        return DXGI_FORMAT_BC1_UNORM_SRGB;
    case WisDataFormatBC2RGBAUnorm:
        return DXGI_FORMAT_BC2_UNORM;
    case WisDataFormatBC2RGBAUnormSrgb:
        return DXGI_FORMAT_BC2_UNORM_SRGB;
    case WisDataFormatBC3RGBAUnorm:
        return DXGI_FORMAT_BC3_UNORM;
    case WisDataFormatBC3RGBAUnormSrgb:
        return DXGI_FORMAT_BC3_UNORM_SRGB;
    case WisDataFormatBC4RUnorm:
        return DXGI_FORMAT_BC4_UNORM;
    case WisDataFormatBC4RSnorm:
        return DXGI_FORMAT_BC4_SNORM;
    case WisDataFormatBC5RGUnorm:
        return DXGI_FORMAT_BC5_UNORM;
    case WisDataFormatBC5RGSnorm:
        return DXGI_FORMAT_BC5_SNORM;
    case WisDataFormatB5G6R5Unorm:
        return DXGI_FORMAT_B5G6R5_UNORM;
    case WisDataFormatB5G5R5A1Unorm:
        return DXGI_FORMAT_B5G5R5A1_UNORM;
    case WisDataFormatBGRA8Unorm:
        return DXGI_FORMAT_B8G8R8A8_UNORM;
    case WisDataFormatBGRA8UnormSrgb:
        return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    case WisDataFormatBC6HUfloat16:
        return DXGI_FORMAT_BC6H_TYPELESS;
    case WisDataFormatBC6HSfloat16:
        return DXGI_FORMAT_BC6H_TYPELESS;
    case WisDataFormatBC7RGBAUnorm:
        return DXGI_FORMAT_BC7_TYPELESS;
    case WisDataFormatBC7RGBAUnormSrgb:
        return DXGI_FORMAT_BC7_TYPELESS;
    case WisDataFormatBGRA4Unorm:
        return DXGI_FORMAT_B4G4R4A4_UNORM;
    case WisDataFormatNV12:
        return DXGI_FORMAT_NV12;
    case WisDataFormatP010:
        return DXGI_FORMAT_P010;
    case WisDataFormatP012:
        return DXGI_FORMAT_P016;
    case WisDataFormatP016:
        return DXGI_FORMAT_P016;
    default:
        return static_cast<DXGI_FORMAT>(0);
    }
}

constexpr inline uint32_t DX12Convert(WisSampleCount value) noexcept { return static_cast<uint32_t>(value); }

constexpr inline DXGI_GPU_PREFERENCE DX12Convert(WisAdapterPreference value) noexcept
{
    return static_cast<DXGI_GPU_PREFERENCE>(value);
}

constexpr inline D3D12_COMMAND_LIST_TYPE DX12Convert(WisCommandQueueType value) noexcept
{
    switch (value) {
    case WisCommandQueueTypeGraphics:
        return D3D12_COMMAND_LIST_TYPE_DIRECT;
    case WisCommandQueueTypeCompute:
        return D3D12_COMMAND_LIST_TYPE_COMPUTE;
    case WisCommandQueueTypeTransfer:
        return D3D12_COMMAND_LIST_TYPE_COPY;
    case WisCommandQueueTypeVideoDecode:
        return D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE;
    case WisCommandQueueTypeVideoEncode:
        return D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE;
    default:
        return D3D12_COMMAND_LIST_TYPE_DIRECT;
    }
}

constexpr inline D3D12_COMMAND_QUEUE_PRIORITY DX12Convert(WisCommandQueuePriority value) noexcept
{
    switch (value) {
    case WisCommandQueuePriorityNormal:
        return D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    case WisCommandQueuePriorityHigh:
        return D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
    case WisCommandQueuePriorityRealtime:
        return D3D12_COMMAND_QUEUE_PRIORITY_GLOBAL_REALTIME;
    default:
        return D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    }
}

constexpr inline D3D12_SHADER_VISIBILITY DX12Convert(WisShaderVisibility value) noexcept
{
    return static_cast<D3D12_SHADER_VISIBILITY>(value);
}

constexpr inline D3D12_DESCRIPTOR_RANGE_TYPE DX12Convert(WisDescriptorType value) noexcept
{
    switch (value) {
    case WisDescriptorTypeSampler:
        return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
    case WisDescriptorTypeConstantBuffer:
        return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    case WisDescriptorTypeTexture:
        return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    case WisDescriptorTypeRWTexture:
        return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
    case WisDescriptorTypeRWBuffer:
        return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
    case WisDescriptorTypeBuffer:
        return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    case WisDescriptorTypeAccelerationStructure:
        return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    default:
        return static_cast<D3D12_DESCRIPTOR_RANGE_TYPE>(0);
    }
}

constexpr inline D3D12_COMPARISON_FUNC DX12Convert(WisCompareOp value) noexcept
{
    return static_cast<D3D12_COMPARISON_FUNC>(value);
}

constexpr inline D3D12_TEXTURE_ADDRESS_MODE DX12Convert(WisAddressMode value) noexcept
{
    switch (value) {
    case WisAddressModeRepeat:
        return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    case WisAddressModeMirroredRepeat:
        return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
    case WisAddressModeClampToEdge:
        return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    case WisAddressModeClampToBorder:
        return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    case WisAddressModeMirrorClampToEdge:
        return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
    default:
        return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    }
}

constexpr inline D3D12_FILTER_TYPE DX12Convert(WisFilter value) noexcept
{
    switch (value) {
    case WisFilterPoint:
        return D3D12_FILTER_TYPE_POINT;
    case WisFilterLinear:
        return D3D12_FILTER_TYPE_LINEAR;
    default:
        return D3D12_FILTER_TYPE_POINT;
    }
}

constexpr inline D3D12_FILTER_REDUCTION_TYPE DX12Convert(WisReductionMode value) noexcept
{
    switch (value) {
    case WisReductionModeStandard:
        return D3D12_FILTER_REDUCTION_TYPE_STANDARD;
    case WisReductionModeMin:
        return D3D12_FILTER_REDUCTION_TYPE_MINIMUM;
    case WisReductionModeMax:
        return D3D12_FILTER_REDUCTION_TYPE_MAXIMUM;
    default:
        return D3D12_FILTER_REDUCTION_TYPE_STANDARD;
    }
}

constexpr inline D3D12_STATIC_BORDER_COLOR DX12Convert(WisStaticBorder value) noexcept
{
    switch (value) {
    case WisStaticBorderTransparentBlack:
        return D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    case WisStaticBorderOpaqueBlack:
        return D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
    case WisStaticBorderOpaqueWhite:
        return D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
    default:
        return D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    }
}

constexpr inline D3D12_DESCRIPTOR_HEAP_TYPE DX12Convert(WisDescriptorHeapType value) noexcept
{
    return static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(value);
}

constexpr inline D3D12_DESCRIPTOR_HEAP_FLAGS DX12Convert(WisDescriptorMemoryType value) noexcept
{
    return static_cast<D3D12_DESCRIPTOR_HEAP_FLAGS>(value);
}

constexpr inline D3D12_SHADER_COMPONENT_MAPPING DX12Convert(WisComponentSwizzle value) noexcept
{
    switch (value) {
    case WisComponentSwizzleRed:
        return D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_0;
    case WisComponentSwizzleGreen:
        return D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_1;
    case WisComponentSwizzleBlue:
        return D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_2;
    case WisComponentSwizzleAlpha:
        return D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_3;
    case WisComponentSwizzleZero:
        return D3D12_SHADER_COMPONENT_MAPPING_FORCE_VALUE_0;
    case WisComponentSwizzleOne:
        return D3D12_SHADER_COMPONENT_MAPPING_FORCE_VALUE_1;
    default:
        return static_cast<D3D12_SHADER_COMPONENT_MAPPING>(0);
    }
}

constexpr inline D3D12_HEAP_TYPE DX12Convert(WisMemoryType value) noexcept
{
    switch (value) {
    case WisMemoryTypeDeviceLocal:
        return D3D12_HEAP_TYPE_DEFAULT;
    case WisMemoryTypeUpload:
        return D3D12_HEAP_TYPE_UPLOAD;
    case WisMemoryTypeReadback:
        return D3D12_HEAP_TYPE_READBACK;
    case WisMemoryTypeGPUUpload:
        return D3D12_HEAP_TYPE_GPU_UPLOAD;
    default:
        return static_cast<D3D12_HEAP_TYPE>(0);
    }
}

constexpr inline D3D12_BARRIER_LAYOUT DX12Convert(WisTextureState value) noexcept
{
    switch (value) {
    case WisTextureStateUndefined:
        return D3D12_BARRIER_LAYOUT_UNDEFINED;
    case WisTextureStateCommon:
        return D3D12_BARRIER_LAYOUT_COMMON;
    case WisTextureStateRead:
        return D3D12_BARRIER_LAYOUT_GENERIC_READ;
    case WisTextureStateRenderTarget:
        return D3D12_BARRIER_LAYOUT_RENDER_TARGET;
    case WisTextureStateUnorderedAccess:
        return D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS;
    case WisTextureStateDepthStencilWrite:
        return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
    case WisTextureStateDepthStencilRead:
        return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_READ;
    case WisTextureStateDepthWriteStencilRead:
        return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
    case WisTextureStateStencilWriteDepthRead:
        return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
    case WisTextureStateShaderResource:
        return D3D12_BARRIER_LAYOUT_SHADER_RESOURCE;
    case WisTextureStateCopySrc:
        return D3D12_BARRIER_LAYOUT_COPY_SOURCE;
    case WisTextureStateCopyDst:
        return D3D12_BARRIER_LAYOUT_COPY_DEST;
    case WisTextureStatePresent:
        return D3D12_BARRIER_LAYOUT_PRESENT;
    case WisTextureStateShadingRate:
        return D3D12_BARRIER_LAYOUT_SHADING_RATE_SOURCE;
    case WisTextureStateVideoDecodeRead:
        return D3D12_BARRIER_LAYOUT_VIDEO_DECODE_READ;
    case WisTextureStateVideoDecodeWrite:
        return D3D12_BARRIER_LAYOUT_VIDEO_DECODE_WRITE;
    case WisTextureStateResolveDepthStensilDst:
        return D3D12_BARRIER_LAYOUT_RESOLVE_DEST;
    case WisTextureStateResolveRenderTargetDst:
        return D3D12_BARRIER_LAYOUT_RESOLVE_DEST;
    default:
        return static_cast<D3D12_BARRIER_LAYOUT>(0);
    }
}

constexpr inline D3D12_STENCIL_OP DX12Convert(WisStencilOp value) noexcept
{
    return static_cast<D3D12_STENCIL_OP>(value);
}

constexpr inline D3D12_PRIMITIVE_TOPOLOGY_TYPE DX12Convert(WisTopologyType value) noexcept
{
    switch (value) {
    case WisTopologyTypePoint:
        return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
    case WisTopologyTypeLine:
        return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
    case WisTopologyTypeTriangle:
        return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    case WisTopologyTypePatch:
        return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
    default:
        return static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(0);
    }
}

constexpr inline D3D12_FILL_MODE DX12Convert(WisFillMode value) noexcept { return static_cast<D3D12_FILL_MODE>(value); }

constexpr inline D3D12_CULL_MODE DX12Convert(WisCullMode value) noexcept { return static_cast<D3D12_CULL_MODE>(value); }

constexpr inline BOOL DX12Convert(WisWindingOrder value) noexcept { return static_cast<BOOL>(value); }

constexpr inline D3D12_CONSERVATIVE_RASTERIZATION_MODE DX12Convert(WisConservativeRasterization value) noexcept
{
    return static_cast<D3D12_CONSERVATIVE_RASTERIZATION_MODE>(value);
}

constexpr inline D3D12_LINE_RASTERIZATION_MODE DX12Convert(WisLineRasterization value) noexcept
{
    return static_cast<D3D12_LINE_RASTERIZATION_MODE>(value);
}

constexpr inline D3D12_BLEND DX12Convert(WisBlendFactor value) noexcept { return static_cast<D3D12_BLEND>(value); }

constexpr inline D3D12_BLEND_OP DX12Convert(WisBlendOp value) noexcept { return static_cast<D3D12_BLEND_OP>(value); }

constexpr inline D3D12_LOGIC_OP DX12Convert(WisLogicOp value) noexcept { return static_cast<D3D12_LOGIC_OP>(value); }

constexpr inline D3D_PRIMITIVE_TOPOLOGY DX12Convert(WisPrimitiveTopology value) noexcept
{
    switch (value) {
    case WisPrimitiveTopologyPointList:
        return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
    case WisPrimitiveTopologyLineList:
        return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
    case WisPrimitiveTopologyLineStrip:
        return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
    case WisPrimitiveTopologyTriangleList:
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    case WisPrimitiveTopologyTriangleStrip:
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    case WisPrimitiveTopologyTriangleFan:
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLEFAN;
    case WisPrimitiveTopologyLineListAdj:
        return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
    case WisPrimitiveTopologyLineStripAdj:
        return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
    case WisPrimitiveTopologyTriangleListAdj:
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
    case WisPrimitiveTopologyTriangleStripAdj:
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
    default:
        return static_cast<D3D_PRIMITIVE_TOPOLOGY>(0);
    }
}

constexpr inline D3D12_INDEX_BUFFER_STRIP_CUT_VALUE DX12Convert(WisPrimitiveRestartValue value) noexcept
{
    switch (value) {
    case WisPrimitiveRestartValueNone:
        return D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
    case WisPrimitiveRestartValueUInt16Max:
        return D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF;
    case WisPrimitiveRestartValueUInt32Max:
        return D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF;
    default:
        return static_cast<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>(0);
    }
}

constexpr inline D3D12_DESCRIPTOR_HEAP_TYPE DX12Convert(WisViewHeapType value) noexcept
{
    switch (value) {
    case WisViewHeapTypeRenderTarget:
        return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    case WisViewHeapTypeDepthStencil:
        return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    default:
        return static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(0);
    }
}

constexpr inline DXGI_SCALING DX12Convert(WisSwapchainScaling value) noexcept
{
    switch (value) {
    case WisSwapchainScalingNone:
        return DXGI_SCALING_NONE;
    case WisSwapchainScalingStretch:
        return DXGI_SCALING_STRETCH;
    case WisSwapchainScalingAspect:
        return DXGI_SCALING_ASPECT_RATIO_STRETCH;
    default:
        return static_cast<DXGI_SCALING>(0);
    }
}

constexpr inline DXGI_ALPHA_MODE DX12Convert(WisCompositeAlpha value) noexcept
{
    switch (value) {
    case WisCompositeAlphaOpaque:
        return DXGI_ALPHA_MODE_IGNORE;
    case WisCompositeAlphaPreMultiplied:
        return DXGI_ALPHA_MODE_PREMULTIPLIED;
    case WisCompositeAlphaPostMultiplied:
        return DXGI_ALPHA_MODE_STRAIGHT;
    case WisCompositeAlphaInherit:
        return DXGI_ALPHA_MODE_UNSPECIFIED;
    default:
        return static_cast<DXGI_ALPHA_MODE>(0);
    }
}

constexpr inline D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE DX12Convert(WisLoadOp value) noexcept
{
    switch (value) {
    case WisLoadOpLoad:
        return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
    case WisLoadOpClear:
        return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
    case WisLoadOpDontCare:
        return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
    default:
        return static_cast<D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE>(0);
    }
}

constexpr inline D3D12_RENDER_PASS_ENDING_ACCESS_TYPE DX12Convert(WisStoreOp value) noexcept
{
    switch (value) {
    case WisStoreOpStore:
        return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
    case WisStoreOpDontCare:
        return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
    default:
        return static_cast<D3D12_RENDER_PASS_ENDING_ACCESS_TYPE>(0);
    }
}

constexpr inline D3D12_RESOLVE_MODE DX12Convert(WisResolveMode value) noexcept
{
    switch (value) {
    case WisResolveModeAverage:
        return D3D12_RESOLVE_MODE_AVERAGE;
    case WisResolveModeMin:
        return D3D12_RESOLVE_MODE_MIN;
    case WisResolveModeMax:
        return D3D12_RESOLVE_MODE_MAX;
    default:
        return static_cast<D3D12_RESOLVE_MODE>(0);
    }
}

constexpr inline DXGI_FORMAT DX12Convert(WisIndexType value) noexcept
{
    switch (value) {
    case WisIndexTypeUInt16:
        return DXGI_FORMAT_R16_UINT;
    case WisIndexTypeUInt32:
        return DXGI_FORMAT_R32_UINT;
    default:
        return static_cast<DXGI_FORMAT>(0);
    }
}

constexpr inline D3D12_SAMPLER_FLAGS DX12Convert(WisSamplerFlags value) noexcept
{
    D3D12_SAMPLER_FLAGS result = static_cast<D3D12_SAMPLER_FLAGS>(0);
    if (value & WisSamplerFlagsNonNormalizedCoordinates) {
        result |= D3D12_SAMPLER_FLAG_NON_NORMALIZED_COORDINATES;
    }
    return result;
}

constexpr inline D3D12_RESOURCE_FLAGS DX12Convert(WisBufferUsageFlags value) noexcept
{
    D3D12_RESOURCE_FLAGS result = static_cast<D3D12_RESOURCE_FLAGS>(0);
    if (value & WisBufferUsageFlagsStorageBuffer) {
        result |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    }
    if (value & WisBufferUsageFlagsAccelerationStructureBuffer) {
        result |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS | D3D12_RESOURCE_FLAG_RAYTRACING_ACCELERATION_STRUCTURE;
    }
    return result;
}

constexpr inline D3D12_RESOURCE_FLAGS DX12Convert(WisTextureUsageFlags value) noexcept
{
    D3D12_RESOURCE_FLAGS result = static_cast<D3D12_RESOURCE_FLAGS>(0);
    if (value & WisTextureUsageFlagsRenderTarget) {
        result |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    }
    if (value & WisTextureUsageFlagsDepthStencil) {
        result |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    }
    if (value & WisTextureUsageFlagsCopySrc) {
        result |= D3D12_RESOURCE_FLAG_NONE;
    }
    if (value & WisTextureUsageFlagsCopyDst) {
        result |= D3D12_RESOURCE_FLAG_NONE;
    }
    if (value & WisTextureUsageFlagsShaderResource) {
        result |= D3D12_RESOURCE_FLAG_NONE;
    }
    if (value & WisTextureUsageFlagsUnorderedAccess) {
        result |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    }
    if (value & WisTextureUsageFlagsHostCopy) {
        result |= D3D12_RESOURCE_FLAG_NONE;
    }
    return result;
}

constexpr inline WisTextureUsageFlags DX12Convert(D3D12_RESOURCE_FLAGS value) noexcept
{
    WisTextureUsageFlags result = static_cast<WisTextureUsageFlags>(0);
    if (value & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) {
        result = static_cast<WisTextureUsageFlags>(result | WisTextureUsageFlagsRenderTarget);
    }
    if (value & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) {
        result = static_cast<WisTextureUsageFlags>(result | WisTextureUsageFlagsDepthStencil);
    }
    if (value & D3D12_RESOURCE_FLAG_NONE) {
        result = static_cast<WisTextureUsageFlags>(result | WisTextureUsageFlagsCopySrc);
    }
    if (value & D3D12_RESOURCE_FLAG_NONE) {
        result = static_cast<WisTextureUsageFlags>(result | WisTextureUsageFlagsCopyDst);
    }
    if (value & D3D12_RESOURCE_FLAG_NONE) {
        result = static_cast<WisTextureUsageFlags>(result | WisTextureUsageFlagsShaderResource);
    }
    if (value & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS) {
        result = static_cast<WisTextureUsageFlags>(result | WisTextureUsageFlagsUnorderedAccess);
    }
    if (value & D3D12_RESOURCE_FLAG_NONE) {
        result = static_cast<WisTextureUsageFlags>(result | WisTextureUsageFlagsHostCopy);
    }
    return result;
}

constexpr inline D3D12MA::ALLOCATION_FLAGS DX12Convert(WisMemoryFlags value) noexcept
{
    D3D12MA::ALLOCATION_FLAGS result = static_cast<D3D12MA::ALLOCATION_FLAGS>(0);
    if (value & WisMemoryFlagsDedicatedAllocation) {
        result = static_cast<D3D12MA::ALLOCATION_FLAGS>(result | D3D12MA::ALLOCATION_FLAG_COMMITTED);
    }
    if (value & WisMemoryFlagsMapped) {
        result = static_cast<D3D12MA::ALLOCATION_FLAGS>(result | D3D12MA::ALLOCATION_FLAG_NONE);
    }
    return result;
}

constexpr inline D3D12_BARRIER_SYNC DX12Convert(WisBarrierSync value) noexcept
{
    D3D12_BARRIER_SYNC result = static_cast<D3D12_BARRIER_SYNC>(0);
    if (value & WisBarrierSyncAll) {
        result |= D3D12_BARRIER_SYNC_ALL;
    }
    if (value & WisBarrierSyncDraw) {
        result |= D3D12_BARRIER_SYNC_DRAW;
    }
    if (value & WisBarrierSyncIndexInput) {
        result |= D3D12_BARRIER_SYNC_INDEX_INPUT;
    }
    if (value & WisBarrierSyncVertexShading) {
        result |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
    }
    if (value & WisBarrierSyncPixelShading) {
        result |= D3D12_BARRIER_SYNC_PIXEL_SHADING;
    }
    if (value & WisBarrierSyncDepthStencil) {
        result |= D3D12_BARRIER_SYNC_DEPTH_STENCIL;
    }
    if (value & WisBarrierSyncRenderTarget) {
        result |= D3D12_BARRIER_SYNC_RENDER_TARGET;
    }
    if (value & WisBarrierSyncCompute) {
        result |= D3D12_BARRIER_SYNC_COMPUTE_SHADING;
    }
    if (value & WisBarrierSyncRaytracing) {
        result |= D3D12_BARRIER_SYNC_RAYTRACING;
    }
    if (value & WisBarrierSyncCopy) {
        result |= D3D12_BARRIER_SYNC_COPY;
    }
    if (value & WisBarrierSyncResolve) {
        result |= D3D12_BARRIER_SYNC_RESOLVE;
    }
    if (value & WisBarrierSyncExecuteIndirect) {
        result |= D3D12_BARRIER_SYNC_EXECUTE_INDIRECT;
    }
    if (value & WisBarrierSyncAllShading) {
        result |= D3D12_BARRIER_SYNC_ALL_SHADING;
    }
    if (value & WisBarrierSyncNonPixelShading) {
        result |= D3D12_BARRIER_SYNC_NON_PIXEL_SHADING;
    }
    if (value & WisBarrierSyncClearUAV) {
        result |= D3D12_BARRIER_SYNC_CLEAR_UNORDERED_ACCESS_VIEW;
    }
    if (value & WisBarrierSyncVideoDecode) {
        result |= D3D12_BARRIER_SYNC_VIDEO_DECODE;
    }
    if (value & WisBarrierSyncVideoEncode) {
        result |= D3D12_BARRIER_SYNC_VIDEO_ENCODE;
    }
    if (value & WisBarrierSyncBuildRTAS) {
        result |= D3D12_BARRIER_SYNC_BUILD_RAYTRACING_ACCELERATION_STRUCTURE;
    }
    if (value & WisBarrierSyncCopyRTAS) {
        result |= D3D12_BARRIER_SYNC_COPY_RAYTRACING_ACCELERATION_STRUCTURE;
    }
    return result;
}

constexpr inline D3D12_BARRIER_ACCESS DX12Convert(WisResourceAccess value) noexcept
{
    D3D12_BARRIER_ACCESS result = static_cast<D3D12_BARRIER_ACCESS>(0);
    if (value & WisResourceAccessVertexBuffer) {
        result |= D3D12_BARRIER_ACCESS_VERTEX_BUFFER;
    }
    if (value & WisResourceAccessConstantBuffer) {
        result |= D3D12_BARRIER_ACCESS_CONSTANT_BUFFER;
    }
    if (value & WisResourceAccessIndexBuffer) {
        result |= D3D12_BARRIER_ACCESS_INDEX_BUFFER;
    }
    if (value & WisResourceAccessRenderTarget) {
        result |= D3D12_BARRIER_ACCESS_RENDER_TARGET;
    }
    if (value & WisResourceAccessUnorderedAccess) {
        result |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
    }
    if (value & WisResourceAccessDepthWrite) {
        result |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_WRITE;
    }
    if (value & WisResourceAccessDepthRead) {
        result |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_READ;
    }
    if (value & WisResourceAccessShaderResource) {
        result |= D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
    }
    if (value & WisResourceAccessStreamOutput) {
        result |= D3D12_BARRIER_ACCESS_STREAM_OUTPUT;
    }
    if (value & WisResourceAccessIndirectArgument) {
        result |= D3D12_BARRIER_ACCESS_INDIRECT_ARGUMENT;
    }
    if (value & WisResourceAccessCopyDst) {
        result |= D3D12_BARRIER_ACCESS_COPY_DEST;
    }
    if (value & WisResourceAccessCopySrc) {
        result |= D3D12_BARRIER_ACCESS_COPY_SOURCE;
    }
    if (value & WisResourceAccessConditionalRendering) {
        result |= D3D12_BARRIER_ACCESS_PREDICATION;
    }
    if (value & WisResourceAccessAccelerationStructureRead) {
        result |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_READ;
    }
    if (value & WisResourceAccessAccelerationStructureWrite) {
        result |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE;
    }
    if (value & WisResourceAccessShadingRate) {
        result |= D3D12_BARRIER_ACCESS_SHADING_RATE_SOURCE;
    }
    if (value & WisResourceAccessVideoDecodeRead) {
        result |= D3D12_BARRIER_ACCESS_VIDEO_DECODE_READ;
    }
    if (value & WisResourceAccessVideoDecodeWrite) {
        result |= D3D12_BARRIER_ACCESS_VIDEO_DECODE_WRITE;
    }
    if (value & WisResourceAccessResolveDst) {
        result |= D3D12_BARRIER_ACCESS_RESOLVE_DEST;
    }
    if (value & WisResourceAccessResolveSrc) {
        result |= D3D12_BARRIER_ACCESS_RESOLVE_SOURCE;
    }
    if (value & WisResourceAccessNone) {
        result |= D3D12_BARRIER_ACCESS_NO_ACCESS;
    }
    return result;
}

constexpr inline uint32_t DX12Convert(WisPresentFlags value) noexcept
{
    uint32_t result = static_cast<uint32_t>(0);
    if (value & WisPresentFlagsTimeoutOnBlock) {
        result |= DXGI_PRESENT_DO_NOT_WAIT;
    }
    return result;
}

constexpr inline D3D12_RENDER_PASS_FLAGS DX12Convert(WisRenderPassFlags value) noexcept
{
    D3D12_RENDER_PASS_FLAGS result = static_cast<D3D12_RENDER_PASS_FLAGS>(0);
    if (value & WisRenderPassFlagsSuspending) {
        result |= D3D12_RENDER_PASS_FLAG_SUSPENDING_PASS;
    }
    if (value & WisRenderPassFlagsResuming) {
        result |= D3D12_RENDER_PASS_FLAG_RESUMING_PASS;
    }
    return result;
}

constexpr inline D3D12_DESCRIPTOR_HEAP_FLAGS DX12Convert(WisViewHeapFlags value) noexcept
{
    D3D12_DESCRIPTOR_HEAP_FLAGS result = static_cast<D3D12_DESCRIPTOR_HEAP_FLAGS>(0);
    return result;
}

} // namespace detail
} // namespace wis
#endif // WISDOM_CORE_CPP_DX12_CONVERT_HPP
