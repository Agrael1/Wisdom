#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>

namespace wis
{
	class VKPipelineState;

	template<>
	class Internal<VKPipelineState>
	{
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::Pipeline> pipeline): pipeline(std::move(pipeline)){};
	public:
		auto GetPipeline()const noexcept
		{
			return pipeline.get();
		}
	private:
		wis::shared_handle<vk::Pipeline> pipeline;
	};

	using VKPipelineStateView = vk::Pipeline;


	/// @brief Pipeline state object
	class VKPipelineState : public QueryInternal<VKPipelineState>
	{
	public:
		VKPipelineState() = default;
		explicit VKPipelineState(wis::shared_handle<vk::Pipeline> pipeline)
			:QueryInternal(std::move(pipeline))
		{}
		operator VKPipelineStateView()const noexcept
		{
			return GetPipeline();
		}
		operator bool()const noexcept
		{
			return bool(GetPipeline());
		}
	};

}