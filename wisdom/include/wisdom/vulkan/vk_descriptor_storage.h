#ifndef WIS_VK_DESCRIPTOR_STORAGE_H
#define WIS_VK_DESCRIPTOR_STORAGE_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_views.h>
#endif // !WISDOM_MODULE_DECL

namespace wis {
WISDOM_EXPORT class VKDescriptorStorage;

WISDOM_EXPORT
template<>
struct Internal<VKDescriptorStorage> {
    wis::SharedDevice device;
    h::VkDescriptorPool pool;

    std::unique_ptr<VkDescriptorSet[]> descriptor_sets; // Big Descriptor set with only unbounded arrays
    uint32_t descriptor_count = 0;

public:
    Internal() noexcept = default;
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&& o) noexcept
    {
        if (this == &o) {
            return *this;
        }
        Destroy();
        device = std::move(o.device);
        pool = std::move(o.pool);
        descriptor_sets = std::move(o.descriptor_sets);
        descriptor_count = o.descriptor_count;
        return *this;
    }
    ~Internal() noexcept
    {
        Destroy();
    }

public:
    void Destroy() noexcept
    {
        if (pool) {
            device.table().vkDestroyDescriptorPool(device.get(), pool, nullptr);
            for (uint32_t i = 0; i < descriptor_count; ++i) {
                device.table().vkDestroyDescriptorSetLayout(device.get(), reinterpret_cast<VkDescriptorSetLayout>(descriptor_sets[i + descriptor_count]), nullptr);
            }
        }
    }
};

class ImplVKDescriptorStorage : public QueryInternal<VKDescriptorStorage>
{
public:
    ImplVKDescriptorStorage() noexcept = default;
    operator bool() const noexcept
    {
        return bool(pool);
    }

    operator VKDescriptorStorageView() const noexcept
    {
        std::span<const VkDescriptorSet> span{
            descriptor_sets.get(), descriptor_count
        };
        return VKDescriptorStorageView{ span };
    }

public:
    void WriteSampler(uint32_t binding, uint32_t index, wis::VKSamplerView sampler) noexcept
    {
        VkDescriptorImageInfo info{
            .sampler = std::get<0>(sampler),
            .imageView = VK_NULL_HANDLE,
            .imageLayout = VK_IMAGE_LAYOUT_UNDEFINED
        };
        VkWriteDescriptorSet write{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptor_sets[binding],
            .dstBinding = 0,
            .dstArrayElement = index,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
            .pImageInfo = &info
        };
        device.table().vkUpdateDescriptorSets(device.get(), 1, &write, 0, nullptr);
    }
    void WriteConstantBuffer(uint32_t binding, uint32_t index, wis::VKBufferView buffer, uint32_t size, uint32_t offset = 0) noexcept
    {
        VkDescriptorBufferInfo info{
            .buffer = std::get<0>(buffer),
            .offset = offset,
            .range = size
        };
        VkWriteDescriptorSet write{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptor_sets[binding],
            .dstBinding = 0,
            .dstArrayElement = index,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pBufferInfo = &info
        };
        device.table().vkUpdateDescriptorSets(device.get(), 1, &write, 0, nullptr);
    }
    void WriteTexture(uint32_t binding, uint32_t index, wis::VKShaderResourceView srv) noexcept
    {
        VkDescriptorImageInfo info{
            .sampler = VK_NULL_HANDLE,
            .imageView = std::get<0>(srv),
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        };
        VkWriteDescriptorSet write{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptor_sets[binding],
            .dstBinding = 0,
            .dstArrayElement = index,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
            .pImageInfo = &info
        };
        device.table().vkUpdateDescriptorSets(device.get(), 1, &write, 0, nullptr);
    }
    void WriteRWTexture(uint32_t binding, uint32_t index, wis::VKUnorderedAccessTextureView uav) noexcept
    {
        VkDescriptorImageInfo info{
            .sampler = VK_NULL_HANDLE,
            .imageView = std::get<0>(uav),
            .imageLayout = VK_IMAGE_LAYOUT_GENERAL
        };
        VkWriteDescriptorSet write{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptor_sets[binding],
            .dstBinding = 0,
            .dstArrayElement = index,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
            .pImageInfo = &info
        };
        device.table().vkUpdateDescriptorSets(device.get(), 1, &write, 0, nullptr);
    }
    void WriteRWStructuredBuffer(uint32_t binding, uint32_t index, wis::VKBufferView buffer, uint32_t stride, uint32_t element_count, uint32_t offset_elements = 0) noexcept
    {
        VkDescriptorBufferInfo info{
            .buffer = std::get<0>(buffer),
            .offset = offset_elements * stride,
            .range = element_count * stride
        };
        VkWriteDescriptorSet write{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptor_sets[binding],
            .dstBinding = 0,
            .dstArrayElement = index,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .pBufferInfo = &info
        };
        device.table().vkUpdateDescriptorSets(device.get(), 1, &write, 0, nullptr);
    }
    void WriteStructuredBuffer(uint32_t binding, uint32_t index, wis::VKBufferView buffer, uint32_t stride, uint32_t element_count, uint32_t offset_elements = 0) noexcept
    {
        VkDescriptorBufferInfo info{
            .buffer = std::get<0>(buffer),
            .offset = offset_elements * stride,
            .range = element_count * stride
        };
        VkWriteDescriptorSet write{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptor_sets[binding],
            .dstBinding = 0,
            .dstArrayElement = index,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .pBufferInfo = &info
        };
        device.table().vkUpdateDescriptorSets(device.get(), 1, &write, 0, nullptr);
    }
    void WriteAccelerationStructure(uint32_t binding, uint32_t index, wis::VKAccelerationStructureView as) const noexcept
    {
        VkWriteDescriptorSetAccelerationStructureKHR as_info{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR,
            .accelerationStructureCount = 1,
            .pAccelerationStructures = &std::get<0>(as),
        };
        VkWriteDescriptorSet write{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = &as_info,
            .dstSet = descriptor_sets[binding],
            .dstBinding = 0,
            .dstArrayElement = index,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
        };
        device.table().vkUpdateDescriptorSets(device.get(), 1, &write, 0, nullptr);
    }
};

