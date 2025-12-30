#ifndef WIS_VK_TYPES_H
#define WIS_VK_TYPES_H
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus

#include <wisdom/impl/vulkan/vk_extensions.hpp>
#include <wisdom/bridge/span.hpp>
#include <wisdom/generated/cpp_api.hpp>
#include <vk_mem_alloc.h>
#include <cstring>
#include <array>
#include <atomic>
#include <mutex>

namespace wis {
//-----------------------------------------------------------------------------
constexpr inline wis::Result convert_result(WisResult result) noexcept
{
    return { static_cast<wis::Status>(result.status), result.platform_code, result.error };
}

namespace detail {

//-----------------------------------------------------------------------------
struct control_block_base {
public:
    size_t add_ref() noexcept
    {
        // Relaxed memory order is sufficient since this does not impose any ordering on other operations
        return m_ref_cnt.fetch_add(1, std::memory_order_relaxed);
    }

    size_t release() noexcept
    {
        // A release memory order to ensure that all releases are ordered
        return m_ref_cnt.fetch_sub(1, std::memory_order_release);
    }

public:
    std::atomic<size_t> m_ref_cnt{ 1 };
};

//-----------------------------------------------------------------------------
struct empty_type {
};

//-----------------------------------------------------------------------------
template<typename HeaderType>
struct control_block : public control_block_base {
    HeaderType header;
};
template<>
struct control_block<empty_type> : public control_block_base {
};

//-----------------------------------------------------------------------------
struct VKDebugCallbackThunk {
public:
    static VkBool32 VKAPI_PTR wisDebugUtilsMessengerCallbackThunk(
            VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void*                                       pUserData)
    {
        auto* thunk = reinterpret_cast<const VKDebugCallbackThunk*>(pUserData);
        thunk->wisDebugUtilsMessengerCallback(messageSeverity, messageTypes, pCallbackData);
        return false;
    }
    void wisDebugUtilsMessengerCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) const
    {
        WisSeverity wis_severity = WisSeverityInfo;
        switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            wis_severity = WisSeverityVerbose;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            wis_severity = WisSeverityInfo;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            wis_severity = WisSeverityWarning;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            wis_severity = WisSeverityError;
            break;
        default:
            wis_severity = WisSeverityInfo;
            break;
        }

        // Get device handle if possible
        uint64_t device = 0;
        for (auto&& obj : wis::span<const VkDebugUtilsObjectNameInfoEXT>{ pCallbackData->pObjects, pCallbackData->objectCount }) {
            if (obj.objectType == VK_OBJECT_TYPE_DEVICE) {
                device = obj.objectHandle;
                break;
            }
        }

        callback(wis_severity, pCallbackData->pMessage, device, user_data);
    }

public:
    WisDebugCallback callback  = nullptr;
    void*            user_data = nullptr;
};

//-----------------------------------------------------------------------------
struct VKStaticSamplerPoolAllocator {
    static constexpr std::uint32_t static_sampler_chunk_size = 1024;
    static constexpr std::uint32_t static_sampler_max_pools  = 8;

    std::array<VkDescriptorPool, static_sampler_max_pools> pools;

    std::mutex    pool_mutex;
    std::uint32_t pool_count         = 0;
    std::uint32_t current_pool_index = 0;

    // Strategy: 3 steps
    // 1) Try to allocate from current pool
    // 2) If current pool is full, try to find a pool with free space
    // 3) If no pool has free space, create a new pool
    std::pair<VkDescriptorSet, VkDescriptorPool>
    AllocateSet(VkDevice device, impl::VKMainDevice& table, VkDescriptorSetLayout dsl) noexcept;

    void DestroyPoolsUnchecked(VkDevice device, impl::VKMainDevice& table) noexcept
    {
        for (std::uint32_t i = 0; i < pool_count; ++i) {
            table.vkDestroyDescriptorPool(device, pools[i], nullptr);
        }
        pool_count         = 0;
        current_pool_index = 0;
    }
};
} // namespace detail

