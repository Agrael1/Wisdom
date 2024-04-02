#pragma once
#include <wisdom/vulkan/vk_resource.h>

namespace wis {
class VKDescriptorBuffer;

template<>
struct Internal<VKDescriptorBuffer> {
    wis::shared_handle<VmaAllocator> allocator;
    h::VmaAllocation allocation;
    h::VkBuffer buffer;
    uint32_t descriptor_size = 0;

    Internal() noexcept = default;
    Internal(wis::shared_handle<VmaAllocator> allocator, VkBuffer buffer, VmaAllocation allocation, uint32_t descriptor_size) noexcept
        : allocator(std::move(allocator)), allocation(allocation), buffer(buffer), descriptor_size(descriptor_size)
    {}
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&&) noexcept = default;
    ~Internal() noexcept
    {
        if (buffer && allocation) {
            vmaDestroyBuffer(allocator.get(), buffer, allocation);
        }
    }
};

class VKDescriptorBuffer : public QueryInternal<VKDescriptorBuffer>
{
public:
    VKDescriptorBuffer() noexcept = default;
    explicit VKDescriptorBuffer(wis::shared_handle<VmaAllocator> allocator, VkBuffer buffer, VmaAllocation allocation, uint32_t descriptor_size) noexcept
        : QueryInternal(std::move(allocator), buffer, allocation, descriptor_size) { }
    operator bool() const noexcept { return bool(buffer); }
    operator VKDescriptorBufferView() const noexcept
    {
        return { buffer, descriptor_size };
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

        uint8_t* data = nullptr;
        vmaMapMemory(allocator.get(), allocation, reinterpret_cast<void**>(&data));
        device.table().vkGetDescriptorEXT(device.get(), &info, descriptor_size, data + index * descriptor_size);
        vmaUnmapMemory(allocator.get(), allocation);
    }
    void WriteShaderResource(uint32_t index, wis::VKShaderResourceView resource) noexcept
    {
        auto& device = allocator.header();
        VkDescriptorImageInfo image_info{
            .sampler = nullptr,
            .imageView = std::get<0>(resource),
            .imageLayout = VK_IMAGE_LAYOUT_UNDEFINED
        };

        VkDescriptorGetInfoEXT info{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT,
            .type = VK_DESCRIPTOR_TYPE_MUTABLE_VALVE,
            .data = { .pSampledImage = &image_info }
        };

        uint8_t* data = nullptr;
        vmaMapMemory(allocator.get(), allocation, reinterpret_cast<void**>(&data));
        device.table().vkGetDescriptorEXT(device.get(), &info, descriptor_size, data + index * descriptor_size);
        vmaUnmapMemory(allocator.get(), allocation);
    }
};
} // namespace wis