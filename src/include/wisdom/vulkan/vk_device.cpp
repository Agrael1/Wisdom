#ifndef WIS_VK_DEVICE_CPP
#define WIS_VK_DEVICE_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>
#include <wisdom/util/allocation.hpp>
#include <bit>

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
    queue_impl.device        = device.device;
    queue_impl.device_header = device.device_header;
    queue_impl.device_header->AddRef(); // hold reference to device header

    // Setup semaphore pointer
    queue_impl.semaphore_ptr = reinterpret_cast<uint8_t*>(device.device_header->header.GetSemaphoreForQueueType(type, queue_info.queueIndex));
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateCommandAllocator(const WisVKDevice*     self,
                                                                    WisCommandQueueType    type,
                                                                    WisVKCommandAllocator* allocator)
{
    auto& device = *reinterpret_cast<const VKDeviceImpl*>(self);
    auto& table  = device.device_header->header.device_table;

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

    std::unique_ptr<detail::VKCommandPoolControlBlock> pool_control_block = wis::make_unique<detail::VKCommandPoolControlBlock>();
    if (!pool_control_block) {
        return make_result<Func(), "Failed to allocate memory for command pool control block">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    uint8_t queue_family = device.device_header->header.queue_families[queue_family_index].family_index;

    // Create command pool
    VkCommandPoolCreateInfo pool_info{
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext            = nullptr,
        .queueFamilyIndex = static_cast<uint32_t>(queue_family),
    };
    VkCommandPool command_pool = VK_NULL_HANDLE;
    VkResult      vr           = table.vkCreateCommandPool(device.device, &pool_info, nullptr, &command_pool);
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to create Vulkan command pool">(vr);
    }

    pool_control_block->header.device        = device.device;
    pool_control_block->header.device_header = device.device_header;
    pool_control_block->header.device_header->AddRef(); // hold reference to device header for command pool control block

    auto& allocator_impl               = *new (allocator) VKCommandAllocatorImpl();
    allocator_impl.command_pool        = command_pool;
    allocator_impl.command_pool_header = pool_control_block.release();

    return vk_success;
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
    out_fence.device_header->AddRef(); // hold reference to device header
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceGetResourceAllocator(const WisVKDevice*      self,
                                                                  WisVKResourceAllocator* allocator)
{
    auto& device = *reinterpret_cast<const VKDeviceImpl*>(self);

    // Fill allocator impl
    auto& allocator_impl         = *new (allocator) VKResourceAllocatorImpl();
    allocator_impl.allocator     = device.device_header->header.allocator;
    allocator_impl.device_header = device.device_header;
    allocator_impl.device_header->AddRef(); // hold reference to device header
    return vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateDescriptorHeap(const WisVKDevice*           self,
                                                                  const WisDescriptorHeapDesc* desc,
                                                                  WisVKDescriptorHeap*         heap)
{
    auto& device   = *reinterpret_cast<const VKDeviceImpl*>(self);
    auto& header   = device.device_header->header;
    auto& features = header.features;
    auto& table    = header.device_table;

    // 0. If heap is supported
    if (!features.descriptor_heap) {
        return make_result<Func(), "Descriptor heaps are not supported by this Vulkan device">(VK_ERROR_FEATURE_NOT_PRESENT);
    }

    // 1. Calculate descriptor memory requirements based on desc
    bool is_shader_heap    = desc->memory_type == WisDescriptorMemoryTypeShaderVisible;
    bool is_sampler_heap   = desc->type == WisDescriptorHeapTypeSampler;
    bool embedded_samplers = !(desc->flags & WisDescriptorHeapFlagsDisallowEmbeddedSamplers);

    std::size_t heap_alignment = is_shader_heap ? is_sampler_heap
                    ? features.sampler_heap_alignment
                    : features.descriptor_heap_alignment
                                                : __STDCPP_DEFAULT_NEW_ALIGNMENT__;

    std::size_t descriptor_size = is_sampler_heap
            ? features.sampler_desc_size
            : features.resource_desc_size;

    std::size_t reserved_size = is_shader_heap ? is_sampler_heap
                    ? embedded_samplers ? features.sampler_heap_reserved_size_with_embedded
                                        : features.sampler_heap_reserved_size
                    : features.descriptor_heap_reserved_size
                                               : 0;

    std::size_t max_heap_size = is_shader_heap ? is_sampler_heap
                    ? features.max_sampler_heap_size
                    : features.max_descriptor_heap_size
                                               : std::numeric_limits<std::size_t>::max();

    std::size_t required_size = wis::aligned_size(
            desc->descriptor_count * descriptor_size + reserved_size,
            heap_alignment);

    if (is_shader_heap && required_size > max_heap_size) {
        return make_result<Func(), "Requested descriptor heap size exceeds the maximum supported by this Vulkan device">(VK_ERROR_INITIALIZATION_FAILED);
    }

    if (!is_shader_heap) {
        // 2a. For non-shader visible heaps, we can use a simple host allocation
        VkBuffer buffer = reinterpret_cast<VkBuffer>(std::malloc(required_size));
        if (!buffer) {
            return make_result<Func(), "Failed to allocate memory for non-shader visible descriptor heap">(VK_ERROR_OUT_OF_HOST_MEMORY);
        }

        // Fill descriptor heap impl
        auto& heap_impl           = *new (heap) VKDescriptorHeapImpl();
        heap_impl.buffer          = buffer;
        heap_impl.allocation      = VK_NULL_HANDLE; // No VMA allocation for non-shader visible heaps
        heap_impl.mapped_ptr      = buffer; // For non-shader visible heaps, the buffer pointer itself serves as the mapped pointer
        heap_impl.gpu_address     = 0; // No GPU address for non-shader visible heaps
        heap_impl.reserved_size   = 0;
        heap_impl.descriptor_size = descriptor_size;
        heap_impl.heap_size       = desc->descriptor_count;
        heap_impl.device          = device.device;
        heap_impl.device_header   = device.device_header;
        heap_impl.device_header->AddRef(); // hold reference to device header
        return vk_success;
    }

    // 2. Create buffer
    VkBufferCreateInfo buffer_info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size  = required_size,
        .usage = VK_BUFFER_USAGE_DESCRIPTOR_HEAP_BIT_EXT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
    };
    VmaAllocationCreateInfo alloc_info{
        .flags          = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage          = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
        .requiredFlags  = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        .preferredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    };

    VkBuffer          buffer     = VK_NULL_HANDLE;
    VmaAllocation     allocation = VK_NULL_HANDLE;
    VmaAllocationInfo alloc_info_out{};
    VkResult          vr = vmaCreateBufferWithAlignment(
            header.allocator,
            &buffer_info,
            &alloc_info,
            heap_alignment,
            &buffer,
            &allocation,
            &alloc_info_out);

    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to create buffer for shader visible descriptor heap">(vr);
    }

    // Get GPU address of the buffer
    VkBufferDeviceAddressInfo address_info{
        .sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .pNext  = nullptr,
        .buffer = buffer
    };

    auto& heap_impl           = *new (heap) VKDescriptorHeapImpl();
    heap_impl.buffer          = buffer;
    heap_impl.allocation      = allocation;
    heap_impl.mapped_ptr      = alloc_info_out.pMappedData;
    heap_impl.gpu_address     = table.vkGetBufferDeviceAddress(device.device, &address_info);
    heap_impl.reserved_size   = reserved_size / descriptor_size;
    heap_impl.descriptor_size = descriptor_size;
    heap_impl.heap_size       = desc->descriptor_count;
    heap_impl.device          = device.device;
    heap_impl.device_header   = device.device_header;
    heap_impl.device_header->AddRef(); // hold reference to device header
    return vk_success;
}

