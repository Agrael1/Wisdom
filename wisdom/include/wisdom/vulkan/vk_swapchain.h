#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_swapchain.h>
#include <wisdom/vulkan/vk_command_list.h>
#include <wisdom/vulkan/vk_command_queue.h>
#include <wisdom/vulkan/vk_resource.h>
#endif


WIS_EXPORT namespace wis
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
		WIS_INLINE explicit VKSwapChain(wis::shared_handle<vk::SwapchainKHR> xswap,
			vk::UniqueSurfaceKHR surface,
			vk::Queue graphics_queue,
			VKCommandQueue present_queue,
			VKCommandList initialization,
			vk::Format format, bool stereo, uint32_t layers = 1);
		VKSwapChain(VKSwapChain&&)noexcept = default;
		VKSwapChain& operator=(VKSwapChain&&)noexcept = default;
		~VKSwapChain() { ReleaseSemaphore(); }
	public:
		/// @brief Get the current image index in the swapchain
		/// @return Index of the current image
		[[nodiscard]] uint32_t GetNextIndex()const noexcept
		{
			return present_index;
		}

		/// @brief Get all the render targets in the swapchain
		/// @return Span of render targets
		[[nodiscard]]
		std::span<const VKTexture> GetRenderTargets()const noexcept
		{
			return back_buffers;
		}

		/// @brief Get the current render target in the swapchain
		/// @return Buffer view of the current render target TODO: Make a texture view
		[[nodiscard]]
		auto GetBackBuffer()const noexcept
		{
			return back_buffers[present_index];
		}

		/// @brief Present the swapchain
		/// @return true if succeeded
		WIS_INLINE bool Present()noexcept;

		/// @brief Check if stereo is supported
		/// @return true if stereo is supported
		[[nodiscard]]
		bool StereoSupported()const noexcept
		{
			return stereo;
		}
	private:
		WIS_INLINE bool AquireNextIndex()noexcept;
		WIS_INLINE void ReleaseSemaphore()noexcept;
	private:
		std::vector<VKTexture> back_buffers; //< Render targets
		mutable uint32_t present_index = 0; //< Current render target index
		bool stereo = false; //< True if stereo is supported by the swapchain
	};
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/vk_swapchain.inl"
#endif