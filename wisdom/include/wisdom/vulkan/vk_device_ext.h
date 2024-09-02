#ifndef VK_DEVICE_EXT_H
#define VK_DEVICE_EXT_H
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
    bool descriptor_buffer : 1 = false;
    bool synchronization_2 : 1 = false;
    bool swapchain : 1 = false;
    bool present_wait : 1 = false;

    // Optional features
    bool has_custom_border_color : 1 = false;
    bool extended_dynamic_state : 1 = false;
    bool mutable_descriptor : 1 = false;
    bool interop_device : 1 = false;
};

// Lightweight struct for descriptor buffer features
struct XDescriptorBufferProperties {
    uint16_t mutable_descriptor_size = 0;
    uint16_t offset_alignment = 0;

    uint16_t constant_buffer_size = 0;
    uint16_t storage_buffer_size = 0;
    uint16_t sampled_image_size = 0;
    uint16_t storage_image_size = 0;
    uint16_t storage_texel_buffer_size = 0;
    uint16_t uniform_texel_buffer_size = 0;
    uint16_t sampler_size = 0;
};

struct XBaseProperties {
    uint32_t max_ia_attributes = 0;
};

template<>
struct Internal<VKDeviceExtensionEmbedded1> {
    XInternalFeatures features;
    XDescriptorBufferProperties descriptor_buffer_features;
    XBaseProperties base_properties;
};

struct VKDeviceExtensionEmbedded1 : public QueryInternalExtension<VKDeviceExtensionEmbedded1, VKDeviceExtension> {
    static constexpr std::array mandatory_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME, // for Swapchain
        VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME, // for barriers

        VK_KHR_MAINTENANCE_4_EXTENSION_NAME, // for Allocator

        VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME, // for PushDescriptor
        VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME, // for Tessellation control point count
        VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME, // for dynamic render pass

        VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME, // for Border Color
        VK_KHR_PRESENT_WAIT_EXTENSION_NAME, // for Present Wait
        VK_KHR_PRESENT_ID_EXTENSION_NAME, // for Present ID
    };

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

        if (available_extensions.contains(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME)) {
            features.descriptor_buffer = true;
            ext_name_set.insert(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT] = sizeof(VkPhysicalDeviceDescriptorBufferFeaturesEXT);
            property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT] = sizeof(VkPhysicalDeviceDescriptorBufferPropertiesEXT);
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

        if (available_extensions.contains(VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME)) {
            features.mutable_descriptor = true;
            ext_name_set.insert(VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_EXT] = sizeof(VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT);
        } else if (available_extensions.contains(VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME)) {
            features.mutable_descriptor = true;
            ext_name_set.insert(VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_VALVE] = sizeof(VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE);
        }

        if (available_extensions.contains(VK_KHR_PRESENT_WAIT_EXTENSION_NAME) && available_extensions.contains(VK_KHR_PRESENT_ID_EXTENSION_NAME)) {
            features.present_wait = true;
            ext_name_set.insert(VK_KHR_PRESENT_ID_EXTENSION_NAME);
            ext_name_set.insert(VK_KHR_PRESENT_WAIT_EXTENSION_NAME);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_ID_FEATURES_KHR] = sizeof(VkPhysicalDevicePresentIdFeaturesKHR);
            structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_WAIT_FEATURES_KHR] = sizeof(VkPhysicalDevicePresentWaitFeaturesKHR);
        }
        return true;
    }
    virtual bool Supported() const noexcept override
    {
        return features.swapchain && features.synchronization_2 && features.descriptor_buffer && features.dynamic_rendering;
    }

    // Not supposed to use device here.
    virtual wis::Result Init(const wis::VKDevice& device,
                             const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                             const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
    {
        auto& vk_physical_device_properties = *reinterpret_cast<VkPhysicalDeviceProperties2*>(property_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2));
        base_properties.max_ia_attributes = vk_physical_device_properties.properties.limits.maxVertexInputAttributes;

        if (features.descriptor_buffer) {
            auto& vk_descriptor_buffer_features = *reinterpret_cast<VkPhysicalDeviceDescriptorBufferFeaturesEXT*>(structure_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT));
            auto& vk_descriptor_buffer_properties = *reinterpret_cast<VkPhysicalDeviceDescriptorBufferPropertiesEXT*>(property_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT));

            descriptor_buffer_features.constant_buffer_size = uint16_t(vk_descriptor_buffer_properties.uniformBufferDescriptorSize);
            descriptor_buffer_features.storage_buffer_size = uint16_t(vk_descriptor_buffer_properties.storageBufferDescriptorSize);

            descriptor_buffer_features.sampled_image_size = uint16_t(vk_descriptor_buffer_properties.sampledImageDescriptorSize);
            descriptor_buffer_features.storage_image_size = uint16_t(vk_descriptor_buffer_properties.storageImageDescriptorSize);

            descriptor_buffer_features.storage_texel_buffer_size = uint16_t(vk_descriptor_buffer_properties.storageTexelBufferDescriptorSize);
            descriptor_buffer_features.uniform_texel_buffer_size = uint16_t(vk_descriptor_buffer_properties.uniformTexelBufferDescriptorSize);

            auto max1 = std::max(vk_descriptor_buffer_properties.uniformBufferDescriptorSize, vk_descriptor_buffer_properties.storageBufferDescriptorSize);
            auto max2 = std::max(vk_descriptor_buffer_properties.sampledImageDescriptorSize, vk_descriptor_buffer_properties.storageImageDescriptorSize);
            auto max3 = std::max(vk_descriptor_buffer_properties.storageTexelBufferDescriptorSize, vk_descriptor_buffer_properties.uniformTexelBufferDescriptorSize);

            auto max12 = std::max(max1, max2);

            descriptor_buffer_features.mutable_descriptor_size = uint16_t(std::max(max12, max3));
            descriptor_buffer_features.sampler_size = uint16_t(vk_descriptor_buffer_properties.samplerDescriptorSize);
            descriptor_buffer_features.offset_alignment = uint16_t(vk_descriptor_buffer_properties.descriptorBufferOffsetAlignment);
        }

        return {};
    }

    XInternalFeatures GetFeatures() const noexcept
    {
        return features;
    }
};
} // namespace wis

#endif // VK_DEVICE_EXT_H
