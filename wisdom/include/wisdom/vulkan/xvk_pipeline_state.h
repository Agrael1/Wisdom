#pragma once
#include <wisdom/api/internal.h>
#include <wisdom/vulkan/xvk_views.h>

namespace wis {
class VKPipelineState;

template<>
struct Internal<VKPipelineState> {
    SharedDevice device;
    VkPipeline pipeline;
};

/// @brief Pipeline state object
class VKPipelineState : public QueryInternal<VKPipelineState>
{
public:
    VKPipelineState() = default;
    explicit VKPipelineState(SharedDevice device, VkPipeline pipeline) noexcept
        : QueryInternal(std::move(device), std::move(pipeline))
    {
    }
    ~VKPipelineState()
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
} // namespace wis
