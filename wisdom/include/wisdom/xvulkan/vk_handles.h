#pragma once
#include <memory>
#include <span>
#include <vector>
#include <vk_mem_alloc.h>
#include <wisvk/vk_loader.hpp>
#include <wisvk/vk_managed_handles.hpp>

namespace wis {
template<typename HandleType>
struct movable_handle {
    HandleType handle = nullptr;

    constexpr movable_handle() = default;
    constexpr explicit movable_handle(HandleType h) noexcept
        : handle(h) { }
    movable_handle(const movable_handle&) = delete;
    constexpr movable_handle(movable_handle&& h) noexcept
        : handle(std::exchange(h.handle, nullptr)) { }
    constexpr movable_handle(nullptr_t) noexcept
        : handle(nullptr) { }

    movable_handle& operator=(const movable_handle&) = delete;
    constexpr movable_handle& operator=(movable_handle&& h) noexcept
    {
        handle = std::exchange(h.handle, nullptr);
        return *this;
    }
    constexpr movable_handle& operator=(HandleType h) noexcept
    {
        handle = h.handle;
        return *this;
    }
    constexpr operator HandleType() const noexcept
    {
        return handle;
    }
    constexpr auto* operator&() noexcept
    {
        return &handle;
    }
};

namespace h {
#if defined(VK_KHR_swapchain)
using VkSwapchainKHR = wis::movable_handle<::VkSwapchainKHR>;
#endif
#if defined(VK_VERSION_1_0)
using VkBufferView = wis::movable_handle<::VkBufferView>;
using VkInstance = wis::movable_handle<::VkInstance>;
using VkQueue = wis::movable_handle<::VkQueue>;
using VkPhysicalDevice = wis::movable_handle<::VkPhysicalDevice>;
using VkBuffer = wis::movable_handle<::VkBuffer>;
using VkSemaphore = wis::movable_handle<::VkSemaphore>;
using VkDevice = wis::movable_handle<::VkDevice>;
using VkCommandPool = wis::movable_handle<::VkCommandPool>;
using VkCommandBuffer = wis::movable_handle<::VkCommandBuffer>;
using VkRenderPass = wis::movable_handle<::VkRenderPass>;
using VkDeviceMemory = wis::movable_handle<::VkDeviceMemory>;
using VkImage = wis::movable_handle<::VkImage>;
using VkImageView = wis::movable_handle<::VkImageView>;
using VkShaderModule = wis::movable_handle<::VkShaderModule>;
using VkPipeline = wis::movable_handle<::VkPipeline>;
using VkPipelineLayout = wis::movable_handle<::VkPipelineLayout>;
using VkSampler = wis::movable_handle<::VkSampler>;
using VkDescriptorSet = wis::movable_handle<::VkDescriptorSet>;
using VkDescriptorPool = wis::movable_handle<::VkDescriptorPool>;
using VkDescriptorSetLayout = wis::movable_handle<::VkDescriptorSetLayout>;
using VkFence = wis::movable_handle<::VkFence>;
using VkEvent = wis::movable_handle<::VkEvent>;
using VkQueryPool = wis::movable_handle<::VkQueryPool>;
using VkFramebuffer = wis::movable_handle<::VkFramebuffer>;
using VkPipelineCache = wis::movable_handle<::VkPipelineCache>;
#endif
#if defined(VK_NVX_binary_import)
using VkCuModuleNVX = wis::movable_handle<::VkCuModuleNVX>;
using VkCuFunctionNVX = wis::movable_handle<::VkCuFunctionNVX>;
#endif
#if defined(VK_NV_cuda_kernel_launch)
using VkCudaFunctionNV = wis::movable_handle<::VkCudaFunctionNV>;
using VkCudaModuleNV = wis::movable_handle<::VkCudaModuleNV>;
#endif
#if defined(VK_EXT_debug_report)
using VkDebugReportCallbackEXT = wis::movable_handle<::VkDebugReportCallbackEXT>;
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_sampler_ycbcr_conversion)
using VkSamplerYcbcrConversion = wis::movable_handle<::VkSamplerYcbcrConversion>;
#endif
#if defined(VK_NV_device_generated_commands)
using VkIndirectCommandsLayoutNV = wis::movable_handle<::VkIndirectCommandsLayoutNV>;
#endif
#if defined(VK_INTEL_performance_query)
using VkPerformanceConfigurationINTEL = wis::movable_handle<::VkPerformanceConfigurationINTEL>;
#endif
#if defined(VK_EXT_opacity_micromap)
using VkMicromapEXT = wis::movable_handle<::VkMicromapEXT>;
#endif
#if defined(VK_KHR_display)
using VkDisplayModeKHR = wis::movable_handle<::VkDisplayModeKHR>;
using VkDisplayKHR = wis::movable_handle<::VkDisplayKHR>;
#endif
#if defined(VK_EXT_debug_utils)
using VkDebugUtilsMessengerEXT = wis::movable_handle<::VkDebugUtilsMessengerEXT>;
#endif
#if defined(VK_EXT_shader_object)
using VkShaderEXT = wis::movable_handle<::VkShaderEXT>;
#endif
#if defined(VK_EXT_validation_cache)
using VkValidationCacheEXT = wis::movable_handle<::VkValidationCacheEXT>;
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_descriptor_update_template)
using VkDescriptorUpdateTemplate = wis::movable_handle<::VkDescriptorUpdateTemplate>;
#endif
#if defined(VK_KHR_deferred_host_operations)
using VkDeferredOperationKHR = wis::movable_handle<::VkDeferredOperationKHR>;
#endif
#if defined(VK_KHR_acceleration_structure)
using VkAccelerationStructureKHR = wis::movable_handle<::VkAccelerationStructureKHR>;
#endif
#if defined(VK_NV_ray_tracing)
using VkAccelerationStructureNV = wis::movable_handle<::VkAccelerationStructureNV>;
#endif
#if defined(VK_FUCHSIA_buffer_collection)
using VkBufferCollectionFUCHSIA = wis::movable_handle<::VkBufferCollectionFUCHSIA>;
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_private_data)
using VkPrivateDataSlot = wis::movable_handle<::VkPrivateDataSlot>;
#endif
#if defined(VK_KHR_video_queue)
using VkVideoSessionParametersKHR = wis::movable_handle<::VkVideoSessionParametersKHR>;
using VkVideoSessionKHR = wis::movable_handle<::VkVideoSessionKHR>;
#endif
#if defined(VK_NV_optical_flow)
using VkOpticalFlowSessionNV = wis::movable_handle<::VkOpticalFlowSessionNV>;
#endif
#if defined(VK_KHR_surface)
using VkSurfaceKHR = wis::movable_handle<::VkSurfaceKHR>;
#endif
#if defined(VK_NV_external_sci_sync2)
using VkSemaphoreSciSyncPoolNV = wis::movable_handle<::VkSemaphoreSciSyncPoolNV>;
#endif
} // namespace h
} // namespace wis

