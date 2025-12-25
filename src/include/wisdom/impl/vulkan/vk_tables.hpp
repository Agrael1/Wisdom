#ifndef WIS_VK_TABLES_HPP
#define WIS_VK_TABLES_HPP
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus
#include <wisdom/impl/vulkan/vk_loader.hpp>
#include <vulkan/vulkan.h>

namespace wis {
namespace impl {
//-----------------------------------------------------------------------------
struct VKMainGlobal {
    PFN_vkGetInstanceProcAddr                  vkGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr                    vkGetDeviceProcAddr;
    PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties;
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    PFN_vkCreateInstance                       vkCreateInstance;
    PFN_vkEnumerateInstanceVersion             vkEnumerateInstanceVersion;

public:
    bool Init(void* library) noexcept
    {
        ASSIGN_PROC_ADDRESS_CHECK(library, vkGetInstanceProcAddr);
        ASSIGN_PROC_ADDRESS_CHECK(library, vkGetDeviceProcAddr);
        ASSIGN_PROC_ADDRESS_CHECK(library, vkEnumerateInstanceLayerProperties);
        ASSIGN_PROC_ADDRESS_CHECK(library, vkEnumerateInstanceExtensionProperties);
        ASSIGN_PROC_ADDRESS_CHECK(library, vkCreateInstance);
        ASSIGN_PROC_ADDRESS_CHECK(library, vkEnumerateInstanceVersion);
        return true;
    }
};

//-----------------------------------------------------------------------------
struct VKMainAdapter {
    PFN_vkGetPhysicalDeviceMemoryProperties        vkGetPhysicalDeviceMemoryProperties;
    PFN_vkGetPhysicalDeviceProperties              vkGetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties   vkGetPhysicalDeviceQueueFamilyProperties;
    PFN_vkEnumerateDeviceExtensionProperties       vkEnumerateDeviceExtensionProperties;
    PFN_vkGetPhysicalDeviceFeatures2               vkGetPhysicalDeviceFeatures2;
    PFN_vkGetPhysicalDeviceProperties2             vkGetPhysicalDeviceProperties2;
    PFN_vkGetPhysicalDeviceMemoryProperties2       vkGetPhysicalDeviceMemoryProperties2;
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR       vkGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR  vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR       vkGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR  vkGetPhysicalDeviceSurfacePresentModesKHR;
    PFN_vkCreateDevice                             vkCreateDevice;

public:
    bool Init(VkInstance instance, PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr) noexcept
    {
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceMemoryProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceQueueFamilyProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkEnumerateDeviceExtensionProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK_VAR(instance, vkGetPhysicalDeviceFeatures2, "vkGetPhysicalDeviceFeatures2KHR");
        ASSIGN_INSTANCE_PROC_ADDR_CHECK_VAR(instance, vkGetPhysicalDeviceProperties2, "vkGetPhysicalDeviceProperties2KHR");
        ASSIGN_INSTANCE_PROC_ADDR_CHECK_VAR(instance, vkGetPhysicalDeviceMemoryProperties2, "vkGetPhysicalDeviceMemoryProperties2KHR");
        ASSIGN_INSTANCE_PROC_ADDR_OPTIONAL(instance, vkGetPhysicalDeviceSurfaceCapabilities2KHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceSurfaceSupportKHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceSurfaceFormatsKHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceSurfacePresentModesKHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkCreateDevice);
        return true;
    }
};

//-----------------------------------------------------------------------------
struct VKMainInstance {
    PFN_vkDestroyInstance          vkDestroyInstance;
    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
    PFN_vkDestroySurfaceKHR        vkDestroySurfaceKHR;

    // Debug utils functions
    PFN_vkCreateDebugUtilsMessengerEXT  vkCreateDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
    PFN_vkSetDebugUtilsObjectNameEXT    vkSetDebugUtilsObjectNameEXT;

public:
    bool Init(VkInstance instance, PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr) noexcept
    {
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkDestroyInstance);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkEnumeratePhysicalDevices);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkDestroySurfaceKHR);
        ASSIGN_INSTANCE_PROC_ADDR_OPTIONAL(instance, vkCreateDebugUtilsMessengerEXT);
        ASSIGN_INSTANCE_PROC_ADDR_OPTIONAL(instance, vkDestroyDebugUtilsMessengerEXT);
        ASSIGN_INSTANCE_PROC_ADDR_OPTIONAL(instance, vkSetDebugUtilsObjectNameEXT);
        return true;
    }
};

