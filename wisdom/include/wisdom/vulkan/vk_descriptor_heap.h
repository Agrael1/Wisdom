#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/util/small_allocator.h>
#include <wisdom/api/api_common.h>
#endif

WIS_EXPORT namespace wis
{
    class VKDescriptorHeap;
    class VKDescriptorSet;
    class VKDescriptorSetLayout;

    using VKDescriptorSetLayoutView = vk::DescriptorSetLayout;
    using VKDescriptorHeapView = vk::DescriptorPool;

    template<>
    struct Internal<VKDescriptorSetLayout> {
        wis::shared_handle<vk::DescriptorSetLayout> layout;
    };

    class VKDescriptorSetLayout : public QueryInternal<VKDescriptorSetLayout>
    {
	public:
		VKDescriptorSetLayout() = default;
		explicit VKDescriptorSetLayout(wis::shared_handle<vk::DescriptorSetLayout> layout)
			: QueryInternal(std::move(layout))
		{
		}
    };

    template<>
    struct Internal<VKDescriptorSet> {
        wis::shared_handle<vk::DescriptorSet> set;
    };

    class VKDescriptorSet : public QueryInternal<VKDescriptorSet>
    {
    public:
        VKDescriptorSet() = default;
        explicit VKDescriptorSet(wis::shared_handle<vk::DescriptorSet> set)
            : QueryInternal(std::move(set))
        {
        }
    };

    template<>
    struct Internal<VKDescriptorHeap> {
        wis::shared_handle<vk::DescriptorPool> pool;
    };

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
            return pool.get();
        }

        VKDescriptorSet AllocateDescriptorSet(VKDescriptorSetLayoutView layout)
        {
            const auto& device = pool.getParent();

            vk::DescriptorSetAllocateInfo alloc_info{
                .descriptorPool = pool.get(),
                .descriptorSetCount = 1u,
                .pSetLayouts = &layout
            };
            auto [result, value] = device->allocateDescriptorSets(alloc_info);
            if (!succeeded(result))
                return {};
            shared_handle<vk::DescriptorSet> set{ value[0], device, { pool } };
            return VKDescriptorSet{ std::move(set) };
        }
    };
}
