#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_swapchain.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/vulkan/vk_rtv.h>
#include <wisdom/vulkan/vk_command_list.h>
#include <wisdom/vulkan/vk_command_queue.h>


namespace wis
{
	class VKSwapChain;

	template<>
	class Internal<VKSwapChain>
	{
		friend class VKSwapChain;
		static constexpr inline bool valid = true;
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::SwapchainKHR> swap, vk::UniqueSurfaceKHR surface, vk::Queue graphics_queue, vk::Queue present_queue, vk::Format format)
			:swap(std::move(swap))
			, surface(std::move(surface))
			, present_queue(present_queue)
			, graphics_queue(graphics_queue)
			, format(format)
			, device(swap.get_parent_handle())
		{
			graphics_semaphore = device->createSemaphoreUnique({});
			present_semaphore = device->createSemaphoreUnique({});
		}
	public:
		vk::SwapchainKHR GetSwapchain()const noexcept
		{
			return swap.get();
		}
		vk::SurfaceKHR GetSurface()const noexcept
		{
			return surface.get();
		}
	protected:
		vk::UniqueSurfaceKHR surface; //order mandated

		wis::shared_handle<vk::Device> device;
		wis::shared_handle<vk::SwapchainKHR> swap;

		vk::Queue graphics_queue;
		vk::Queue present_queue;
		vk::Format format = vk::Format::eB8G8R8A8Unorm;

		vk::UniqueSemaphore graphics_semaphore;
		vk::UniqueSemaphore present_semaphore;
	};

	class VKSwapChain : public QueryInternal<VKSwapChain>
	{
	public:
		VKSwapChain() = default;
		explicit VKSwapChain(wis::shared_handle<vk::SwapchainKHR> xswap,
			vk::UniqueSurfaceKHR surface,
			vk::Queue graphics_queue,
			VKCommandQueue present_queue,
			VKCommandList initialization,
			vk::Format format, bool stereo, uint32_t layers = 1)
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
						.range = wis::EntireTexture
					},
					{ i, format });
			}
			initialization.Close();
			present_queue.ExecuteCommandList(initialization);

			
			present_queue.GetInternal().GetQueue().waitIdle();

			AquireNextIndex();
		}
		VKSwapChain(VKSwapChain&&)noexcept = default;
		VKSwapChain& operator=(VKSwapChain&&)noexcept = default;
		~VKSwapChain()
		{
			ReleaseSemaphore();
		}
	public:
		[[nodiscard]] uint32_t GetNextIndex()const noexcept
		{
			return present_index;
		}
		[[nodiscard]]
		std::span<const VKTexture> GetRenderTargets()const noexcept
		{
			return back_buffers;
		}

		[[nodiscard]]
		auto GetBackBuffer()const noexcept
		{
			return back_buffers[present_index];
		}

		bool Present()noexcept
		{
			vk::Semaphore a = graphics_semaphore.get();
			vk::SubmitInfo desc{
				0u, nullptr, nullptr, 0u, nullptr, 1u, & a
			};
			graphics_queue.submit(desc); //finish all the work on graphics queue

			auto x = swap.get();
			vk::PresentInfoKHR present_info{
				1, & a, 1, & x, & present_index, nullptr
			};

			return wis::succeded(present_queue.presentKHR(present_info)) &&
				AquireNextIndex();
		}

		[[nodiscard]]
		bool StereoSupported()const noexcept
		{
			return stereo;
		}
	private:
		bool AquireNextIndex()noexcept
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
		void ReleaseSemaphore()noexcept
		{
			if (!present_semaphore)return;
			vk::SubmitInfo signal_submit_info = {};
			signal_submit_info.pNext = nullptr;
			signal_submit_info.signalSemaphoreCount = 1;
			signal_submit_info.pSignalSemaphores = &present_semaphore.get();
			wis::succeded(present_queue.submit(1, &signal_submit_info, {}));
			present_queue.waitIdle();
		}
	private:
		std::vector<VKTexture> back_buffers;
		mutable uint32_t present_index = 0;
		bool stereo = false;
	};
}