#pragma region VKDescriptorStorage
/**
 * @brief Represents descriptor storage object for storing descriptors.
 * Used for bindless and non-uniform bindings. Don't combine with Descriptor buffers, this may reduce performance.
 * */
WISDOM_EXPORT
class VKDescriptorStorage : public wis::ImplVKDescriptorStorage
{
public:
    using wis::ImplVKDescriptorStorage::ImplVKDescriptorStorage;

public:
    /**
     * @brief Writes the sampler to the sampler descriptor storage.
     * @param set_index Index in storage sets, defined by the place in the binding array at the creation.
     * @param binding Index in array of samplers to fill.
     * @param sampler The sampler to write.
     * */
    inline void WriteSampler(uint32_t set_index, uint32_t binding, wis::VKSamplerView sampler) noexcept
    {
        wis::ImplVKDescriptorStorage::WriteSampler(set_index, binding, std::move(sampler));
    }
    /**
     * @brief Writes the constant buffer to the constant buffer descriptor storage.
     * @param set_index Index in storage sets, defined by the place in the binding array at the creation.
     * @param binding Index in array of constant buffers to fill.
     * @param buffer The buffer to write.
     * @param size The size of the constant buffer in bytes.
     * @param offset The offset in the buffer to write the constant buffer to.
     * size + offset must be less or equal the overall size of the bound buffer.
     * */
    inline void WriteConstantBuffer(uint32_t set_index, uint32_t binding, wis::VKBufferView buffer, uint32_t size, uint32_t offset = 0) noexcept
    {
        wis::ImplVKDescriptorStorage::WriteConstantBuffer(set_index, binding, std::move(buffer), size, offset);
    }
    /**
     * @brief Writes the texture to the shader resource descriptor storage.
     * @param set_index Index in storage sets, defined by the place in the binding array at the creation.
     * @param binding Index in array of shader resources to fill.
     * @param resource The shader resource to write.
     * */
    inline void WriteTexture(uint32_t set_index, uint32_t binding, wis::VKShaderResourceView resource) noexcept
    {
        wis::ImplVKDescriptorStorage::WriteTexture(set_index, binding, std::move(resource));
    }
    /**
     * @brief Writes the storage texture to the storage texture descriptor storage.
     * @param set_index Index in storage sets, defined by the place in the binding array at the creation.
     * @param binding Index in array of storage textures to fill.
     * @param uav The storage texture to write.
     * */
    inline void WriteRWTexture(uint32_t set_index, uint32_t binding, wis::VKUnorderedAccessTextureView uav) noexcept
    {
        wis::ImplVKDescriptorStorage::WriteRWTexture(set_index, binding, std::move(uav));
    }
    /**
     * @brief Writes the storage structured buffer to the storage buffer descriptor storage.
     * @param set_index Index in storage sets, defined by the place in the binding array at the creation.
     * @param binding Index in array of storage buffers to fill.
     * @param buffer The buffer to write.
     * @param stride The stride of each element in the structured buffer in bytes.
     * @param element_count The number of elements in the structured buffer.
     * @param offset_elements The offset in elements from the beginning of the buffer. Default is 0.
     * */
    inline void WriteRWStructuredBuffer(uint32_t set_index, uint32_t binding, wis::VKBufferView buffer, uint32_t stride, uint32_t element_count, uint32_t offset_elements = 0) noexcept
    {
        wis::ImplVKDescriptorStorage::WriteRWStructuredBuffer(set_index, binding, std::move(buffer), stride, element_count, offset_elements);
    }
    /**
     * @brief Writes the structured buffer to the shader resource descriptor storage.
     * @param set_index Index in storage sets, defined by the place in the binding array at the creation.
     * @param binding Index in array of structured buffers to fill.
     * @param buffer The buffer to write.
     * @param stride The stride of each element in the structured buffer in bytes.
     * @param element_count The number of elements in the structured buffer.
     * @param offset_elements The offset in elements from the beginning of the buffer. Default is 0.
     * */
    inline void WriteStructuredBuffer(uint32_t set_index, uint32_t binding, wis::VKBufferView buffer, uint32_t stride, uint32_t element_count, uint32_t offset_elements = 0) noexcept
    {
        wis::ImplVKDescriptorStorage::WriteStructuredBuffer(set_index, binding, std::move(buffer), stride, element_count, offset_elements);
    }
    /**
     * @brief Writes the acceleration structure to the acceleration structure descriptor storage.
     * @param set_index Index in storage sets, defined by the place in the binding array at the creation.
     * @param binding Index in array of acceleration structures to fill.
     * @param acceleration_structure The acceleration structure to write.
     * */
    inline void WriteAccelerationStructure(uint32_t set_index, uint32_t binding, wis::VKAccelerationStructureView acceleration_structure) noexcept
    {
        wis::ImplVKDescriptorStorage::WriteAccelerationStructure(set_index, binding, acceleration_structure);
    }
};
#pragma endregion VKDescriptorStorage

} // namespace wis

#endif // !WIS_VK_DESCRIPTOR_STORAGE_H
