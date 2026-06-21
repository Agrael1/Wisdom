#ifndef WIS_VK_DETAIL_HPP
#define WIS_VK_DETAIL_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

#include <wisdom/bridge/span.hpp>
#include <wisdom/generated/c_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/vk_tables.hpp>

#include <vk_mem_alloc.h>

#include <array>
#include <atomic>
#include <bit>
#include <semaphore>
#include <algorithm>
#include <utility>

namespace wis::impl {
struct VKSwapchainImpl;
}

namespace wis::detail {
template <typename HandleType, typename F>
struct VKScopeGuard {
    HandleType handle;
    F f;

    VKScopeGuard(HandleType handle, F&& f) noexcept
        : handle(handle)
        , f(std::forward<F>(f))
    {}
    ~VKScopeGuard() noexcept
    {
        if (handle) {
            f();
        }
    }

    // Prevent copying
    VKScopeGuard(const VKScopeGuard&) = delete;
    VKScopeGuard& operator=(const VKScopeGuard&) = delete;

    HandleType* PutUnchecked() noexcept { return &handle; }
    HandleType Release() noexcept { return std::exchange(handle, nullptr); }
};

template <typename HandleType, typename F>
VKScopeGuard<HandleType, F> VKMakeScopeGuard(HandleType handle, F&& f) noexcept
{
    return VKScopeGuard<HandleType, F>(handle, std::forward<F>(f));
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief A control block structure that manages reference counting for Vulkan objects. This template struct is designed
 * to be used as a base for various Vulkan object headers, providing a common mechanism for reference counting and
 * resource management. The AddRef and Release methods allow for thread-safe incrementing and decrementing of the
 * reference count, ensuring proper lifetime management of Vulkan resources.
 * @tparam HeaderType The type of the header that will be stored in the control block. This allows for flexibility in
 * defining different types of Vulkan object headers while still utilizing the same reference counting mechanism
 * provided by VKControlBlock.
 */
template <typename HeaderType>
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
    std::atomic<size_t> m_ref_cnt{1};
    HeaderType header;
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief A structure that serves as a thunk for Vulkan debug callbacks. This structure provides a static callback
 * function that can be registered with Vulkan's debug utilities, and it forwards the callback to a user-defined
 * WisDebugCallback. The DebugUtilsMessengerCallbackThunk function is designed to be compatible with Vulkan's expected
 * callback signature, while the DebugUtilsMessengerCallback method allows for processing the debug messages and
 * invoking the user-defined callback with the appropriate severity and message information.
 */
struct VKDebugCallbackThunk {
public:
    static VkBool32 VKAPI_PTR DebugUtilsMessengerCallbackThunk(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    )
    {
        auto* thunk = static_cast<const VKDebugCallbackThunk*>(pUserData);
        thunk->DebugUtilsMessengerCallback(messageSeverity, messageTypes, pCallbackData);
        return false;
    }

    /**
     * @brief A method that processes Vulkan debug messages and invokes the user-defined callback with the appropriate
     * severity and message information. This method translates Vulkan's message severity flags into the corresponding
     * WisSeverity values and extracts the device handle from the callback data if available. The user-defined callback
     * is then called with the translated severity, message, device handle, and user data.
     * @param messageSeverity The severity of the debug message, represented as a Vulkan flag.
     * @param messageTypes The type of the debug message, represented as Vulkan flags (unused in this implementation).
     * @param pCallbackData A pointer to a structure containing details about the debug message, including the message
     * string and associated Vulkan objects.
     */
    void DebugUtilsMessengerCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData
    ) const
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
        for (auto&& obj :
             wis::span<const VkDebugUtilsObjectNameInfoEXT>{pCallbackData->pObjects, pCallbackData->objectCount}) {
            if (obj.objectType == VK_OBJECT_TYPE_DEVICE) {
                device = obj.objectHandle;
                break;
            }
        }

        callback(wis_severity, pCallbackData->pMessage, device, user_data);
    }

public:
    WisDebugCallback callback = nullptr;
    void* user_data = nullptr;
};

//----------------------------------------------------------------------------------------------------------------------
struct VKInstanceHeader {
    impl::VKMainGlobal global_table;
    impl::VKMainInstance instance_table;
    impl::VKMainAdapter adapter_table;
    VkDebugUtilsMessengerEXT debug_messenger;
    unique_library library;
    uint32_t api_version;
    VkInstance instance;

