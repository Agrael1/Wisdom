#ifndef WIS_VK_DEVICE_EXT_H
#define WIS_VK_DEVICE_EXT_H
#include <wisdom/generated/api/api.h>
#include <wisdom/util/misc.h>
#include <wisdom/global/internal.h>
#include <tuple>
#include <span>
#include <unordered_set>
#include <unordered_map>
#include <string_view>
#include <vulkan/vulkan_core.h>

namespace wis {
class VKDevice;
}

namespace wis {
struct VKDeviceExtension {
    virtual ~VKDeviceExtension() = default;

    // Unfortunate that we have to pass all the maps and sets here, but it's the only way to get the information
    virtual bool GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
                                  std::unordered_set<std::string_view>& ext_name_set,
                                  std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                                  std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
    {
        return true;
    }
    virtual bool Supported() const noexcept
    {
        return false;
    }

    virtual wis::Result Init(const wis::VKDevice& instance,
                             const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                             const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
    {
        return {};
    }
};

// Device embedded extensions
struct VKDeviceExtensionEmbedded1;

struct XInternalFeatures {
    // Mandatory features
    bool dynamic_rendering : 1 = false;
    bool synchronization_2 : 1 = false;
    bool swapchain : 1 = false;
    bool push_descriptor : 1 = false;

    // Optional features
    bool present_wait : 1 = false;
    bool has_custom_border_color : 1 = false;
    bool extended_dynamic_state : 1 = false;
    bool interop_device : 1 = false;
    bool index_buffer_range : 1 = false;
    bool dynamic_vsync : 1 = false; // Shall remain false until the perf is not fixed
    bool dynamic_render_unused_attachments : 1 = false;
    bool multiview : 1 = false;
};

struct XBaseProperties {
    uint32_t max_ia_attributes = 0;
};

template<>
struct Internal<VKDeviceExtensionEmbedded1> {
    XInternalFeatures features;
    XBaseProperties base_properties;
};

struct VKDeviceExtensionEmbedded1 : public QueryInternalExtension<VKDeviceExtensionEmbedded1, VKDeviceExtension> {
    virtual bool GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
                                  std::unordered_set<std::string_view>& ext_name_set,
                                  std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                                  std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept override
    {
        // Optional extensions
        if (available_extensions.contains(VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME)) {
            features.has_custom_border_color = true;
            ext_name_set.insert(VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT] = sizeof(VkPhysicalDeviceCustomBorderColorFeaturesEXT);
            property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT] = sizeof(VkPhysicalDeviceCustomBorderColorPropertiesEXT);
        }

        if (available_extensions.contains(VK_KHR_MAINTENANCE_4_EXTENSION_NAME)) {
            ext_name_set.insert(VK_KHR_MAINTENANCE_4_EXTENSION_NAME);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES] = sizeof(VkPhysicalDeviceMaintenance4Features);
            property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_PROPERTIES] = sizeof(VkPhysicalDeviceMaintenance4Properties);
        }

        if (available_extensions.contains(VK_KHR_MAINTENANCE_5_EXTENSION_NAME)) {
            ext_name_set.insert(VK_KHR_MAINTENANCE_5_EXTENSION_NAME);
            features.index_buffer_range = true;
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_5_FEATURES_KHR] = sizeof(VkPhysicalDeviceMaintenance5FeaturesKHR);
            property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_5_PROPERTIES_KHR] = sizeof(VkPhysicalDeviceMaintenance5PropertiesKHR);
        }

        if (available_extensions.contains(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)) {
            features.dynamic_rendering = true;
            ext_name_set.insert(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES] = sizeof(VkPhysicalDeviceDynamicRenderingFeatures);
        }

        if (available_extensions.contains(VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME)) {
            features.extended_dynamic_state = true;
            ext_name_set.insert(VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT] = sizeof(VkPhysicalDeviceExtendedDynamicState2FeaturesEXT);
        }

        if (available_extensions.contains(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)) {
            features.synchronization_2 = true;
            ext_name_set.insert(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES] = sizeof(VkPhysicalDeviceSynchronization2FeaturesKHR);
        }

        if (available_extensions.contains(VK_KHR_SWAPCHAIN_EXTENSION_NAME)) {
            features.swapchain = true;
            ext_name_set.insert(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        }

        if (available_extensions.contains(VK_KHR_PRESENT_WAIT_EXTENSION_NAME) && available_extensions.contains(VK_KHR_PRESENT_ID_EXTENSION_NAME)) {
            features.present_wait = true;
            ext_name_set.insert(VK_KHR_PRESENT_ID_EXTENSION_NAME);
            ext_name_set.insert(VK_KHR_PRESENT_WAIT_EXTENSION_NAME);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_ID_FEATURES_KHR] = sizeof(VkPhysicalDevicePresentIdFeaturesKHR);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_WAIT_FEATURES_KHR] = sizeof(VkPhysicalDevicePresentWaitFeaturesKHR);
        }

        // REASON: Unstable extension
        // if (available_extensions.contains(VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME)) {
        //    features.dynamic_vsync = true;
        //    ext_name_set.insert(VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME);
        //    structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SWAPCHAIN_MAINTENANCE_1_FEATURES_EXT] = sizeof(VkPhysicalDeviceSwapchainMaintenance1FeaturesEXT);
        //}

        if (available_extensions.contains(VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME)) {
            features.dynamic_render_unused_attachments = true;
            ext_name_set.insert(VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_FEATURES_EXT] = sizeof(VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT);
        }

        if (available_extensions.contains(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME)) {
            features.push_descriptor = true;
            ext_name_set.insert(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME);
            property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR] = sizeof(VkPhysicalDevicePushDescriptorPropertiesKHR);
        }

        if (available_extensions.contains(VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME)) {
            features.dynamic_render_unused_attachments = true;
            ext_name_set.insert(VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_FEATURES_EXT] = sizeof(VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT);
        }

        return true;
    }
    virtual bool Supported() const noexcept override
    {
        return features.swapchain && features.synchronization_2 && features.dynamic_rendering && features.push_descriptor;
    }

    // Not supposed to use device here.
    virtual wis::Result Init(const wis::VKDevice& device,
                             const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                             const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
    {
        auto& vk_physical_device_properties = *reinterpret_cast<VkPhysicalDeviceProperties2*>(property_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2));
        base_properties.max_ia_attributes = vk_physical_device_properties.properties.limits.maxVertexInputAttributes;

        auto& vk_11_features = *reinterpret_cast<VkPhysicalDeviceVulkan11Features*>(structure_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES));
        features.multiview = vk_11_features.multiview;
        return {};
    }

    XInternalFeatures GetFeatures() const noexcept
    {
        return features;
    }
};
} // namespace wis

#endif // VK_DEVICE_EXT_H
