#ifndef WISDOM_VK_EXT1_HPP
#define WISDOM_VK_EXT1_HPP

#include <wisdom/vulkan/vk_extensions.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <algorithm>

namespace wis::impl {
struct VKDeviceImpl;
}

namespace wis::detail {
struct DeviceExtension1 : VKDeviceExtensionImpl<DeviceExtension1> {
    VKDeviceFeatures features;

public:
    ::WisResult CollectInfo(VKDeviceExtensionCollector& collector) noexcept
    {
        // Optional extensions
        if (collector.IsExtensionPresent(VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME)) {
            features.has_custom_border_color = true;

            collector.EnableExtension({
                    .name                 = VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME,
                    .feature_struct       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT,
                    .feature_struct_size  = sizeof(VkPhysicalDeviceCustomBorderColorFeaturesEXT),
                    .property_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT,
                    .property_struct_size = sizeof(VkPhysicalDeviceCustomBorderColorPropertiesEXT),
            });
        }

        if (collector.IsExtensionPresent(VK_KHR_MAINTENANCE_4_EXTENSION_NAME)) {
            collector.EnableExtension({
                    .name                 = VK_KHR_MAINTENANCE_4_EXTENSION_NAME,
                    .feature_struct       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES,
                    .feature_struct_size  = sizeof(VkPhysicalDeviceMaintenance4Features),
                    .property_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_PROPERTIES,
                    .property_struct_size = sizeof(VkPhysicalDeviceMaintenance4Properties),
            });
        }

        if (collector.IsExtensionPresent(VK_KHR_MAINTENANCE_5_EXTENSION_NAME)) {
            features.index_buffer_range = true;
            collector.EnableExtension({
                    .name                 = VK_KHR_MAINTENANCE_5_EXTENSION_NAME,
                    .feature_struct       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_5_FEATURES_KHR,
                    .feature_struct_size  = sizeof(VkPhysicalDeviceMaintenance5FeaturesKHR),
                    .property_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_5_PROPERTIES_KHR,
                    .property_struct_size = sizeof(VkPhysicalDeviceMaintenance5PropertiesKHR),
            });
        }

        if (collector.IsExtensionPresent(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)) {
            features.dynamic_rendering = true;
            collector.EnableExtension({
                    .name                = VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
                    .feature_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES,
                    .feature_struct_size = sizeof(VkPhysicalDeviceDynamicRenderingFeatures),
            });
        }

        if (collector.IsExtensionPresent(VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME)) {
            features.extended_dynamic_state = true;
            collector.EnableExtension({
                    .name                = VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME,
                    .feature_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT,
                    .feature_struct_size = sizeof(VkPhysicalDeviceExtendedDynamicState2FeaturesEXT),
            });
        }

        if (collector.IsExtensionPresent(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)) {
            features.synchronization_2 = true;
            collector.EnableExtension({
                    .name                = VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
                    .feature_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES,
                    .feature_struct_size = sizeof(VkPhysicalDeviceSynchronization2FeaturesKHR),
            });
        }

        if (collector.IsExtensionPresent(VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME)) {
            features.dynamic_render_unused_attachments = true;
            collector.EnableExtension({
                    .name                = VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME,
                    .feature_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_FEATURES_EXT,
                    .feature_struct_size = sizeof(VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT),
            });
        }

        if (collector.IsExtensionPresent(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME)) {
            features.push_descriptor = true;
            collector.EnableExtension({
                    .name                 = VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME,
                    .property_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES,
                    .property_struct_size = sizeof(VkPhysicalDevicePushDescriptorPropertiesKHR),
            });
        }

        // Descriptor heap extension is required for bindless support, so we enable it if present
        if (collector.IsExtensionPresent(VK_EXT_DESCRIPTOR_HEAP_EXTENSION_NAME)) {
            features.descriptor_heap = true;
            collector.EnableExtension({
                    .name                 = VK_EXT_DESCRIPTOR_HEAP_EXTENSION_NAME,
                    .feature_struct       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_HEAP_FEATURES_EXT,
                    .feature_struct_size  = sizeof(VkPhysicalDeviceDescriptorHeapFeaturesEXT),
                    .property_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_HEAP_PROPERTIES_EXT,
                    .property_struct_size = sizeof(VkPhysicalDeviceDescriptorHeapPropertiesEXT),
            });
        }

        // Global priority queues
        if (collector.IsExtensionPresent(VK_KHR_GLOBAL_PRIORITY_EXTENSION_NAME)) {
            features.global_priority = true;
            collector.EnableExtension({
                    .name                = VK_KHR_GLOBAL_PRIORITY_EXTENSION_NAME,
                    .feature_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GLOBAL_PRIORITY_QUERY_FEATURES_KHR,
                    .feature_struct_size = sizeof(VkPhysicalDeviceGlobalPriorityQueryFeatures),
            });
        }

        // Host copy support
        if (collector.IsExtensionPresent(VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME)) {
            features.host_image_copy = true;
            collector.EnableExtension({
                    .name                = VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME,
                    .feature_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_FEATURES_EXT,
                    .feature_struct_size = sizeof(VkPhysicalDeviceHostImageCopyFeaturesEXT),
            });
        }

        return vk_success;
    }
    ::WisResult Init([[maybe_unused]] const impl::VKDeviceImpl& device_impl,
                     const VKDeviceExtensionCollector&          collector) noexcept
    {
        if (features.descriptor_heap) {
            // Descriptor heap properties
            auto& descriptor_heap_properties = *collector.GetEnabledPropertyStruct<VkPhysicalDeviceDescriptorHeapPropertiesEXT>(
                    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_HEAP_PROPERTIES_EXT);

            // A lot of space is going to be wasted, but the usage will be simpler and more efficient if we use the same size for both resource and sampler descriptors, so we take the max of the two alignments as the descriptor size
            features.resource_desc_size = static_cast<uint16_t>(
                    std::max(descriptor_heap_properties.imageDescriptorAlignment,
                             descriptor_heap_properties.bufferDescriptorAlignment));
            features.sampler_desc_size                   = static_cast<uint16_t>(descriptor_heap_properties.samplerDescriptorAlignment);
            features.descriptor_heap_alignment           = static_cast<uint32_t>(descriptor_heap_properties.resourceHeapAlignment);
            features.sampler_heap_alignment              = static_cast<uint32_t>(descriptor_heap_properties.samplerHeapAlignment);
            features.min_descriptor_heap_size            = static_cast<uint32_t>(descriptor_heap_properties.minResourceHeapReservedRange);
            features.min_sampler_heap_size               = static_cast<uint32_t>(descriptor_heap_properties.minSamplerHeapReservedRangeWithEmbedded);
            features.max_descriptor_heap_size            = descriptor_heap_properties.maxResourceHeapSize;
            features.max_sampler_heap_size               = descriptor_heap_properties.maxSamplerHeapSize;
            features.max_sampler_heap_size_with_embedded = descriptor_heap_properties.maxSamplerHeapSize - descriptor_heap_properties.minSamplerHeapReservedRangeWithEmbedded;
        }

        // Nothing to initialize for now
        return vk_success;
    }
};
} // namespace wis::detail

#endif // WISDOM_VK_EXT1_HPP