//-----------------------------------------------------------------------------
struct VKMainCommandList {
    PFN_vkCmdCopyImage2           vkCmdCopyImage2;
    PFN_vkCmdCopyBufferToImage2   vkCmdCopyBufferToImage2;
    PFN_vkCmdCopyImageToBuffer2   vkCmdCopyImageToBuffer2;
    PFN_vkBeginCommandBuffer      vkBeginCommandBuffer;
    PFN_vkCmdSetScissor           vkCmdSetScissor;
    PFN_vkCmdSetViewport          vkCmdSetViewport;
    PFN_vkCmdBindPipeline         vkCmdBindPipeline;
    PFN_vkCmdBindDescriptorSets   vkCmdBindDescriptorSets;
    PFN_vkCmdDispatch             vkCmdDispatch;
    PFN_vkCmdDraw                 vkCmdDraw;
    PFN_vkCmdDrawIndexed          vkCmdDrawIndexed;
    PFN_vkResetCommandBuffer      vkResetCommandBuffer;
    PFN_vkCmdCopyBuffer           vkCmdCopyBuffer;
    PFN_vkCmdBindIndexBuffer      vkCmdBindIndexBuffer;
    PFN_vkCmdPushConstants        vkCmdPushConstants;
    PFN_vkCmdPipelineBarrier2     vkCmdPipelineBarrier2;
    PFN_vkCmdBeginRendering       vkCmdBeginRendering;
    PFN_vkCmdEndRendering         vkCmdEndRendering;
    PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology;
    PFN_vkCmdBindVertexBuffers2   vkCmdBindVertexBuffers2;
    PFN_vkCmdPushDescriptorSet    vkCmdPushDescriptorSet;
    PFN_vkCmdBindIndexBuffer2     vkCmdBindIndexBuffer2;
    PFN_vkEndCommandBuffer        vkEndCommandBuffer;

public:
    bool Init(VkDevice device, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr) noexcept
    {
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkCmdCopyImage2, "vkCmdCopyImage2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkCmdCopyBufferToImage2, "vkCmdCopyBufferToImage2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkCmdCopyImageToBuffer2, "vkCmdCopyImageToBuffer2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkBeginCommandBuffer);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdSetScissor);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdSetViewport);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdBindPipeline);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdBindDescriptorSets);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdDispatch);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdDraw);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdDrawIndexed);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkResetCommandBuffer);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdCopyBuffer);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdBindIndexBuffer);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdPushConstants);
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkCmdPipelineBarrier2, "vkCmdPipelineBarrier2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkCmdBeginRendering, "vkCmdBeginRenderingKHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkCmdEndRendering, "vkCmdEndRenderingKHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkCmdSetPrimitiveTopology, "vkCmdSetPrimitiveTopologyEXT");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkCmdBindVertexBuffers2, "vkCmdBindVertexBuffers2EXT");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkCmdPushDescriptorSet, "vkCmdPushDescriptorSetKHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkCmdBindIndexBuffer2, "vkCmdBindIndexBuffer2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkEndCommandBuffer);
        return true;
    }
};

struct VKMainCommandQueue {
    PFN_vkQueueSubmit     vkQueueSubmit;
    PFN_vkQueueSubmit2    vkQueueSubmit2;
    PFN_vkQueueWaitIdle   vkQueueWaitIdle;
    PFN_vkQueuePresentKHR vkQueuePresentKHR;

public:
    bool Init(VkDevice device, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr) noexcept
    {
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkQueueSubmit);
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkQueueSubmit2, "vkQueueSubmit2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkQueueWaitIdle);
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkQueuePresentKHR);
        return true;
    }
};