    std::unique_ptr<VKDebugCallbackThunk> debug_callback_thunk;
};

//----------------------------------------------------------------------------------------------------------------------
struct VKInstanceControlBlock : public VKControlBlock<VKInstanceHeader> {};

//----------------------------------------------------------------------------------------------------------------------
struct VKDeviceFeatures {
    uint32_t multiple_viewports                : 1 = false;
    uint32_t dynamic_render_unused_attachments : 1 = false;
    uint32_t index_buffer_range                : 1 = false;
    uint32_t descriptor_heap                   : 1 = false;
    uint32_t global_priority                   : 1 = false;
    uint32_t host_image_copy                   : 1 = false;
    uint32_t maintenance9                      : 1 = false; // nop QFOT barriers and empty device
    uint32_t line_rasterization                : 1 = false;
    uint32_t conservative_rasterization        : 1 = false;
    uint32_t memory_priority                   : 1 = false;
    uint32_t dynamic_memory_priority           : 1 = false;
    uint32_t address_commands                  : 1 = false;

    // Swapchain
    uint32_t swapchain_maintenance : 1 = false;
    uint32_t incremental_present   : 1 = false;

    // Properties
    uint8_t max_vertex_attributes = 0; // rarely greater than 32, so 8 bits is sufficient
    uint8_t max_vertex_bindings = 0;
    uint16_t resource_desc_size = 0;
    uint16_t sampler_desc_size = 0;
    uint16_t max_root_space = 0;

    // WisImageLayout. A bit value of 1 indicates support for the
    // transition.
    uint32_t descriptor_heap_reserved_size = 0;
    uint32_t sampler_heap_reserved_size = 0;
    uint32_t sampler_heap_reserved_size_with_embedded = 0;
    uint32_t descriptor_heap_alignment = 0;
    uint32_t sampler_heap_alignment = 0;
    uint64_t max_descriptor_heap_size = 0;
    uint64_t max_sampler_heap_size = 0;
};

//----------------------------------------------------------------------------------------------------------------------
struct VKQueueFamilyProperties {
    static constexpr uint8_t invalid_family_index = 0xFF;
    uint8_t family_index = invalid_family_index;
    uint8_t queue_priority = WisCommandQueuePriorityNormal;
    uint8_t queue_count = 0;
    std::atomic<uint8_t> current_index{0};
    uint32_t semaphore_offset = 0;

public:
    uint8_t GetNextQueueIndex() noexcept
    {
        // Atomically get the next queue index in a round-robin fashion
        uint8_t index = current_index.fetch_add(1, std::memory_order_relaxed);
        return index % queue_count;
    }
};

//----------------------------------------------------------------------------------------------------------------------
struct VKQueueFamilyExtras {
    static constexpr uint8_t invalid_family_index = 0xFF;
    uint8_t family_index = invalid_family_index;
    uint32_t compatible_to_families = 0; // Bitmask of compatible queue families for relaxed transitions, indexed by
    // family index. A bit value of 1 indicates compatibility.
};

//----------------------------------------------------------------------------------------------------------------------
struct VKDeviceHeader {
    impl::VKMainDevice device_table;
    impl::VKMainCommandQueue command_queue_table;
    impl::VKMainCommandList command_list_table;
    impl::VKMainSwapchain swapchain_table;
    VkDevice device;
    VKInstanceControlBlock* shared_header;
    VkInstance instance;
    VmaAllocator allocator;

    // Enabled features
    VKDeviceFeatures features;

    // Queue family indices for each command queue type
    std::array<VKQueueFamilyExtras, WisCommandQueueTypeCount> queue_family_extras{}; // Used in other parts of code
    std::array<uint8_t, WisCommandQueueTypeCount> queue_residency{};
    std::array<VKQueueFamilyProperties, WisCommandQueueTypeCount> queue_families{};
    uint32_t family_count = 0;

