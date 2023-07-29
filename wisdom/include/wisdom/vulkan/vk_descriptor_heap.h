#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#include <wisdom/util/small_allocator.h>
#include <wisdom/api/api_common.h>
#endif

WIS_EXPORT namespace wis
{
    class VKDescriptorSet;
    template<>
    class Internal<VKDescriptorSet>
    {
    public:
        Internal() = default;
        Internal(wis::shared_handle<vk::DescriptorSetLayout> layout, wis::shared_handle<vk::DescriptorSet> set)
            : layout(std::move(layout)), set(std::move(set)){};

    public:
        auto GetDescriptorSet() const noexcept
        {
            return set.get();
        }
        auto GetDescriptorSetLayout() const noexcept
        {
            return layout.get();
        }

    protected:
        wis::shared_handle<vk::DescriptorSetLayout> layout;
        wis::shared_handle<vk::DescriptorSet> set;
    };

    class VKDescriptorSet : public QueryInternal<VKDescriptorSet>
    {
    public:
        VKDescriptorSet() = default;
        explicit VKDescriptorSet(wis::shared_handle<vk::DescriptorSetLayout> layout, wis::shared_handle<vk::DescriptorSet> set)
            : QueryInternal(std::move(layout), std::move(set))
        {
        }
        operator bool() const noexcept
        {
            return bool(set) && bool(layout);
        }
    };

    class VKDescriptorHeap;

    template<>
    class Internal<VKDescriptorHeap>
    {
    public:
        Internal() = default;
        Internal(wis::shared_handle<vk::DescriptorPool> pool)
            : pool(std::move(pool)){};

    public:
        auto GetDescriptorPool() const noexcept
        {
            return pool.get();
        }

    protected:
        wis::shared_handle<vk::DescriptorPool> pool;
    };

    using VKDescriptorHeapView = vk::DescriptorPool;

    /// @brief Descriptor Heap object
    class VKDescriptorHeap : public QueryInternal<VKDescriptorHeap>
    {
    public:
        VKDescriptorHeap() = default;
        explicit VKDescriptorHeap(wis::shared_handle<vk::DescriptorPool> pool)
            : QueryInternal(std::move(pool))
        {
        }
        operator bool() const noexcept
        {
            return bool(pool);
        }
        operator VKDescriptorHeapView() const noexcept
        {
            return GetDescriptorPool();
        }

    public:
        VKDescriptorSet AllocateDescriptorSet(std::span<BindingDescriptor> bindings)
        {
            auto &device = pool.getParent();
            constexpr static vk::DescriptorType cbvSrvUavTypes[] = {
                vk::DescriptorType::eSampledImage,
                vk::DescriptorType::eStorageImage,
                vk::DescriptorType::eUniformTexelBuffer,
                vk::DescriptorType::eStorageTexelBuffer,
                vk::DescriptorType::eUniformBuffer,
                vk::DescriptorType::eStorageBuffer,
                // vk::DescriptorType::eAccelerationStructureKHR /* Need to check support if this is desired. */
            };

            constexpr static vk::MutableDescriptorTypeListVALVE cbvSrvUavTypeList{
                sizeof(cbvSrvUavTypes) / sizeof(VkDescriptorType),
                cbvSrvUavTypes
            };

            constexpr static vk::MutableDescriptorTypeCreateInfoEXT mutableTypeInfo{
                1u,
                &cbvSrvUavTypeList
            };

            wis::internals::uniform_allocator<vk::DescriptorSetLayoutBinding, 16> binding_allocator;
            for (auto binding : bindings) {
                binding_allocator.allocate(vk::DescriptorSetLayoutBinding{
                        binding.binding,
                        vk::DescriptorType::eMutableVALVE,
                        1u,
                        vk::ShaderStageFlagBits(binding.stages),
                });
            }

            vk::DescriptorSetLayoutCreateInfo createInfo{
                {}, uint32_t(binding_allocator.size()), binding_allocator.data(), &mutableTypeInfo
            };
            shared_handle<vk::DescriptorSetLayout> layout{
                device->createDescriptorSetLayout(createInfo), device
            };

            auto lay = layout.get();
            vk::DescriptorSetAllocateInfo alloc_info{
                pool.get(), 1u, &lay
            };
            shared_handle<vk::DescriptorSet> set{ device->allocateDescriptorSets(alloc_info)[0], device, {pool} };
            return VKDescriptorSet{std::move(layout), std::move(set)};
        }
    };
}