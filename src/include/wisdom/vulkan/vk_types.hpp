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

struct VKCommandListImpl {
    VkCommandBuffer               command_buffer;
    VkCommandPool                 command_pool;
    VkDevice                      device;
    detail::VKDeviceControlBlock* device_header;
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

struct VKPipelineLayoutImpl {
    VkPipelineLayout                        layout;
    VkDescriptorSet                         static_samplers;
    detail::VKDescriptorSetLayoutContainer* dsl_container;
    VkDevice                                device;
    detail::VKDeviceControlBlock*           device_header;
};

struct VKDescriptorHeapImpl {
    VkBuffer                      buffer;
    VmaAllocation                 allocation;
    void*                         mapped_ptr;
    WisDescriptorMemoryType       memory_type;
    VkDevice                      device;
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
#include "vk_descriptor_heap.cpp"
#include "vk_resource_allocator.cpp"
#endif // WISDOM_BUILD_BINARIES
#endif // WIS_VK_TYPES_HPP
