#pragma once
#include <wisdom/api/internal.h>
#include <wisdom/vulkan/xvk_views.h>

namespace wis {
class VKRenderPass;

template<>
struct Internal<VKRenderPass> {
    wis::managed_handle_ex<VkRenderPass> rp;
    wis::managed_handle_ex<VkFramebuffer> frame;
};

class VKRenderPass : public QueryInternal<VKRenderPass>
{
public:
    VKRenderPass() noexcept = default;
    explicit VKRenderPass(wis::managed_handle_ex<VkRenderPass> rp,
                          wis::managed_handle_ex<VkFramebuffer> frame) noexcept
        : QueryInternal(std::move(rp), std::move(frame))
    {
    }
    operator bool () const noexcept
    {
        return rp && frame;
    }
    operator VKRenderPassView() const noexcept
    {
        return { rp.get(), frame.get() };
    }
};
} // namespace wis