    // store semaphores

public:
    ~VKDeviceHeader()
    {
        if (family_count == 0) {
            return; // No queues, no semaphores to destroy
        }

        // Destroy semaphores
        auto& last_family = queue_families[family_count - 1];
        std::binary_semaphore* begin = reinterpret_cast<std::binary_semaphore*>(
            reinterpret_cast<uint8_t*>(this) + sizeof(*this)
        );
        std::binary_semaphore* end = last_family.semaphore_offset + last_family.queue_count + begin;
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
        return reinterpret_cast<std::binary_semaphore*>(reinterpret_cast<uint8_t*>(this) + sizeof(*this))
             + queue_families[type].semaphore_offset + queue_index;
    }
};

//----------------------------------------------------------------------------------------------------------------------
struct VKDeviceControlBlock : public VKControlBlock<VKDeviceHeader> {};

struct VKCommandPoolHeader {
    VkDevice device;
    VKDeviceControlBlock* device_header;
    VkCommandPool command_pool;
};

//----------------------------------------------------------------------------------------------------------------------
struct VKCommandPoolControlBlock : public VKControlBlock<VKCommandPoolHeader> {};

struct VKSurfaceHeader {
    VKInstanceControlBlock* instance_header;
    VkSurfaceKHR surface;
};

//----------------------------------------------------------------------------------------------------------------------
struct VKSurfaceControlBlock : public VKControlBlock<VKSurfaceHeader> {};

//----------------------------------------------------------------------------------------------------------------------
struct VKSwapchainHeader {
    static constexpr uint32_t reasonable_mode_count = 16;

    VKSurfaceControlBlock* surface_header; // hold reference to surface control block to ensure surface lifetime
    VKDeviceControlBlock* device_header; // hold reference to device control block to ensure device lifetime
    VkSurfaceKHR surface; // store surface handle for later use in presentation and swapchain recreation
    VkPhysicalDevice physical_device; // store physical device for later use in swapchain recreation
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR
        vkGetPhysicalDeviceSurfaceCapabilities2KHR; // store function pointer for later use in swapchain recreation

    VkSwapchainCreateInfoKHR create_info; // store create info for later use in presentation and swapchain recreation
    VkSwapchainPresentScalingCreateInfoKHR
        scaling_create_info; // store scaling create info for later use in presentation and swapchain recreation

    VkPresentModeKHR modes[reasonable_mode_count];
    uint8_t mode_count;
    uint8_t format_count;
    bool tearing;

    wis::span<const VkSemaphore> GetImageAvailableSemaphores() const noexcept
    {
        return wis::span<const VkSemaphore>{reinterpret_cast<const VkSemaphore*>(this + 1), create_info.minImageCount};
    }
    wis::span<const VkSemaphore> GetRenderFinishedSemaphores() const noexcept
    {
        return wis::span<const VkSemaphore>{
            reinterpret_cast<const VkSemaphore*>(this + 1) + create_info.minImageCount,
            create_info.minImageCount
        };
    }
    wis::span<const VkSemaphore> GetSemaphores() const noexcept
    {
        return wis::span<const VkSemaphore>{
            reinterpret_cast<const VkSemaphore*>(this + 1),
            create_info.minImageCount * 2
        };
    }
    wis::span<const VkPresentModeKHR> GetSupportedPresentModes() const noexcept
    {
        return wis::span<const VkPresentModeKHR>{modes, mode_count};
    }
    wis::span<VkSurfaceFormatKHR> GetSupportedFormats() noexcept
    {
        return wis::span<VkSurfaceFormatKHR>{
            reinterpret_cast<VkSurfaceFormatKHR*>(this + 1) + create_info.minImageCount * 2,
            format_count
        };
    }
};

//----------------------------------------------------------------------------------------------------------------------
struct VKSwapchainControlBlock : public VKControlBlock<VKSwapchainHeader> {};

//----------------------------------------------------------------------------------------------------------------------
struct alignas(void*) VKRootSignatureControlBlock {
    static constexpr uint32_t invalid_index = std::numeric_limits<uint32_t>::max();

    constexpr static std::array<uint32_t, WisShaderVisibilityCount> FillInvalid()
    {
        std::array<uint32_t, WisShaderVisibilityCount> arr{};
        std::fill(arr.begin(), arr.end(), invalid_index);
        return arr;
    }
    uint32_t constant_data_size = 0; // must be aligned to 8 bytes
    uint32_t mapping_count = 0;
    uint32_t embedded_sampler_count = 0;
    uint32_t root_parameter_count = 0;

