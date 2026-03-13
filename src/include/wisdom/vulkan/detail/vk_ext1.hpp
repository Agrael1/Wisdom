#ifndef WISDOM_VK_EXT1_HPP
#define WISDOM_VK_EXT1_HPP

#include <wisdom/vulkan/vk_extensions.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/util/allocation.hpp>
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

        if (collector.IsExtensionPresent(VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME)) {
            features.dynamic_render_unused_attachments = true;
            collector.EnableExtension({
                    .name                = VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME,
                    .feature_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_FEATURES_EXT,
                    .feature_struct_size = sizeof(VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT),
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

        // maintenance 9
        if (collector.IsExtensionPresent(VK_KHR_MAINTENANCE_9_EXTENSION_NAME)) {
            features.maintenance9 = true;
            collector.EnableExtension({
                    .name                 = VK_KHR_MAINTENANCE_9_EXTENSION_NAME,
                    .feature_struct       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_9_FEATURES_KHR,
                    .feature_struct_size  = sizeof(VkPhysicalDeviceMaintenance9FeaturesKHR),
                    .property_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_9_PROPERTIES_KHR,
                    .property_struct_size = sizeof(VkPhysicalDeviceMaintenance9PropertiesKHR),
            });
        }

        // Line rasterization
        if (collector.IsExtensionPresent(VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME)) {
            features.line_rasterization = true;
            collector.EnableExtension({
                    .name                = VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME,
                    .feature_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_FEATURES_EXT,
                    .feature_struct_size = sizeof(VkPhysicalDeviceLineRasterizationFeaturesEXT),
            });
        }

        // Conservative rasterization
        if (collector.IsExtensionPresent(VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME)) {
            features.conservative_rasterization = true;
            collector.EnableExtension({
                    .name                = VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME,
                    .property_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT,
                    .property_struct_size = sizeof(VkPhysicalDeviceConservativeRasterizationPropertiesEXT),
            });
        }

        return wis::detail::vk_success;
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
            features.sampler_desc_size                        = static_cast<uint16_t>(descriptor_heap_properties.samplerDescriptorAlignment);
            features.max_root_space                           = static_cast<uint16_t>(descriptor_heap_properties.maxPushDataSize);
            features.descriptor_heap_reserved_size            = wis::aligned_size(static_cast<uint32_t>(descriptor_heap_properties.minResourceHeapReservedRange), features.resource_desc_size);
            features.sampler_heap_reserved_size               = wis::aligned_size(static_cast<uint32_t>(descriptor_heap_properties.minSamplerHeapReservedRange), features.sampler_desc_size);
            features.sampler_heap_reserved_size_with_embedded = wis::aligned_size(static_cast<uint32_t>(descriptor_heap_properties.minSamplerHeapReservedRangeWithEmbedded), features.sampler_desc_size);
            features.descriptor_heap_alignment                = static_cast<uint32_t>(descriptor_heap_properties.resourceHeapAlignment);
            features.sampler_heap_alignment                   = static_cast<uint32_t>(descriptor_heap_properties.samplerHeapAlignment);
            features.max_descriptor_heap_size                 = descriptor_heap_properties.maxResourceHeapSize;
            features.max_sampler_heap_size                    = descriptor_heap_properties.maxSamplerHeapSize;
        }

        // Get Device properties
        auto& device_properties = *collector.GetEnabledPropertyStruct<VkPhysicalDeviceProperties2>(
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2);
        features.max_vertex_attributes = static_cast<uint8_t>(device_properties.properties.limits.maxVertexInputAttributes);
        features.max_vertex_bindings   = static_cast<uint8_t>(device_properties.properties.limits.maxVertexInputBindings);

        // Nothing to initialize for now
        return wis::detail::vk_success;
    }
};
} // namespace wis::detail

#endif // WISDOM_VK_EXT1_HPP
