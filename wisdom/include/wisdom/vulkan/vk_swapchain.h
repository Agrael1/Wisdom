#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_swapchain.h>


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
		Internal(wis::shared_handle<vk::SwapchainKHR> swap, vk::UniqueSurfaceKHR surface)
			:swap(std::move(swap)), surface(std::move(surface)){}
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
	};

	class VKSwapChain : public QueryInternal<VKSwapChain>
	{
	public:
		VKSwapChain() = default;
		explicit VKSwapChain(wis::shared_handle<vk::SwapchainKHR> swap, vk::UniqueSurfaceKHR surface)
			:QueryInternal(std::move(swap), std::move(surface))
		{
			
		}
	public:
		[[nodiscard]] uint32_t GetBackBufferIndex()const noexcept
		{

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

		bool Present()noexcept
		{

		}
	private:
		
	};
}
