#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_swapchain.h>
#include <wisdom/vulkan/vk_checks.h>


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
		Internal(wis::shared_handle<vk::SwapchainKHR> swap, vk::UniqueSurfaceKHR surface, vk::Queue present_queue)
			:swap(std::move(swap)), surface(std::move(surface)), present_queue(present_queue){}
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
		vk::UniqueSurfaceKHR surface;
		wis::shared_handle<vk::SwapchainKHR> swap;
		vk::Queue present_queue;
		uint32_t present_index = 0;
	};

	class VKSwapChain : public QueryInternal<VKSwapChain>
	{
	public:
		VKSwapChain() = default;
		explicit VKSwapChain(wis::shared_handle<vk::SwapchainKHR> swap, vk::UniqueSurfaceKHR surface, vk::Queue present_queue)
			:QueryInternal(std::move(swap), std::move(surface), present_queue)
		{
			
		}
	public:
		[[nodiscard]] uint32_t GetBackBufferIndex()const noexcept
		{
			return present_index;
		}
		template<class Self>
		[[nodiscard]] auto GetRenderTargets(this Self&& s)noexcept
		{

		}
		template<class Self>
		[[nodiscard]] auto& GetBackBuffer(this Self&& s)noexcept
		{
			
		}
		[[nodiscard]]
		auto GetRTV(size_t frame)const noexcept
		{
			
		}
		[[nodiscard]]
		auto GetBackBufferRTV()const noexcept
		{
			
		}

		// The API does not wait until the execution ended,
		// The caller must ensure correct execution (DX12 compatibility)
		bool Present()noexcept
		{
			auto x = swap.get();
			vk::PresentInfoKHR present_info{
				0,nullptr, 1, &x, &present_index, nullptr
			};
			return wis::succeded(present_queue.presentKHR(present_info));
		}
	private:
		
	};
}
