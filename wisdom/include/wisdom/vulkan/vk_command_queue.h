#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_fence.h>


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
		void ExecuteCommandList()
		{

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
				1, &fence.first,
				&submit
			};
			return wis::succeded(queue.submit(1, &info, nullptr));
		}
	};
}