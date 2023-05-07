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
		Internal(wis::shared_handle<vk::RenderPass> rp, wis::shared_handle<vk::Framebuffer> frame)
			:rp(std::move(rp)), frame(std::move(frame)){}
	public:
		auto GetRenderPass()const noexcept
		{
			return rp.get();
		}
	private:
		wis::shared_handle<vk::RenderPass> rp;
		wis::shared_handle<vk::Framebuffer> frame;
	};

	using VKRenderPassView = vk::RenderPass;

	class VKRenderPass : public QueryInternal<VKRenderPass>
	{
	public:
		VKRenderPass() = default;
		explicit VKRenderPass(wis::shared_handle<vk::RenderPass> rp, wis::shared_handle<vk::Framebuffer> frame)
			:QueryInternal(std::move(rp), std::move(frame))
		{}
	public:
		operator VKRenderPassView()const noexcept
		{
			return GetRenderPass();
		}
	};
}