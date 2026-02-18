#ifndef WIS_VK_DEVICE_CPP
#define WIS_VK_DEVICE_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>
#include <wisdom/util/allocation.hpp>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyDevice(WisVKDevice* self)
{
    auto& impl = *reinterpret_cast<VKDeviceImpl*>(self);
    if (!impl.device) {
        return;
    }
    detail::release_vk_device(impl.device, impl.device_header);
    impl.device_header = nullptr;
    impl.device        = VK_NULL_HANDLE;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateCommandQueue(const WisVKDevice*  self,
                                                                WisCommandQueueType type,
                                                                WisVKCommandQueue*  queue)
{
    WisResult res      = vk_success;
    auto&     device   = *reinterpret_cast<const VKDeviceImpl*>(self);
    VkQueue   vk_queue = VK_NULL_HANDLE;

    // Sanity check: lower and upper bound
    using QueueTypeUnderlying = std::underlying_type_t<WisCommandQueueType>;
    if (static_cast<QueueTypeUnderlying>(type) < 0 ||
        static_cast<size_t>(type) >= WisCommandQueueTypeCount) {
        return make_result<Func(), "Invalid command queue type specified">(VK_ERROR_INITIALIZATION_FAILED);
    }

    // Get queue family index based on type
    uint8_t queue_family_index = device.device_header->header.queue_residency[static_cast<size_t>(type)];
    if (queue_family_index == VKQueueFamilyProperties::invalid_family_index) {
        return make_result<Func(), "No suitable queue family found for the requested queue type">(VK_ERROR_FEATURE_NOT_PRESENT);
    }

    auto& queue_family = device.device_header->header.queue_families[queue_family_index];

    VkDeviceQueueInfo2 queue_info{
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
        .pNext            = nullptr,
        .flags            = 0,
        .queueFamilyIndex = static_cast<uint32_t>(queue_family.family_index),
        .queueIndex       = queue_family.GetNextQueueIndex(),
    };
    device.device_header->header.device_table.vkGetDeviceQueue2(device.device, &queue_info, &vk_queue);

    // Fill command queue impl
    auto& queue_impl         = *new (queue) VKCommandQueueImpl();
    queue_impl.queue         = vk_queue;
    queue_impl.device_header = device.device_header;
    device.device_header->AddRef(); // hold reference to device header
    queue_impl.device = device.device;

    // Setup semaphore pointer
    queue_impl.semaphore_ptr = reinterpret_cast<uint8_t*>(device.device_header->header.GetSemaphoreForQueueType(type, queue_info.queueIndex));
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateCommandList(const WisVKDevice*  self,
                                                               WisCommandQueueType type,
                                                               WisVKCommandList*   list)
{
    WisResult res    = vk_success;
    auto&     device = *reinterpret_cast<const VKDeviceImpl*>(self);

    // Sanity check: lower and upper bound
    using QueueTypeUnderlying = std::underlying_type_t<WisCommandQueueType>;
    if (static_cast<QueueTypeUnderlying>(type) < 0 ||
        static_cast<size_t>(type) >= WisCommandQueueTypeCount) {
        return make_result<Func(), "Invalid command queue type specified">(VK_ERROR_INITIALIZATION_FAILED);
    }

    // Get queue family index based on type
    uint8_t queue_family_index = device.device_header->header.queue_residency[static_cast<size_t>(type)];
    if (queue_family_index == VKQueueFamilyProperties::invalid_family_index) {
        return make_result<Func(), "No suitable queue family found for the requested queue type">(VK_ERROR_FEATURE_NOT_PRESENT);
    }

    uint8_t queue_family = device.device_header->header.queue_families[queue_family_index].family_index;

    // Create command pool
    VkCommandPoolCreateInfo pool_info{
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext            = nullptr,
        .queueFamilyIndex = static_cast<uint32_t>(queue_family),
    };
    VkCommandPool command_pool = VK_NULL_HANDLE;
    auto&         table        = device.device_header->header.device_table;
    VkResult      vr           = table.vkCreateCommandPool(device.device, &pool_info, nullptr, &command_pool);
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to create Vulkan command pool">(vr);
    }
    // Create command buffer
    VkCommandBufferAllocateInfo alloc_info{
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext              = nullptr,
        .commandPool        = command_pool,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer command_buffer = VK_NULL_HANDLE;

    vr = table.vkAllocateCommandBuffers(device.device, &alloc_info, &command_buffer);
    if (!succeeded(vr)) {
        table.vkDestroyCommandPool(device.device, command_pool, nullptr); // cleanup
        return make_result<Func(), "Failed to allocate Vulkan command buffer">(vr);
    }

    // Fill command list impl
    auto& list_impl          = *new (list) VKCommandListImpl();
    list_impl.command_pool   = command_pool;
    list_impl.command_buffer = command_buffer;
    list_impl.device_header  = device.device_header;
    device.device_header->AddRef(); // hold reference to device header
    list_impl.device = device.device;
    return res;
}

WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateFence(const WisVKDevice* self,
                                                         uint64_t           initial_value,
                                                         WisVKFence*        fence)
{
    WisResult res    = vk_success;
    auto&     device = *reinterpret_cast<const VKDeviceImpl*>(self);

    VkSemaphoreTypeCreateInfo timeline_desc{
        .sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        .pNext         = nullptr,
        .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
        .initialValue  = initial_value,
    };

    VkSemaphoreCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = &timeline_desc,
        .flags = 0,
    };
    VkSemaphore semaphore = VK_NULL_HANDLE;
    auto&       table     = device.device_header->header.device_table;
    VkResult    vr        = table.vkCreateSemaphore(device.device, &desc, nullptr, &semaphore);
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to create Vulkan timeline semaphore">(vr);
    }
    // Fill fence impl
    auto& out_fence         = *new (fence) VKFenceImpl();
    out_fence.fence         = semaphore;
    out_fence.device        = device.device;
    out_fence.device_header = device.device_header;
    device.device_header->AddRef(); // hold reference to device header
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateResourceAllocator(const WisVKDevice*      self,
                                                                     WisVKResourceAllocator* allocator)
{
    WisResult res             = vk_success;
    auto&     device          = *reinterpret_cast<const VKDeviceImpl*>(self);
    auto&     device_header   = device.device_header->header;
    auto&     instance_header = device_header.shared_header->header;
    auto&     adapter         = device.physical_device;

    uint32_t version = instance_header.api_version;
    auto&    gtable  = instance_header.global_table;
    auto&    dtable  = device_header.device_table;
    auto&    atable  = instance_header.adapter_table;
    auto&    ctable  = device_header.command_list_table;

    VmaVulkanFunctions allocator_functions{
        .vkGetInstanceProcAddr                   = gtable.vkGetInstanceProcAddr,
        .vkGetDeviceProcAddr                     = gtable.vkGetDeviceProcAddr,
        .vkGetPhysicalDeviceProperties           = atable.vkGetPhysicalDeviceProperties,
        .vkGetPhysicalDeviceMemoryProperties     = atable.vkGetPhysicalDeviceMemoryProperties,
        .vkAllocateMemory                        = dtable.vkAllocateMemory,
        .vkFreeMemory                            = dtable.vkFreeMemory,
        .vkMapMemory                             = dtable.vkMapMemory,
        .vkUnmapMemory                           = dtable.vkUnmapMemory,
        .vkFlushMappedMemoryRanges               = dtable.vkFlushMappedMemoryRanges,
        .vkInvalidateMappedMemoryRanges          = dtable.vkInvalidateMappedMemoryRanges,
        .vkBindBufferMemory                      = dtable.vkBindBufferMemory,
        .vkBindImageMemory                       = dtable.vkBindImageMemory,
        .vkGetBufferMemoryRequirements           = dtable.vkGetBufferMemoryRequirements,
        .vkGetImageMemoryRequirements            = dtable.vkGetImageMemoryRequirements,
        .vkCreateBuffer                          = dtable.vkCreateBuffer,
        .vkDestroyBuffer                         = dtable.vkDestroyBuffer,
        .vkCreateImage                           = dtable.vkCreateImage,
        .vkDestroyImage                          = dtable.vkDestroyImage,
        .vkCmdCopyBuffer                         = ctable.vkCmdCopyBuffer,
        .vkGetBufferMemoryRequirements2KHR       = dtable.vkGetBufferMemoryRequirements2,
        .vkGetImageMemoryRequirements2KHR        = dtable.vkGetImageMemoryRequirements2,
        .vkBindBufferMemory2KHR                  = dtable.vkBindBufferMemory2,
        .vkBindImageMemory2KHR                   = dtable.vkBindImageMemory2,
        .vkGetPhysicalDeviceMemoryProperties2KHR = atable.vkGetPhysicalDeviceMemoryProperties2,
        .vkGetDeviceBufferMemoryRequirements     = dtable.vkGetDeviceBufferMemoryRequirements,
        .vkGetDeviceImageMemoryRequirements      = dtable.vkGetDeviceImageMemoryRequirements,
        .vkGetMemoryWin32HandleKHR               = nullptr, // set later if available
    };

    VkPhysicalDeviceMemoryProperties2 mem_props{};
    mem_props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
    atable.vkGetPhysicalDeviceMemoryProperties2(adapter, &mem_props);

    VmaAllocatorCreateInfo allocatorInfo{
        .flags                          = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
        .physicalDevice                 = adapter,
        .device                         = device.device,
        .preferredLargeHeapBlockSize    = 0,
        .pAllocationCallbacks           = nullptr,
        .pDeviceMemoryCallbacks         = nullptr,
        .pHeapSizeLimit                 = nullptr,
        .pVulkanFunctions               = &allocator_functions,
        .instance                       = device_header.instance,
        .vulkanApiVersion               = version,
        .pTypeExternalMemoryHandleTypes = nullptr,
    };

    // Enable maintenance5 if available and maintenance4
    if (dtable.vkGetDeviceBufferMemoryRequirements) {
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_MAINTENANCE4_BIT;
    }
    if (device_header.features.index_buffer_range) {
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_MAINTENANCE5_BIT;
    }

#ifdef _WIN32
    // Only if there is an interop extension
    if (dtable.vkGetMemoryWin32HandleKHR) {
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_EXTERNAL_MEMORY_WIN32_BIT;
        allocator_functions.vkGetMemoryWin32HandleKHR = dtable.vkGetMemoryWin32HandleKHR;
    }
#endif // _WIN32

    VmaAllocator out_allocator;
    VkResult     vr = vmaCreateAllocator(&allocatorInfo, &out_allocator);

    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to create Vulkan memory allocator">(vr);
    }

    // Fill allocator impl
    auto& allocator_impl         = *new (allocator) VKResourceAllocatorImpl();
    allocator_impl.allocator     = out_allocator;
    allocator_impl.device        = device.device;
    allocator_impl.device_header = device.device_header;
    device.device_header->AddRef(); // hold reference to device header

    return res;
}

#endif // WIS_VK_DEVICE_CPP
