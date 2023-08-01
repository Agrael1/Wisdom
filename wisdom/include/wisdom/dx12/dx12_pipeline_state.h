#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_views.h>
#include <d3d12.h>
#include <winrt/base.h>
#endif // !WISDOM_MODULES

WIS_EXPORT namespace wis
{
    class DX12PipelineState;

    template<>
    class Internal<DX12PipelineState>
    {
    public:
        Internal() = default;
        Internal(winrt::com_ptr<ID3D12PipelineState> xpipeline)
            : pipeline(std::move(xpipeline)) { }

    public:
        ID3D12PipelineState* GetPipeline() const noexcept
        {
            return pipeline.get();
        }

    protected:
        winrt::com_ptr<ID3D12PipelineState> pipeline;
    };

    /// @brief Pipeline state object, holds the state of the pipeline
    class DX12PipelineState : public QueryInternal<DX12PipelineState>
    {
    public:
        DX12PipelineState() = default;
        explicit DX12PipelineState(winrt::com_ptr<ID3D12PipelineState> xpipeline)
            : QueryInternal(std::move(xpipeline)) { }
        operator DX12PipelineStateView() const noexcept
        {
            return GetPipeline();
        }
    };
}
