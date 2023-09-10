#ifndef VULKAN_CSHARED_HPP
#define VULKAN_CSHARED_HPP

#include <atomic> // std::atomic_size_t
#include <vulkan/vulkan.h>

namespace wis {
class empty_type
{
};

#if !defined(VULKAN_HPP_NO_SMART_HANDLE)

//======================
//=== SHARED HANDLEs ===
//======================

template<typename T>
using function_pointer_t = std::conditional_t<std::is_function_v<std::remove_pointer_t<T>>, std::remove_pointer_t<T>*, nullptr_t>;

template<typename HandleType>
class handle_traits
{
public:
    using deleter_parent = empty_type;
    using deleter_pool = empty_type;
    using deleter_pfn = nullptr_t;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return nullptr;
    }
};

//=== VK_VERSION_1_0 ===
template<>
class handle_traits<VkInstance>
{
public:
    using deleter_parent = empty_type;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyInstance)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyInstance;
    }
};

template<>
class handle_traits<VkDevice>
{
public:
    using deleter_parent = empty_type;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyDevice)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyDevice;
    }
};

template<>
class handle_traits<VkDeviceMemory>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkFreeMemory)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkFreeMemory;
    }
};

template<>
class handle_traits<VkFence>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyFence)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyFence;
    }
};

template<>
class handle_traits<VkSemaphore>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroySemaphore)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroySemaphore;
    }
};

template<>
class handle_traits<VkEvent>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyEvent)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyEvent;
    }
};

template<>
class handle_traits<VkQueryPool>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyQueryPool)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyQueryPool;
    }
};

template<>
class handle_traits<VkBuffer>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyBuffer)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyBuffer;
    }
};

template<>
class handle_traits<VkBufferView>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyBufferView)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyBufferView;
    }
};

template<>
class handle_traits<VkImage>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyImage)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyImage;
    }
};

template<>
class handle_traits<VkImageView>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyImageView)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyImageView;
    }
};

template<>
class handle_traits<VkShaderModule>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyShaderModule)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyShaderModule;
    }
};

template<>
class handle_traits<VkPipelineCache>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyPipelineCache)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyPipelineCache;
    }
};

template<>
class handle_traits<VkPipeline>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyPipeline)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyPipeline;
    }
};

template<>
class handle_traits<VkPipelineLayout>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyPipelineLayout)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyPipelineLayout;
    }
};

template<>
class handle_traits<VkSampler>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroySampler)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroySampler;
    }
};

template<>
class handle_traits<VkDescriptorPool>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyDescriptorPool)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyDescriptorPool;
    }
};

template<>
class handle_traits<VkDescriptorSet>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = VkDescriptorPool;
    using deleter_pfn = function_pointer_t<decltype(vkFreeDescriptorSets)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkFreeDescriptorSets;
    }
};

template<>
class handle_traits<VkDescriptorSetLayout>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyDescriptorSetLayout)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyDescriptorSetLayout;
    }
};

template<>
class handle_traits<VkFramebuffer>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyFramebuffer)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyFramebuffer;
    }
};

template<>
class handle_traits<VkRenderPass>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyRenderPass)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyRenderPass;
    }
};

template<>
class handle_traits<VkCommandPool>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyCommandPool)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyCommandPool;
    }
};

template<>
class handle_traits<VkCommandBuffer>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = VkCommandPool;
    using deleter_pfn = function_pointer_t<decltype(vkFreeCommandBuffers)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkFreeCommandBuffers;
    }
};

//=== VK_VERSION_1_1 ===
template<>
class handle_traits<VkSamplerYcbcrConversion>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroySamplerYcbcrConversionKHR)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroySamplerYcbcrConversionKHR;
    }
};

template<>
class handle_traits<VkDescriptorUpdateTemplate>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyDescriptorUpdateTemplateKHR)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyDescriptorUpdateTemplateKHR;
    }
};

//=== VK_VERSION_1_3 ===
template<>
class handle_traits<VkPrivateDataSlot>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyPrivateDataSlotEXT)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyPrivateDataSlotEXT;
    }
};

//=== VK_KHR_surface ===
template<>
class handle_traits<VkSurfaceKHR>
{
public:
    using deleter_parent = VkInstance;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroySurfaceKHR)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroySurfaceKHR;
    }
};

//=== VK_KHR_swapchain ===
template<>
class handle_traits<VkSwapchainKHR>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroySwapchainKHR)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroySwapchainKHR;
    }
};

//=== VK_EXT_debug_report ===
template<>
class handle_traits<VkDebugReportCallbackEXT>
{
public:
    using deleter_parent = VkInstance;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyDebugReportCallbackEXT)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyDebugReportCallbackEXT;
    }
};

