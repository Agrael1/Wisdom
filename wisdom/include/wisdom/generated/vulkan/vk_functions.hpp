#pragma once
#include <array>
#include <vulkan/vulkan.h>
#include <wisvk/vk_libinit.hpp>

namespace wis {

#if !(defined(VK_VERSION_1_1))
#if defined(VK_KHR_get_physical_device_properties2)
using PFN_vkGetPhysicalDeviceFeatures2 = PFN_vkGetPhysicalDeviceFeatures2KHR;
#endif
#endif
#if !(defined(VK_VERSION_1_1))
#if defined(VK_KHR_get_physical_device_properties2)
using PFN_vkGetPhysicalDeviceProperties2 = PFN_vkGetPhysicalDeviceProperties2KHR;
#endif
#endif
#if !(defined(VK_VERSION_1_1))
#if defined(VK_KHR_get_physical_device_properties2)
using PFN_vkGetPhysicalDeviceMemoryProperties2 = PFN_vkGetPhysicalDeviceMemoryProperties2KHR;
#endif
#endif
#if !(defined(VK_VERSION_1_1))
#if defined(VK_KHR_bind_memory2)
using PFN_vkBindBufferMemory2 = PFN_vkBindBufferMemory2KHR;
#endif
#endif
#if !(defined(VK_VERSION_1_1))
#if defined(VK_KHR_bind_memory2)
using PFN_vkBindImageMemory2 = PFN_vkBindImageMemory2KHR;
#endif
#endif
#if !(defined(VK_VERSION_1_3))
#if defined(VK_EXT_shader_object) || defined(VK_EXT_extended_dynamic_state)
using PFN_vkCmdSetPrimitiveTopology = PFN_vkCmdSetPrimitiveTopologyEXT;
#endif
#endif
#if !(defined(VK_VERSION_1_1))
#if defined(VK_KHR_get_memory_requirements2)
using PFN_vkGetBufferMemoryRequirements2 = PFN_vkGetBufferMemoryRequirements2KHR;
#endif
#endif
#if !(defined(VK_VERSION_1_1))
#if defined(VK_KHR_get_memory_requirements2)
using PFN_vkGetImageMemoryRequirements2 = PFN_vkGetImageMemoryRequirements2KHR;
#endif
#endif
#if !(defined(VK_VERSION_1_3))
#if defined(VK_KHR_maintenance4)
using PFN_vkGetDeviceBufferMemoryRequirements = PFN_vkGetDeviceBufferMemoryRequirementsKHR;
#endif
#endif
#if !(defined(VK_VERSION_1_3))
#if defined(VK_KHR_maintenance4)
using PFN_vkGetDeviceImageMemoryRequirements = PFN_vkGetDeviceImageMemoryRequirementsKHR;
#endif
#endif
#if !(defined(VK_VERSION_1_3))
#if defined(VK_EXT_shader_object) || defined(VK_EXT_extended_dynamic_state)
using PFN_vkCmdBindVertexBuffers2 = PFN_vkCmdBindVertexBuffers2EXT;
#endif
#endif
#if !(defined(VK_VERSION_1_2))
#if defined(VK_KHR_timeline_semaphore)
using PFN_vkWaitSemaphores = PFN_vkWaitSemaphoresKHR;
#endif
#endif
#if !(defined(VK_VERSION_1_2))
#if defined(VK_KHR_timeline_semaphore)
using PFN_vkGetSemaphoreCounterValue = PFN_vkGetSemaphoreCounterValueKHR;
#endif
#endif
#if !(defined(VK_VERSION_1_2))
#if defined(VK_KHR_timeline_semaphore)
using PFN_vkSignalSemaphore = PFN_vkSignalSemaphoreKHR;
#endif
#endif
#if !(defined(VK_VERSION_1_2))
#if defined(VK_KHR_buffer_device_address)
using PFN_vkGetBufferDeviceAddress = PFN_vkGetBufferDeviceAddressKHR;
#elif defined(VK_EXT_buffer_device_address)
using PFN_vkGetBufferDeviceAddress = PFN_vkGetBufferDeviceAddressEXT;
#endif
#endif
#if !(defined(VK_VERSION_1_3))
#if defined(VK_KHR_copy_commands2)
using PFN_vkCmdCopyBufferToImage2 = PFN_vkCmdCopyBufferToImage2KHR;
#endif
#endif
#if !(defined(VK_VERSION_1_3))
#if defined(VK_KHR_copy_commands2)
using PFN_vkCmdCopyImageToBuffer2 = PFN_vkCmdCopyImageToBuffer2KHR;
#endif
#endif
#if !(defined(VK_VERSION_1_3))
#if defined(VK_KHR_synchronization2)
using PFN_vkCmdPipelineBarrier2 = PFN_vkCmdPipelineBarrier2KHR;
#endif
#endif
#if !(defined(VK_VERSION_1_3))
#if defined(VK_KHR_dynamic_rendering)
using PFN_vkCmdBeginRendering = PFN_vkCmdBeginRenderingKHR;
#endif
#endif
#if !(defined(VK_VERSION_1_3))
#if defined(VK_KHR_dynamic_rendering)
using PFN_vkCmdEndRendering = PFN_vkCmdEndRenderingKHR;
#endif
#endif

struct VKMainGlobal {
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
    PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    PFN_vkCreateInstance vkCreateInstance;
#if defined(VK_VERSION_1_1)
    PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
#else
    void* vkEnumerateInstanceVersion;
#endif

public:
    bool Init(LibTokenView token) noexcept
    {
        vkGetInstanceProcAddr = token.GetProcAddress<decltype(vkGetInstanceProcAddr)>("vkGetInstanceProcAddr");
        if (vkGetInstanceProcAddr == nullptr)
            return false;
        vkGetDeviceProcAddr = token.GetProcAddress<decltype(vkGetDeviceProcAddr)>("vkGetDeviceProcAddr");
        if (vkGetDeviceProcAddr == nullptr)
            return false;
        vkEnumerateInstanceLayerProperties = token.GetProcAddress<decltype(vkEnumerateInstanceLayerProperties)>("vkEnumerateInstanceLayerProperties");
        if (vkEnumerateInstanceLayerProperties == nullptr)
            return false;
        vkEnumerateInstanceExtensionProperties = token.GetProcAddress<decltype(vkEnumerateInstanceExtensionProperties)>("vkEnumerateInstanceExtensionProperties");
        if (vkEnumerateInstanceExtensionProperties == nullptr)
            return false;
        vkCreateInstance = token.GetProcAddress<decltype(vkCreateInstance)>("vkCreateInstance");
        if (vkCreateInstance == nullptr)
            return false;
        vkEnumerateInstanceVersion = token.GetProcAddress<decltype(vkEnumerateInstanceVersion)>("vkEnumerateInstanceVersion");
        return true;
    }
};

struct VKMainInstance {
    PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
    PFN_vkDestroyInstance vkDestroyInstance;
    PFN_vkCreateDevice vkCreateDevice;
    PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
    PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
    PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2;
    PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
#if defined(VK_VERSION_1_1) || defined(VK_KHR_get_physical_device_properties2)
    PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2;
#else
    void* vkGetPhysicalDeviceMemoryProperties2;
#endif
    PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;

public:
    bool Init(VkInstance instance, PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr) noexcept
    {
        vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties");
        if (vkGetPhysicalDeviceMemoryProperties == nullptr)
            return false;
        vkDestroyInstance = (PFN_vkDestroyInstance)vkGetInstanceProcAddr(instance, "vkDestroyInstance");
        if (vkDestroyInstance == nullptr)
            return false;
        vkCreateDevice = (PFN_vkCreateDevice)vkGetInstanceProcAddr(instance, "vkCreateDevice");
        if (vkCreateDevice == nullptr)
            return false;
        vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties");
        if (vkGetPhysicalDeviceProperties == nullptr)
            return false;
        vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDevices");
        if (vkEnumeratePhysicalDevices == nullptr)
            return false;
        vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties");
        if (vkGetPhysicalDeviceQueueFamilyProperties == nullptr)
            return false;
        vkEnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)vkGetInstanceProcAddr(instance, "vkEnumerateDeviceExtensionProperties");
        if (vkEnumerateDeviceExtensionProperties == nullptr)
            return false;
        static constexpr std::array vkGetPhysicalDeviceFeatures2_strings
        {
#if defined(VK_VERSION_1_1)
            "vkGetPhysicalDeviceFeatures2",
#endif
#if defined(VK_KHR_get_physical_device_properties2)
            "vkGetPhysicalDeviceFeatures2KHR",
#endif
        };
        for (auto vkGetPhysicalDeviceFeatures2_it : vkGetPhysicalDeviceFeatures2_strings)
            if ((vkGetPhysicalDeviceFeatures2 = (PFN_vkGetPhysicalDeviceFeatures2)vkGetInstanceProcAddr(instance, vkGetPhysicalDeviceFeatures2_it)))
                break;
        if (vkGetPhysicalDeviceFeatures2 == nullptr)
            return false;
        static constexpr std::array vkGetPhysicalDeviceProperties2_strings
        {
#if defined(VK_VERSION_1_1)
            "vkGetPhysicalDeviceProperties2",
#endif
#if defined(VK_KHR_get_physical_device_properties2)
            "vkGetPhysicalDeviceProperties2KHR",
#endif
        };
        for (auto vkGetPhysicalDeviceProperties2_it : vkGetPhysicalDeviceProperties2_strings)
            if ((vkGetPhysicalDeviceProperties2 = (PFN_vkGetPhysicalDeviceProperties2)vkGetInstanceProcAddr(instance, vkGetPhysicalDeviceProperties2_it)))
                break;
        if (vkGetPhysicalDeviceProperties2 == nullptr)
            return false;
        static constexpr std::array vkGetPhysicalDeviceMemoryProperties2_strings
        {
#if defined(VK_VERSION_1_1)
            "vkGetPhysicalDeviceMemoryProperties2",
#endif
#if defined(VK_KHR_get_physical_device_properties2)
            "vkGetPhysicalDeviceMemoryProperties2KHR",
#endif
        };
        for (auto vkGetPhysicalDeviceMemoryProperties2_it : vkGetPhysicalDeviceMemoryProperties2_strings)
            if ((vkGetPhysicalDeviceMemoryProperties2 = (PFN_vkGetPhysicalDeviceMemoryProperties2)vkGetInstanceProcAddr(instance, vkGetPhysicalDeviceMemoryProperties2_it)))
                break;
        vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(instance, "vkDestroySurfaceKHR");
        if (vkDestroySurfaceKHR == nullptr)
            return false;
        vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
        if (vkGetPhysicalDeviceSurfaceSupportKHR == nullptr)
            return false;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
        if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR == nullptr)
            return false;
        vkGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
        if (vkGetPhysicalDeviceSurfaceFormatsKHR == nullptr)
            return false;
        vkGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
        if (vkGetPhysicalDeviceSurfacePresentModesKHR == nullptr)
            return false;
        return true;
    }
};

