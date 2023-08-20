#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_common.h>
#include <wisdom/vulkan/vk_views.h>
#endif

WIS_EXPORT namespace wis
{
    class VKRenderPass;

    template<>
    class Internal<VKRenderPass>
    {
    public:
        wis::shared_handle<vk::RenderPass> rp;
        wis::shared_handle<vk::Framebuffer> frame;
    };

    // TODO: Dynamic rendering
    class VKRenderPass : public QueryInternal<VKRenderPass>
    {
    public:
        VKRenderPass() = default;
        explicit VKRenderPass(wis::shared_handle<vk::RenderPass> rp, wis::shared_handle<vk::Framebuffer> frame, Size2D frame_size)
            : QueryInternal(std::move(rp), std::move(frame)), framebuffer_size(frame_size)
        {
        }

        operator VKRenderPassView() const noexcept
        {
            return { rp.get(), frame.get(), framebuffer_size };
        }
        [[nodiscard]] Size2D GetFramebufferSize() const noexcept
        {
            return framebuffer_size;
        }

    private:
        Size2D framebuffer_size{ 0, 0 };
    };
}
