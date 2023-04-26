#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_fence.h>
#include <wisdom/vulkan/vk_command_list.h>


namespace wis
{
	class VKCommandQueue;

	template<>
	class Internal<VKCommandQueue>
	{
		static constexpr inline bool valid = true;
	public:
		Internal() = default;
		Internal(vk::Queue queue):queue(queue){}
	public:
		auto GetQueue()const noexcept
		{
			return queue;
		}
	protected:
		vk::Queue queue;
	};
	using VKCommandQueueView = vk::Queue;

	class VKCommandQueue : public QueryInternal<VKCommandQueue>
	{
		using intern = QueryInternal<VKCommandQueue>;
	public:
		VKCommandQueue() = default;
		explicit VKCommandQueue(vk::Queue queue)
		:QueryInternal(queue){}
		operator VKCommandQueueView()const noexcept
		{
			return queue;
		}
	public:
		void ExecuteCommandList(VKCommandListView command_list)
		{
			vk::PipelineStageFlags wait_dst_stage_mask = vk::PipelineStageFlagBits::eAllCommands;

			vk::SubmitInfo submit_info = {};
			submit_info.commandBufferCount = 1;
			submit_info.pCommandBuffers = &command_list;
			submit_info.pWaitDstStageMask = &wait_dst_stage_mask;

			queue.submit(submit_info);
		}
		bool Signal(VKFenceView fence, uint64_t value)
		{
			vk::TimelineSemaphoreSubmitInfo submit
			{
				0,nullptr,1,&value
			};

			vk::SubmitInfo info
			{
				0, nullptr,
				nullptr, 0u,nullptr,
				1, &fence,
				&submit
			};
			return wis::succeded(queue.submit(1, &info, nullptr));
		}
	};
}