WIS_EXTERN_C WISDOM_API void wisVKDeviceQueryProperties(const WisVKDevice* self,
                                                        void*              properties)
{
    if (!properties) {
        return;
    }

    auto& device = *reinterpret_cast<const VKDeviceImpl*>(self);
    auto& header = device.device_header->header;
    void* next   = properties;

    do {
        WisQueryStructHeader header_local{};
        std::memcpy(&header_local, next, sizeof(WisQueryStructHeader));

        switch (header_local.property_type) {
        case WisQueryPropertyTypeDeviceCommandQueueProperties: {
            auto* props = static_cast<WisDeviceCommandQueuesProperties*>(next);
            for (size_t i = 0; i < WisCommandQueueTypeCount; ++i) {
                auto&                   family_index = header.queue_residency[i];
                bool                    supported    = family_index != VKQueueFamilyProperties::invalid_family_index;
                WisCommandQueuePriority priority     = WisCommandQueuePriority(supported ? (header.queue_families[family_index].queue_priority) : 0);

                props->supported_queues[i]   = supported;
                props->max_queue_priority[i] = priority;
            }
        } break;
        case WisQueryPropertyTypeDeviceDescriptorHeapProperties: {
            auto* props = static_cast<WisDeviceDescriptorHeapProperties*>(next);
            if (!header.features.descriptor_heap) {
                break;
            }

            auto real_dheap_size               = header.features.max_descriptor_heap_size - header.features.descriptor_heap_reserved_size;
            auto real_sheap_size               = header.features.max_sampler_heap_size - header.features.sampler_heap_reserved_size;
            auto real_sheap_size_with_embedded = header.features.max_sampler_heap_size - header.features.sampler_heap_reserved_size_with_embedded;

            props->max_descriptor_heap_size            = real_dheap_size / header.features.resource_desc_size;
            props->max_sampler_heap_size               = real_sheap_size / header.features.sampler_desc_size;
            props->max_sampler_heap_size_with_embedded = real_sheap_size_with_embedded / header.features.sampler_desc_size;
            props->descriptor_increment_size           = header.features.resource_desc_size;
            props->sampler_increment_size              = header.features.sampler_desc_size;
        } break;
        case WisQueryPropertyTypeDeviceMemoryProperties: {
            auto* props                      = static_cast<WisDeviceMemoryProperties*>(next);
            props->host_image_copy_supported = header.features.host_image_copy;

            const VkPhysicalDeviceMemoryProperties* mem_props;
            vmaGetMemoryProperties(header.allocator, &mem_props);

            for (uint32_t i = 0; i < mem_props->memoryTypeCount; ++i) {
                const VkMemoryPropertyFlags flags = mem_props->memoryTypes[i].propertyFlags;
                if ((flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
                    (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) &&
                    (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
                    props->gpu_upload_supported = true;
                    break;
                }
            }
        } break;
        default:
            break;
        }
        next = header_local.next_in_chain;
    } while (next);
}

#endif // WIS_VK_DEVICE_CPP
