#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_views.h>
#endif

WIS_EXPORT namespace wis
{
class VKPipelineState;

template<>
class Internal<VKPipelineState>
{
public:
    wis::shared_handle<vk::Pipeline> pipeline;
};

/// @brief Pipeline state object
class VKPipelineState : public QueryInternal<VKPipelineState>
{
public:
    VKPipelineState() = default;
    explicit VKPipelineState(wis::shared_handle<vk::Pipeline> pipeline)
        : QueryInternal(std::move(pipeline))
    {
    }
    operator VKPipelineStateView() const noexcept
    {
        return pipeline.get();
    }
    operator bool() const noexcept
    {
        return bool(pipeline);
    }
};
}
