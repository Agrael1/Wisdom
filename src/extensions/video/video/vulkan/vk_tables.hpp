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

public:
    bool Init(
        VkDevice device,
        PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr,
        VkInstance instance,
        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr
    ) noexcept
    {
        ASSIGN_INSTANCE_PROC_ADDR_CHECK(instance, vkGetPhysicalDeviceVideoCapabilitiesKHR);
        // ASSIGN_DEVICE_PROC_ADDR_CHECK(device, vkCreateAccelerationStructureKHR);
        return true;
    }
};
} // namespace impl
} // namespace wis

#endif // !WIS_VK_VIDEO_TABLES_HPP
