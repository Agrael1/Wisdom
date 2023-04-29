#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>

namespace wis
{
	class VKRenderPass;

	template<>
	class Internal<VKRenderPass>
	{
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::RenderPass> rp)
			:rp(std::move(rp)){}
	public:
		auto GetRenderPass()const noexcept
		{
			return rp.get();
		}
	private:
		wis::shared_handle<vk::RenderPass> rp;
	};

	class VKRenderPass : public QueryInternal<VKRenderPass>
	{
	public:
		VKRenderPass() = default;
		explicit VKRenderPass(wis::shared_handle<vk::RenderPass> rp)
			:QueryInternal(std::move(rp))
		{

		};
	public:
	};
}