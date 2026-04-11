#ifndef WIS_VK_TABLES_HPP
#define WIS_VK_TABLES_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus
#include <wisdom/vulkan/vk_loader.hpp>

#include <vulkan/vulkan.h>

#ifndef VK_KHR_device_address_commands
#    define VK_KHR_DEVICE_ADDRESS_COMMANDS_EXTENSION_NAME "VK_KHR_device_address_commands"
typedef struct VkPhysicalDeviceDeviceAddressCommandsFeaturesKHR {
    VkStructureType sType;
    void* pNext;
    VkBool32 deviceAddressCommands;
} VkPhysicalDeviceDeviceAddressCommandsFeaturesKHR;

static constexpr VkStructureType
VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_ADDRESS_COMMANDS_FEATURES_KHR = VkStructureType(1000318006);
static constexpr VkStructureType VK_STRUCTURE_TYPE_BIND_VERTEX_BUFFER_3_INFO_KHR = VkStructureType(1000318008);
static constexpr VkStructureType VK_STRUCTURE_TYPE_BIND_INDEX_BUFFER_3_INFO_KHR = VkStructureType(1000318007);

typedef VkFlags VkAddressCommandFlagsKHR;

typedef struct VkDeviceAddressRangeKHR {
    VkDeviceAddress address;
    VkDeviceSize size;
} VkDeviceAddressRangeKHR;

typedef struct VkBindVertexBuffer3InfoKHR {
    VkStructureType sType;
    const void* pNext;
    VkBool32 setStride;
    VkStridedDeviceAddressRangeKHR addressRange;
    VkAddressCommandFlagsKHR addressFlags;
} VkBindVertexBuffer3InfoKHR;

typedef struct VkBindIndexBuffer3InfoKHR {
    VkStructureType sType;
    const void* pNext;
    VkDeviceAddressRangeKHR addressRange;
    VkAddressCommandFlagsKHR addressFlags;
    VkIndexType indexType;
} VkBindIndexBuffer3InfoKHR;

using PFN_vkCmdBindVertexBuffers3KHR = void (*)(
        VkCommandBuffer commandBuffer,
        uint32_t firstBinding,
        uint32_t bindingCount,
        const VkBindVertexBuffer3InfoKHR* pBindingInfos
                                       );
using PFN_vkCmdBindIndexBuffer3KHR = void (*)(VkCommandBuffer commandBuffer, const VkBindIndexBuffer3InfoKHR* pInfo);

#endif // VK_KHR_device_address_commands

namespace wis {
namespace impl {
//----------------------------------------------------------------------------------------------------------------------
struct VKMainGlobal {
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
    PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    PFN_vkCreateInstance vkCreateInstance;
    PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;

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

//----------------------------------------------------------------------------------------------------------------------
struct VKMainAdapter {
    PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
    PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2;
    PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
    PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2;
    PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
    PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2;
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
    PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
    PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
    PFN_vkCreateDevice vkCreateDevice;

public:
    bool Init(VkInstance instance, PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr) noexcept
    {
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceMemoryProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceQueueFamilyProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK_VAR(
            instance,
            vkGetPhysicalDeviceQueueFamilyProperties2,
            "vkGetPhysicalDeviceQueueFamilyProperties2KHR"
        );
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkEnumerateDeviceExtensionProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK_VAR(instance, vkGetPhysicalDeviceFeatures2, "vkGetPhysicalDeviceFeatures2KHR");
        ASSIGN_INSTANCE_PROC_ADDR_CHECK_VAR(
            instance,
            vkGetPhysicalDeviceProperties2,
            "vkGetPhysicalDeviceProperties2KHR"
        );
        ASSIGN_INSTANCE_PROC_ADDR_CHECK_VAR(
            instance,
            vkGetPhysicalDeviceMemoryProperties2,
            "vkGetPhysicalDeviceMemoryProperties2KHR"
        );
        ASSIGN_INSTANCE_PROC_ADDR_OPTIONAL(instance, vkGetPhysicalDeviceSurfaceCapabilities2KHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceSurfaceSupportKHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceSurfaceFormatsKHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceSurfacePresentModesKHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceFormatProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceImageFormatProperties);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkCreateDevice);
        return true;
    }
};

//----------------------------------------------------------------------------------------------------------------------
struct VKMainInstance {
    PFN_vkDestroyInstance vkDestroyInstance;
    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
    PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;

