#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/vulkan/xvk_device.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/util/flags.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/bridge/format.h>
#include <wisdom/global/definitions.h>
#include <wisdom/util/misc.h>
#include <wisdom/vulkan/xvk_factory.h>
#include <cassert>
#include <unordered_set>

constexpr inline std::array required_extensions{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME,
    VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME, // for Allocator
    "VK_KHR_bind_memory2", // for Allocator
    "VK_KHR_get_physical_device_properties2", // for Allocator
    "VK_KHR_maintenance4", // for Allocator

    // VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
    // VK_KHR_RAY_QUERY_EXTENSION_NAME,
    // VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
    // VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
    // VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    // VK_KHR_MAINTENANCE3_EXTENSION_NAME,
    // VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
    // VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
    // VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME,
    // VK_KHR_MAINTENANCE1_EXTENSION_NAME,
    // VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
    // VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME,
    // VK_EXT_MEMORY_BUDGET_EXTENSION_NAME,
    // VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
    // VK_NV_MESH_SHADER_EXTENSION_NAME,
    // VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
    // VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME,
    // VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME,
};

auto RequestExtensions(wis::VKAdapterHandle adapter_hnd) noexcept
{
    auto* adapter = std::get<0>(adapter_hnd);
    auto* itbl = std::get<1>(adapter_hnd);

    std::vector<VkExtensionProperties> ext_props;
    uint32_t count = 0;
    itbl->vkEnumerateDeviceExtensionProperties(adapter, nullptr, &count, nullptr);
    ext_props.resize(count);
    itbl->vkEnumerateDeviceExtensionProperties(adapter, nullptr, &count, ext_props.data());

    std::unordered_set<std::string_view, wis::string_hash> ext_set;
    ext_set.reserve(count);

    for (const auto& e : ext_props)
        ext_set.emplace(e.extensionName);

    wis::detail::uniform_allocator<const char*, required_extensions.size()> avail_exts{};

    for (const auto* i : required_extensions) {
        if (!ext_set.contains(i))
            continue;
        avail_exts.allocate(i);
    }

    if constexpr (wis::debug_mode) {
        wis::lib_info("Active Device Extensions:");
        for (auto& i : avail_exts)
            wis::lib_info(wis::format("\t{}", i));
    }

    return avail_exts;
}

wis::detail::QueueResidency GetQueueFamilies(wis::VKAdapterHandle adapter_hnd) noexcept
{
    using namespace river::flags;
    using namespace wis::detail;

    auto* adapter = std::get<0>(adapter_hnd);
    auto* itbl = std::get<1>(adapter_hnd);

    std::vector<VkQueueFamilyProperties> family_props;
    VkResult result = VK_SUCCESS;
    uint32_t count = 0;
    itbl->vkGetPhysicalDeviceQueueFamilyProperties(adapter, &count, nullptr);
    family_props.resize(count);
    itbl->vkGetPhysicalDeviceQueueFamilyProperties(adapter, &count, family_props.data());

    wis::lib_info(wis::format("The system supports {} queue families", family_props.size()));
    assert(family_props.size() < 256);

    QueueResidency queues;

    // NOLINTNEXTLINE
    for (uint8_t i = 0; i < family_props.size(); i++) {
        using enum VkQueueFlagBits;
        auto& family = family_props[i];
        if ((family.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) == VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT &&
            queues.available_queues[+QueueTypes::Graphics].Empty()) {
            queues.available_queues[+QueueTypes::Graphics] = {
                uint16_t(uint32_t(family.queueFlags)),
                uint8_t(family.queueCount),
                i,
            };
            wis::lib_info(wis::format("\tDedicated Graphics queues: {}", family.queueCount));
            continue;
        }
        if ((family.queueFlags & VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT) == VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT &&
            queues.available_queues[+QueueTypes::Compute].Empty()) {
            queues.available_queues[+QueueTypes::Compute] = {
                uint16_t(uint32_t(family.queueFlags)),
                uint8_t(family.queueCount),
                i,
            };
            wis::lib_info(wis::format("\tDedicated Compute queues: {}", family.queueCount));
            continue;
        }
        if ((family.queueFlags & VkQueueFlagBits::VK_QUEUE_VIDEO_DECODE_BIT_KHR) == VkQueueFlagBits::VK_QUEUE_VIDEO_DECODE_BIT_KHR &&
            queues.available_queues[+QueueTypes::VideoDecode].Empty()) {
            queues.available_queues[+QueueTypes::VideoDecode] = {
                uint16_t(uint32_t(family.queueFlags)),
                uint8_t(family.queueCount),
                i,
            };
            wis::lib_info(wis::format("\tDedicated Video Decode queues: {}", family.queueCount));
            continue;
        }
        if ((family.queueFlags & VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT) == VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT &&
            queues.available_queues[+QueueTypes::Copy].Empty()) {
            queues.available_queues[+QueueTypes::Copy] = {
                uint16_t(uint32_t(family.queueFlags)),
                uint8_t(family.queueCount),
                i,
            };
            wis::lib_info(wis::format("\tDedicated Data Transfer queues: {}", family.queueCount));
            continue;
        }
    }
    return queues;
}

