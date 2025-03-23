#ifndef WIS_DX12_PIPELINE_STATE_H
#define WIS_DX12_PIPELINE_STATE_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/internal.h>
#include <wisdom/util/com_ptr.h>
#include <d3d12.h>
#endif

namespace wis {
class DX12PipelineState;

WISDOM_EXPORT
template<>
struct Internal<DX12PipelineState> {
    wis::com_ptr<ID3D12PipelineState> pipeline;
};

WISDOM_EXPORT
class DX12PipelineState : public QueryInternal<DX12PipelineState>
{
public:
    DX12PipelineState() = default;
    operator DX12PipelineView() const noexcept
    {
        return pipeline.get();
    }
    operator bool() const noexcept
    {
        return bool(pipeline);
    }
};
} // namespace wis

#endif // WIS_DX12_PIPELINE_STATE_H
