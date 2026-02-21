#ifndef WIS_VK_DETAIL_HPP
#define WIS_VK_DETAIL_HPP
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus

#include <wisdom/vulkan/vk_tables.hpp>
#include <wisdom/generated/c_api.h>
#include <wisdom/bridge/span.hpp>
#include <vk_mem_alloc.h>
#include <atomic>
#include <semaphore>

namespace wis::detail {
//-----------------------------------------------------------------------------
/**
 * @brief A control block structure that manages reference counting for Vulkan objects. This template struct is designed to be used as a base for various Vulkan object headers, providing a common mechanism for reference counting and resource management. The AddRef and Release methods allow for thread-safe incrementing and decrementing of the reference count, ensuring proper lifetime management of Vulkan resources.
 * @tparam HeaderType The type of the header that will be stored in the control block. This allows for flexibility in defining different types of Vulkan object headers while still utilizing the same reference counting mechanism provided by VKControlBlock.
 */
template<typename HeaderType>
struct VKControlBlock {
    size_t AddRef() noexcept
    {
        // Relaxed memory order is sufficient since this does not impose any ordering on other operations
        return m_ref_cnt.fetch_add(1, std::memory_order::relaxed);
    }

    size_t Release() noexcept
    {
        // A release memory order to ensure that all releases are ordered
        return m_ref_cnt.fetch_sub(1, std::memory_order::release);
    }

public:
    std::atomic<size_t> m_ref_cnt{ 1 };
    HeaderType          header;
};

//-----------------------------------------------------------------------------
/**
 * @brief A structure that serves as a thunk for Vulkan debug callbacks. This structure provides a static callback function that can be registered with Vulkan's debug utilities, and it forwards the callback to a user-defined WisDebugCallback. The DebugUtilsMessengerCallbackThunk function is designed to be compatible with Vulkan's expected callback signature, while the DebugUtilsMessengerCallback method allows for processing the debug messages and invoking the user-defined callback with the appropriate severity and message information.
 */
struct VKDebugCallbackThunk {
public:
    static VkBool32 VKAPI_PTR DebugUtilsMessengerCallbackThunk(
            VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void*                                       pUserData)
    {
        auto* thunk = static_cast<const VKDebugCallbackThunk*>(pUserData);
        thunk->DebugUtilsMessengerCallback(messageSeverity, messageTypes, pCallbackData);
        return false;
    }

