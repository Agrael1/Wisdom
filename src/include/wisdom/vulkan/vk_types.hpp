#ifndef WIS_VK_TYPES_HPP
#define WIS_VK_TYPES_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

#include <wisdom/generated/c_api.h>
#include <wisdom/vulkan/vk_tables.hpp>

#include <vk_mem_alloc.h>

#include <array>
#include <cstring>

namespace wis {
struct VKInstanceExtensionCollector;
struct VKDeviceExtensionCollector;
//----------------------------------------------------------------------------------------------------------------------
namespace detail {
struct VKInstanceControlBlock;
struct VKDeviceControlBlock;
struct VKDescriptorSetLayoutContainer;
struct VKCommandPoolControlBlock;
struct VKRootSignatureControlBlock;
struct VKSurfaceControlBlock;
struct VKQueueFamilyExtras;
struct VKSwapchainControlBlock;
struct VKRenderTargetView;
} // namespace detail

namespace impl {
//----------------------------------------------------------------------------------------------------------------------
struct VKInstanceImpl {
    VkInstance instance;
    detail::VKInstanceControlBlock* shared_header;
};

struct VKAdapterQueryImpl {
    VkPhysicalDevice* physical_devices;
    std::size_t adapter_count;
    VkInstance instance;
    detail::VKInstanceControlBlock* shared_header;
};

struct VKDeviceImpl {
    VkDevice device;
    VkPhysicalDevice physical_device;
    detail::VKDeviceControlBlock* device_header;
};

struct VKCommandQueueImpl {
    VkQueue queue;
    uint8_t* semaphore_ptr;
    VkDevice device;
    detail::VKDeviceControlBlock* device_header;
};

struct VKCommandAllocatorImpl {
    VkCommandPool command_pool;
    detail::VKCommandPoolControlBlock* command_pool_header;
    uint32_t queue_family_index;
};

struct VKCommandListImpl {
    VkCommandBuffer command_buffer;
    impl::VKMainCommandList* command_list_table; // local copy of the main command list table for faster access
    mutable detail::VKRootSignatureControlBlock* root_signature_header;
    detail::VKCommandPoolControlBlock* command_pool_header;

    detail::VKQueueFamilyExtras* queue_indices;
    uint32_t maintenance9          : 1;
    WisCommandQueueType queue_type : 31;

    mutable uint32_t scratch_memory_size; // Size of the scratch memory in bytes.

    // Used for temporary allocations that need to be freed when the command list is destroyed.
    mutable uint8_t* scratch_memory;
};

struct VKFenceImpl {
    VkSemaphore fence;
    VkDevice device;
    detail::VKDeviceControlBlock* device_header;
};

struct VKResourceAllocatorImpl {
    // Using Vulkan Memory Allocator (VMA)
    VmaAllocator allocator;
    detail::VKDeviceControlBlock* device_header;
};

struct VKRootSignatureImpl {
    detail::VKRootSignatureControlBlock* root_signature_header;
};

struct VKDescriptorHeapImpl {
    VkBuffer buffer;
    VmaAllocation allocation;
    void* mapped_ptr;
    VkDeviceAddress gpu_address;
    uint16_t descriptor_size;
    uint16_t reserved_size; // in descriptor sizes
    uint32_t heap_size; // in descriptor sizes
    VkDevice device;
    detail::VKDeviceControlBlock* device_header;
};

struct VKViewHeapImpl {
    detail::VKRenderTargetView* view_heap;
    uint32_t capacity;
    detail::VKDeviceControlBlock* device_header;
};

struct VKBufferImpl {
    VkBuffer buffer;
    VmaAllocation allocation;
    void* mapped_ptr;
    detail::VKDeviceControlBlock* device_header;
};

struct VKTextureImpl {
    VkImage image;
    VmaAllocation allocation;
    detail::VKDeviceControlBlock* device_header;
    uint16_t width;
    uint16_t height;
    uint16_t depth_or_array_size;
    bool owned_by_swapchain;
};

struct VKPipelineCacheImpl {
    VkPipelineCache cache;
    detail::VKDeviceControlBlock* device_header;
};

struct VKShaderImpl {
    VkShaderModule shader_module;
    detail::VKDeviceControlBlock* device_header;
};

struct VKPipelineImpl {
    VkPipeline pipeline;
    detail::VKDeviceControlBlock* device_header;
};

struct VKSurfaceImpl {
    VkSurfaceKHR surface;
    detail::VKSurfaceControlBlock* surface_header;
};

struct VKSwapchainImpl {
    mutable VkSwapchainKHR swapchain;
    detail::VKSwapchainControlBlock* swapchain_header;
    impl::VKMainSwapchain* swapchain_table;
    VkQueue present_queue; // store a copy of the present queue handle for faster access during presentation
    VkDevice device;
    VkFence destroy_fence; // Fence used to synchronize swapchain destruction.
    mutable uint32_t present_index;
    mutable uint32_t acquire_index;
    // If true, the next call to GetCurrentIndex will acquire the next image from the swapchain.
    mutable bool lazy_acquire;
};

} // namespace impl

//----------------------------------------------------------------------------------------------------------------------
struct VKInstanceExtensionHeader {
    WisResult (*init_fptr)(
        VKInstanceExtensionHeader* self,
        impl::VKInstanceImpl* instance_impl,
        VKInstanceExtensionCollector* collector
    ) noexcept;
};
struct VKDeviceExtensionHeader {
    WisResult (*init_fptr)(
        VKDeviceExtensionHeader* self,
        impl::VKDeviceImpl* device_impl,
        VKDeviceExtensionCollector* collector
    ) noexcept;
};
} // namespace wis

// Include implementation for header-only mode
#if defined(WISDOM_HEADER_ONLY)
#    if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#        error "C++20 is required to build wisdom as header-only library"
#    endif // !WIS_HAS_CPP20

#    include "vk_adapter_query.cpp"
#    include "vk_command_allocator.cpp"
#    include "vk_command_list.cpp"
#    include "vk_command_queue.cpp"
#    include "vk_descriptor_heap.cpp"
#    include "vk_device.cpp"
#    include "vk_extensions.cpp"
#    include "vk_fence.cpp"
#    include "vk_impl.cpp"
#    include "vk_instance.cpp"
#    include "vk_pipeline_cache.cpp"
#    include "vk_resource_allocator.cpp"
#endif // WISDOM_HEADER_ONLY
#endif // WIS_VK_TYPES_HPP