    // offset from the start of the control block to the shader visibility mapping for each shader stage, or UINT32_MAX
    // if not used

    struct alignas(void*) {
        std::array<uint32_t, WisShaderVisibilityCount> shader_mapping_offset = FillInvalid();
        std::array<uint32_t, WisShaderVisibilityCount> shader_mapping_sizes{};
    };

    // aligned to 8 bytes, immediately followed by binding data and then mapping data

    wis::span<const uint32_t> GetRootBindingOffsets() const noexcept
    {
        return wis::span<const uint32_t>{reinterpret_cast<const uint32_t*>(this + 1), root_parameter_count};
    }

    wis::span<uint32_t> GetRootBindingOffsets() noexcept
    {
        return wis::span<uint32_t>{reinterpret_cast<uint32_t*>(this + 1), root_parameter_count};
    }

    wis::span<VkDescriptorSetAndBindingMappingEXT> GetMappings() noexcept
    {
        return wis::span<VkDescriptorSetAndBindingMappingEXT>{
            reinterpret_cast<VkDescriptorSetAndBindingMappingEXT*>(GetRootBindingOffsets().end()),
            mapping_count
        };
    }
    wis::span<const VkDescriptorSetAndBindingMappingEXT> GetMappings() const noexcept
    {
        return wis::span<const VkDescriptorSetAndBindingMappingEXT>{
            reinterpret_cast<const VkDescriptorSetAndBindingMappingEXT*>(GetRootBindingOffsets().end()),
            mapping_count
        };
    }
};

//----------------------------------------------------------------------------------------------------------------------
struct VKRenderTargetView {
    VkImageView view = VK_NULL_HANDLE;
    uint16_t width = 0, height = 0;
    uint16_t array_layer_count = 0;
};

//----------------------------------------------------------------------------------------------------------------------
inline constexpr WisTextureState VKConvertToTextureState(VkImageLayout layout) noexcept
{
    switch (layout) {
    default:
    case VK_IMAGE_LAYOUT_UNDEFINED:
        return WisTextureStateUndefined;
    case VK_IMAGE_LAYOUT_GENERAL:
        return WisTextureStateCommon;
    case VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL:
        return WisTextureStateRead;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        return WisTextureStateRenderTarget;
    case VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL:
        return WisTextureStateUnorderedAccess;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        return WisTextureStateDepthStencilWrite;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
        return WisTextureStateDepthStencilRead;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        return WisTextureStateShaderResource;
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        return WisTextureStateCopySrc;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        return WisTextureStateCopyDst;
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
        return WisTextureStatePresent;
    case VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR:
        return WisTextureStateShadingRate;
    case VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR:
        return WisTextureStateVideoDecodeRead;
    case VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR:
        return WisTextureStateVideoDecodeWrite;
    case VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR:
        return WisTextureStateVideoDecodeDPB;
    }
}

//----------------------------------------------------------------------------------------------------------------------
inline constexpr VkImageAspectFlags VKAspectFlags(VkFormat format) noexcept
{
    switch (format) {
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D16_UNORM_S8_UINT:
        return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_D32_SFLOAT:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
        return VK_IMAGE_ASPECT_DEPTH_BIT;
    case VK_FORMAT_S8_UINT:
        return VK_IMAGE_ASPECT_STENCIL_BIT;
    default:
        return VK_IMAGE_ASPECT_COLOR_BIT;
    }
}

