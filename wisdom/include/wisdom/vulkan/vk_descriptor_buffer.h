#pragma once
#include <wisdom/vulkan/vk_resource.h>

namespace wis {
class VKDescriptorBuffer;

template<>
struct Internal<VKDescriptorBuffer> {
    wis::shared_handle<VmaAllocator> allocator;
    h::VmaAllocation allocation;
    h::VkBuffer buffer;
    VkDeviceAddress address = 0;
    uint8_t* data = nullptr;
    VkPhysicalDeviceDescriptorBufferPropertiesEXT* properties = nullptr;


    wis::DescriptorHeapType type = wis::DescriptorHeapType::Descriptor;
    uint32_t descriptor_offset = 0;
    uint32_t descriptor_size = 0;


    Internal() noexcept = default;
    Internal(wis::shared_handle<VmaAllocator> allocator, VkBuffer buffer, VmaAllocation allocation, wis::DescriptorHeapType type, VkPhysicalDeviceDescriptorBufferPropertiesEXT& properties, uint32_t descriptor_size, uint32_t descriptor_offset) noexcept
        : allocator(std::move(allocator)), allocation(allocation), buffer(buffer), type(type), properties(&properties), descriptor_offset(descriptor_offset), descriptor_size(descriptor_size)
    {
        if (buffer) {
            auto& device = this->allocator.header();
            VkBufferDeviceAddressInfo info{
                .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
                .buffer = buffer
            };
            address = device.table().vkGetBufferDeviceAddress(device.get(), &info);

            vmaMapMemory(this->allocator.get(), allocation, reinterpret_cast<void**>(&data));
        }
    }
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&&) noexcept = default;
    ~Internal() noexcept
    {
        if (buffer && allocation) {
            vmaUnmapMemory(allocator.get(), allocation);
            vmaDestroyBuffer(allocator.get(), buffer, allocation);
        }
    }
};

class VKDescriptorBuffer : public QueryInternal<VKDescriptorBuffer>
{
public:
    VKDescriptorBuffer() noexcept = default;
    explicit VKDescriptorBuffer(wis::shared_handle<VmaAllocator> allocator, 
        VkBuffer buffer, 
        VmaAllocation allocation, 
        wis::DescriptorHeapType type, 
        VkPhysicalDeviceDescriptorBufferPropertiesEXT& properties,
        uint32_t descriptor_size, 
        uint32_t descriptor_offset) noexcept
        : QueryInternal(std::move(allocator), buffer, allocation, type, properties, descriptor_size, descriptor_offset) { }
    operator bool() const noexcept { return bool(buffer); }
    operator VKDescriptorBufferView() const noexcept
    {
        return { address, type, descriptor_size, descriptor_offset };
    }

public:
    void WriteSampler(uint32_t index, wis::VKSamplerView sampler) noexcept
    {
        auto& device = allocator.header();
        VkDescriptorGetInfoEXT info{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT,
            .type = VK_DESCRIPTOR_TYPE_SAMPLER,
            .data = { .pSampler = &std::get<0>(sampler) }
        };

        device.table().vkGetDescriptorEXT(device.get(), &info, descriptor_size, data + index * descriptor_size);
    }
    void WriteShaderResource(uint32_t index, wis::VKShaderResourceView resource) noexcept
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
    void WriteConstantBuffer(uint32_t index, wis::VKBufferView buffer, uint32_t buffer_size) noexcept
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
};
} // namespace wis