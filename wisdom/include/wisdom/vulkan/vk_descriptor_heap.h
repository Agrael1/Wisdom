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
	};
}