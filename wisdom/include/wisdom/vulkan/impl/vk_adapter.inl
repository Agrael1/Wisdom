#ifndef WISDOM_MODULES
// #include <wisdom/vulkan/vk_adapter.h>
#include <wisdom/vulkan/vk_dynamic_loader.h>
#endif

void wis::VKAdapter::GetDesc(wis::AdapterDesc& out_desc) const noexcept
{
    using namespace river::flags;
    vk::PhysicalDeviceProperties2 properties;
    vk::PhysicalDeviceIDProperties id_props;
    properties.pNext = &id_props;
    DynamicLoader::loader.vkGetPhysicalDeviceProperties2(adapter, reinterpret_cast<VkPhysicalDeviceProperties2*>(&properties));

    auto& desc = properties.properties;
    auto desc2 = adapter.getMemoryProperties();

    uint64_t local_mem = 0;
    uint64_t system_mem = 0;
    std::span types{ desc2.memoryTypes.data(), desc2.memoryTypeCount };
    for (auto& i : types) {
        if (i.propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal &&
            desc2.memoryHeaps[i.heapIndex].flags & vk::MemoryHeapFlagBits::eDeviceLocal) {
            local_mem = desc2.memoryHeaps[i.heapIndex].size;
        }

        if (i.propertyFlags & vk::MemoryPropertyFlagBits::eHostCoherent) {
            system_mem = desc2.memoryHeaps[i.heapIndex].size;
        }
        if ((system_mem != 0u) && (local_mem != 0u))
            break;
    }

    AdapterFlags flag{
        AdapterFlags(((+AdapterFlags::Remote) != 0u) && ((uint32_t(desc.deviceType) & uint32_t(vk::PhysicalDeviceType::eVirtualGpu)) != 0u)) | AdapterFlags(((+AdapterFlags::Software) != 0u) && ((uint32_t(desc.deviceType) & uint32_t(vk::PhysicalDeviceType::eCpu)) != 0u))
    };

    std::copy_n(desc.deviceName.data(), sizeof(out_desc.description) - 1, out_desc.description);
    out_desc.vendor_id = desc.vendorID;

    out_desc.device_id = desc.deviceID;
    out_desc.subsys_id = desc.apiVersion;
    out_desc.revision = desc.driverVersion;

    out_desc.dedicated_video_memory = local_mem;
    out_desc.dedicated_system_memory = 0;
    out_desc.shared_system_memory = system_mem;
    out_desc.adapter_id = reinterpret_cast<uint64_t&>(id_props.deviceLUID);
    out_desc.flags = wisAdapterFlags(flag);
}