#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>

namespace wis
{
	class VKFence;

	template<>
	class Internal<VKFence>
	{
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::Semaphore> fence)
			:fence(std::move(fence)){}
	public:
		vk::Semaphore GetFence()const
		{
			return fence.get();
		}
	protected:
		wis::shared_handle<vk::Semaphore> fence;
	};

	class VKFence : public QueryInternal<VKFence>
	{
	public:
		VKFence() = default;
		explicit VKFence(wis::shared_handle<vk::Semaphore> fence)
			:QueryInternal(std::move(fence))
		{}
	public:
		uint64_t GetCompletedValue()const noexcept
		{
			//return fence->GetCompletedValue();
		}
		bool Wait(uint64_t value)const noexcept
		{
			//return GetCompletedValue() >= value ?
			//	true :
			//	wis::succeded_weak(fence->SetEventOnCompletion(value, fence_event.get()))
			//	&& fence_event.wait();
		}
		void Signal(uint64_t value)noexcept
		{
			VkSemaphoreSignalInfo signalInfo
			{
				.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO,
				.pNext = nullptr,
				.semaphore = fence.get(),
				.value = value
			};
			vkSignalSemaphore(fence.get_parent(), &signalInfo);
		}
	};
}