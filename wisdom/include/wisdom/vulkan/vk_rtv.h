#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#endif

namespace wis {
class VKRenderTargetView;

template<>
struct Internal<VKRenderTargetView> {
    wis::shared_handle<vk::ImageView> view;
};

WIS_EXPORT class VKRenderTargetView : public QueryInternal<VKRenderTargetView>
{
public:
    VKRenderTargetView() = default;
    explicit VKRenderTargetView(wis::shared_handle<vk::ImageView> view) noexcept
        : QueryInternal(std::move(view))
    {
    }
    operator bool() const noexcept { return bool(view); }
};

using VKDepthStencilView = VKRenderTargetView;
using VKShaderResourceView = VKRenderTargetView;
} // namespace wis
