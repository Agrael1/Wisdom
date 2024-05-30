#ifndef VK_DESCRIPTOR_BUFFER_CPP
#define VK_DESCRIPTOR_BUFFER_CPP
#include <wisdom/vulkan/vk_descriptor_buffer.h>

void wis::VKDescriptorBuffer::WriteSampler(uint32_t index, wis::VKSamplerView sampler) noexcept
{
    auto& device = allocator.header();
    VkDescriptorGetInfoEXT info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT,
        .type = VK_DESCRIPTOR_TYPE_SAMPLER,
        .data = { .pSampler = &std::get<0>(sampler) }
    };

    device.table().vkGetDescriptorEXT(device.get(), &info, descriptor_size, data + index * descriptor_size);
}

void wis::VKDescriptorBuffer::WriteShaderResource2(uint32_t index, wis::VKShaderResourceView resource) noexcept
{
    auto& device = allocator.header();
    VkDescriptorImageInfo image_info{
        .sampler = nullptr,
        .imageView = std::get<0>(resource),
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };

    VkDescriptorGetInfoEXT info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT,
        .type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        .data = { .pSampledImage = &image_info }
    };
    device.table().vkGetDescriptorEXT(device.get(), &info, properties->sampledImageDescriptorSize, data + index * descriptor_size);
}

uint64_t wis::VKDescriptorBuffer::WriteShaderResource(uint64_t buffer_offset_before_table, uint32_t root_table_index, uint32_t binding, uint32_t array_member, wis::VKRootSignatureView2 root_signature, wis::VKShaderResourceView resource) noexcept
{
    auto& device = allocator.header();
    VkDescriptorImageInfo image_info{
        .sampler = nullptr,
        .imageView = std::get<0>(resource),
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };

    VkDescriptorGetInfoEXT info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT,
        .type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        .data = { .pSampledImage = &image_info }
    };

    return WriteDescriptor(buffer_offset_before_table, root_table_index, binding, array_member, info, root_signature);
}

uint64_t wis::VKDescriptorBuffer::WriteConstantBuffer(uint64_t buffer_offset_before_table,
        uint32_t root_table_index,
        uint32_t binding,
        uint32_t array_member,
        wis::VKRootSignatureView2 root_signature,
        wis::VKBufferView buffer,
        uint32_t buffer_size) noexcept
{
    auto& device = allocator.header();
    VkBufferDeviceAddressInfo address_info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .buffer = std::get<0>(buffer)
    };

    VkDescriptorAddressInfoEXT buffer_info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_ADDRESS_INFO_EXT,
        .address = device.table().vkGetBufferDeviceAddress(device.get(), &address_info),
        .range = buffer_size
    };

    VkDescriptorGetInfoEXT info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT,
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .data = { .pUniformBuffer = &buffer_info }
    };

    return WriteDescriptor(buffer_offset_before_table, root_table_index, binding, array_member, info, root_signature);
}

uint64_t wis::VKDescriptorBuffer::WriteDescriptor(uint64_t buffer_offset_before_table,
        uint32_t root_table_index,
        uint32_t binding,
        uint32_t array_member,
        const VkDescriptorGetInfoEXT& info,
        wis::VKRootSignatureView2 root_signature) noexcept
{
    auto& device = allocator.header();
    auto& dtable = device.table();

    auto* descriptor_layouts = std::get<0>(root_signature);
    VkDeviceSize descriptor_offsetx = 0;
    VkDeviceSize descriptor_sizex = std::max(descriptor_size, uint32_t(properties->sampledImageDescriptorSize));
    dtable.vkGetDescriptorSetLayoutBindingOffsetEXT(device.get(), descriptor_layouts[root_table_index], binding, &descriptor_offsetx);
    dtable.vkGetDescriptorEXT(device.get(), &info, properties->sampledImageDescriptorSize, data + buffer_offset_before_table + descriptor_offsetx + array_member * descriptor_sizex);
    return buffer_offset_before_table + descriptor_offsetx + descriptor_sizex + array_member * descriptor_sizex;
}

void wis::VKDescriptorBuffer::WriteConstantBuffer2(uint32_t index, wis::VKBufferView buffer, uint32_t buffer_size) noexcept
{
    auto& device = allocator.header();
    VkBufferDeviceAddressInfo address_info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .buffer = std::get<0>(buffer)
    };

    VkDescriptorAddressInfoEXT buffer_info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_ADDRESS_INFO_EXT,
        .address = device.table().vkGetBufferDeviceAddress(device.get(), &address_info),
        .range = buffer_size
    };

    VkDescriptorGetInfoEXT info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT,
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .data = { .pUniformBuffer = &buffer_info }
    };
    device.table().vkGetDescriptorEXT(device.get(), &info, properties->uniformBufferDescriptorSize, data + index * descriptor_size);
}
#endif // ! VK_DESCRIPTOR_BUFFER_CPP
