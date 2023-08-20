#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#include <wisdom/vulkan/vk_views.h>
#endif

namespace wis {
class VKRenderTarget;

template<>
class Internal<VKRenderTarget> {
public:
    wis::shared_handle<vk::ImageView> view;
};

WIS_EXPORT class VKRenderTarget : public QueryInternal<VKRenderTarget>
{
public:
    VKRenderTarget() = default;
    explicit VKRenderTarget(wis::shared_handle<vk::ImageView> view) noexcept
        : QueryInternal(std::move(view))
    {
    }
    operator VKRenderTargetHandle() const noexcept {
        return view.get();
    }
    operator bool() const noexcept {
        return bool(view);
    }
};

using VKDepthStencil = VKRenderTarget;
using VKDepthStencilHandle = VKRenderTargetHandle;
} // namespace wis