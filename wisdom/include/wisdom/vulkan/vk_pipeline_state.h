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
        Internal() = default;
        Internal(wis::shared_handle<vk::Pipeline> pipeline)
            : pipeline(std::move(pipeline)){};

        [[nodiscard]] auto GetPipeline() const noexcept
        {
            return pipeline.get();
        }

    private:
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
            return GetPipeline();
        }
        operator bool() const noexcept
        {
            return bool(GetPipeline());
        }
    };
}
