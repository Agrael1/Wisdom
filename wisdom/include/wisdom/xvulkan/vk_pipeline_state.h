#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/xvulkan/vk_views.h>

namespace wis {
class VKPipelineState;

template<>
struct Internal<VKPipelineState> {
    wis::managed_handle_ex<VkPipeline> pipeline;
};

/// @brief Pipeline state object
class VKPipelineState : public QueryInternal<VKPipelineState>
{
public:
    VKPipelineState() = default;
    explicit VKPipelineState(wis::managed_handle_ex<VkPipeline> pipeline) noexcept
        : QueryInternal(std::move(pipeline))
    {
    }
    operator bool() const noexcept
    {
        return bool(pipeline);
    }
};
} // namespace wis
