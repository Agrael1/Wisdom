#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#endif

WIS_EXPORT namespace wis
{
	class VKDescriptorHeap;

	template<>
	class Internal<VKDescriptorHeap>
	{
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::DescriptorPool> pool) :pool(std::move(pool)) {};
	public:
		auto GetDescriptorPool()const noexcept
		{
			return pool.get();
		}
	protected:
		wis::shared_handle<vk::DescriptorPool> pool;
	};

	using VKDescriptorSet = vk::DescriptorSet;
	using VKDescriptorLayout = vk::DescriptorSetLayout;
	using VKDescriptorHeapView = vk::DescriptorPool;

	/// @brief Descriptor Heap object
	class VKDescriptorHeap : public QueryInternal<VKDescriptorHeap>
	{
	public:
		VKDescriptorHeap() = default;
		explicit VKDescriptorHeap(wis::shared_handle<vk::DescriptorPool> pool)
			:QueryInternal(std::move(pool))
		{}
		operator bool()const noexcept
		{
			return bool(pool);
		}
		operator VKDescriptorHeapView()const noexcept
		{
			return GetDescriptorPool();
		}
	public:
		std::vector<VKDescriptorSet> AllocateDescriptorSets(std::span<VKDescriptorLayout> layouts)
		{
			vk::DescriptorSetAllocateInfo alloc_info{
				pool.get(), uint32_t(layouts.size()), layouts.data()
			};
			return pool.get_parent().allocateDescriptorSets(alloc_info);
		}
		VKDescriptorSet AllocateDescriptorSet(VKDescriptorLayout layout)
		{
			vk::DescriptorSetAllocateInfo alloc_info{
				pool.get(), 1u, &layout
			};
			return pool.get_parent().allocateDescriptorSets(alloc_info)[0];
		}
	};
}