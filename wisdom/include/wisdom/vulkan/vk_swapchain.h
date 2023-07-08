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
		Internal(wis::shared_handle<vk::SwapchainKHR> swap, vk::UniqueSurfaceKHR surface, vk::Queue graphics_queue, vk::SurfaceFormatKHR format, vk::PresentModeKHR present_mode)
			:swap(std::move(swap))
			, surface(std::move(surface))
			, graphics_queue(graphics_queue)
			, format(format)
			, present_mode(present_mode)
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
		vk::SurfaceFormatKHR format;
		vk::PresentModeKHR present_mode = vk::PresentModeKHR::eFifo;

		vk::UniqueSemaphore graphics_semaphore;
		vk::UniqueSemaphore present_semaphore;
	};


	struct SwapChainDesc
	{
		wis::shared_handle<vk::SwapchainKHR> xswap;
		vk::UniqueSurfaceKHR surface;
		vk::Queue graphics_queue;
		VKCommandQueue present_queue;
		VKCommandList initialization;
		vk::SurfaceFormatKHR format; 
		vk::PresentModeKHR present_mode;
		bool stereo; 
	};

	class VKSwapChain : public QueryInternal<VKSwapChain>
	{
	public:
		VKSwapChain() = default;
		WIS_INLINE explicit VKSwapChain(SwapChainDesc desc);
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

		/// @brief Resize the swapchain
		/// For the method to succeed, all swapchain buffers must be released first
		/// @param width New width
		/// @param height New height
		/// @return true if succeeded
		[[nodiscard]] bool Resize(uint32_t width, uint32_t height)noexcept
		{
			uint32_t nbuffers = back_buffers.size();
			back_buffers.clear();

			vk::SwapchainCreateInfoKHR desc
			{
				vk::SwapchainCreateFlagBitsKHR{}, surface.get(),
					nbuffers, format.format, format.colorSpace,
					vk::Extent2D{ width, height },
					stereo ? 2u : 1u,
					vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst,
					vk::SharingMode::eExclusive, 0u, nullptr,
					vk::SurfaceTransformFlagBitsKHR::eIdentity,
					vk::CompositeAlphaFlagBitsKHR::eOpaque,
					present_mode, true, swap.get()
			};
			swap = wis::shared_handle{ device->createSwapchainKHR(desc), device };
			CreateImages();
			return true;
		}
	private:
		void CreateImages()noexcept
		{
			initialization.Reset();
			auto xback_buffers = device->getSwapchainImagesKHR(swap.get());
			back_buffers.reserve(xback_buffers.size());
			for (auto& i : xback_buffers)
			{
				back_buffers.emplace_back(
					format.format, wis::shared_handle<vk::Image>{i, swap.get_parent_handle(), true});
				initialization.TextureBarrier(
					{
						.state_before = TextureState::Undefined,
						.state_after = TextureState::Present,
						.access_before = ResourceAccess::NoAccess,
						.access_after = ResourceAccess::Common,
					},
					{ i, format.format });
			}
			initialization.Close();
			present_queue.ExecuteCommandList(initialization);
			present_queue.GetInternal().GetQueue().waitIdle();

			AquireNextIndex();
		}
		WIS_INLINE bool AquireNextIndex()noexcept;
		WIS_INLINE void ReleaseSemaphore()noexcept;
	private:
		VKCommandQueue present_queue;
		VKCommandList initialization; //< Initialization command list
		std::vector<VKTexture> back_buffers; //< Render targets
		mutable uint32_t present_index = 0; //< Current render target index
		bool stereo = false; //< True if stereo is supported by the swapchain
	};
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/vk_swapchain.inl"
#endif