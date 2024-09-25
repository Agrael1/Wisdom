#ifndef WIS_VK_PIPELINE_STATE_H
#define WIS_VK_PIPELINE_STATE_H
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_views.h>

namespace wis {
class VKPipelineState;

template<>
struct Internal<VKPipelineState> {
    wis::SharedPipeline pipeline;
};

/// @brief Pipeline state object
class VKPipelineState : public QueryInternal<VKPipelineState>
{
public:
    VKPipelineState() = default;
    explicit VKPipelineState(wis::SharedPipeline pipeline) noexcept
        : QueryInternal(std::move(pipeline))
    {
    }
    operator VKPipelineHandle() const noexcept
    {
        return pipeline;
    }
    operator bool() const noexcept
    {
        return bool(pipeline);
    }
};
} // namespace wis

#endif // VK_PIPELINE_STATE_H
