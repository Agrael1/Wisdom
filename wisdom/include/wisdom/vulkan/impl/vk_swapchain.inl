//#include <wisdom/vulkan/vk_swapchain.h>

wis::VKSwapChain::VKSwapChain(wis::shared_handle<vk::SwapchainKHR> xswap,
	vk::UniqueSurfaceKHR surface,
	vk::Queue graphics_queue,
	VKCommandQueue present_queue,
	VKCommandList initialization,
	vk::Format format, bool stereo, uint32_t layers)
	:QueryInternal(std::move(xswap), std::move(surface), graphics_queue, present_queue, format), stereo(stereo)
{
	initialization.Reset();
	auto xback_buffers = device->getSwapchainImagesKHR(swap.get());
	back_buffers.reserve(xback_buffers.size());
	for (auto& i : xback_buffers)
	{
		back_buffers.emplace_back(
			format, wis::shared_handle<vk::Image>{i, swap.get_parent_handle(), true});
		initialization.TextureBarrier(
			{
				.state_before = TextureState::Undefined,
				.state_after = TextureState::Present,
				.access_before = ResourceAccess::NoAccess,
				.access_after = ResourceAccess::Common,
			},
			{ i, format });
	}
	initialization.Close();
	present_queue.ExecuteCommandList(initialization);


	present_queue.GetInternal().GetQueue().waitIdle();

	AquireNextIndex();
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

	return wis::succeded(present_queue.presentKHR(present_info)) &&
		AquireNextIndex();
}

bool wis::VKSwapChain::AquireNextIndex()noexcept
{
	present_index = device->acquireNextImageKHR(swap.get(), std::numeric_limits<uint64_t>::max(), present_semaphore.get()).value;

	vk::SubmitInfo signal_submit_info = {};
	signal_submit_info.pNext = nullptr;
	signal_submit_info.waitSemaphoreCount = 1;
	signal_submit_info.pWaitSemaphores = &present_semaphore.get();
	vk::PipelineStageFlags waitDstStageMask = vk::PipelineStageFlagBits::eTransfer;
	signal_submit_info.pWaitDstStageMask = &waitDstStageMask;
	return wis::succeded(present_queue.submit(1, &signal_submit_info, {}));
}

void wis::VKSwapChain::ReleaseSemaphore()noexcept
{
	if (!present_semaphore)return;
	vk::SubmitInfo signal_submit_info = {};
	signal_submit_info.pNext = nullptr;
	signal_submit_info.signalSemaphoreCount = 1;
	signal_submit_info.pSignalSemaphores = &present_semaphore.get();
	wis::succeded(present_queue.submit(1, &signal_submit_info, {}));
	present_queue.waitIdle();
}