namespace impl {
//-----------------------------------------------------------------------------
struct VKInstanceHeader {
    VKMainGlobal                global_table;
    VKMainInstance              instance_table;
    VKMainAdapter               adapter_table;
    VkDebugUtilsMessengerEXT    debug_messenger;
    wis::detail::unique_library library;
    uint32_t                    api_version;

    std::unique_ptr<wis::detail::VKDebugCallbackThunk> debug_callback_thunk;
};

struct VKDeviceFeatures {
    bool has_custom_border_color           : 1;
    bool dynamic_rendering                 : 1;
    bool extended_dynamic_state            : 1;
    bool synchronization_2                 : 1;
    bool present_wait                      : 1;
    bool dynamic_render_unused_attachments : 1;
    bool push_descriptor                   : 1;
    bool index_buffer_range                : 1;

    // Properties
    uint32_t max_push_descriptors   = 0;
    uint32_t max_push_constant_size = 0;
};

struct VKDescriptorSetLayoutContainer {
    std::uint32_t         dsl_count            = 0;
    std::uint32_t         static_sampler_count = 0;
    VkDescriptorPool      static_sampler_pool;
    VkDescriptorSetLayout vk_dsls[1];

    // VkSampler samplers[];
};

struct VKDeviceHeader {
    VKMainDevice                             device_table;
    VKMainCommandQueue                       command_queue_table;
    VKMainCommandList                        command_list_table;
    detail::control_block<VKInstanceHeader>* shared_header;
    VkInstance                               instance;

    // Command queue
    std::size_t                                queue_family_count;
    uint8_t*                                   queue_semaphores;
    std::unique_ptr<VkQueueFamilyProperties[]> queue_family_properties;
    uint16_t                                   common_queue_family_indices[WisCommandQueueTypeCount];
    VKDeviceFeatures                           features;

    // Static sampler pool allocator
    detail::VKStaticSamplerPoolAllocator static_sampler_pool_allocator;
};

struct VKInstanceImpl {
    VkInstance                               instance;
    detail::control_block<VKInstanceHeader>* shared_header;
};

struct VKAdapterQueryImpl {
    VkPhysicalDevice*                        physical_devices;
    std::size_t                              adapter_count;
    VkInstance                               instance;
    detail::control_block<VKInstanceHeader>* shared_header;
};
struct VKDeviceImpl {
    VkDevice                               device;
    VkPhysicalDevice                       physical_device;
    detail::control_block<VKDeviceHeader>* device_header;
};
struct VKCommandQueueImpl {
    VkQueue                                queue;
    uint8_t*                               semaphore_ptr;
    VkDevice                               device;
    detail::control_block<VKDeviceHeader>* device_header;
};

struct VKCommandListImpl {
    VkCommandBuffer                        command_buffer;
    VkCommandPool                          command_pool;
    VkDevice                               device;
    detail::control_block<VKDeviceHeader>* device_header;
};

struct VKFenceImpl {
    VkSemaphore                            fence;
    VkDevice                               device;
    detail::control_block<VKDeviceHeader>* device_header;
};

struct VKResourceAllocatorImpl {
    // Using Vulkan Memory Allocator (VMA)
    VmaAllocator                           allocator;
    VkDevice                               device;
    detail::control_block<VKDeviceHeader>* device_header;
};

struct VKPipelineLayoutImpl {
    VkPipelineLayout                       layout;
    VkDescriptorSet                        static_samplers;
    VKDescriptorSetLayoutContainer*        dsl_container;
    VkDevice                               device;
    detail::control_block<VKDeviceHeader>* device_header;
};
} // namespace impl
} // namespace wis

// Include implementation if header only build
#ifndef WISDOM_BUILD_BINARIES
#if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#error "C++20 is required to build wisdom as header-only library"
#endif // !WIS_HAS_CPP20

#include "vk_instance.cpp"
#include "vk_types.cpp"
#endif // WISDOM_BUILD_BINARIES
#endif // DX12_FACTORY_H
