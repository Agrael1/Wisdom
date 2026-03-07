// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_DX12_CONVERT_HPP
#define WISDOM_CPP_DX12_CONVERT_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include <wisdom/generated/c_api.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <D3D12MemAlloc.h>

namespace wis {
namespace detail {

inline DXGI_FORMAT convert_dx(WisDataFormat value) noexcept
{
    return static_cast<DXGI_FORMAT>(value);
}

inline uint32_t convert_dx(WisSampleCount value) noexcept
{
    return static_cast<uint32_t>(value);
}

inline DXGI_GPU_PREFERENCE convert_dx(WisAdapterPreference value) noexcept
{
    return static_cast<DXGI_GPU_PREFERENCE>(value);
}

inline D3D12_COMMAND_LIST_TYPE convert_dx(WisCommandQueueType value) noexcept
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

inline D3D12_COMMAND_QUEUE_PRIORITY convert_dx(WisCommandQueuePriority value) noexcept
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

inline D3D12_SHADER_VISIBILITY convert_dx(WisShaderVisibility value) noexcept
{
    return static_cast<D3D12_SHADER_VISIBILITY>(value);
}

inline D3D12_DESCRIPTOR_RANGE_TYPE convert_dx(WisDescriptorType value) noexcept
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

inline D3D12_COMPARISON_FUNC convert_dx(WisCompareOperation value) noexcept
{
    return static_cast<D3D12_COMPARISON_FUNC>(value);
}

inline D3D12_TEXTURE_ADDRESS_MODE convert_dx(WisAddressMode value) noexcept
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

inline D3D12_FILTER_TYPE convert_dx(WisFilter value) noexcept
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

inline D3D12_FILTER_REDUCTION_TYPE convert_dx(WisReductionMode value) noexcept
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

inline D3D12_STATIC_BORDER_COLOR convert_dx(WisStaticBorder value) noexcept
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

inline D3D12_DESCRIPTOR_HEAP_TYPE convert_dx(WisDescriptorHeapType value) noexcept
{
    return static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(value);
}

inline D3D12_DESCRIPTOR_HEAP_FLAGS convert_dx(WisDescriptorMemoryType value) noexcept
{
    return static_cast<D3D12_DESCRIPTOR_HEAP_FLAGS>(value);
}

inline D3D12_SHADER_COMPONENT_MAPPING convert_dx(WisComponentSwizzle value) noexcept
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

inline D3D12_HEAP_TYPE convert_dx(WisMemoryType value) noexcept
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

inline D3D12_SAMPLER_FLAGS convert_dx(WisSamplerFlags value) noexcept
{
    D3D12_SAMPLER_FLAGS result = static_cast<D3D12_SAMPLER_FLAGS>(0);
    if (value & WisSamplerFlagsNonNormalizedCoordinates) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_SAMPLER_FLAG_NON_NORMALIZED_COORDINATES));
    }
    return result;
}

inline D3D12_RESOURCE_FLAGS convert_dx(WisBufferUsageFlags value) noexcept
{
    D3D12_RESOURCE_FLAGS result = static_cast<D3D12_RESOURCE_FLAGS>(0);
    if (value & WisBufferUsageFlagsStorageBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS));
    }
    if (value & WisBufferUsageFlagsAccelerationStructureBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS | D3D12_RESOURCE_FLAG_RAYTRACING_ACCELERATION_STRUCTURE));
    }
    return result;
}

inline D3D12_RESOURCE_FLAGS convert_dx(WisTextureUsageFlags value) noexcept
{
    D3D12_RESOURCE_FLAGS result = static_cast<D3D12_RESOURCE_FLAGS>(0);
    if (value & WisTextureUsageFlagsRenderTarget) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET));
    }
    if (value & WisTextureUsageFlagsDepthStencil) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL));
    }
    if (value & WisTextureUsageFlagsCopySrc) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_RESOURCE_FLAG_NONE));
    }
    if (value & WisTextureUsageFlagsCopyDst) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_RESOURCE_FLAG_NONE));
    }
    if (value & WisTextureUsageFlagsShaderResource) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_RESOURCE_FLAG_NONE));
    }
    if (value & WisTextureUsageFlagsUnorderedAccess) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS));
    }
    if (value & WisTextureUsageFlagsHostCopy) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_RESOURCE_FLAG_NONE));
    }
    return result;
}

inline D3D12MA::ALLOCATION_FLAGS convert_dx(WisMemoryFlags value) noexcept
{
    D3D12MA::ALLOCATION_FLAGS result = static_cast<D3D12MA::ALLOCATION_FLAGS>(0);
    if (value & WisMemoryFlagsDedicatedAllocation) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12MA::ALLOCATION_FLAG_COMMITTED));
    }
    if (value & WisMemoryFlagsMapped) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12MA::ALLOCATION_FLAG_NONE));
    }
    return result;
}

