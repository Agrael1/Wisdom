#ifndef WISDOM_RAYTRACING_HPP
#define WISDOM_RAYTRACING_HPP
#if defined(WISDOM_DX12)
#include <wisdom/dx12/dx12_device.h>
#include <wisdom/dx12/dx12_checks.h>
#include <d3d12.h>

namespace wis {
class DX12Raytracing;

template<>
struct Internal<DX12Raytracing> {
    wis::com_ptr<ID3D12Device10> shared_device;

    bool supports_raytracing = false;
};

class ImplDX12Raytracing : public QueryInternalExtension<DX12Raytracing, DX12DeviceExtension>
{
protected:
    virtual wis::Result Init(const wis::DX12Device& instance) noexcept override
    {
        shared_device = instance.GetInternal().device;
        D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureSupportData = {};
        supports_raytracing = wis::succeeded(shared_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureSupportData, sizeof(featureSupportData))) &&
                featureSupportData.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
        return wis::success;
    }

public:
    virtual bool Supported() const noexcept override
    {
        return supports_raytracing;
    }

public:
};

#pragma region DX12Raytracing
#pragma endregion DX12Raytracing
} // namespace wis
#endif // WISDOM_DX12

#if defined(WISDOM_VULKAN)
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_allocator.h>

namespace wis {
class VKRaytracing;

template<>
struct Internal<VKRaytracing> {
    wis::SharedDevice device;

    bool supports_raytracing = false;
};

class ImplVKRaytracing : public QueryInternalExtension<VKRaytracing, wis::VKDeviceExtension>
{
protected:
    virtual WIS_INLINE bool
    GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
                     std::unordered_set<std::string_view>& ext_name_set,
                     std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                     std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override;

    virtual WIS_INLINE wis::Result
    Init(const wis::VKDevice& instance,
         const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
         const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override;

public:
    virtual bool Supported() const noexcept override
    {
        return supports_raytracing;
    }

public:
};

#pragma region VKRaytracing
#pragma endregion VKRaytracing
} // namespace wis
#endif // WISDOM_VULKAN

namespace wis {
#if defined(WISDOM_DX12) && !defined(WISDOM_FORCE_VULKAN)
using Raytracing = ImplDX12Raytracing;
#elif defined(WISDOM_VULKAN)
using Raytracing = ImplVKRaytracing;
#endif // WISDOM_DX12
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/impl.vk.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // !WISDOM_RAYTRACING_HPP
