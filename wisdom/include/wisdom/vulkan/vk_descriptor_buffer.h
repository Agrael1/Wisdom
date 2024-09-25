#ifndef WIS_VK_DESCRIPTOR_BUFFER_H
#define WIS_VK_DESCRIPTOR_BUFFER_H
#include <wisdom/vulkan/vk_resource.h>
#include <wisdom/vulkan/vk_device_ext.h>

namespace wis {
class VKDescriptorBuffer;

template<>
struct Internal<VKDescriptorBuffer> {
    wis::shared_handle<VmaAllocator> allocator;
    h::VmaAllocation allocation;
    h::VkBuffer buffer;
    VkDeviceAddress address = 0;
    uint8_t* data = nullptr;
    XDescriptorBufferProperties properties;

    wis::DescriptorHeapType type = wis::DescriptorHeapType::Descriptor;
    uint32_t descriptor_size = 0;

    Internal() noexcept = default;
    Internal(wis::shared_handle<VmaAllocator> allocator, VkBuffer buffer, VmaAllocation allocation, wis::DescriptorHeapType type, XDescriptorBufferProperties& properties, uint32_t descriptor_size) noexcept
        : allocator(std::move(allocator)), allocation(allocation), buffer(buffer), type(type), properties(properties), descriptor_size(descriptor_size)
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
    Internal& operator=(Internal&& o) noexcept
    {
        if (this == &o) {
            return *this;
        }
        Destroy();
        allocator = std::move(o.allocator);
        allocation = std::move(o.allocation);
        buffer = std::move(o.buffer);
        address = std::move(o.address);
        data = std::move(o.data);
        properties = std::move(o.properties);
        type = std::move(o.type);
        descriptor_size = std::move(o.descriptor_size);
        return *this;
    }
    ~Internal() noexcept
    {
        Destroy();
    }

    void Destroy() noexcept
    {
        if (buffer && allocation) {
            vmaUnmapMemory(allocator.get(), allocation);
            vmaDestroyBuffer(allocator.get(), buffer, allocation);
        }
    }

public:
    auto& GetDevice() const noexcept
    {
        return allocator.header();
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
                                XDescriptorBufferProperties properties,
                                uint32_t descriptor_size) noexcept
        : QueryInternal(std::move(allocator), buffer, allocation, type, properties, descriptor_size) { }
    operator bool() const noexcept
    {
        return bool(buffer);
    }
    operator VKDescriptorBufferView() const noexcept
    {
        return { address, type, descriptor_size };
    }

public:
    WIS_INLINE uint64_t WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::VKSamplerView sampler) noexcept;
    WIS_INLINE uint64_t WriteShaderResource2(uint64_t aligned_table_offset, uint32_t index, wis::VKShaderResourceView resource) noexcept;
    WIS_INLINE uint64_t WriteConstantBuffer2(uint64_t aligned_table_offset, uint32_t index, wis::VKBufferView buffer, uint32_t buffer_size) noexcept;

    // Returns the offset for the next set
    WIS_INLINE uint64_t WriteShaderResource(uint64_t buffer_offset_before_table,
                                            uint32_t root_table_index,
                                            uint32_t binding,
                                            uint32_t array_member,
                                            wis::VKRootSignatureView2 root_signature,
                                            wis::VKShaderResourceView resource) noexcept;

    WIS_INLINE uint64_t WriteConstantBuffer(uint64_t buffer_offset_before_table,
                                            uint32_t root_table_index,
                                            uint32_t binding,
                                            uint32_t array_member,
                                            wis::VKRootSignatureView2 root_signature,
                                            wis::VKBufferView buffer,
                                            uint32_t buffer_size) noexcept;

protected:
    WIS_INLINE uint64_t WriteDescriptor(uint64_t buffer_offset_before_table,
                                        uint32_t root_table_index,
                                        uint32_t binding,
                                        uint32_t array_member,
                                        uint32_t descriptor_true_size,
                                        const VkDescriptorGetInfoEXT& info,
                                        wis::VKRootSignatureView2 root_signature) noexcept;
};
} // namespace wis
#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_descriptor_buffer.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_DESCRIPTOR_BUFFER_H
