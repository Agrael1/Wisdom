#pragma once
#include <wisdom/api/api_internal.h>
#include <d3d12.h>
#include <winrt/base.h>

namespace wis
{
	class DX12PipelineState;

	template<>
	class Internal<DX12PipelineState>
	{
	public:
		ID3D12PipelineState* GetPipeline()const noexcept
		{
			return pipeline.get();
		}
	protected:
		winrt::com_ptr<ID3D12PipelineState> pipeline;
	};
	using DX12PipelineStateView = ID3D12PipelineState*;


	/// @brief Pipeline state object, holds the state of the pipeline
	class DX12PipelineState : public QueryInternal<DX12PipelineState>
	{
	public:
		DX12PipelineState() = default;
		explicit DX12PipelineState(winrt::com_ptr<ID3D12PipelineState> xpipeline)
		{
			pipeline = std::move(xpipeline);
		}
		operator DX12PipelineStateView()const noexcept
		{
			return GetPipeline();
		}
	};

}