    /**
     * @brief A method that processes Vulkan debug messages and invokes the user-defined callback with the appropriate severity and message information. This method translates Vulkan's message severity flags into the corresponding WisSeverity values and extracts the device handle from the callback data if available. The user-defined callback is then called with the translated severity, message, device handle, and user data.
     * @param messageSeverity The severity of the debug message, represented as a Vulkan flag.
     * @param messageTypes The type of the debug message, represented as Vulkan flags (unused in this implementation).
     * @param pCallbackData A pointer to a structure containing details about the debug message, including the message string and associated Vulkan objects.
     */
    void DebugUtilsMessengerCallback(
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
struct VKInstanceHeader {
    impl::VKMainGlobal       global_table;
    impl::VKMainInstance     instance_table;
    impl::VKMainAdapter      adapter_table;
    VkDebugUtilsMessengerEXT debug_messenger;
    unique_library           library;
    uint32_t                 api_version;

    std::unique_ptr<VKDebugCallbackThunk> debug_callback_thunk;
};

//-----------------------------------------------------------------------------
struct VKInstanceControlBlock : public VKControlBlock<VKInstanceHeader> {
};

//-----------------------------------------------------------------------------
struct VKDeviceFeatures {
    bool has_custom_border_color           : 1 = false;
    bool dynamic_rendering                 : 1 = false;
    bool extended_dynamic_state            : 1 = false;
    bool synchronization_2                 : 1 = false;
    bool dynamic_render_unused_attachments : 1 = false;
    bool push_descriptor                   : 1 = false;
    bool index_buffer_range                : 1 = false;
    bool descriptor_heap                   : 1 = false;
    bool global_priority                   : 1 = false;

    // Properties
    uint16_t resource_desc_size        = 0;
    uint16_t sampler_desc_size         = 0;
    uint32_t descriptor_heap_alignment = 0;
    uint32_t sampler_heap_alignment    = 0;
    uint32_t min_descriptor_heap_size  = 0;
    uint32_t min_sampler_heap_size     = 0;
    uint64_t max_descriptor_heap_size  = 0;
    uint64_t max_sampler_heap_size     = 0;
};

//-----------------------------------------------------------------------------
struct VKQueueFamilyProperties {
    static constexpr uint8_t invalid_family_index = 0xFF;
    uint8_t                  family_index         = invalid_family_index;
    uint8_t                  queue_priority       = WisCommandQueuePriorityNormal;
    uint8_t                  queue_count          = 0;
    std::atomic<uint8_t>     current_index{ 0 };
    uint32_t                 semaphore_offset = 0;

public:
    uint8_t GetNextQueueIndex() noexcept
    {
        // Atomically get the next queue index in a round-robin fashion
        uint8_t index = current_index.fetch_add(1, std::memory_order_relaxed);
        return index % queue_count;
    }
};

//-----------------------------------------------------------------------------
struct VKDeviceHeader {
    impl::VKMainDevice       device_table;
    impl::VKMainCommandQueue command_queue_table;
    impl::VKMainCommandList  command_list_table;
    VKInstanceControlBlock*  shared_header;
    VkInstance               instance;
    VmaAllocator             allocator;

    // Enabled features
    VKDeviceFeatures features;

    // Queue family indices for each command queue type
    std::array<uint8_t, WisCommandQueueTypeCount>                 queue_residency{};
    std::array<VKQueueFamilyProperties, WisCommandQueueTypeCount> queue_families{};
    uint32_t                                                      family_count = 0;

    // store semaphores

public:
    ~VKDeviceHeader()
    {
        if (family_count == 0) {
            return; // No queues, no semaphores to destroy
        }

        // Destroy semaphores
        auto&                  last_family = queue_families[family_count - 1];
        std::binary_semaphore* begin       = reinterpret_cast<std::binary_semaphore*>(reinterpret_cast<uint8_t*>(this) + sizeof(*this));
        std::binary_semaphore* end         = last_family.semaphore_offset + last_family.queue_count + begin;
        for (std::binary_semaphore* sem = begin; sem < end; ++sem) {
            sem->release();
            if constexpr (!std::is_trivially_destructible_v<std::binary_semaphore>) {
                std::destroy_at(sem);
            }
        }
    }

public:
    std::binary_semaphore* GetSemaphoreForQueueType(WisCommandQueueType type, uint32_t queue_index) noexcept
    {
        if (type >= WisCommandQueueTypeCount) {
            return nullptr; // Invalid queue type
        }
        uint8_t family_index = queue_families[type].family_index;
        if (family_index == VKQueueFamilyProperties::invalid_family_index) {
            return nullptr; // No valid family index for this queue type
        }
        return reinterpret_cast<std::binary_semaphore*>(reinterpret_cast<uint8_t*>(this) + sizeof(*this)) + queue_families[type].semaphore_offset + queue_index;
    }
};

//-----------------------------------------------------------------------------
struct VKDeviceControlBlock : public VKControlBlock<VKDeviceHeader> {
};

//-----------------------------------------------------------------------------
/**
 * @brief Releases a Vulkan instance, destroying it if this is the last reference. Also destroys the debug messenger if it exists.
 * @param instance The Vulkan instance to release
 * @param header The control block header associated with the instance, which holds the reference count and
 * the debug messenger handle
 */
inline void release_vk_instance(VkInstance instance, VKInstanceControlBlock* header) noexcept
{
    if (header && header->Release() == 1) {
        // Destroy debug messenger if exists
        if (header->header.debug_messenger != VK_NULL_HANDLE &&
            header->header.instance_table.vkDestroyDebugUtilsMessengerEXT) {
            header->header.instance_table.vkDestroyDebugUtilsMessengerEXT(
                    instance,
                    header->header.debug_messenger,
                    nullptr);
        }

        // Last reference, destroy instance
        std::atomic_thread_fence(std::memory_order_acquire);
        header->header.instance_table.vkDestroyInstance(instance, nullptr);
        delete header;
    }
}

//-----------------------------------------------------------------------------
/**
 * @brief Releases a Vulkan device, destroying it if this is the last reference. Also releases the associated instance.
 * @param device The Vulkan device to release
 * @param header The control block header associated with the device, which holds the reference count and a pointer to the instance control block header
 */
inline void release_vk_device(VkDevice device, VKDeviceControlBlock* header) noexcept
{
    if (header && header->Release() == 1) {
        // Last reference, destroy device
        std::atomic_thread_fence(std::memory_order_acquire);

        // Destroy allocator
        vmaDestroyAllocator(header->header.allocator);

        header->header.device_table.vkDestroyDevice(device, nullptr);

        // Destroy instance
        release_vk_instance(header->header.instance,
                            header->header.shared_header);

        delete header;
    }
}
} // namespace wis::detail

#endif // WIS_VK_DETAIL_HPP