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

class ImplVKDescriptorBuffer : public QueryInternal<VKDescriptorBuffer>
{
public:
    ImplVKDescriptorBuffer() noexcept = default;
    explicit ImplVKDescriptorBuffer(wis::shared_handle<VmaAllocator> allocator,
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

#pragma region VKDescriptorBuffer
/**
 * @brief Represents descriptor buffer for binding descriptors.
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
     * Alignment may be queried with wis::VKDevice.
     * @param index Binding index in descriptor table.
     * @param sampler The sampler to write.
     * @return Byte offset from buffer beginning. May help determine next table address.
     * */
    inline uint64_t WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::VKSamplerView sampler) noexcept
    {
        return wis::ImplVKDescriptorBuffer::WriteSampler(aligned_table_offset, index, std::move(sampler));
    }
    /**
     * @brief Writes the shader resource to the shader resource descriptor buffer.
     * Must be called with Shader Resource descriptor buffer, which was created with wis::DescriptorHeapType::Descriptor.
     * Requires wis::DeviceFeature::DescriptorEqualSize to run, otherwise program is ill-formed.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with wis::VKDevice.
     * @param index Binding index in descriptor table.
     * @param resource The shader resource to write.
     * @return Byte offset from buffer beginning. May help determine next table address.
     * */
    inline uint64_t WriteShaderResource2(uint64_t aligned_table_offset, uint32_t index, wis::VKShaderResourceView resource) noexcept
    {
        return wis::ImplVKDescriptorBuffer::WriteShaderResource2(aligned_table_offset, index, std::move(resource));
    }
    /**
     * @brief Writes the constant buffer to the constant buffer descriptor buffer.
     * Must be called with Constant Buffer descriptor buffer, which was created with wis::DescriptorHeapType::Descriptor.
     * Requires wis::DeviceFeature::DescriptorEqualSize to run, otherwise program is ill-formed.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with wis::VKDevice.
     * @param index Binding index in descriptor table.
     * @param buffer The buffer to write.
     * @param buffer_size The size of the buffer in bytes.
     * @return Byte offset from buffer beginning. May help determine next table address.
     * */
    inline uint64_t WriteConstantBuffer2(uint64_t aligned_table_offset, uint32_t index, wis::VKBufferView buffer, uint32_t buffer_size) noexcept
    {
        return wis::ImplVKDescriptorBuffer::WriteConstantBuffer2(aligned_table_offset, index, std::move(buffer), buffer_size);
    }
    /**
     * @brief Writes the shader resource to the shader resource descriptor buffer.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with wis::VKDevice.
     * @param root_table_index Index of the descriptor table in wis::VKRootSignature
     * @param binding Binding index in descriptor table.
     * @param array_member Array member index in the binding.
     * @param root_signature The root signature to get the binding position from.
     * @param resource The shader resource to write.
     * @return Byte offset from buffer beginning. May help determine next table address.
     * */
    inline uint64_t WriteShaderResource(uint64_t aligned_table_offset, uint32_t root_table_index, uint32_t binding, uint32_t array_member, wis::VKRootSignatureView2 root_signature, wis::VKShaderResourceView resource) noexcept
    {
        return wis::ImplVKDescriptorBuffer::WriteShaderResource(aligned_table_offset, root_table_index, binding, array_member, std::move(root_signature), std::move(resource));
    }
    /**
     * @brief Writes the constant buffer to the constant buffer descriptor buffer.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with wis::VKDevice.
     * @param root_table_index Index of the descriptor table in wis::VKRootSignature
     * @param binding Binding index in descriptor table.
     * @param array_member Array member index in the binding.
     * @param root_signature The root signature to get the binding position from.
     * @param buffer The buffer to write.
     * @param buffer_size The size of the buffer in bytes.
     * @return Byte offset from buffer beginning. May help determine next table address.
     * */
    inline uint64_t WriteConstantBuffer(uint64_t aligned_table_offset, uint32_t root_table_index, uint32_t binding, uint32_t array_member, wis::VKRootSignatureView2 root_signature, wis::VKBufferView buffer, uint32_t buffer_size) noexcept
    {
        return wis::ImplVKDescriptorBuffer::WriteConstantBuffer(aligned_table_offset, root_table_index, binding, array_member, std::move(root_signature), std::move(buffer), buffer_size);
    }
};
#pragma endregion VKDescriptorBuffer

} // namespace wis
#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_descriptor_buffer.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_DESCRIPTOR_BUFFER_H