wis::VKDevice::VKDevice(wis::shared_handle<VkInstance> instance,
                        wis::SharedDevice device,
                        wis::VKAdapterHandle adapter) noexcept
    : QueryInternal(std::move(instance), std::move(device), adapter)
{
    queues = GetQueueFamilies(adapter);
}

std::pair<wis::Result, wis::VKDevice>
wis::VKCreateDevice(wis::VKFactoryHandle factory, wis::VKAdapterHandle adapter) noexcept
{
    constexpr static auto max_queue_count = +wis::detail::QueueTypes::Count;
    wis::detail::uniform_allocator<VkDeviceQueueCreateInfo, max_queue_count> queue_infos{};
    auto queues = GetQueueFamilies(adapter);

    constexpr static auto priorities = []() {
        std::array<float, 64> priorities{};
        priorities.fill(1.0f);
        return priorities;
    }();

    for (size_t queue_info_size = 0; queue_info_size < max_queue_count; queue_info_size++) {
        auto& q = queues.available_queues[queue_info_size];
        if (q.count == 0u)
            continue;
        queue_infos.allocate() = {
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            nullptr,
            0,
            q.family_index,
            q.count,
            priorities.data(),
        };
    }

    auto exts = RequestExtensions(adapter);

    // Loading features

    VkPhysicalDeviceFeatures2 features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = nullptr,
        .features = {},
    };
    vkGetPhysicalDeviceFeatures(std::get<0>(adapter), &features.features);

    VkPhysicalDeviceTimelineSemaphoreFeaturesKHR timeline_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR,
        .pNext = nullptr,
        .timelineSemaphore = VK_TRUE,
    };
    features.pNext = &timeline_features;

    VkDeviceCreateInfo device_info{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &features,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size()),
        .pQueueCreateInfos = queue_infos.data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<uint32_t>(exts.size()),
        .ppEnabledExtensionNames = exts.data(),
        .pEnabledFeatures = nullptr,
    };

    // Creating device


    auto* xadapter = std::get<0>(adapter);
    auto* itbl = std::get<1>(factory);

    VkDevice device{};
    VkResult result = VK_SUCCESS;
    if (!wis::succeeded(result = itbl->vkCreateDevice(xadapter, &device_info, nullptr, &device)))
        return { wis::make_result<FUNC, "vkCreateDevice failed to create device">(result), wis::VKDevice{} };

    std::unique_ptr<VkDeviceTable> device_table = std::make_unique<VkDeviceTable>();
    device_table->Init(device, Internal<VKFactory>::global_table);

    return { wis::success, wis::VKDevice{
                                   std::move(std::get<0>(factory)),
                                   wis::SharedDevice{ device, std::move(device_table) },
                                   std::move(adapter),
                           } };
}