struct VKMainDevice {
    PFN_vkCmdCopyImageToBuffer2 vkCmdCopyImageToBuffer2;
    PFN_vkCmdCopyBufferToImage2 vkCmdCopyBufferToImage2;
    PFN_vkUnmapMemory vkUnmapMemory;
    PFN_vkDestroyDevice vkDestroyDevice;
    PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
    PFN_vkQueueSubmit vkQueueSubmit;
    PFN_vkCreateSampler vkCreateSampler;
    PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
    PFN_vkAllocateMemory vkAllocateMemory;
    PFN_vkCreateImage vkCreateImage;
    PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
    PFN_vkFreeMemory vkFreeMemory;
    PFN_vkMapMemory vkMapMemory;
    PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
    PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
    PFN_vkCmdSetScissor vkCmdSetScissor;
    PFN_vkBindBufferMemory vkBindBufferMemory;
    PFN_vkBindImageMemory vkBindImageMemory;
    PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
    PFN_vkCmdSetViewport vkCmdSetViewport;
    PFN_vkDestroySampler vkDestroySampler;
    PFN_vkDestroyImageView vkDestroyImageView;
    PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
    PFN_vkCreateFence vkCreateFence;
    PFN_vkDestroyFence vkDestroyFence;
    PFN_vkCreateCommandPool vkCreateCommandPool;
    PFN_vkCmdBindPipeline vkCmdBindPipeline;
    PFN_vkResetFences vkResetFences;
    PFN_vkWaitForFences vkWaitForFences;
    PFN_vkCreateSemaphore vkCreateSemaphore;
    PFN_vkDestroyBuffer vkDestroyBuffer;
    PFN_vkDestroySemaphore vkDestroySemaphore;
    PFN_vkCreateBuffer vkCreateBuffer;
    PFN_vkDestroyImage vkDestroyImage;
    PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
    PFN_vkCreateImageView vkCreateImageView;
    PFN_vkCreateShaderModule vkCreateShaderModule;
    PFN_vkDestroyShaderModule vkDestroyShaderModule;
    PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
    PFN_vkDestroyPipeline vkDestroyPipeline;
    PFN_vkResetCommandBuffer vkResetCommandBuffer;
    PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
    PFN_vkDestroyCommandPool vkDestroyCommandPool;
    PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
    PFN_vkEndCommandBuffer vkEndCommandBuffer;
    PFN_vkCmdDraw vkCmdDraw;
    PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
    PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
    PFN_vkCmdPushConstants vkCmdPushConstants;
    PFN_vkCmdSetDescriptorBufferOffsetsEXT vkCmdSetDescriptorBufferOffsetsEXT;
    PFN_vkGetDescriptorEXT vkGetDescriptorEXT;
    PFN_vkGetDescriptorSetLayoutSizeEXT vkGetDescriptorSetLayoutSizeEXT;
    PFN_vkGetDescriptorSetLayoutBindingOffsetEXT vkGetDescriptorSetLayoutBindingOffsetEXT;
    PFN_vkCmdBindDescriptorBuffersEXT vkCmdBindDescriptorBuffersEXT;
    PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2;
    PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology;
#if defined(VK_VERSION_1_1) || defined(VK_KHR_get_memory_requirements2)
    PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
#else
    void* vkGetImageMemoryRequirements2;
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_get_memory_requirements2)
    PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
