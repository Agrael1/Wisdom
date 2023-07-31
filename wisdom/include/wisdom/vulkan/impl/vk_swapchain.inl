#include <wisdom/vulkan/vk_swapchain.h>

wis::VKSwapChain::VKSwapChain(wis::shared_handle<vk::SwapchainKHR> swap,
                              vk::Queue graphics_queue,
                              VKCommandQueue present_queue,
                              VKCommandList initialization,
                              vk::SurfaceFormatKHR format,
                              vk::PresentModeKHR present_mode,
                              bool stereo)
	:QueryInternal(std::move(swap), graphics_queue, format, present_mode)
	, stereo(stereo)
	, present_queue(std::move(present_queue))
	,initialization(std::move(initialization))
{
	CreateImages();
}

bool wis::VKSwapChain::Present()noexcept
{
	vk::Semaphore a = graphics_semaphore.get();
	vk::SubmitInfo desc{
		0u, nullptr, nullptr, 0u, nullptr, 1u, &a
	};
	graphics_queue.submit(desc); //finish all the work on graphics queue

	auto x = swap.get();
	vk::PresentInfoKHR present_info{
		1, &a, 1, &x, &present_index, nullptr
	};

	return wis::succeded(present_queue.GetInternal().GetQueue().presentKHR(&present_info)) &&
		AquireNextIndex();
}

bool wis::VKSwapChain::AquireNextIndex()noexcept
{
	present_index = device.acquireNextImageKHR(swap.get(), std::numeric_limits<uint64_t>::max(), present_semaphore.get()).value;

	vk::SubmitInfo signal_submit_info = {};
	signal_submit_info.pNext = nullptr;
	signal_submit_info.waitSemaphoreCount = 1;
	signal_submit_info.pWaitSemaphores = &present_semaphore.get();
	vk::PipelineStageFlags waitDstStageMask = vk::PipelineStageFlagBits::eTransfer;
	signal_submit_info.pWaitDstStageMask = &waitDstStageMask;
	return wis::succeded(present_queue.GetInternal().GetQueue().submit(1, &signal_submit_info, {}));
}

void wis::VKSwapChain::ReleaseSemaphore()noexcept
{
	if (!present_semaphore)return;
	auto queue = present_queue.GetInternal().GetQueue();

	vk::SubmitInfo signal_submit_info = {};
	signal_submit_info.pNext = nullptr;
	signal_submit_info.signalSemaphoreCount = 1;
	signal_submit_info.pSignalSemaphores = &present_semaphore.get();
	wis::succeded(queue.submit(1, &signal_submit_info, {}));
	queue.waitIdle();
}