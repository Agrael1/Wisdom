#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#include <wisdom/vulkan/vk_checks.h>

namespace wis
{
	class VKFence;

	template<>
	class Internal<VKFence>
	{
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::Semaphore> fence)
			:fence(std::move(fence)), device(this->fence.get_parent()){ }
	public:
		vk::Semaphore GetFence()const noexcept
		{
			return fence.get();
		}
	protected:
		wis::shared_handle<vk::Semaphore> fence;
		vk::Device device; //little overhead for better performance
	};

	using VKFenceView = vk::Semaphore;

	class VKFence : public QueryInternal<VKFence>
	{
	public:
		VKFence() = default;
		explicit VKFence(wis::shared_handle<vk::Semaphore> fence)
			:QueryInternal(std::move(fence))
		{}
		operator VKFenceView()const noexcept
		{
			return GetFence();
		}
	public:
		uint64_t GetCompletedValue()const noexcept
		{
			return device.getSemaphoreCounterValue(fence.get());
		}
		bool Wait(uint64_t value)const noexcept
		{
			if (GetCompletedValue() >= value)
				return true;

			auto s = fence.get();
			vk::SemaphoreWaitInfo waitInfo{{}, 1, & s, & value};
			return succeded(device.waitSemaphores(waitInfo, std::numeric_limits<uint64_t>::max()));
		}
		void Signal(uint64_t value)noexcept
		{
			vk::SemaphoreSignalInfo signalInfo
			{
				fence.get(),
				value
			};
			std::ignore = device.signalSemaphore(&signalInfo);
		}
	};
}