//-----------------------------------------------------------------------------
struct VKMainDevice {
    PFN_vkUnmapMemory                       vkUnmapMemory;
    PFN_vkDestroyDevice                     vkDestroyDevice;
    PFN_vkCreateSampler                     vkCreateSampler;
    PFN_vkFlushMappedMemoryRanges           vkFlushMappedMemoryRanges;
    PFN_vkAllocateMemory                    vkAllocateMemory;
    PFN_vkDestroyPipeline                   vkDestroyPipeline;
    PFN_vkCreateImage                       vkCreateImage;
    PFN_vkCreateDescriptorSetLayout         vkCreateDescriptorSetLayout;
    PFN_vkFreeMemory                        vkFreeMemory;
    PFN_vkMapMemory                         vkMapMemory;
    PFN_vkDestroyDescriptorSetLayout        vkDestroyDescriptorSetLayout;
    PFN_vkInvalidateMappedMemoryRanges      vkInvalidateMappedMemoryRanges;
    PFN_vkBindBufferMemory                  vkBindBufferMemory;
    PFN_vkBindImageMemory                   vkBindImageMemory;
    PFN_vkGetBufferMemoryRequirements       vkGetBufferMemoryRequirements;
    PFN_vkDestroySampler                    vkDestroySampler;
    PFN_vkDestroyImageView                  vkDestroyImageView;
    PFN_vkGetImageMemoryRequirements        vkGetImageMemoryRequirements;
    PFN_vkCreateFence                       vkCreateFence;
    PFN_vkDestroyFence                      vkDestroyFence;
    PFN_vkCreateCommandPool                 vkCreateCommandPool;
    PFN_vkResetFences                       vkResetFences;
    PFN_vkWaitForFences                     vkWaitForFences;
    PFN_vkCreateSemaphore                   vkCreateSemaphore;
    PFN_vkDestroyBuffer                     vkDestroyBuffer;
    PFN_vkDestroySemaphore                  vkDestroySemaphore;
    PFN_vkCreateBuffer                      vkCreateBuffer;
    PFN_vkDestroyImage                      vkDestroyImage;
    PFN_vkCreatePipelineLayout              vkCreatePipelineLayout;
    PFN_vkCreateImageView                   vkCreateImageView;
    PFN_vkDestroyCommandPool                vkDestroyCommandPool;
    PFN_vkCreateDescriptorPool              vkCreateDescriptorPool;
    PFN_vkAllocateCommandBuffers            vkAllocateCommandBuffers;
    PFN_vkDestroyPipelineLayout             vkDestroyPipelineLayout;
    PFN_vkCreateShaderModule                vkCreateShaderModule;
    PFN_vkDestroyShaderModule               vkDestroyShaderModule;
    PFN_vkCreateGraphicsPipelines           vkCreateGraphicsPipelines;
    PFN_vkCreateComputePipelines            vkCreateComputePipelines;
    PFN_vkDestroyDescriptorPool             vkDestroyDescriptorPool;
    PFN_vkAllocateDescriptorSets            vkAllocateDescriptorSets;
    PFN_vkUpdateDescriptorSets              vkUpdateDescriptorSets;
    PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements;
    PFN_vkGetDeviceImageMemoryRequirements  vkGetDeviceImageMemoryRequirements;
    PFN_vkGetDeviceQueue2                   vkGetDeviceQueue2;
    PFN_vkGetImageMemoryRequirements2       vkGetImageMemoryRequirements2;
    PFN_vkGetBufferMemoryRequirements2      vkGetBufferMemoryRequirements2;
    PFN_vkBindBufferMemory2                 vkBindBufferMemory2;
    PFN_vkBindImageMemory2                  vkBindImageMemory2;
    PFN_vkWaitSemaphores                    vkWaitSemaphores;
    PFN_vkSignalSemaphore                   vkSignalSemaphore;
    PFN_vkGetSemaphoreCounterValue          vkGetSemaphoreCounterValue;
    PFN_vkGetBufferDeviceAddress            vkGetBufferDeviceAddress;
    PFN_vkCreateSwapchainKHR                vkCreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR               vkDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR             vkGetSwapchainImagesKHR;
    PFN_vkAcquireNextImageKHR               vkAcquireNextImageKHR;
    PFN_vkWaitForPresent2KHR                vkWaitForPresent2KHR;
    PFN_vkWaitForPresentKHR                 vkWaitForPresentKHR;

public:
    bool Init(VkDevice device, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr) noexcept
    {
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkUnmapMemory);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyDevice);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateSampler);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkFlushMappedMemoryRanges);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkAllocateMemory);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyPipeline);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateImage);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateDescriptorSetLayout);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkFreeMemory);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkMapMemory);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyDescriptorSetLayout);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkInvalidateMappedMemoryRanges);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkBindBufferMemory);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkBindImageMemory);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkGetBufferMemoryRequirements);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroySampler);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyImageView);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkGetImageMemoryRequirements);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateFence);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyFence);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateCommandPool);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkResetFences);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkWaitForFences);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateSemaphore);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyBuffer);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroySemaphore);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateBuffer);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyImage);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreatePipelineLayout);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateImageView);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyCommandPool);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateDescriptorPool);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkAllocateCommandBuffers);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyPipelineLayout);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateShaderModule);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyShaderModule);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateGraphicsPipelines);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateComputePipelines);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyDescriptorPool);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkAllocateDescriptorSets);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkUpdateDescriptorSets);
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkGetDeviceBufferMemoryRequirements, "vkGetDeviceBufferMemoryRequirementsKHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkGetDeviceImageMemoryRequirements, "vkGetDeviceImageMemoryRequirementsKHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkGetDeviceQueue2);
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkGetImageMemoryRequirements2, "vkGetImageMemoryRequirements2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkGetBufferMemoryRequirements2, "vkGetBufferMemoryRequirements2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkBindBufferMemory2, "vkBindBufferMemory2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkBindImageMemory2, "vkBindImageMemory2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkWaitSemaphores, "vkWaitSemaphoresKHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkSignalSemaphore, "vkSignalSemaphoreKHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkGetSemaphoreCounterValue, "vkGetSemaphoreCounterValueKHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkGetBufferDeviceAddress, "vkGetBufferDeviceAddressKHR", "vkGetBufferDeviceAddressEXT");
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkCreateSwapchainKHR);
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkDestroySwapchainKHR);
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkGetSwapchainImagesKHR);
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkAcquireNextImageKHR);
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkWaitForPresent2KHR);
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkWaitForPresentKHR);
        return true;
    }
};
} // namespace impl
} // namespace wis

#endif // !WIS_VK_TABLES_HPP