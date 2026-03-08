#ifndef WIS_VK_TYPES_HPP
#define WIS_VK_TYPES_HPP
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus

#include <wisdom/vulkan/vk_extensions.hpp>
#include <wisdom/generated/cpp_api.hpp>
#include <vk_mem_alloc.h>
#include <cstring>
#include <array>

namespace wis {
//-----------------------------------------------------------------------------
constexpr wis::Result convert_result(WisResult result) noexcept
{
    return { static_cast<wis::Status>(result.status), result.platform_code, result.error };
}

namespace detail {
struct VKInstanceControlBlock;
struct VKDeviceControlBlock;
struct VKDescriptorSetLayoutContainer;
struct VKCommandPoolControlBlock;
struct VKRootSignatureControlBlock;
struct VKQueueFamilyExtras;
} // namespace detail

namespace impl {
//-----------------------------------------------------------------------------
struct VKInstanceImpl {
    VkInstance                      instance;
    detail::VKInstanceControlBlock* shared_header;
};

struct VKAdapterQueryImpl {
    VkPhysicalDevice*               physical_devices;
    std::size_t                     adapter_count;
    VkInstance                      instance;
    detail::VKInstanceControlBlock* shared_header;
};

struct VKDeviceImpl {
    VkDevice                      device;
    VkPhysicalDevice              physical_device;
    detail::VKDeviceControlBlock* device_header;
};

struct VKCommandQueueImpl {
    VkQueue                       queue;
    uint8_t*                      semaphore_ptr;
    VkDevice                      device;
    detail::VKDeviceControlBlock* device_header;
};

struct VKCommandAllocatorImpl {
    VkCommandPool                      command_pool;
    detail::VKCommandPoolControlBlock* command_pool_header;
    uint32_t                           queue_family_index;
};

struct VKCommandListImpl {
    VkCommandBuffer                              command_buffer;
    impl::VKMainCommandList*                     command_list_table; // local copy of the main command list table for faster access
    mutable detail::VKRootSignatureControlBlock* root_signature_header;
    detail::VKCommandPoolControlBlock*           command_pool_header;

    detail::VKQueueFamilyExtras* queue_indices;
    uint32_t                     maintenance9 : 1;
    WisCommandQueueType          queue_type   : 31;

    mutable uint32_t scratch_memory_size; // Size of the scratch memory in bytes.
    mutable uint8_t* scratch_memory; // Used for temporary allocations that need to be freed when the command list is destroyed.
};

struct VKFenceImpl {
    VkSemaphore                   fence;
    VkDevice                      device;
    detail::VKDeviceControlBlock* device_header;
};

struct VKResourceAllocatorImpl {
    // Using Vulkan Memory Allocator (VMA)
    VmaAllocator                  allocator;
    detail::VKDeviceControlBlock* device_header;
};

struct VKRootSignatureImpl {
    detail::VKRootSignatureControlBlock* root_signature_header;
};

struct VKDescriptorHeapImpl {
    VkBuffer                      buffer;
    VmaAllocation                 allocation;
    void*                         mapped_ptr;
    VkDeviceAddress               gpu_address;
    uint16_t                      descriptor_size;
    uint16_t                      reserved_size; // in descriptor sizes
    uint32_t                      heap_size; // in descriptor sizes
    VkDevice                      device;
    detail::VKDeviceControlBlock* device_header;
};

struct VKBufferImpl {
    VkBuffer                      buffer;
    VmaAllocation                 allocation;
    void*                         mapped_ptr;
    detail::VKDeviceControlBlock* device_header;
};

struct VKTextureImpl {
    VkImage                       image;
    VmaAllocation                 allocation;
    detail::VKDeviceControlBlock* device_header;
};
} // namespace impl
} // namespace wis

// Include implementation if header only build
#if !defined(WISDOM_BUILD_BINARIES)
#if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#error "C++20 is required to build wisdom as header-only library"
#endif // !WIS_HAS_CPP20

#include "vk_impl.cpp"
#include "vk_types.cpp"
#include "vk_device.cpp"
#include "vk_instance.cpp"
#include "vk_adapter_query.cpp"
#include "vk_fence.cpp"
#include "vk_command_queue.cpp"
#include "vk_command_list.cpp"
#include "vk_descriptor_heap.cpp"
#include "vk_resource_allocator.cpp"
#include "vk_command_allocator.cpp"
#endif // WISDOM_BUILD_BINARIES
#endif // WIS_VK_TYPES_HPP