std::pair<wis::Result, wis::VKFence>
wis::VKDevice::CreateFence(uint64_t initial_value) const noexcept
{
    VkSemaphoreTypeCreateInfo timeline_desc{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        .pNext = nullptr,
        .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
        .initialValue = initial_value,
    };

    VkSemaphoreCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = &timeline_desc,
        .flags = 0,
    };
    VkSemaphore sem;
    VkResult result = VK_SUCCESS;

    return wis::succeeded(result = device.table()->vkCreateSemaphore(device.get(), &desc, nullptr, &sem))
            ? std::pair{ wis::success, VKFence{ { sem, device, device.table()->vkDestroySemaphore } } }
            : std::pair{ wis::make_result<FUNC, "vkCreateSemaphore failed to create semaphore">(result), VKFence{} };
}

std::pair<wis::Result, wis::VKResourceAllocator>
wis::VKDevice::CreateAllocator()const noexcept
{
    uint32_t version = 0;
    auto& gt = wis::Internal<VKFactory>::global_table;
    auto& it = *GetInstanceTable();
    auto& dt = *device.table();
    gt.vkEnumerateInstanceVersion(&version);

    std::unique_ptr<VmaVulkanFunctions> vkfuncs{ new VmaVulkanFunctions{
            .vkGetInstanceProcAddr = gt.vkGetInstanceProcAddr,
            .vkGetDeviceProcAddr = gt.vkGetDeviceProcAddr,
            .vkGetPhysicalDeviceProperties = it.vkGetPhysicalDeviceProperties,
            .vkGetPhysicalDeviceMemoryProperties = it.vkGetPhysicalDeviceMemoryProperties,
            .vkAllocateMemory = dt.vkAllocateMemory,
            .vkFreeMemory = dt.vkFreeMemory,
            .vkMapMemory = dt.vkMapMemory,
            .vkUnmapMemory = dt.vkUnmapMemory,
            .vkFlushMappedMemoryRanges = dt.vkFlushMappedMemoryRanges,
            .vkInvalidateMappedMemoryRanges = dt.vkInvalidateMappedMemoryRanges,
            .vkBindBufferMemory = dt.vkBindBufferMemory,
            .vkBindImageMemory = dt.vkBindImageMemory,
            .vkGetBufferMemoryRequirements = dt.vkGetBufferMemoryRequirements,
            .vkGetImageMemoryRequirements = dt.vkGetImageMemoryRequirements,
            .vkCreateBuffer = dt.vkCreateBuffer,
            .vkDestroyBuffer = dt.vkDestroyBuffer,
            .vkCreateImage = dt.vkCreateImage,
            .vkDestroyImage = dt.vkDestroyImage,
            .vkCmdCopyBuffer = dt.vkCmdCopyBuffer,
            .vkGetBufferMemoryRequirements2KHR = dt.vkGetBufferMemoryRequirements2,
            .vkGetImageMemoryRequirements2KHR = dt.vkGetImageMemoryRequirements2,
            .vkBindBufferMemory2KHR = dt.vkBindBufferMemory2,
            .vkBindImageMemory2KHR = dt.vkBindImageMemory2,
            .vkGetPhysicalDeviceMemoryProperties2KHR = it.vkGetPhysicalDeviceMemoryProperties2,
            .vkGetDeviceBufferMemoryRequirements = dt.vkGetDeviceBufferMemoryRequirements,
            .vkGetDeviceImageMemoryRequirements = dt.vkGetDeviceImageMemoryRequirements,
    } };

    VmaAllocatorCreateInfo allocatorInfo{
        .flags = 0,
        .physicalDevice = std::get<0>(adapter),
        .device = device.get(),
        .pVulkanFunctions = vkfuncs.get(),
        .instance = instance.get(),
    };

    VmaAllocator al;
    VkResult vr;
    return wis::succeeded(vr = vmaCreateAllocator(&allocatorInfo, &al))
            ? std::make_pair(wis::success, VKResourceAllocator{ wis::managed_handle_ex<VmaAllocator>{ device, al }, std::move(vkfuncs) })
            : std::make_pair(wis::make_result<FUNC, "Failed to create an Allocator">(vr), VKResourceAllocator{});
}