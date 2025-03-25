#ifndef WIS_VK_DESCRIPTOR_BUFFER_H
#define WIS_VK_DESCRIPTOR_BUFFER_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_memory.h>
#include <wisdom/generated/vk_functions.hpp>
#endif // !WISDOM_MODULE_DECL

WISDOM_EXPORT
namespace wis {
class VKDescriptorBuffer;

using VKDescriptorBufferView = std::tuple<VkDeviceAddress, wis::DescriptorHeapType>;
using VKDescriptorBufferGPUView = VKDescriptorBufferView;

struct VKDescriptorBufferProperties {
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

template<>
struct Internal<VKDescriptorBuffer> {
    PFN_vkGetDescriptorEXT vkGetDescriptorEXT;

    wis::shared_handle<VmaAllocator> allocator;
    h::VmaAllocation allocation;
    h::VkBuffer buffer;
    VkDeviceAddress address = 0;

    uint8_t* data = nullptr;
    VKDescriptorBufferProperties properties;
    uint16_t descriptor_size = 0;

    wis::DescriptorHeapType type = wis::DescriptorHeapType::Descriptor;

public:
    Internal() noexcept = default;
    Internal(wis::shared_handle<VmaAllocator> allocator,
             VkBuffer buffer,
             VmaAllocation allocation,
             wis::DescriptorHeapType type,
             VKDescriptorBufferProperties properties,
             const VKDescBufferExtDevice& ftable) noexcept
        : allocator(std::move(allocator))
        , allocation(allocation)
        , buffer(buffer)
        , type(type)
        , properties(properties)
        , vkGetDescriptorEXT(ftable.vkGetDescriptorEXT)
    {
        auto& device = this->allocator.header();
        VkBufferDeviceAddressInfo info{
            .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .buffer = buffer
        };
        address = device.table().vkGetBufferDeviceAddress(device.get(), &info);
        vmaMapMemory(this->allocator.get(), allocation, reinterpret_cast<void**>(&data));

        descriptor_size = type == wis::DescriptorHeapType::Descriptor
                          ? properties.mutable_descriptor_size
                          : properties.sampler_size;
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
        address = o.address;
        data = o.data;
        properties = o.properties;
        type = o.type;
        descriptor_size = o.descriptor_size;
        vkGetDescriptorEXT = o.vkGetDescriptorEXT;
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

class ImplVKDescriptorBuffer : public QueryInternal<VKDescriptorBuffer>
{
public:
    ImplVKDescriptorBuffer() noexcept = default;
    explicit ImplVKDescriptorBuffer(Internal<VKDescriptorBuffer>&& internal) noexcept
        : QueryInternal(std::move(internal)) { }

    operator bool() const noexcept
    {
        return bool(buffer);
    }
    operator VKDescriptorBufferView() const noexcept
    {
        return { address, type };
    }

public:
    WIS_INLINE void WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::VKSamplerView sampler) noexcept;
    WIS_INLINE void WriteShaderResource(uint64_t aligned_table_offset, uint32_t index, wis::VKShaderResourceView resource) noexcept;
    WIS_INLINE void WriteConstantBuffer(uint64_t aligned_table_offset, uint32_t index, wis::VKBufferView buffer, uint32_t buffer_size, uint32_t offset = 0) noexcept;
};

#pragma region VKDescriptorBuffer
/**
 * @brief Buffer storage for descriptors.
 * It is the most flexible implementation of storage for descriptors, as it supports both bindful and bindless
 * Descriptor approaches. Works like an aligned table of elements of the same size.
 * Alignment and size of descriptors are varying between implementation and heap types.
 * Both metrixs are available to retrieval from
 * */
class VKDescriptorBuffer : public wis::ImplVKDescriptorBuffer
{
public:
    using wis::ImplVKDescriptorBuffer::ImplVKDescriptorBuffer;

public:
    /**
     * @brief Writes the sampler to the sampler descriptor buffer.
     * Must be called with Sampler descriptor buffer, which was created with wis::DescriptorHeapType::Sampler.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with .
     * @param index Binding index in descriptor table.
     * @param sampler The sampler to write.
     * */
    inline void WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::VKSamplerView sampler) noexcept
    {
        wis::ImplVKDescriptorBuffer::WriteSampler(aligned_table_offset, index, std::move(sampler));
    }
    /**
     * @brief Writes the shader resource to the shader resource descriptor buffer.
     * Must be called with Shader Resource descriptor buffer, which was created with wis::DescriptorHeapType::Descriptor.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with .
     * @param index Binding index in descriptor table.
     * @param resource The shader resource to write.
     * */
    inline void WriteShaderResource(uint64_t aligned_table_offset, uint32_t index, wis::VKShaderResourceView resource) noexcept
    {
        wis::ImplVKDescriptorBuffer::WriteShaderResource(aligned_table_offset, index, std::move(resource));
    }
    /**
     * @brief Writes the constant buffer to the constant buffer descriptor buffer.
     * Must be called with Constant Buffer descriptor buffer, which was created with wis::DescriptorHeapType::Descriptor.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with wis::VKDevice.
     * @param index Binding index in descriptor table.
     * @param buffer The buffer to write.
     * @param buffer_size The size of the part of the buffer in bytes.
     * @param offset Offset from buffer beginning. offset + buffer_size must be less or equal buffer overall size.
     * */
    inline void WriteConstantBuffer(uint64_t aligned_table_offset, uint32_t index, wis::VKBufferView buffer, uint32_t buffer_size, uint32_t offset = 0) noexcept
    {
        wis::ImplVKDescriptorBuffer::WriteConstantBuffer(aligned_table_offset, index, std::move(buffer), buffer_size, offset);
    }
};
#pragma endregion VKDescriptorBuffer

} // namespace wis
#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_descriptor_buffer.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_DESCRIPTOR_BUFFER_H