//----------------------------------------------------------------------------------------------------------------------
inline constexpr VkImageAspectFlags VKExtractAspectFlags(
    WisSubresourceRange subresource,
    WisBarrierFlags flags
) noexcept
{
    VkImageAspectFlags aspect_flags = 0;
    if (flags & WisBarrierFlagsDepthResource) {
        aspect_flags |= VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    if (flags & WisBarrierFlagsStencilResource) {
        aspect_flags |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    if (aspect_flags != 0) {
        // If depth or stencil specified, ignore plane slice and return early
        // since depth/stencil views of multi-planar formats are not allowed to have a plane slice.
        return aspect_flags;
    }

    if ((flags & WisBarrierFlagsPlanarImage) == 0) {
        return VK_IMAGE_ASPECT_COLOR_BIT; // If not a planar image, return color aspect for simplicity.
    }

    for (uint16_t plane = subresource.plane_slice; plane < subresource.plane_slice_count; ++plane) {
        aspect_flags |= VK_IMAGE_ASPECT_PLANE_0_BIT << plane;
    }
    return aspect_flags;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Releases a Vulkan instance, destroying it if this is the last reference. Also destroys the debug messenger if
 * it exists.
 * @param instance The Vulkan instance to release
 * @param header The control block header associated with the instance, which holds the reference count and
 * the debug messenger handle
 */
inline void VKReleaseInstance(VKInstanceControlBlock* header) noexcept
{
    if (header && header->Release() == 1) {
        auto& head = header->header;
        // Destroy debug messenger if exists
        if (head.debug_messenger != VK_NULL_HANDLE && head.instance_table.vkDestroyDebugUtilsMessengerEXT) {
            head.instance_table.vkDestroyDebugUtilsMessengerEXT(head.instance, header->header.debug_messenger, nullptr);
        }

        // Last reference, destroy instance
        std::atomic_thread_fence(std::memory_order_acquire);
        header->header.instance_table.vkDestroyInstance(head.instance, nullptr);
        delete header;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Releases a Vulkan device, destroying it if this is the last reference. Also releases the associated instance.
 * @param device The Vulkan device to release
 * @param header The control block header associated with the device, which holds the reference count and a pointer to
 * the instance control block header
 */
inline void VKReleaseDevice(VKDeviceControlBlock* header) noexcept
{
    if (header && header->Release() == 1) {
        // Last reference, destroy device
        std::atomic_thread_fence(std::memory_order_acquire);

        // Destroy allocator
        vmaDestroyAllocator(header->header.allocator);

        header->header.device_table.vkDestroyDevice(header->header.device, nullptr);

        // Destroy instance
        VKReleaseInstance(header->header.shared_header);

        ::operator delete(header);
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Releases a Vulkan command pool, destroying it if this is the last reference. Also releases the associated
 * device.
 * @param command_pool The Vulkan command pool to release
 * @param header The control block header associated with the command pool, which holds the reference count and a
 * pointer to the device control block header
 */
inline void VKReleaseCommandPool(VKCommandPoolControlBlock* header) noexcept
{
    if (header && header->Release() == 1) {
        // Last reference, destroy command pool
        std::atomic_thread_fence(std::memory_order_acquire);

        // Destroy command pool
        auto& table = header->header.device_header->header.device_table;
        table.vkDestroyCommandPool(header->header.device, header->header.command_pool, nullptr);

        VKReleaseDevice(header->header.device_header);
        delete[] header;
    }
}
//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Releases a Vulkan command pool, destroying it if this is the last reference. Also releases the associated
 * device.
 * @param command_pool The Vulkan command pool to release
 * @param header The control block header associated with the command pool, which holds the reference count and a
 * pointer to the device control block header
 */
inline void VKReleaseSurface(VKSurfaceControlBlock* header) noexcept
{
    if (header && header->Release() == 1) {
        // Last reference, destroy command pool
        std::atomic_thread_fence(std::memory_order_acquire);

        // Destroy command pool
        auto& table = header->header.instance_header->header.instance_table;
        table.vkDestroySurfaceKHR(header->header.instance_header->header.instance, header->header.surface, nullptr);

        VKReleaseInstance(header->header.instance_header);
        delete[] header;
    }
}

//----------------------------------------------------------------------------------------------------------------------
inline void VKReleaseSwapchain(VkSwapchainKHR swap, VKSwapchainControlBlock* header) noexcept
{
    if (header && header->Release() == 1) {
        // Last reference, destroy swapchain
        std::atomic_thread_fence(std::memory_order_acquire);
        VKReleaseDevice(header->header.device_header);
        VKReleaseSurface(header->header.surface_header);
        ::operator delete(header);
    }
}

//----------------------------------------------------------------------------------------------------------------------
// Barrier helper constants
constexpr static uint32_t vk_max_barrier_size = std::max(
    {sizeof(VkBufferMemoryBarrier), sizeof(VkImageMemoryBarrier2), sizeof(VkMemoryBarrier2)}
);
constexpr static uint32_t vk_static_barrier_size = WIS_TRANSIENT_MAX_BARRIER_COUNT * vk_max_barrier_size;

template <typename Impl>
inline uint8_t* VKAllocateScratchSpace(const Impl& impl, uint32_t new_size)
{
    if (new_size > impl.scratch_memory_size) {
        delete[] impl.scratch_memory;
        impl.scratch_memory = new (std::nothrow) uint8_t[new_size];
        impl.scratch_memory_size = impl.scratch_memory ? new_size : 0;
    }
    return impl.scratch_memory;
}

template <typename Impl>
inline std::array<wis::span<uint8_t>, 3> VKAllocateBarriers(
    const Impl& impl,
    uint8_t* local_scratch,
    const WisVKBarrierGroup& barriers
)
{
    std::array<wis::span<uint8_t>, 3> spans;
    std::size_t needed_size = barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)
                            + barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)
                            + barriers.global_barrier_count * sizeof(VkMemoryBarrier2);

    if (needed_size <= vk_static_barrier_size) {
        spans[0] = {local_scratch, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {spans[0].end(), barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {spans[1].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    }

    std::size_t sizes[] = {
        barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2),
        barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2),
        barriers.global_barrier_count * sizeof(VkMemoryBarrier2),
        0,
        0,
        0
    };

    sizes[3] = sizes[0] + sizes[1];
    sizes[4] = sizes[1] + sizes[2];
    sizes[5] = sizes[0] + sizes[2];

    uint32_t closest_size = 0;
    int index = -1;
    for (int i = std::size(sizes) - 1; i >= 0; --i) {
        if (sizes[i] > vk_static_barrier_size) {
            continue;
        }
        if (vk_static_barrier_size - sizes[i] < vk_static_barrier_size - closest_size) {
            closest_size = sizes[i];
            index = i;
        }
    }

    uint32_t allocated_size = needed_size - closest_size;
    auto* allocated_data = VKAllocateScratchSpace(impl, allocated_size);

    switch (index) {
    default:
    case -1:
        spans[0] = {allocated_data, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {spans[0].end(), barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {spans[1].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    case 0:
        spans[0] = {local_scratch, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {allocated_data, barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {spans[1].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    case 1:
        spans[0] = {allocated_data, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {local_scratch, barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {spans[0].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    case 2:
        spans[0] = {allocated_data, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {spans[0].end(), barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {local_scratch, barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    case 3:
        spans[0] = {local_scratch, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {spans[0].end(), barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {allocated_data, barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    case 4:
        spans[0] = {allocated_data, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {local_scratch, barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {spans[1].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    case 5:
        spans[0] = {local_scratch, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {allocated_data, barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {spans[0].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    }
}

template <typename Impl>
inline void VKInsertBarriers(const Impl& impl, const WisVKBarrierGroup* barriers)
{
    if (barriers->buffer_barrier_count + barriers->texture_barrier_count + barriers->global_barrier_count == 0) {
        return;
    }

    uint8_t local_scratch[vk_static_barrier_size]{};

    auto [buffer_span, texture_span, global_span] = VKAllocateBarriers(impl, local_scratch, *barriers);

    wis::span<VkBufferMemoryBarrier2> buffer_barriers_span{
        reinterpret_cast<VkBufferMemoryBarrier2*>(buffer_span.data()),
        barriers->buffer_barrier_count
    };
    uint32_t real_buffer_barrier_count = barriers->buffer_barrier_count;

    for (size_t i = 0; i < barriers->buffer_barrier_count; i++) {
        const auto& src = barriers->buffer_barriers[i];

        auto q1 = VK_QUEUE_FAMILY_IGNORED;
        auto q2 = VK_QUEUE_FAMILY_IGNORED;
        if (src.queue_type_before != src.queue_type_after) {
            if (impl.maintenance9) {
                real_buffer_barrier_count--;
                continue;
            }
            q1 = impl.queue_indices[src.queue_type_before].family_index;
            q2 = impl.queue_indices[src.queue_type_after].family_index;
        }

        buffer_barriers_span[i] = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2,
            .pNext = nullptr,
            .srcStageMask = VKConvert(src.sync_before),
            .srcAccessMask = VKConvert(src.access_before),
            .dstStageMask = VKConvert(src.sync_after),
            .dstAccessMask = VKConvert(src.access_after),
            .srcQueueFamilyIndex = q1,
            .dstQueueFamilyIndex = q2,
            .buffer = std::bit_cast<VkBuffer>(src.buffer),
            .offset = src.offset,
            .size = src.size,
        };
    }

    wis::span<VkImageMemoryBarrier2> texture_barriers_span{
        reinterpret_cast<VkImageMemoryBarrier2*>(texture_span.data()),
        barriers->texture_barrier_count
    };
    uint32_t real_texture_barrier_count = barriers->texture_barrier_count;

    for (size_t i = 0; i < barriers->texture_barrier_count; i++) {
        const auto& src = barriers->texture_barriers[i];
        auto q1 = VK_QUEUE_FAMILY_IGNORED;
        auto q2 = VK_QUEUE_FAMILY_IGNORED;

        if (src.queue_type_before != src.queue_type_after) {
            if (impl.maintenance9
                && (impl.queue_indices[src.queue_type_before].compatible_to_families
                    & (1 << impl.queue_indices[src.queue_type_after].family_index))) {
                if (src.queue_type_before == impl.queue_type) {
                    real_texture_barrier_count--;
                    continue;
                }
            } else {
                q1 = impl.queue_indices[src.queue_type_before].family_index;
                q2 = impl.queue_indices[src.queue_type_after].family_index;
            }
        }

        texture_barriers_span[i] = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
            .pNext = nullptr,
            .srcStageMask = VKConvert(src.sync_before),
            .srcAccessMask = VKConvert(src.access_before),
            .dstStageMask = VKConvert(src.sync_after),
            .dstAccessMask = VKConvert(src.access_after),
            .oldLayout = VKConvert(src.state_before),
            .newLayout = VKConvert(src.state_after),
            .srcQueueFamilyIndex = q1,
            .dstQueueFamilyIndex = q2,
            .image = std::bit_cast<VkImage>(src.texture)
        };

        auto aspect_flags = VKExtractAspectFlags(src.subresource_range, src.flags);
        if (src.flags & WisBarrierFlagsWholeRange) {
            texture_barriers_span[i].subresourceRange = {
                .aspectMask = aspect_flags,
                .baseMipLevel = 0,
                .levelCount = VK_REMAINING_MIP_LEVELS,
                .baseArrayLayer = 0,
                .layerCount = VK_REMAINING_ARRAY_LAYERS,
            };
        } else {
            texture_barriers_span[i].subresourceRange = {
                .aspectMask = aspect_flags,
                .baseMipLevel = src.subresource_range.base_mip_level,
                .levelCount = src.subresource_range.mip_level_count,
                .baseArrayLayer = src.subresource_range.base_array_layer,
                .layerCount = src.subresource_range.array_layer_count,
            };
        }
    }

    wis::span<VkMemoryBarrier2> global_barriers_span{
        reinterpret_cast<VkMemoryBarrier2*>(global_span.data()),
        barriers->global_barrier_count
    };

    for (size_t i = 0; i < barriers->global_barrier_count; i++) {
        const auto& src = barriers->global_barriers[i];
        global_barriers_span[i] = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2,
            .pNext = nullptr,
            .srcStageMask = VKConvert(src.sync_before),
            .srcAccessMask = VKConvert(src.access_before),
            .dstStageMask = VKConvert(src.sync_after),
            .dstAccessMask = VKConvert(src.access_after),
        };
    }

    VkDependencyInfo dependency_info{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .memoryBarrierCount = static_cast<uint32_t>(barriers->global_barrier_count),
        .pMemoryBarriers = global_barriers_span.data(),
        .bufferMemoryBarrierCount = real_buffer_barrier_count,
        .pBufferMemoryBarriers = buffer_barriers_span.data(),
        .imageMemoryBarrierCount = real_texture_barrier_count,
        .pImageMemoryBarriers = texture_barriers_span.data(),
    };
    impl.command_list_table->vkCmdPipelineBarrier2(impl.command_buffer, &dependency_info);
}
} // namespace wis::detail

#endif // WIS_VK_DETAIL_HPP
