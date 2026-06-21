// This file is generated. Do not edit directly.
#ifndef WISDOM_RAYTRACING_CPP_DX12_CONVERT_HPP
#define WISDOM_RAYTRACING_CPP_DX12_CONVERT_HPP
#ifndef __cplusplus
#    error "This is a C++ only header"
#endif // __cplusplus

#include <D3D12MemAlloc.h>
#include <dxgi1_6.h>
#include "c_api.h"

namespace wis {
namespace detail {

constexpr inline D3D12_RAYTRACING_GEOMETRY_TYPE DX12Convert(WisGeometryType value) noexcept
{
    switch (value) {
    case WisGeometryTypeTriangles:
        return D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
    case WisGeometryTypeAABBs:
        return D3D12_RAYTRACING_GEOMETRY_TYPE_PROCEDURAL_PRIMITIVE_AABBS;
    default:
        return static_cast<D3D12_RAYTRACING_GEOMETRY_TYPE>(0);
    }
}

constexpr inline D3D12_RAYTRACING_GEOMETRY_FLAGS DX12Convert(WisGeometryFlags value) noexcept
{
    D3D12_RAYTRACING_GEOMETRY_FLAGS result = static_cast<D3D12_RAYTRACING_GEOMETRY_FLAGS>(0);
    if (value & WisGeometryFlagsOpaque) {
        result |= D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
    }
    if (value & WisGeometryFlagsNoDuplicateAnyHitInvocation) {
        result |= D3D12_RAYTRACING_GEOMETRY_FLAG_NO_DUPLICATE_ANYHIT_INVOCATION;
    }
    return result;
}

constexpr inline D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS DX12Convert(
    WisAccelerationStructureFlags value
) noexcept
{
    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS
        result = static_cast<D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS>(0);
    if (value & WisAccelerationStructureFlagsAllowUpdate) {
        result |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
    }
    if (value & WisAccelerationStructureFlagsAllowCompaction) {
        result |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_COMPACTION;
    }
    if (value & WisAccelerationStructureFlagsPreferFastTrace) {
        result |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
    }
    if (value & WisAccelerationStructureFlagsPreferFastBuild) {
        result |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_BUILD;
    }
    if (value & WisAccelerationStructureFlagsMinimizeMemory) {
        result |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_MINIMIZE_MEMORY;
    }
    return result;
}

} // namespace detail
} // namespace wis
#endif // WISDOM_RAYTRACING_CPP_DX12_CONVERT_HPP
