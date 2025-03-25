#ifndef WISDOM_RAYTRACING_HPP
#define WISDOM_RAYTRACING_HPP
#ifndef WISDOM_MODULE_DECL
#include "impl.dx12.h"
#include "impl.vk.h"
#include "impl.h"
#endif // !WISDOM_MODULE_DECL

WISDOM_EXPORT
namespace wis {
#if defined(WISDOM_DX12) && !defined(WISDOM_FORCE_VULKAN)
using Raytracing = ImplDX12Raytracing;
using AccelerationStructure = DX12AccelerationStructure;
using AcceleratedGeometryDesc = DX12AcceleratedGeometryDesc;
using RaytracingPipelineDesc = DX12RaytracingPipeineDesc;
using RaytracingPipeline = DX12RaytracingPipeline;

[[nodiscard]] inline constexpr wis::AcceleratedGeometryDesc
CreateGeometryDesc(const wis::AcceleratedGeometryInput& desc) noexcept
{
    return DX12CreateGeometryDesc(desc);
}

#elif defined(WISDOM_VULKAN)
using Raytracing = ImplVKRaytracing;
using AccelerationStructure = VKAccelerationStructure;
using AcceleratedGeometryDesc = VKAcceleratedGeometryDesc;
using RaytracingPipelineDesc = VKRaytracingPipeineDesc;
using RaytracingPipeline = VKRaytracingPipeline;

[[nodiscard]] inline constexpr wis::VKAcceleratedGeometryDesc
CreateGeometryDesc(const wis::AcceleratedGeometryInput& desc) noexcept
{
    return VKCreateGeometryDesc(desc);
}
#endif // WISDOM_DX12
} // namespace wis
#endif // !WISDOM_RAYTRACING_HPP
