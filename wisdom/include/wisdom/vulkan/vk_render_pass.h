#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_common.h>
#include <wisdom/vulkan/vk_views.h>
#endif

namespace wis {
class VKRenderPass;

template<>
struct Internal<VKRenderPass> {
    wis::shared_handle<vk::RenderPass> rp;
    wis::shared_handle<vk::Framebuffer> frame;
};

WIS_EXPORT class VKRenderPass : public QueryInternal<VKRenderPass>
{
public:
    VKRenderPass() = default;
    explicit VKRenderPass(wis::shared_handle<vk::RenderPass> rp, wis::shared_handle<vk::Framebuffer> frame, Size2D frame_size) noexcept
        : QueryInternal(std::move(rp), std::move(frame)), framebuffer_size(frame_size)
    {
    }
    operator bool() const noexcept
	{
		return bool(rp)&&bool(frame);
	}
    [[nodiscard]] Size2D GetFramebufferSize() const noexcept
    {
        return framebuffer_size;
    }

private:
    Size2D framebuffer_size{ 0, 0 };
};
} // namespace wis