    // Debug utils functions
    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
    PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;

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

//----------------------------------------------------------------------------------------------------------------------
struct VKMainCommandList {
    PFN_vkCmdCopyImage vkCmdCopyImage;
    PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
    PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;

    PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
    PFN_vkCmdSetScissorWithCount vkCmdSetScissorWithCount;
    PFN_vkCmdSetViewportWithCount vkCmdSetViewportWithCount;
    PFN_vkCmdBindPipeline vkCmdBindPipeline;
    PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
    PFN_vkCmdDispatch vkCmdDispatch;
    PFN_vkCmdDraw vkCmdDraw;
    PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
    PFN_vkResetCommandBuffer vkResetCommandBuffer;
    PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
    PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
    PFN_vkCmdPushConstants vkCmdPushConstants;
    PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2;
    PFN_vkCmdBeginRendering vkCmdBeginRendering;
    PFN_vkCmdEndRendering vkCmdEndRendering;
    PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology;
    PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2;
    PFN_vkCmdPushDescriptorSet vkCmdPushDescriptorSet;
    PFN_vkCmdBindIndexBuffer2 vkCmdBindIndexBuffer2;
    PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
    PFN_vkCmdSetPrimitiveRestartEnable vkCmdSetPrimitiveRestartEnable;
    PFN_vkEndCommandBuffer vkEndCommandBuffer;
    PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
    PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;

    // Descriptor heap functions
    PFN_vkCmdBindResourceHeapEXT vkCmdBindResourceHeapEXT;
    PFN_vkCmdBindSamplerHeapEXT vkCmdBindSamplerHeapEXT;
    PFN_vkCmdPushDataEXT vkCmdPushDataEXT;

    // Device address commands functions
    PFN_vkCmdBindVertexBuffers3KHR vkCmdBindVertexBuffers3KHR;
    PFN_vkCmdBindIndexBuffer3KHR vkCmdBindIndexBuffer3KHR;

public:
    bool Init(VkDevice device, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr) noexcept
    {
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdCopyImage);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdCopyBufferToImage);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdCopyImageToBuffer);

        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkBeginCommandBuffer);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdSetScissorWithCount);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdSetViewportWithCount);
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
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdSetDepthBias);
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkCmdSetPrimitiveRestartEnable, "vkCmdSetPrimitiveRestartEnableEXT");
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkEndCommandBuffer);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkFreeCommandBuffers);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCmdSetBlendConstants);

        // Descriptor heap functions (optional, since support is not wide)
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkCmdBindResourceHeapEXT);
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkCmdBindSamplerHeapEXT);
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkCmdPushDataEXT);

        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkCmdBindVertexBuffers3KHR);
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkCmdBindIndexBuffer3KHR);
        return true;
    }
};

struct VKMainCommandQueue {
    PFN_vkQueueSubmit vkQueueSubmit;
    PFN_vkQueueSubmit2 vkQueueSubmit2;
    PFN_vkQueueWaitIdle vkQueueWaitIdle;
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

//----------------------------------------------------------------------------------------------------------------------
struct VKMainDevice {
    PFN_vkUnmapMemory vkUnmapMemory;
    PFN_vkDestroyDevice vkDestroyDevice;
    PFN_vkCreateSampler vkCreateSampler;
    PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
    PFN_vkAllocateMemory vkAllocateMemory;
    PFN_vkDestroyPipeline vkDestroyPipeline;
    PFN_vkCreateImage vkCreateImage;
    PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
    PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
    PFN_vkFreeMemory vkFreeMemory;
    PFN_vkMapMemory vkMapMemory;
    PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
    PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
    PFN_vkBindBufferMemory vkBindBufferMemory;
    PFN_vkBindImageMemory vkBindImageMemory;
    PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
    PFN_vkDestroySampler vkDestroySampler;
    PFN_vkDestroyImageView vkDestroyImageView;
    PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
    PFN_vkCreateFence vkCreateFence;
    PFN_vkDestroyFence vkDestroyFence;
    PFN_vkCreateCommandPool vkCreateCommandPool;
    PFN_vkResetCommandPool vkResetCommandPool;
    PFN_vkResetFences vkResetFences;
    PFN_vkWaitForFences vkWaitForFences;
    PFN_vkCreateSemaphore vkCreateSemaphore;
    PFN_vkDestroyBuffer vkDestroyBuffer;
    PFN_vkDestroySemaphore vkDestroySemaphore;
    PFN_vkCreateBuffer vkCreateBuffer;
    PFN_vkDestroyImage vkDestroyImage;
    PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
    PFN_vkCreateImageView vkCreateImageView;
    PFN_vkDestroyCommandPool vkDestroyCommandPool;
    PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
    PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
    PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
    PFN_vkCreateShaderModule vkCreateShaderModule;
    PFN_vkDestroyShaderModule vkDestroyShaderModule;
    PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
    PFN_vkCreateComputePipelines vkCreateComputePipelines;
    PFN_vkCreatePipelineCache vkCreatePipelineCache;
    PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
    PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
    PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
    PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
    PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
    PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements;
    PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements;
    PFN_vkGetDeviceQueue2 vkGetDeviceQueue2;
    PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
    PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
    PFN_vkBindBufferMemory2 vkBindBufferMemory2;
    PFN_vkBindImageMemory2 vkBindImageMemory2;
    PFN_vkWaitSemaphores vkWaitSemaphores;
    PFN_vkSignalSemaphore vkSignalSemaphore;
    PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue;
    PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
    PFN_vkWriteResourceDescriptorsEXT vkWriteResourceDescriptorsEXT;
    PFN_vkWriteSamplerDescriptorsEXT vkWriteSamplerDescriptorsEXT;
    PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;

