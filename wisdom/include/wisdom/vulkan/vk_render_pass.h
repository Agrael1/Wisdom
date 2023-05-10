#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_common.h>
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
			:rp(std::move(rp)), frame(std::move(frame)) {}
	public:
		auto GetRenderPass()const noexcept
		{
			return rp.get();
		}
		auto GetFramebuffer()const noexcept
		{
			return frame.get();
		}
	private:
		wis::shared_handle<vk::RenderPass> rp;
		wis::shared_handle<vk::Framebuffer> frame;
	};

	using VKRenderPassView = struct { vk::RenderPass pass; vk::Framebuffer frame; Size2D frame_size; };

	class VKRenderPass : public QueryInternal<VKRenderPass>
	{
	public:
		VKRenderPass() = default;
		explicit VKRenderPass(wis::shared_handle<vk::RenderPass> rp, wis::shared_handle<vk::Framebuffer> frame, Size2D frame_size)
			:QueryInternal(std::move(rp), std::move(frame)), framebuffer_size(frame_size)
		{}
	public:
		operator VKRenderPassView()const noexcept
		{
			return { GetRenderPass(), GetFramebuffer(), framebuffer_size };
		}
		Size2D GetFramebufferSize()const noexcept
		{
			return framebuffer_size;
		}
	private:
		Size2D framebuffer_size{ 0,0 };
	};
}