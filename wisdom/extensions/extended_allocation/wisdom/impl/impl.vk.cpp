#ifndef WISDOM_EXTENDED_ALLOCATION_VK_CPP
#define WISDOM_EXTENDED_ALLOCATION_VK_CPP
#include <wisdom/wisdom_extended_allocation.h>

#if defined(WISDOM_VULKAN)
bool wis::VKExtendedAllocation::GetExtensionInfo(const std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>& available_extensions,
                                                 std::unordered_set<std::string_view>& ext_name_set,
                                                 std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                                                 std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
{
    if (available_extensions.contains(VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME)) {
        ext_name_set.emplace(VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME);
        ext_name_set.emplace(VK_KHR_COPY_COMMANDS_2_EXTENSION_NAME); // Required for VK_EXT_HOST_IMAGE_COPY
        ext_name_set.emplace(VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME); // Required for VK_EXT_HOST_IMAGE_COPY

        structure_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_FEATURES_EXT] = sizeof(VkPhysicalDeviceHostImageCopyFeaturesEXT);
        property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_PROPERTIES_EXT] = sizeof(VkPhysicalDeviceHostImageCopyPropertiesEXT);
        return true;
    }
    return false;
}

wis::Result
wis::VKExtendedAllocation::Init(const wis::VKDevice& instance,
                                const std::unordered_map<VkStructureType, uintptr_t>& structure_map,
                                const std::unordered_map<VkStructureType, uintptr_t>& property_map) noexcept
{
    if (!structure_map.contains(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_FEATURES_EXT)) {
        return {};
    }

    VkPhysicalDeviceHostImageCopyFeaturesEXT& host_image_copy_features = *reinterpret_cast<VkPhysicalDeviceHostImageCopyFeaturesEXT*>(structure_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_FEATURES_EXT));
    VkPhysicalDeviceHostImageCopyPropertiesEXT& host_image_copy_properties = *reinterpret_cast<VkPhysicalDeviceHostImageCopyPropertiesEXT*>(property_map.at(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_PROPERTIES_EXT));

    auto& device_i = instance.GetInternal();
    device = device_i.device;
    adapter = device_i.adapter.GetInternal().adapter;

    if (!host_image_copy_features.hostImageCopy) {
        return {};
    }

    vkCopyMemoryToImageEXT = device.GetDeviceProcAddr<PFN_vkCopyMemoryToImageEXT>("vkCopyMemoryToImageEXT");
    vkTransitionImageLayoutEXT = device.GetDeviceProcAddr<PFN_vkTransitionImageLayoutEXT>("vkTransitionImageLayoutEXT");
    vkGetPhysicalDeviceImageFormatProperties2 = device.GetDeviceProcAddr<PFN_vkGetPhysicalDeviceImageFormatProperties2>("vkGetPhysicalDeviceImageFormatProperties2");
    return {};
}

wis::ResultValue<wis::VKTexture>
wis::VKExtendedAllocation::CreateTexture(const wis::VKResourceAllocator& allocator,
                                         wis::TextureDesc desc,
                                         wis::MemoryType memory,
                                         wis::MemoryFlags flags) const noexcept
{
    VkImageCreateInfo img_desc = VKResourceAllocator::VKCreateTextureDesc(desc);

    VmaAllocationCreateFlags xflags = 0;
    if (flags & wis::MemoryFlags::Mapped)
        xflags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
    if (flags & wis::MemoryFlags::DedicatedAllocation)
        xflags |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

    VmaAllocationCreateInfo alloc{
        .flags = xflags,
        .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
        .requiredFlags = wis::convert_vk(memory),
    };

    auto [res, texture] = allocator.VKCreateTexture(img_desc, alloc);
    if (res.status != wis::Status::Ok)
        return res;

    if (!(img_desc.usage & VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT)) {
        return std::move(texture);
    }

    // Transition image layout to general for host copy layouts
    auto& tex_i = texture.GetInternal();
    VkHostImageLayoutTransitionInfoEXT transition{
        .sType = VK_STRUCTURE_TYPE_HOST_IMAGE_LAYOUT_TRANSITION_INFO_EXT,
        .pNext = nullptr,
        .image = texture.GetInternal().buffer,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_GENERAL,
        .subresourceRange = {
                .aspectMask = aspect_flags(tex_i.format),
                .baseMipLevel = 0,
                .levelCount = img_desc.mipLevels,
                .baseArrayLayer = 0,
                .layerCount = img_desc.arrayLayers,
        },
    };

    auto vr = vkTransitionImageLayoutEXT(device.get(), 1, &transition);
    if (!wis::succeeded(vr))
        return wis::make_result<FUNC, "Failed to transition image layout">(vr);
    return std::move(texture);
}

wis::Result
wis::VKExtendedAllocation::WriteMemoryToSubresourceDirect(const void* host_data,
                                                          wis::VKTextureView dst_texture,
                                                          wis::TextureState initial_state,
                                                          wis::TextureRegion region) const noexcept
{
    auto aspects = aspect_flags(std::get<1>(dst_texture));

    VkMemoryToImageCopyEXT copy_region{
        .sType = VK_STRUCTURE_TYPE_MEMORY_TO_IMAGE_COPY_EXT,
        .pNext = nullptr,
        .pHostPointer = host_data,
        .memoryRowLength = {},
        .memoryImageHeight = {},
        .imageSubresource = {
                .aspectMask = aspects,
                .mipLevel = region.mip,
                .baseArrayLayer = region.array_layer,
                .layerCount = 1u,
        },
        .imageOffset = { int(region.offset.width), int(region.offset.height), int(region.offset.depth_or_layers) },
        .imageExtent = { region.size.width, region.size.height, region.size.depth_or_layers },
    };

    VkCopyMemoryToImageInfoEXT copy_info{
        .sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_IMAGE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,
        .dstImage = std::get<0>(dst_texture),
        .dstImageLayout = convert_vk(initial_state),
        .regionCount = 1,
        .pRegions = &copy_region,
    };

    auto vr = vkCopyMemoryToImageEXT(device.get(), &copy_info);
    if (!wis::succeeded(vr))
        return wis::make_result<FUNC, "Failed to copy memory to image">(vr);
    return wis::success;
}

bool wis::VKExtendedAllocation::SupportedDirectGPUUpload(wis::DataFormat format) const noexcept
{
    VkHostImageCopyDevicePerformanceQueryEXT query{
        .sType = VK_STRUCTURE_TYPE_HOST_IMAGE_COPY_DEVICE_PERFORMANCE_QUERY_EXT,
        .pNext = nullptr,
    };
    VkPhysicalDeviceImageFormatInfo2 format_info{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_FORMAT_INFO_2,
        .pNext = nullptr,
        .format = convert_vk(format),
        .type = VK_IMAGE_TYPE_2D,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT,
        .flags = 0,
    };
    VkImageFormatProperties2 format_props{
        .sType = VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2,
        .pNext = &query,
    };
    vkGetPhysicalDeviceImageFormatProperties2(adapter, &format_info, &format_props);

    return Supported() && query.optimalDeviceAccess;
}

#endif // WISDOM_VULKAN
#endif // WISDOM_EXTENDED_ALLOCATION_CPP
