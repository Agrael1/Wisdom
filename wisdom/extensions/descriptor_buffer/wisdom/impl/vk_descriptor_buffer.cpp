#ifndef WIS_VK_DESCRIPTOR_BUFFER_CPP
#define WIS_VK_DESCRIPTOR_BUFFER_CPP
#include <wisdom/vk_descriptor_buffer.h>

void wis::ImplVKDescriptorBuffer::WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::VKSamplerView sampler) noexcept
{
    auto& device = allocator.header();
    VkDescriptorGetInfoEXT info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT,
        .type = VK_DESCRIPTOR_TYPE_SAMPLER,
        .data = { .pSampler = &std::get<0>(sampler) }
    };

    uint64_t desc_offset = aligned_table_offset + index * descriptor_size;
    vkGetDescriptorEXT(device.get(), &info, descriptor_size, data + desc_offset);
}

void wis::ImplVKDescriptorBuffer::WriteShaderResource(uint64_t aligned_table_offset, uint32_t index, wis::VKShaderResourceView resource) noexcept
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

    uint64_t desc_offset = aligned_table_offset + index * descriptor_size;
    vkGetDescriptorEXT(device.get(), &info, properties.sampled_image_size, data + desc_offset);
}

void wis::ImplVKDescriptorBuffer::WriteConstantBuffer(uint64_t aligned_table_offset, uint32_t index, wis::VKBufferView buffer, uint32_t buffer_size, uint32_t offset) noexcept
{
    auto& device = allocator.header();
    VkBufferDeviceAddressInfo address_info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .buffer = std::get<0>(buffer)
    };

    VkDescriptorAddressInfoEXT buffer_info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_ADDRESS_INFO_EXT,
        .address = device.table().vkGetBufferDeviceAddress(device.get(), &address_info) + offset,
        .range = buffer_size
    };

    VkDescriptorGetInfoEXT info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT,
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .data = { .pUniformBuffer = &buffer_info }
    };
    uint64_t desc_offset = aligned_table_offset + index * descriptor_size;
    vkGetDescriptorEXT(device.get(), &info, properties.constant_buffer_size, data + desc_offset);
}
#endif // ! VK_DESCRIPTOR_BUFFER_CPP
