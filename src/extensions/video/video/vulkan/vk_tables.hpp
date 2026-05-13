#ifndef WIS_VK_VIDEO_TABLES_HPP
#define WIS_VK_VIDEO_TABLES_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus
#include <wisdom/vulkan/vk_loader.hpp>

#include <vulkan/vulkan.h>

namespace wis {
namespace impl {
struct VKVideoTable {
    PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR vkGetPhysicalDeviceVideoCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR vkGetPhysicalDeviceVideoFormatPropertiesKHR;
    PFN_vkCreateVideoSessionKHR vkCreateVideoSessionKHR;
    PFN_vkDestroyVideoSessionKHR vkDestroyVideoSessionKHR;
    PFN_vkGetVideoSessionMemoryRequirementsKHR vkGetVideoSessionMemoryRequirementsKHR;
    PFN_vkBindVideoSessionMemoryKHR vkBindVideoSessionMemoryKHR;
    PFN_vkCreateVideoSessionParametersKHR vkCreateVideoSessionParametersKHR;
    PFN_vkDestroyVideoSessionParametersKHR vkDestroyVideoSessionParametersKHR;

public:
    bool Init(
        VkDevice device,
        PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr,
        VkInstance instance,
        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr
    ) noexcept
    {
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceVideoCapabilitiesKHR);
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceVideoFormatPropertiesKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateVideoSessionKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyVideoSessionKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkGetVideoSessionMemoryRequirementsKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkBindVideoSessionMemoryKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateVideoSessionParametersKHR);
        ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkDestroyVideoSessionParametersKHR);
        return true;
    }
};
} // namespace impl
} // namespace wis

#endif // !WIS_VK_VIDEO_TABLES_HPP