inline D3D12_BARRIER_SYNC convert_dx(WisBarrierSync value) noexcept
{
    D3D12_BARRIER_SYNC result = static_cast<D3D12_BARRIER_SYNC>(0);
    if (value & WisBarrierSyncAll) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_ALL));
    }
    if (value & WisBarrierSyncDraw) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_DRAW));
    }
    if (value & WisBarrierSyncIndexInput) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_INDEX_INPUT));
    }
    if (value & WisBarrierSyncVertexShading) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_VERTEX_SHADING));
    }
    if (value & WisBarrierSyncPixelShading) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_PIXEL_SHADING));
    }
    if (value & WisBarrierSyncDepthStencil) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_DEPTH_STENCIL));
    }
    if (value & WisBarrierSyncRenderTarget) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_RENDER_TARGET));
    }
    if (value & WisBarrierSyncCompute) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_COMPUTE_SHADING));
    }
    if (value & WisBarrierSyncRaytracing) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_RAYTRACING));
    }
    if (value & WisBarrierSyncCopy) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_COPY));
    }
    if (value & WisBarrierSyncResolve) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_RESOLVE));
    }
    if (value & WisBarrierSyncExecuteIndirect) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_EXECUTE_INDIRECT));
    }
    if (value & WisBarrierSyncAllShading) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_ALL_SHADING));
    }
    if (value & WisBarrierSyncNonPixelShading) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_NON_PIXEL_SHADING));
    }
    if (value & WisBarrierSyncClearUAV) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_CLEAR_UNORDERED_ACCESS_VIEW));
    }
    if (value & WisBarrierSyncVideoDecode) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_VIDEO_DECODE));
    }
    if (value & WisBarrierSyncVideoEncode) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_VIDEO_ENCODE));
    }
    if (value & WisBarrierSyncBuildRTAS) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_BUILD_RAYTRACING_ACCELERATION_STRUCTURE));
    }
    if (value & WisBarrierSyncCopyRTAS) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_SYNC_COPY_RAYTRACING_ACCELERATION_STRUCTURE));
    }
    return result;
}

inline D3D12_BARRIER_ACCESS convert_dx(WisResourceAccess value) noexcept
{
    D3D12_BARRIER_ACCESS result = static_cast<D3D12_BARRIER_ACCESS>(0);
    if (value & WisResourceAccessVertexBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_VERTEX_BUFFER));
    }
    if (value & WisResourceAccessConstantBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_CONSTANT_BUFFER));
    }
    if (value & WisResourceAccessIndexBuffer) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_INDEX_BUFFER));
    }
    if (value & WisResourceAccessRenderTarget) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_RENDER_TARGET));
    }
    if (value & WisResourceAccessUnorderedAccess) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_UNORDERED_ACCESS));
    }
    if (value & WisResourceAccessDepthWrite) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_DEPTH_STENCIL_WRITE));
    }
    if (value & WisResourceAccessDepthRead) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_DEPTH_STENCIL_READ));
    }
    if (value & WisResourceAccessShaderResource) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_SHADER_RESOURCE));
    }
    if (value & WisResourceAccessStreamOutput) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_STREAM_OUTPUT));
    }
    if (value & WisResourceAccessIndirectArgument) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_INDIRECT_ARGUMENT));
    }
    if (value & WisResourceAccessCopyDest) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_COPY_DEST));
    }
    if (value & WisResourceAccessCopySource) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_COPY_SOURCE));
    }
    if (value & WisResourceAccessConditionalRendering) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_PREDICATION));
    }
    if (value & WisResourceAccessAccelerationStructureRead) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_READ));
    }
    if (value & WisResourceAccessAccelerationStructureWrite) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE));
    }
    if (value & WisResourceAccessShadingRate) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_SHADING_RATE_SOURCE));
    }
    if (value & WisResourceAccessVideoDecodeRead) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_VIDEO_DECODE_READ));
    }
    if (value & WisResourceAccessVideoDecodeWrite) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_VIDEO_DECODE_WRITE));
    }
    if (value & WisResourceAccessResolveDest) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_RESOLVE_DEST));
    }
    if (value & WisResourceAccessResolveSource) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_RESOLVE_SOURCE));
    }
    if (value & WisResourceAccessNoAccess) {
        result = static_cast<decltype(result)>(static_cast<uint32_t>(result) | static_cast<uint32_t>(D3D12_BARRIER_ACCESS_NO_ACCESS));
    }
    return result;
}

} // namespace detail
} // namespace wis
#endif // WISDOM_DX12_CONVERT_HPP
