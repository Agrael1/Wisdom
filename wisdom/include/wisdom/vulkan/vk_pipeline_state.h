#ifndef WIS_VK_PIPELINE_STATE_H
#define WIS_VK_PIPELINE_STATE_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_views.h>
#endif // !WISDOM_MODULE_DECL

namespace wis {
class VKPipelineState;

WISDOM_EXPORT
template<>
struct Internal<VKPipelineState> {
    wis::SharedPipeline pipeline;
};

WISDOM_EXPORT
class VKPipelineState : public QueryInternal<VKPipelineState>
{
public:
    VKPipelineState() = default;
    operator VKPipelineView() const noexcept
    {
        return pipeline.get();
    }
    operator bool() const noexcept
    {
        return bool(pipeline);
    }
};
} // namespace wis

#endif // VK_PIPELINE_STATE_H
