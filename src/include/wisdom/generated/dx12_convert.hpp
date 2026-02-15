// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_DX12_CONVERT_HPP
#define WISDOM_CPP_DX12_CONVERT_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include <wisdom/generated/c_api.h>
#include <dxgi1_6.h>
#include <d3d12.h>

namespace wis {
namespace detail {

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
    case WisCommandQueuePriorityGlobalRealtime:
        return D3D12_COMMAND_QUEUE_PRIORITY_GLOBAL_REALTIME;
    default:
        return D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    }
}

inline D3D12_SHADER_VISIBILITY convert_dx(WisShaderStages value) noexcept
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

inline D3D12_SAMPLER_FLAGS convert_dx(WisSamplerFlags value) noexcept
{
    D3D12_SAMPLER_FLAGS result = static_cast<D3D12_SAMPLER_FLAGS>(0);
    if (value & WisSamplerFlagsNonNormalizedCoordinates) {
        result |= D3D12_SAMPLER_FLAG_NON_NORMALIZED_COORDINATES;
    }
    return result;
}

} // namespace detail
} // namespace wis
#endif // WISDOM_DX12_CONVERT_HPP