    // Host copy
    PFN_vkTransitionImageLayoutEXT vkTransitionImageLayoutEXT;
    PFN_vkCopyMemoryToImageEXT vkCopyMemoryToImageEXT;

#ifdef _WIN32
    PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR;
#endif //_WIN32

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
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkFreeDescriptorSets);
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
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkResetCommandPool);
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
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreatePipelineCache);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyPipelineCache);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkGetPipelineCacheData);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyDescriptorPool);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkAllocateDescriptorSets);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkUpdateDescriptorSets);
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(
            device,
            vkGetDeviceBufferMemoryRequirements,
            "vkGetDeviceBufferMemoryRequirementsKHR"
        );
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(
            device,
            vkGetDeviceImageMemoryRequirements,
            "vkGetDeviceImageMemoryRequirementsKHR"
        );
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkGetDeviceQueue2);
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkGetImageMemoryRequirements2, "vkGetImageMemoryRequirements2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkGetBufferMemoryRequirements2, "vkGetBufferMemoryRequirements2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkBindBufferMemory2, "vkBindBufferMemory2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkBindImageMemory2, "vkBindImageMemory2KHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkWaitSemaphores, "vkWaitSemaphoresKHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkSignalSemaphore, "vkSignalSemaphoreKHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(device, vkGetSemaphoreCounterValue, "vkGetSemaphoreCounterValueKHR");
        ASSIGN_DEVICE_PROC_ADDR_CHECK_VAR(
            device,
            vkGetBufferDeviceAddress,
            "vkGetBufferDeviceAddressKHR",
            "vkGetBufferDeviceAddressEXT"
        );

        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkWriteResourceDescriptorsEXT);
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkWriteSamplerDescriptorsEXT);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateSwapchainKHR);

        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkTransitionImageLayoutEXT);
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkCopyMemoryToImageEXT);

#ifdef _WIN32
        ASSIGN_DEVICE_PROC_ADDR_OPTIONAL(device, vkGetMemoryWin32HandleKHR);
#endif //_WIN32
        return true;
    }
};

struct VKMainSwapchain {
    PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
    PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
    PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
    PFN_vkQueuePresentKHR vkQueuePresentKHR; // technically a queue function, but for speed store here
    PFN_vkQueueSubmit2 vkQueueSubmit2; // technically a queue function, but for speed store here
    PFN_vkWaitForFences vkWaitForFences; // used during swapchain destruction to synchronize with the GPU
    PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR; // used for swapchain recreation, stored here for speed

    bool Init(VkDevice device, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr) noexcept
    {
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkAcquireNextImageKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkGetSwapchainImagesKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroySwapchainKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkQueuePresentKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkQueueSubmit2);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkWaitForFences);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateSwapchainKHR);
        return true;
    }
};

} // namespace impl
} // namespace wis

#endif // !WIS_VK_TABLES_HPP
