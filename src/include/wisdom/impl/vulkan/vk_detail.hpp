#ifndef WIS_VK_DETAIL_H
#define WIS_VK_DETAIL_H
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus

#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/impl/vulkan/vk_tables.hpp>
#include <wisdom/bridge/span.hpp>
#include <atomic>
#include <mutex>

namespace wis {
namespace detail {

//-----------------------------------------------------------------------------
template<typename HeaderType>
struct control_block {
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
    HeaderType          header;
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
        (void)messageTypes; // Unused
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
    AllocateSet(VkDevice device, const impl::VKMainDevice& table, VkDescriptorSetLayout dsl) noexcept;

    void DestroyPoolsUnchecked(VkDevice device, const impl::VKMainDevice& table) noexcept
    {
        for (std::uint32_t i = 0; i < pool_count; ++i) {
            table.vkDestroyDescriptorPool(device, pools[i], nullptr);
        }
        pool_count         = 0;
        current_pool_index = 0;
    }
};

struct VKInstanceHeader {
    impl::VKMainGlobal          global_table;
    impl::VKMainInstance        instance_table;
    impl::VKMainAdapter         adapter_table;
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
    bool descriptor_buffer                 : 1;
    bool mutable_descriptor_type           : 1;

    // Properties
    uint32_t max_push_descriptors      = 0;
    uint32_t max_push_constant_size    = 0;
    uint32_t max_bound_descriptor_sets = 0;
    uint32_t max_descriptors_in_set    = 0;
    uint32_t max_samplers_in_set       = 0;

    // Descriptor buffer properties
    uint16_t constant_buffer_descriptor_size        = 0;
    uint16_t storage_buffer_descriptor_size         = 0;
    uint16_t sampled_image_descriptor_size          = 0;
    uint16_t storage_image_descriptor_size          = 0;
    uint16_t acceleration_structure_descriptor_size = 0;

    uint16_t mutable_descriptor_size    = 0;
    uint16_t sampler_descriptor_size    = 0;
    uint16_t descriptor_table_alignment = 0;

    WisDescriptorStorageTier max_descriptor_storage_tier = WisDescriptorStorageTierTier1;
};

struct VKDescriptorSetLayoutContainer {
    std::uint32_t    dsl_count            = 0;
    std::uint32_t    static_sampler_count = 0;
    VkDescriptorPool static_sampler_pool;

    // Followed by VkDescriptorSetLayout[dsl_count]
    wis::span<VkDescriptorSetLayout> vk_dsls() noexcept
    {
        return { reinterpret_cast<VkDescriptorSetLayout*>(this + 1), dsl_count };
    }

    // Followed by VkSampler[static_sampler_count]
    wis::span<VkSampler> vk_static_samplers() noexcept
    {
        return { reinterpret_cast<VkSampler*>(reinterpret_cast<std::uint8_t*>(this + 1) + dsl_count * sizeof(VkDescriptorSetLayout)),
                 static_sampler_count };
    }

    // destroy helpers
    void destroy_static_samplers(VkDevice device, const impl::VKMainDevice& table) noexcept
    {
        wis::span<VkSampler> static_samplers_span = vk_static_samplers();
        for (std::uint32_t i = 0; i < static_sampler_count; i++) {
            if (static_samplers_span[i] != VK_NULL_HANDLE) {
                table.vkDestroySampler(device, static_samplers_span[i], nullptr);
                static_samplers_span[i] = VK_NULL_HANDLE;
            }
        }
    }

    void destroy_descriptor_set_layouts(VkDevice device, const impl::VKMainDevice& table) noexcept
    {
        wis::span<VkDescriptorSetLayout> dsl_span     = vk_dsls();
        VkDescriptorSetLayout            previous_dsl = VK_NULL_HANDLE;
        for (std::uint32_t i = 0; i < dsl_count; ++i) {
            if (dsl_span[i] != VK_NULL_HANDLE && dsl_span[i] != previous_dsl) {
                table.vkDestroyDescriptorSetLayout(device, dsl_span[i], nullptr);
                previous_dsl = dsl_span[i];
                dsl_span[i]  = VK_NULL_HANDLE;
            }
        }
    }

    void free_static_sampler_set(VkDevice device, const impl::VKMainDevice& table, VkDescriptorSet static_sampler_set) noexcept
    {
        if (static_sampler_set != VK_NULL_HANDLE) {
            table.vkFreeDescriptorSets(
                    device,
                    static_sampler_pool,
                    1,
                    &static_sampler_set);
        }
    }

    void destroy(VkDevice device, const impl::VKMainDevice& table, VkDescriptorSet static_sampler_set) noexcept
    {
        free_static_sampler_set(device, table, static_sampler_set);
        destroy_static_samplers(device, table);
        destroy_descriptor_set_layouts(device, table);
    }
};

struct VKDeviceHeader {
    impl::VKMainDevice                       device_table;
    impl::VKMainCommandQueue                 command_queue_table;
    impl::VKMainCommandList                  command_list_table;
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

} // namespace detail
} // namespace wis

#endif // WIS_VK_DETAIL_H