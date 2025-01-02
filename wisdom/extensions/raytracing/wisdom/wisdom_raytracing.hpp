#ifndef WISDOM_RAYTRACING_HPP
#define WISDOM_RAYTRACING_HPP
#include <cstdint>

namespace wis {
struct AccelerationInstance {
    float transform[3][4];
    uint32_t instance_id : 24;
    uint32_t mask : 8;
    uint32_t instance_offset : 24;
    uint32_t flags : 8;
    uint64_t acceleration_structure_handle;
};
} // namespace wis

#if defined(WISDOM_DX12)
#include <wisdom/dx12/dx12_device.h>
#include <wisdom/dx12/dx12_checks.h>
#include <d3d12.h>

namespace wis {
class DX12Raytracing;

using DX12AccelerationStructure = D3D12_GPU_VIRTUAL_ADDRESS;
using DX12AccelerationStructureView = std::tuple<DX12AccelerationStructure>;

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
    [[nodiscard]] wis::ASAllocationInfo
    GetTopLevelASSize(const wis::TopLevelASBuildDesc& tlas_desc) const noexcept
    {
        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs{
            .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL,
            .Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS(convert_dx(tlas_desc.flags) | (tlas_desc.update ? D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE : 0)),
            .NumDescs = tlas_desc.instance_count,
            .DescsLayout = tlas_desc.indirect ? D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS : D3D12_ELEMENTS_LAYOUT_ARRAY,
            .InstanceDescs = tlas_desc.gpu_address
        };
        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuild_info = {};
        shared_device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &prebuild_info);
        return { prebuild_info.ResultDataMaxSizeInBytes, prebuild_info.ScratchDataSizeInBytes, prebuild_info.UpdateScratchDataSizeInBytes };
    }

    [[nodiscard]] wis::DX12AccelerationStructure
    CreateAccelerationStructure(wis::Result& result, wis::DX12BufferView buffer, uint64_t buffer_offset, [[maybe_unused]] uint64_t structure_size, [[maybe_unused]] ASLevel structure_level) const noexcept
    {
        return std::get<0>(buffer)->GetGPUVirtualAddress() + buffer_offset;
    }

    [[nodiscard]] uint64_t GetAccelerationStructureDeviceAddress(wis::DX12AccelerationStructureView as) const noexcept
    {
        return std::get<0>(as);
    }

    void BuildTopLevelAS(const wis::DX12CommandList& cmd_list, const wis::TopLevelASBuildDesc& tlas_desc)
    {
        auto& cmd_list_i = cmd_list.GetInternal().list;

        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC build_desc{
            .Inputs = {
                    .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL,
                    .Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS(convert_dx(tlas_desc.flags) | (tlas_desc.update ? D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE : 0)),
                    .NumDescs = tlas_desc.instance_count,
                    .DescsLayout = tlas_desc.indirect ? D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS : D3D12_ELEMENTS_LAYOUT_ARRAY,
                    .InstanceDescs = tlas_desc.gpu_address },
        };

        cmd_list_i->BuildRaytracingAccelerationStructure(&build_desc, 0, nullptr);
    }
};

#pragma region DX12Raytracing
#pragma endregion DX12Raytracing
} // namespace wis
#endif // WISDOM_DX12

#if defined(WISDOM_VULKAN)
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vk_rtas.h>

namespace wis {
class VKRaytracing;

template<>
struct Internal<VKRaytracing> {
    wis::SharedDevice device;

    PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR = nullptr;
    PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR = nullptr;
    PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR = nullptr;
    PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR = nullptr;
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
        return vkGetAccelerationStructureBuildSizesKHR;
    }

public:
    [[nodiscard]] wis::ASAllocationInfo
    GetTopLevelASSize(const wis::TopLevelASBuildDesc& tlas_desc)
    {
        VkAccelerationStructureGeometryKHR geometry{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
            .geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR,
            .geometry = {
                    .instances = {
                            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR,
                            .arrayOfPointers = tlas_desc.indirect,
                            .data = { .deviceAddress = tlas_desc.gpu_address } } },
            .flags = convert_vk(tlas_desc.flags)
        };
        VkAccelerationStructureBuildGeometryInfoKHR build_info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR,
            .type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR,
            .flags = convert_vk(tlas_desc.flags),
            .mode = tlas_desc.update ? VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR : VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR,
            .srcAccelerationStructure = VK_NULL_HANDLE,
            .dstAccelerationStructure = VK_NULL_HANDLE,
            .geometryCount = 1u,
            .pGeometries = &geometry,
            .scratchData = { .deviceAddress = 0 }
        };
        VkAccelerationStructureBuildSizesInfoKHR build_sizes_info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR,
        };

        uint32_t max_instance_count = tlas_desc.instance_count;
        vkGetAccelerationStructureBuildSizesKHR(device.get(),
                                                VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
                                                &build_info,
                                                &max_instance_count,
                                                &build_sizes_info);
        return { build_sizes_info.accelerationStructureSize, build_sizes_info.buildScratchSize, build_sizes_info.updateScratchSize };
    }

    [[nodiscard]] wis::VKAccelerationStructure
    CreateAccelerationStructure(wis::Result& result, wis::VKBufferView buffer, uint64_t buffer_offset, uint64_t structure_size, ASLevel structure_level) const noexcept
    {
        VkAccelerationStructureCreateInfoKHR create_info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR,
            .buffer = std::get<0>(buffer),
            .offset = buffer_offset,
            .size = structure_size,
            .type = structure_level == ASLevel::Top ? VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR : VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR
        };
        VKAccelerationStructure as;
        auto& as_i = as.GetMutableInternal();

        auto res = vkCreateAccelerationStructureKHR(device.get(), &create_info, nullptr, &as_i.handle);
        if (res != VK_SUCCESS) {
            result = wis::make_result<FUNC, "Acceleration structure creation failed">(res);
            return as;
        }
        as_i.device = device;
        as_i.vkDestroyAccelerationStructureKHR = vkDestroyAccelerationStructureKHR;
        return as;
    }

    [[nodiscard]] uint64_t GetAccelerationStructureDeviceAddress(wis::VKAccelerationStructureView as) const noexcept
    {
        VkAccelerationStructureDeviceAddressInfoKHR info{
            .sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR,
            .accelerationStructure = std::get<0>(as)
        };
        return vkGetAccelerationStructureDeviceAddressKHR(device.get(), &info);
    }
};

#pragma region VKRaytracing
#pragma endregion VKRaytracing
} // namespace wis
#endif // WISDOM_VULKAN

namespace wis {
#if defined(WISDOM_DX12) && !defined(WISDOM_FORCE_VULKAN)
using Raytracing = ImplDX12Raytracing;
using AccelerationStructure = DX12AccelerationStructure;
#elif defined(WISDOM_VULKAN)
using Raytracing = ImplVKRaytracing;
using AccelerationStructure = VKAccelerationStructure;
#endif // WISDOM_DX12
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/impl.vk.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // !WISDOM_RAYTRACING_HPP