//=== VK_KHR_video_queue ===
template<>
class handle_traits<VkVideoSessionKHR>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyVideoSessionKHR)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyVideoSessionKHR;
    }
};

template<>
class handle_traits<VkVideoSessionParametersKHR>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyVideoSessionParametersKHR)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyVideoSessionParametersKHR;
    }
};

//=== VK_NVX_binary_import ===
template<>
class handle_traits<VkCuModuleNVX>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyCuModuleNVX)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyCuModuleNVX;
    }
};

template<>
class handle_traits<VkCuFunctionNVX>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyCuFunctionNVX)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyCuFunctionNVX;
    }
};

//=== VK_EXT_debug_utils ===
template<>
class handle_traits<VkDebugUtilsMessengerEXT>
{
public:
    using deleter_parent = VkInstance;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyDebugUtilsMessengerEXT)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyDebugUtilsMessengerEXT;
    }
};

//=== VK_KHR_acceleration_structure ===
template<>
class handle_traits<VkAccelerationStructureKHR>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyAccelerationStructureKHR)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyAccelerationStructureKHR;
    }
};

//=== VK_EXT_validation_cache ===
template<>
class handle_traits<VkValidationCacheEXT>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyValidationCacheEXT)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyValidationCacheEXT;
    }
};

//=== VK_NV_ray_tracing ===
template<>
class handle_traits<VkAccelerationStructureNV>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyAccelerationStructureNV)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyAccelerationStructureNV;
    }
};

//=== VK_KHR_deferred_host_operations ===
template<>
class handle_traits<VkDeferredOperationKHR>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyDeferredOperationKHR)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyDeferredOperationKHR;
    }
};

//=== VK_NV_device_generated_commands ===
template<>
class handle_traits<VkIndirectCommandsLayoutNV>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyIndirectCommandsLayoutNV)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyIndirectCommandsLayoutNV;
    }
};

#if defined(VK_USE_PLATFORM_FUCHSIA)
//=== VK_FUCHSIA_buffer_collection ===
template<>
class handle_traits<VkBufferCollectionFUCHSIA>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyBufferCollectionFUCHSIA)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyBufferCollectionFUCHSIA;
    }
};
#endif /*VK_USE_PLATFORM_FUCHSIA*/

//=== VK_EXT_opacity_micromap ===
template<>
class handle_traits<VkMicromapEXT>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyMicromapEXT)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyMicromapEXT;
    }
};

//=== VK_NV_optical_flow ===
template<>
class handle_traits<VkOpticalFlowSessionNV>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyOpticalFlowSessionNV)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyOpticalFlowSessionNV;
    }
};

//=== VK_EXT_shader_object ===
template<>
class handle_traits<VkShaderEXT>
{
public:
    using deleter_parent = VkDevice;
    using deleter_pool = empty_type;
    using deleter_pfn = function_pointer_t<decltype(vkDestroyShaderEXT)>;

    constexpr static inline deleter_pfn default_deleter() noexcept
    {
        return vkDestroyShaderEXT;
    }
};

template<typename HandleType>
class deleter
{
    using deleter_pfn = typename handle_traits<HandleType>::deleter_pfn;
    using deleter_parent = typename handle_traits<HandleType>::deleter_parent;
    static inline constexpr bool has_parent = !std::is_same<deleter_parent, empty_type>::value;

public:
    deleter(deleter_pfn pfn = handle_traits<HandleType>::default_deleter(), VkAllocationCallbacks* pallocator = nullptr) noexcept
        : m_pfn(pfn), m_pallocator(pallocator)
    {
    }

public:
    void operator()(deleter_parent parent, HandleType handle) const noexcept
        requires has_parent
    {
        m_pfn(parent, handle, m_pallocator);
    }

    void operator()(HandleType handle) const noexcept
        requires !has_parent
    {
        m_pfn(handle, m_pallocator);
    }

public:
    deleter_pfn m_pfn;
    VkAllocationCallbacks* m_pallocator;
};

template<typename HandleType>
class pool_deleter
{
    using deleter_pfn = typename handle_traits<HandleType>::deleter_pfn;
    using deleter_parent = typename handle_traits<HandleType>::deleter_parent;
    using deleter_pool = typename handle_traits<HandleType>::deleter_pool;

public:
    pool_deleter(deleter_pfn pfn = handle_traits<HandleType>::default_deleter()) noexcept
        : m_pfn(pfn) { }

public:
    void operator()(deleter_parent parent, deleter_pool pool, uint32_t num_handles, HandleType* handles) const noexcept
    {
        m_pfn(parent, pool, num_handles, handles);
    }

public:
    deleter_pfn m_pfn;
};
#endif // !VULKAN_HPP_NO_SMART_HANDLE
} // namespace wis
#endif // VULKAN_SHARED_HPP