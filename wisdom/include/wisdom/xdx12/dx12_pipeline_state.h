#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/util/com_ptr.h>
#include <d3d12.h>

namespace wis {
class DX12PipelineState;

template<>
struct Internal<DX12PipelineState> {
    wis::com_ptr<ID3D12PipelineState> pipeline;
};

/// @brief Pipeline state object, holds the state of the pipeline
class DX12PipelineState : public QueryInternal<DX12PipelineState>
{
public:
    DX12PipelineState() = default;
    explicit DX12PipelineState(wis::com_ptr<ID3D12PipelineState> xpipeline) noexcept
        : QueryInternal(std::move(xpipeline)) { }
    operator bool() const noexcept
    {
        return bool(pipeline);
    }
};
} // namespace wis
