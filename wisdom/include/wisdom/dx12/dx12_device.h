#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/dx12/dx12_swapchain.h>
#include <wisdom/dx12/dx12_command_queue.h>
#include <wisdom/dx12/dx12_command_list.h>
#include <wisdom/dx12/dx12_fence.h>
#include <wisdom/dx12/dx12_state_builder.h>
#include <wisdom/api/api_input_layout.h>
#endif

WIS_EXPORT namespace wis
{
	class DX12Device;

	template<>
	class Internal<DX12Device>
	{
	protected:
		static inline constexpr auto heap_size = 128u;
		static constexpr inline bool valid = true;
	public:
		[[nodiscard]]ID3D12Device10* GetDevice()const noexcept {
			return device.get();
		}
	protected:
		winrt::com_ptr<ID3D12Device10> device{};

		winrt::com_ptr<ID3D12DescriptorHeap> rtv_heap{};
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_start;
		uint32_t rtv_increment = 0;
	};


	/// @brief A DX12 device
	class DX12Device final : public QueryInternal<DX12Device>
	{
	public:
		DX12Device() = default;
		explicit DX12Device(DX12AdapterView adapter) {
			Initialize(adapter);
		};
	public:
		/// @brief Initialize the device
		/// @param adapter The adapter to use
		/// @return true if the device was initialized successfully
		WIS_INLINE bool Initialize(DX12AdapterView adapter)noexcept;
		explicit operator bool()const noexcept{
			return bool(device);
		}
		operator DX12DeviceView()const noexcept{
			return GetDevice();
		}
	public:

		/// @brief Create a swapchain
		/// @param queue The queue to use
		/// @param options The options to use
		/// @param surface The surface to use
		/// @return The created swapchain
		[[nodiscard]] WIS_INLINE DX12SwapChain CreateSwapchain(
			DX12CommandQueueView queue, 
			wis::SwapchainOptions options, 
			wis::SurfaceParameters surface)const;

		/// @brief Create a command queue
		/// @param options The options to use
		/// @return The created command queue
		[[nodiscard]] WIS_INLINE DX12CommandQueue
			CreateCommandQueue(QueueOptions options = QueueOptions{})const;

		/// @brief Create a command list
		/// @param list_type The type of list to create
		/// @return The created command list
		[[nodiscard]] WIS_INLINE DX12CommandList CreateCommandList(QueueType list_type)const;

		/// @brief Create a fence
		[[nodiscard]] WIS_INLINE DX12Fence CreateFence()const;

		/// @brief Create a root signature (empty)
		[[nodiscard]] WIS_INLINE DX12RootSignature CreateRootSignature()const;

		/// @brief Create a graphics pipeline
		[[nodiscard]] WIS_INLINE DX12PipelineState CreateGraphicsPipeline(
			DX12GraphicsPipelineDesc desc,
			std::span<const InputLayoutDesc> input_layout)const;

		/// @brief Create a shader
		/// @param blob The shader blob
		/// @param type The shader type
		[[nodiscard]] DX12Shader CreateShader(shared_blob blob, ShaderType type)const noexcept{
			return DX12Shader{ std::move(blob), type };
		}

		/// @brief Create a render pass
		/// @param rtv_descs The render target descriptions
		/// @param dsv_desc The depth stencil description
		/// @param samples The sample count
		/// @param vrs_format The variable rate shading format
		[[nodiscard]] WIS_INLINE DX12RenderPass CreateRenderPass(
			wis::Size2D,
			std::span<ColorAttachment> rtv_descs,
			DepthStencilAttachment dsv_desc = DepthStencilAttachment{},
			SampleCount samples = SampleCount::s1,
			DataFormat vrs_format = DataFormat::unknown)const;


		//TODO: other formats, better allocator
		/// @brief Create a render target view
		/// @param texture The texture to create the view for
		/// @param range The range of the view
		[[nodiscard]] WIS_INLINE DX12RenderTargetView CreateRenderTargetView(DX12TextureView texture, RenderSelector range = {});
	};
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_device.inl"
#endif