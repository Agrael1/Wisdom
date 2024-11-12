#ifndef WIS_VK_DESCRIPTOR_STORAGE_H
#define WIS_VK_DESCRIPTOR_STORAGE_H

#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_views.h>

namespace wis {
class VKDescriptorStorage;

template<>
struct Internal<VKDescriptorStorage> {
    // sampler, Uniform buffer, storage RW buffer, sampled image, storage RW image, maybe storage read buffer will be needed.
    constexpr static uint32_t max_sets = max_desc_storage_desc_sets_vk;

    wis::SharedDevice device;
    h::VkDescriptorPool pool;
    std::array<VkDescriptorSet, max_sets> set{}; // Big Descriptor set with only unbounded arrays

public:
    Internal() noexcept = default;
    Internal(wis::SharedDevice device, VkDescriptorPool pool, std::array<VkDescriptorSet, max_sets> set) noexcept
        : device(std::move(device)), pool(pool), set(std::move(set)) { }
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&& o) noexcept
    {
        if (this == &o) {
            return *this;
        }
        Destroy();
        device = std::move(o.device);
        pool = std::move(o.pool);
        set = std::move(o.set);
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
        }
    }
};

class ImplVKDescriptorStorage : public QueryInternal<VKDescriptorStorage>
{
    enum SetIndex : uint32_t {
        Sampler = 0,
        ConstantBuffer = 1,
        StorageBuffer = 2,
        SampledImage = 3,
        StorageImage = 4,
        StorageReadBuffer = 5
    };

public:
    ImplVKDescriptorStorage() noexcept = default;
    explicit ImplVKDescriptorStorage(wis::SharedDevice device, VkDescriptorPool pool, std::array<VkDescriptorSet, max_sets> set) noexcept
        : QueryInternal(std::move(device), pool, std::move(set)) { }

    operator bool() const noexcept
    {
        return bool(pool);
    }

    operator VKDescriptorStorageView() const noexcept
    {
        std::span<const VkDescriptorSet, max_sets> span{
            set
        };
        return VKDescriptorStorageView{ span };
    }

public:
    void WriteSampler(uint32_t index, wis::VKSamplerView sampler) noexcept
    {
        VkDescriptorImageInfo info{
            .sampler = std::get<0>(sampler),
            .imageView = VK_NULL_HANDLE,
            .imageLayout = VK_IMAGE_LAYOUT_UNDEFINED
        };
        VkWriteDescriptorSet write{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = set[Sampler],
            .dstBinding = 0,
            .dstArrayElement = index,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
            .pImageInfo = &info
        };
        device.table().vkUpdateDescriptorSets(device.get(), 1, &write, 0, nullptr);
    }
    void WriteConstantBuffer(uint32_t index, wis::VKBufferView buffer, uint32_t size, uint32_t offset = 0) noexcept
    {
        VkDescriptorBufferInfo info{
            .buffer = std::get<0>(buffer),
            .offset = offset,
            .range = size
        };
        VkWriteDescriptorSet write{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = set[ConstantBuffer],
            .dstBinding = 0,
            .dstArrayElement = index,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pBufferInfo = &info
        };
        device.table().vkUpdateDescriptorSets(device.get(), 1, &write, 0, nullptr);
    }
};

#pragma region VKDescriptorStorage
/**
 * @brief Represents descriptor storage object for storing descriptors.
 * Used for bindless and non-uniform bindings. Don't combine with Descriptor buffers, this may reduce performance.
 * */
class VKDescriptorStorage : public wis::ImplVKDescriptorStorage
{
public:
    using wis::ImplVKDescriptorStorage::ImplVKDescriptorStorage;

public:
    /**
     * @brief Writes the sampler to the sampler descriptor storage.
     * @param index Index in array of samplers to fill.
     * @param sampler The sampler to write.
     * */
    inline void WriteSampler(uint32_t index, wis::VKSamplerView sampler) noexcept
    {
        return wis::ImplVKDescriptorStorage::WriteSampler(index, std::move(sampler));
    }
};
#pragma endregion VKDescriptorStorage

} // namespace wis

#endif // !WIS_VK_DESCRIPTOR_STORAGE_H