#else
    void* vkGetBufferMemoryRequirements2;
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_bind_memory2)
    PFN_vkBindBufferMemory2 vkBindBufferMemory2;
#else
    void* vkBindBufferMemory2;
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_bind_memory2)
    PFN_vkBindImageMemory2 vkBindImageMemory2;
#else
    void* vkBindImageMemory2;
#endif
    PFN_vkWaitSemaphores vkWaitSemaphores;
    PFN_vkSignalSemaphore vkSignalSemaphore;
    PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue;
    PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2;
    PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements;
    PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements;
    PFN_vkGetDeviceQueue2 vkGetDeviceQueue2;
    PFN_vkCmdEndRendering vkCmdEndRendering;
    PFN_vkCmdBeginRendering vkCmdBeginRendering;
    PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
    PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
    PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
    PFN_vkQueuePresentKHR vkQueuePresentKHR;
#if defined(VK_KHR_present_wait)
    PFN_vkWaitForPresentKHR vkWaitForPresentKHR;
#else
    void* vkWaitForPresentKHR;
#endif

public:
    bool Init(VkDevice device, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr) noexcept
    {
        static constexpr std::array vkCmdCopyImageToBuffer2_strings
        {
#if defined(VK_VERSION_1_3)
            "vkCmdCopyImageToBuffer2",
#endif
#if defined(VK_KHR_copy_commands2)
            "vkCmdCopyImageToBuffer2KHR",
#endif
        };
        for (auto vkCmdCopyImageToBuffer2_it : vkCmdCopyImageToBuffer2_strings)
            if ((vkCmdCopyImageToBuffer2 = (PFN_vkCmdCopyImageToBuffer2)vkGetDeviceProcAddr(device, vkCmdCopyImageToBuffer2_it)))
                break;
        if (vkCmdCopyImageToBuffer2 == nullptr)
            return false;
        static constexpr std::array vkCmdCopyBufferToImage2_strings
        {
#if defined(VK_VERSION_1_3)
            "vkCmdCopyBufferToImage2",
#endif
#if defined(VK_KHR_copy_commands2)
            "vkCmdCopyBufferToImage2KHR",
#endif
        };
        for (auto vkCmdCopyBufferToImage2_it : vkCmdCopyBufferToImage2_strings)
            if ((vkCmdCopyBufferToImage2 = (PFN_vkCmdCopyBufferToImage2)vkGetDeviceProcAddr(device, vkCmdCopyBufferToImage2_it)))
                break;
        if (vkCmdCopyBufferToImage2 == nullptr)
            return false;
        vkUnmapMemory = (PFN_vkUnmapMemory)vkGetDeviceProcAddr(device, "vkUnmapMemory");
        if (vkUnmapMemory == nullptr)
            return false;
        vkDestroyDevice = (PFN_vkDestroyDevice)vkGetDeviceProcAddr(device, "vkDestroyDevice");
        if (vkDestroyDevice == nullptr)
            return false;
        vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)vkGetDeviceProcAddr(device, "vkBeginCommandBuffer");
        if (vkBeginCommandBuffer == nullptr)
            return false;
        vkQueueSubmit = (PFN_vkQueueSubmit)vkGetDeviceProcAddr(device, "vkQueueSubmit");
        if (vkQueueSubmit == nullptr)
            return false;
        vkCreateSampler = (PFN_vkCreateSampler)vkGetDeviceProcAddr(device, "vkCreateSampler");
        if (vkCreateSampler == nullptr)
            return false;
        vkFlushMappedMemoryRanges = (PFN_vkFlushMappedMemoryRanges)vkGetDeviceProcAddr(device, "vkFlushMappedMemoryRanges");
        if (vkFlushMappedMemoryRanges == nullptr)
            return false;
        vkAllocateMemory = (PFN_vkAllocateMemory)vkGetDeviceProcAddr(device, "vkAllocateMemory");
        if (vkAllocateMemory == nullptr)
            return false;
        vkCreateImage = (PFN_vkCreateImage)vkGetDeviceProcAddr(device, "vkCreateImage");
        if (vkCreateImage == nullptr)
            return false;
        vkCreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout)vkGetDeviceProcAddr(device, "vkCreateDescriptorSetLayout");
        if (vkCreateDescriptorSetLayout == nullptr)
            return false;
        vkFreeMemory = (PFN_vkFreeMemory)vkGetDeviceProcAddr(device, "vkFreeMemory");
        if (vkFreeMemory == nullptr)
            return false;
        vkMapMemory = (PFN_vkMapMemory)vkGetDeviceProcAddr(device, "vkMapMemory");
        if (vkMapMemory == nullptr)
            return false;
        vkDestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout)vkGetDeviceProcAddr(device, "vkDestroyDescriptorSetLayout");
        if (vkDestroyDescriptorSetLayout == nullptr)
            return false;
        vkInvalidateMappedMemoryRanges = (PFN_vkInvalidateMappedMemoryRanges)vkGetDeviceProcAddr(device, "vkInvalidateMappedMemoryRanges");
        if (vkInvalidateMappedMemoryRanges == nullptr)
            return false;
        vkCmdSetScissor = (PFN_vkCmdSetScissor)vkGetDeviceProcAddr(device, "vkCmdSetScissor");
        if (vkCmdSetScissor == nullptr)
            return false;
        vkBindBufferMemory = (PFN_vkBindBufferMemory)vkGetDeviceProcAddr(device, "vkBindBufferMemory");
        if (vkBindBufferMemory == nullptr)
            return false;
        vkBindImageMemory = (PFN_vkBindImageMemory)vkGetDeviceProcAddr(device, "vkBindImageMemory");
        if (vkBindImageMemory == nullptr)
            return false;
        vkGetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements)vkGetDeviceProcAddr(device, "vkGetBufferMemoryRequirements");
        if (vkGetBufferMemoryRequirements == nullptr)
            return false;
        vkCmdSetViewport = (PFN_vkCmdSetViewport)vkGetDeviceProcAddr(device, "vkCmdSetViewport");
        if (vkCmdSetViewport == nullptr)
            return false;
        vkDestroySampler = (PFN_vkDestroySampler)vkGetDeviceProcAddr(device, "vkDestroySampler");
        if (vkDestroySampler == nullptr)
            return false;
        vkDestroyImageView = (PFN_vkDestroyImageView)vkGetDeviceProcAddr(device, "vkDestroyImageView");
        if (vkDestroyImageView == nullptr)
            return false;
        vkGetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)vkGetDeviceProcAddr(device, "vkGetImageMemoryRequirements");
        if (vkGetImageMemoryRequirements == nullptr)
            return false;
        vkCreateFence = (PFN_vkCreateFence)vkGetDeviceProcAddr(device, "vkCreateFence");
        if (vkCreateFence == nullptr)
            return false;
        vkDestroyFence = (PFN_vkDestroyFence)vkGetDeviceProcAddr(device, "vkDestroyFence");
        if (vkDestroyFence == nullptr)
            return false;
        vkCreateCommandPool = (PFN_vkCreateCommandPool)vkGetDeviceProcAddr(device, "vkCreateCommandPool");
        if (vkCreateCommandPool == nullptr)
            return false;
        vkCmdBindPipeline = (PFN_vkCmdBindPipeline)vkGetDeviceProcAddr(device, "vkCmdBindPipeline");
        if (vkCmdBindPipeline == nullptr)
            return false;
        vkResetFences = (PFN_vkResetFences)vkGetDeviceProcAddr(device, "vkResetFences");
        if (vkResetFences == nullptr)
            return false;
        vkWaitForFences = (PFN_vkWaitForFences)vkGetDeviceProcAddr(device, "vkWaitForFences");
        if (vkWaitForFences == nullptr)
            return false;
        vkCreateSemaphore = (PFN_vkCreateSemaphore)vkGetDeviceProcAddr(device, "vkCreateSemaphore");
        if (vkCreateSemaphore == nullptr)
            return false;
        vkDestroyBuffer = (PFN_vkDestroyBuffer)vkGetDeviceProcAddr(device, "vkDestroyBuffer");
        if (vkDestroyBuffer == nullptr)
            return false;
        vkDestroySemaphore = (PFN_vkDestroySemaphore)vkGetDeviceProcAddr(device, "vkDestroySemaphore");
        if (vkDestroySemaphore == nullptr)
            return false;
        vkCreateBuffer = (PFN_vkCreateBuffer)vkGetDeviceProcAddr(device, "vkCreateBuffer");
        if (vkCreateBuffer == nullptr)
            return false;
        vkDestroyImage = (PFN_vkDestroyImage)vkGetDeviceProcAddr(device, "vkDestroyImage");
        if (vkDestroyImage == nullptr)
            return false;
        vkCreatePipelineLayout = (PFN_vkCreatePipelineLayout)vkGetDeviceProcAddr(device, "vkCreatePipelineLayout");
        if (vkCreatePipelineLayout == nullptr)
            return false;
        vkCreateImageView = (PFN_vkCreateImageView)vkGetDeviceProcAddr(device, "vkCreateImageView");
        if (vkCreateImageView == nullptr)
            return false;
        vkCreateShaderModule = (PFN_vkCreateShaderModule)vkGetDeviceProcAddr(device, "vkCreateShaderModule");
        if (vkCreateShaderModule == nullptr)
            return false;
        vkDestroyShaderModule = (PFN_vkDestroyShaderModule)vkGetDeviceProcAddr(device, "vkDestroyShaderModule");
        if (vkDestroyShaderModule == nullptr)
            return false;
        vkCreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines)vkGetDeviceProcAddr(device, "vkCreateGraphicsPipelines");
        if (vkCreateGraphicsPipelines == nullptr)
            return false;
        vkDestroyPipeline = (PFN_vkDestroyPipeline)vkGetDeviceProcAddr(device, "vkDestroyPipeline");
        if (vkDestroyPipeline == nullptr)
            return false;
        vkResetCommandBuffer = (PFN_vkResetCommandBuffer)vkGetDeviceProcAddr(device, "vkResetCommandBuffer");
        if (vkResetCommandBuffer == nullptr)
            return false;
        vkDestroyPipelineLayout = (PFN_vkDestroyPipelineLayout)vkGetDeviceProcAddr(device, "vkDestroyPipelineLayout");
        if (vkDestroyPipelineLayout == nullptr)
            return false;
        vkDestroyCommandPool = (PFN_vkDestroyCommandPool)vkGetDeviceProcAddr(device, "vkDestroyCommandPool");
        if (vkDestroyCommandPool == nullptr)
            return false;
        vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)vkGetDeviceProcAddr(device, "vkAllocateCommandBuffers");
        if (vkAllocateCommandBuffers == nullptr)
            return false;
        vkEndCommandBuffer = (PFN_vkEndCommandBuffer)vkGetDeviceProcAddr(device, "vkEndCommandBuffer");
        if (vkEndCommandBuffer == nullptr)
            return false;
        vkCmdDraw = (PFN_vkCmdDraw)vkGetDeviceProcAddr(device, "vkCmdDraw");
        if (vkCmdDraw == nullptr)
            return false;
        vkCmdDrawIndexed = (PFN_vkCmdDrawIndexed)vkGetDeviceProcAddr(device, "vkCmdDrawIndexed");
        if (vkCmdDrawIndexed == nullptr)
            return false;
        vkCmdCopyBuffer = (PFN_vkCmdCopyBuffer)vkGetDeviceProcAddr(device, "vkCmdCopyBuffer");
        if (vkCmdCopyBuffer == nullptr)
            return false;
        vkCmdPushConstants = (PFN_vkCmdPushConstants)vkGetDeviceProcAddr(device, "vkCmdPushConstants");
        if (vkCmdPushConstants == nullptr)
            return false;
        vkCmdSetDescriptorBufferOffsetsEXT = (PFN_vkCmdSetDescriptorBufferOffsetsEXT)vkGetDeviceProcAddr(device, "vkCmdSetDescriptorBufferOffsetsEXT");
        if (vkCmdSetDescriptorBufferOffsetsEXT == nullptr)
            return false;
        vkGetDescriptorEXT = (PFN_vkGetDescriptorEXT)vkGetDeviceProcAddr(device, "vkGetDescriptorEXT");
        if (vkGetDescriptorEXT == nullptr)
            return false;
        vkGetDescriptorSetLayoutSizeEXT = (PFN_vkGetDescriptorSetLayoutSizeEXT)vkGetDeviceProcAddr(device, "vkGetDescriptorSetLayoutSizeEXT");
        if (vkGetDescriptorSetLayoutSizeEXT == nullptr)
            return false;
        vkGetDescriptorSetLayoutBindingOffsetEXT = (PFN_vkGetDescriptorSetLayoutBindingOffsetEXT)vkGetDeviceProcAddr(device, "vkGetDescriptorSetLayoutBindingOffsetEXT");
        if (vkGetDescriptorSetLayoutBindingOffsetEXT == nullptr)
            return false;
        vkCmdBindDescriptorBuffersEXT = (PFN_vkCmdBindDescriptorBuffersEXT)vkGetDeviceProcAddr(device, "vkCmdBindDescriptorBuffersEXT");
        if (vkCmdBindDescriptorBuffersEXT == nullptr)
            return false;
        static constexpr std::array vkCmdBindVertexBuffers2_strings
        {
#if defined(VK_VERSION_1_3)
            "vkCmdBindVertexBuffers2",
#endif
#if defined(VK_EXT_shader_object) || defined(VK_EXT_extended_dynamic_state)
            "vkCmdBindVertexBuffers2EXT",
#endif
        };
        for (auto vkCmdBindVertexBuffers2_it : vkCmdBindVertexBuffers2_strings)
            if ((vkCmdBindVertexBuffers2 = (PFN_vkCmdBindVertexBuffers2)vkGetDeviceProcAddr(device, vkCmdBindVertexBuffers2_it)))
                break;
        if (vkCmdBindVertexBuffers2 == nullptr)
            return false;
        static constexpr std::array vkCmdSetPrimitiveTopology_strings
        {
#if defined(VK_VERSION_1_3)
            "vkCmdSetPrimitiveTopology",
#endif
#if defined(VK_EXT_shader_object) || defined(VK_EXT_extended_dynamic_state)
            "vkCmdSetPrimitiveTopologyEXT",
#endif
        };
        for (auto vkCmdSetPrimitiveTopology_it : vkCmdSetPrimitiveTopology_strings)
            if ((vkCmdSetPrimitiveTopology = (PFN_vkCmdSetPrimitiveTopology)vkGetDeviceProcAddr(device, vkCmdSetPrimitiveTopology_it)))
                break;
        if (vkCmdSetPrimitiveTopology == nullptr)
            return false;
        static constexpr std::array vkGetImageMemoryRequirements2_strings
        {
#if defined(VK_VERSION_1_1)
            "vkGetImageMemoryRequirements2",
#endif
#if defined(VK_KHR_get_memory_requirements2)
            "vkGetImageMemoryRequirements2KHR",
#endif
        };
        for (auto vkGetImageMemoryRequirements2_it : vkGetImageMemoryRequirements2_strings)
            if ((vkGetImageMemoryRequirements2 = (PFN_vkGetImageMemoryRequirements2)vkGetDeviceProcAddr(device, vkGetImageMemoryRequirements2_it)))
                break;
        static constexpr std::array vkGetBufferMemoryRequirements2_strings
        {
#if defined(VK_VERSION_1_1)
            "vkGetBufferMemoryRequirements2",
#endif
#if defined(VK_KHR_get_memory_requirements2)
            "vkGetBufferMemoryRequirements2KHR",
#endif
        };
        for (auto vkGetBufferMemoryRequirements2_it : vkGetBufferMemoryRequirements2_strings)
            if ((vkGetBufferMemoryRequirements2 = (PFN_vkGetBufferMemoryRequirements2)vkGetDeviceProcAddr(device, vkGetBufferMemoryRequirements2_it)))
                break;
        static constexpr std::array vkBindBufferMemory2_strings
        {
#if defined(VK_VERSION_1_1)
            "vkBindBufferMemory2",
#endif
#if defined(VK_KHR_bind_memory2)
            "vkBindBufferMemory2KHR",
#endif
        };
        for (auto vkBindBufferMemory2_it : vkBindBufferMemory2_strings)
            if ((vkBindBufferMemory2 = (PFN_vkBindBufferMemory2)vkGetDeviceProcAddr(device, vkBindBufferMemory2_it)))
                break;
        static constexpr std::array vkBindImageMemory2_strings
        {
#if defined(VK_VERSION_1_1)
            "vkBindImageMemory2",
#endif
#if defined(VK_KHR_bind_memory2)
            "vkBindImageMemory2KHR",
#endif
        };
        for (auto vkBindImageMemory2_it : vkBindImageMemory2_strings)
            if ((vkBindImageMemory2 = (PFN_vkBindImageMemory2)vkGetDeviceProcAddr(device, vkBindImageMemory2_it)))
                break;
        static constexpr std::array vkWaitSemaphores_strings
        {
#if defined(VK_VERSION_1_2)
            "vkWaitSemaphores",
#endif
#if defined(VK_KHR_timeline_semaphore)
            "vkWaitSemaphoresKHR",
#endif
        };
        for (auto vkWaitSemaphores_it : vkWaitSemaphores_strings)
            if ((vkWaitSemaphores = (PFN_vkWaitSemaphores)vkGetDeviceProcAddr(device, vkWaitSemaphores_it)))
                break;
        if (vkWaitSemaphores == nullptr)
            return false;
        static constexpr std::array vkSignalSemaphore_strings
        {
#if defined(VK_VERSION_1_2)
            "vkSignalSemaphore",
#endif
#if defined(VK_KHR_timeline_semaphore)
            "vkSignalSemaphoreKHR",
#endif
        };
        for (auto vkSignalSemaphore_it : vkSignalSemaphore_strings)
            if ((vkSignalSemaphore = (PFN_vkSignalSemaphore)vkGetDeviceProcAddr(device, vkSignalSemaphore_it)))
                break;
        if (vkSignalSemaphore == nullptr)
            return false;
        static constexpr std::array vkGetSemaphoreCounterValue_strings
        {
#if defined(VK_VERSION_1_2)
            "vkGetSemaphoreCounterValue",
#endif
#if defined(VK_KHR_timeline_semaphore)
            "vkGetSemaphoreCounterValueKHR",
#endif
        };
        for (auto vkGetSemaphoreCounterValue_it : vkGetSemaphoreCounterValue_strings)
            if ((vkGetSemaphoreCounterValue = (PFN_vkGetSemaphoreCounterValue)vkGetDeviceProcAddr(device, vkGetSemaphoreCounterValue_it)))
                break;
        if (vkGetSemaphoreCounterValue == nullptr)
            return false;
        static constexpr std::array vkCmdPipelineBarrier2_strings
        {
#if defined(VK_VERSION_1_3)
            "vkCmdPipelineBarrier2",
#endif
#if defined(VK_KHR_synchronization2)
            "vkCmdPipelineBarrier2KHR",
#endif
        };
        for (auto vkCmdPipelineBarrier2_it : vkCmdPipelineBarrier2_strings)
            if ((vkCmdPipelineBarrier2 = (PFN_vkCmdPipelineBarrier2)vkGetDeviceProcAddr(device, vkCmdPipelineBarrier2_it)))
                break;
        if (vkCmdPipelineBarrier2 == nullptr)
            return false;
        static constexpr std::array vkGetDeviceBufferMemoryRequirements_strings
        {
#if defined(VK_VERSION_1_3)
            "vkGetDeviceBufferMemoryRequirements",
#endif
#if defined(VK_KHR_maintenance4)
            "vkGetDeviceBufferMemoryRequirementsKHR",
#endif
        };
        for (auto vkGetDeviceBufferMemoryRequirements_it : vkGetDeviceBufferMemoryRequirements_strings)
            if ((vkGetDeviceBufferMemoryRequirements = (PFN_vkGetDeviceBufferMemoryRequirements)vkGetDeviceProcAddr(device, vkGetDeviceBufferMemoryRequirements_it)))
                break;
        if (vkGetDeviceBufferMemoryRequirements == nullptr)
            return false;
        static constexpr std::array vkGetDeviceImageMemoryRequirements_strings
        {
#if defined(VK_VERSION_1_3)
            "vkGetDeviceImageMemoryRequirements",
#endif
#if defined(VK_KHR_maintenance4)
            "vkGetDeviceImageMemoryRequirementsKHR",
#endif
        };
        for (auto vkGetDeviceImageMemoryRequirements_it : vkGetDeviceImageMemoryRequirements_strings)
            if ((vkGetDeviceImageMemoryRequirements = (PFN_vkGetDeviceImageMemoryRequirements)vkGetDeviceProcAddr(device, vkGetDeviceImageMemoryRequirements_it)))
                break;
        if (vkGetDeviceImageMemoryRequirements == nullptr)
            return false;
        vkGetDeviceQueue2 = (PFN_vkGetDeviceQueue2)vkGetDeviceProcAddr(device, "vkGetDeviceQueue2");
        if (vkGetDeviceQueue2 == nullptr)
            return false;
        static constexpr std::array vkCmdEndRendering_strings
        {
#if defined(VK_VERSION_1_3)
            "vkCmdEndRendering",
#endif
#if defined(VK_KHR_dynamic_rendering)
            "vkCmdEndRenderingKHR",
#endif
        };
        for (auto vkCmdEndRendering_it : vkCmdEndRendering_strings)
            if ((vkCmdEndRendering = (PFN_vkCmdEndRendering)vkGetDeviceProcAddr(device, vkCmdEndRendering_it)))
                break;
        if (vkCmdEndRendering == nullptr)
            return false;
        static constexpr std::array vkCmdBeginRendering_strings
        {
#if defined(VK_VERSION_1_3)
            "vkCmdBeginRendering",
#endif
#if defined(VK_KHR_dynamic_rendering)
            "vkCmdBeginRenderingKHR",
#endif
        };
        for (auto vkCmdBeginRendering_it : vkCmdBeginRendering_strings)
            if ((vkCmdBeginRendering = (PFN_vkCmdBeginRendering)vkGetDeviceProcAddr(device, vkCmdBeginRendering_it)))
                break;
        if (vkCmdBeginRendering == nullptr)
            return false;
        static constexpr std::array vkGetBufferDeviceAddress_strings
        {
#if defined(VK_VERSION_1_2)
            "vkGetBufferDeviceAddress",
#endif
#if defined(VK_KHR_buffer_device_address)
            "vkGetBufferDeviceAddressKHR",
#endif
#if defined(VK_EXT_buffer_device_address)
            "vkGetBufferDeviceAddressEXT",
#endif
        };
        for (auto vkGetBufferDeviceAddress_it : vkGetBufferDeviceAddress_strings)
            if ((vkGetBufferDeviceAddress = (PFN_vkGetBufferDeviceAddress)vkGetDeviceProcAddr(device, vkGetBufferDeviceAddress_it)))
                break;
        if (vkGetBufferDeviceAddress == nullptr)
            return false;
        vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)vkGetDeviceProcAddr(device, "vkCreateSwapchainKHR");
        if (vkCreateSwapchainKHR == nullptr)
            return false;
        vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)vkGetDeviceProcAddr(device, "vkDestroySwapchainKHR");
        if (vkDestroySwapchainKHR == nullptr)
            return false;
        vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)vkGetDeviceProcAddr(device, "vkGetSwapchainImagesKHR");
        if (vkGetSwapchainImagesKHR == nullptr)
            return false;
        vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)vkGetDeviceProcAddr(device, "vkAcquireNextImageKHR");
        if (vkAcquireNextImageKHR == nullptr)
            return false;
        vkQueuePresentKHR = (PFN_vkQueuePresentKHR)vkGetDeviceProcAddr(device, "vkQueuePresentKHR");
        if (vkQueuePresentKHR == nullptr)
            return false;
        vkWaitForPresentKHR = (PFN_vkWaitForPresentKHR)vkGetDeviceProcAddr(device, "vkWaitForPresentKHR");
        return true;
    }
};

} // namespace wis