namespace wis {
struct SharedDeviceHeader : public managed_header<VkDevice> {
    std::unique_ptr<VkDeviceTable> device_table;
};

class SharedDevice : public shared_handle_base<VkDevice, SharedDeviceHeader, SharedDevice>
{
public:
    SharedDevice() noexcept = default;
    explicit SharedDevice(VkDevice device, std::unique_ptr<VkDeviceTable> device_table) noexcept
        : shared_handle_base(device, nullptr, std::move(device_table)), m_device_table(m_control->m_header.device_table.get())
    {
        m_control->m_header.deleter.m_pfn = m_control->m_header.device_table->vkDestroyDevice;
    }

public:
    auto* table() const noexcept { return m_device_table; }

protected:
    VkDeviceTable* m_device_table;
};

template<typename HandleType>
struct managed_header_ex : public managed_header<HandleType> {
};

template<typename HandleType>
requires std::same_as<parent_of_t<HandleType>, VkDevice>
struct managed_header_ex<HandleType> {
    SharedDevice parent;
    deleter_of_t<HandleType> deleter;
};

template<typename HandleType>
class managed_handle_ex : public managed_handle_base<HandleType, managed_header_ex<HandleType>,
                                                     managed_handle_ex<HandleType>>
{
public:
    using managed_handle_base<HandleType, managed_header_ex<HandleType>,
                              managed_handle_ex<HandleType>>::managed_handle_base;
};

/// Specializations

template<>
class shared_handle<VmaAllocator>
    : public wis::shared_handle_base<VmaAllocator, wis::SharedDevice, shared_handle<VmaAllocator>>
{
    using base =
            wis::shared_handle_base<VmaAllocator, wis::SharedDevice, shared_handle<VmaAllocator>>;
    friend base;

public:
    shared_handle() = default;
    explicit shared_handle(wis::SharedDevice device, VmaAllocator handle) noexcept
        : wis::shared_handle_base<VmaAllocator, wis::SharedDevice, shared_handle<VmaAllocator>>(
                  handle, std::move(device)) { }

    [[nodiscard]] const auto& parent() const noexcept { return header(); }

protected:
    void internal_destroy() noexcept { vmaDestroyAllocator(get()); }
};

struct swapchain_header_ex {
    wis::shared_handle<VkSurfaceKHR> surface;
    wis::SharedDevice parent;
    deleter_of_t<VkSwapchainKHR> deleter;
};

template<>
class managed_handle_ex<VkSwapchainKHR>
    : public managed_handle_base<VkSwapchainKHR, swapchain_header_ex,
                                 managed_handle_ex<VkSwapchainKHR>>
{
public:
    using managed_handle_base<VkSwapchainKHR, swapchain_header_ex,
                              managed_handle_ex<VkSwapchainKHR>>::managed_handle_base;
};
} // namespace wis