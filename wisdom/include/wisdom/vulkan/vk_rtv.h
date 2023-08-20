#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#endif

WIS_EXPORT namespace wis
{
class VKRenderTargetView;

template<>
class Internal<VKRenderTargetView>
{
public:
    wis::shared_handle<vk::ImageView> view;
};

class VKRenderTargetView : public QueryInternal<VKRenderTargetView>
{
public:
    VKRenderTargetView() = default;
    explicit VKRenderTargetView(wis::shared_handle<vk::ImageView> view)
        : QueryInternal(std::move(view))
    {
    }
};

using VKDepthStencilView = VKRenderTargetView;
}
