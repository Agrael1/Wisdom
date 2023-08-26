#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_views.h>
#include <d3d12.h>
#include <winrt/base.h>
#endif // !WISDOM_MODULES

namespace wis {
class DX12PipelineState;

template<>
class Internal<DX12PipelineState>
{
public:
    winrt::com_ptr<ID3D12PipelineState> pipeline;
};

/// @brief Pipeline state object, holds the state of the pipeline
WIS_EXPORT class DX12PipelineState : public QueryInternal<DX12PipelineState>
{
public:
    DX12PipelineState() = default;
    explicit DX12PipelineState(winrt::com_ptr<ID3D12PipelineState> xpipeline) noexcept
        : QueryInternal(std::move(xpipeline)) { }
    operator DX12PipelineStateView() const noexcept
    {
        return pipeline.get();
    }
    operator bool() const noexcept
    {
        return pipeline != nullptr;
    }
};
} // namespace wis
