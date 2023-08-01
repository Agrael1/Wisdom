#ifndef WISDOM_MODULES
// #include <wisdom/vulkan/vk_adapter.h>
#include <wisdom/vulkan/vk_dynamic_loader.h>
#include <span>
#endif

wis::AdapterDesc wis::VKAdapter::GetDesc() const noexcept
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

    std::string_view x = desc.deviceName;
    return AdapterDesc{
        .description{ x.begin(), x.end() },
        .vendor_id = desc.vendorID,
        .device_id = desc.deviceID,
        .subsys_id = desc.apiVersion,
        .revision = desc.driverVersion,

        .dedicated_video_memory = local_mem,
        .dedicated_system_memory = 0,
        .shared_system_memory = system_mem,
        .adapter_id = reinterpret_cast<uint64_t&>(id_props.deviceLUID),
        .flags = flag
    };
}
