#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_adapter.h>
#include <wisdom/vulkan/vk_views.h>
#endif // !WISDOM_MODULES


WIS_EXPORT namespace wis
{
	class VKAdapter;

	template<>
	class Internal<VKAdapter>
	{
	public:
		Internal() = default;
		Internal(vk::PhysicalDevice adapter)noexcept
			:adapter(adapter) {}
	public:
		[[nodiscard]]
		vk::PhysicalDevice GetAdapter()const noexcept
		{
			return adapter;
		}
	protected:
		vk::PhysicalDevice adapter;
	};


	/// @brief Vulkan physcial adapter
	class VKAdapter : public QueryInternal<VKAdapter>
	{
	public:
		VKAdapter() = default;
		explicit VKAdapter(vk::PhysicalDevice adapter)
			:QueryInternal(adapter)
		{}

		/// @brief Get the adapter internal view
		/// @return Adapter internal view
		/// @note Do not use the contents of a view directly unless you know what you are doing
		operator VKAdapterView()const noexcept
		{
			return GetAdapter();
		}
	public:
		/// @brief Get the adapter description
		/// @return Adapter Description
		/// @note This function is thread safe
		[[nodiscard]] WIS_INLINE AdapterDesc GetDesc()const noexcept;
	};
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/vk_adapter.inl"
#endif