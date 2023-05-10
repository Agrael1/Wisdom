#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>

namespace wis
{
	class VKRenderTargetView;

	template<>
	class Internal<VKRenderTargetView>
	{
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::ImageView> view)
			:view(std::move(view))
		{}
	public:
		auto GetViewHandle()const noexcept
		{
			return view;
		}
		auto GetImageView()const noexcept
		{
			return view.get();
		}
	protected:
		wis::shared_handle<vk::ImageView> view;
	};


	class VKRenderTargetView : public QueryInternal<VKRenderTargetView>
	{
	public:
		VKRenderTargetView() = default;
		explicit VKRenderTargetView(wis::shared_handle<vk::ImageView> view)
			:QueryInternal(std::move(view))
		{}
	};
}