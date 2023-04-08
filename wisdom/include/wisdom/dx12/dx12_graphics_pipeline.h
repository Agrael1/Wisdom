#pragma once
#include <wisdom/api/api_internal.h>
#include <d3d12.h>
#include <winrt/base.h>

namespace wis
{
	class DX12GraphicsPipeline;

	template<>
	class Internal<DX12GraphicsPipeline>
	{
	public:
		auto GetPipeline()const noexcept
		{
			return pipeline;
		}
	protected:
		winrt::com_ptr<ID3D12PipelineState> pipeline;
	};


	class DX12GraphicsPipeline : public QueryInternal<DX12GraphicsPipeline>
	{
	public:
		DX12GraphicsPipeline() = default;
		explicit DX12GraphicsPipeline(winrt::com_ptr<ID3D12PipelineState> xpipeline)
		{
			pipeline = std::move(xpipeline);
		}
	};
}