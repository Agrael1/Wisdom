#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_swapchain.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_factory.h>
#include <wisdom/dx12/dx12_format.h>
#include <wisdom/dx12/dx12_command_queue.h>
#include <wisdom/dx12/dx12_command_list.h>
#include <wisdom/dx12/dx12_fence.h>
#include <wisdom/dx12/dx12_root_signature.h>
#include <wisdom/dx12/dx12_render_pass.h>
#include <wisdom/dx12/dx12_pipeline_state.h>
#include <wisdom/dx12/dx12_state_builder.h>
#include <d3d12.h>
#include <d3dx12/d3dx12.h>
#include <wisdom/api/api_input_layout.h>
#include <wisdom/api/api_render_pass.h>
#include <wisdom/util/small_allocator.h>

namespace wis
{
	class DX12Device;

	template<>
	class Internal<DX12Device>
	{
	protected:
		static inline constexpr auto heap_size = 128u;
		static constexpr inline bool valid = true;
	public:
		[[nodiscard]]
		ID3D12Device10* GetDevice()const noexcept {
			return device.get();
		}
	protected:
		winrt::com_ptr<ID3D12Device10> device{};

		winrt::com_ptr<ID3D12DescriptorHeap> rtv_heap{};
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_start;
		uint32_t rtv_increment = 0;
	};
	using DX12DeviceView = ID3D12Device10*;


	class DX12Device final : public QueryInternal<DX12Device>
	{
	public:
		DX12Device() = default;
		DX12Device(DX12AdapterView adapter) {
			Initialize(adapter);
		};
	public:
		bool Initialize(DX12AdapterView adapter)noexcept {
			if (!wis::succeded(D3D12CreateDevice(adapter,
				D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device9), device.put_void())))
				return false;

			// Describe and create a render target view (RTV) descriptor heap.
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = heap_size;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			wis::check_hresult(device->CreateDescriptorHeap(&rtvHeapDesc, __uuidof(*rtv_heap), rtv_heap.put_void()));
			rtv_increment = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			rtv_start = rtv_heap->GetCPUDescriptorHandleForHeapStart();
			return true;
		}
		explicit operator bool()const noexcept
		{
			return bool(device);
		}
		operator DX12DeviceView()const noexcept
		{
			return GetDevice();
		}
	public:

		[[nodiscard]]
		DX12SwapChain CreateSwapchain(DX12CommandQueueView queue, wis::SwapchainOptions options, wis::SurfaceParameters surface)const
		{
			DXGI_SWAP_CHAIN_DESC1 desc
			{
				.Width = options.width,
				.Height = options.height,
				.Format = DXGI_FORMAT(options.format),
				.Stereo = DX12Factory::GetFactory()->IsWindowedStereoEnabled() && options.stereo,
				.SampleDesc{.Count = 1, .Quality = 0},
				.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
				.BufferCount = options.frame_count,
				.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH,
				.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
				.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
				.Flags = 0
			};

			winrt::com_ptr<IDXGISwapChain4> chain;
			switch (surface.type)
			{
			default:
			case SurfaceParameters::Type::Win32:
				chain = DX12Factory::SwapChainForWin32(desc, surface.hwnd, queue);
				break;
#ifdef WISDOM_UWP
			case SurfaceParameters::Type::WinRT:
				chain = DX12Factory::SwapChainForCoreWindow(desc, surface.core_window, queue);
				break;
#endif
			}
			return DX12SwapChain{ std::move(chain) ,options.frame_count, bool(desc.Stereo) };
		}

		[[nodiscard]]
		DX12CommandQueue CreateCommandQueue(QueueOptions options = QueueOptions{})const
		{
			winrt::com_ptr<ID3D12CommandQueue> queue;
			D3D12_COMMAND_QUEUE_DESC desc
			{
				.Type = D3D12_COMMAND_LIST_TYPE(options.type),
				.Priority = int(options.priority),
				.Flags = D3D12_COMMAND_QUEUE_FLAGS(options.flags),
				.NodeMask = options.node_mask
			};
			wis::check_hresult(device->CreateCommandQueue(&desc, __uuidof(*queue), queue.put_void()));
			return { std::move(queue) };
		}

		[[nodiscard]]
		DX12CommandList CreateCommandList(QueueType list_type)const
		{
			D3D12_COMMAND_LIST_TYPE clty = D3D12_COMMAND_LIST_TYPE(list_type);
			winrt::com_ptr<ID3D12CommandAllocator> xallocator;
			winrt::com_ptr<ID3D12GraphicsCommandList9> xcommand_list;

			wis::check_hresult(device->CreateCommandAllocator(clty, __uuidof(*xallocator), xallocator.put_void()));
			wis::check_hresult(device->CreateCommandList1(0, clty, D3D12_COMMAND_LIST_FLAG_NONE, __uuidof(ID3D12GraphicsCommandList7), xcommand_list.put_void()));

			return DX12CommandList{
				std::move(xallocator),
				std::move(xcommand_list)
			};
		}

		[[nodiscard]]
		DX12Fence CreateFence()const
		{
			winrt::com_ptr<ID3D12Fence1> fence;
			wis::check_hresult(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(*fence), fence.put_void()));
			return DX12Fence{ std::move(fence) };
		}

		[[nodiscard]]
		DX12RootSignature CreateRootSignature()const
		{
			winrt::com_ptr<ID3D12RootSignature> rsig;
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc{ 0, nullptr, 0, nullptr,
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
			};

			winrt::com_ptr<ID3DBlob> signature;
			winrt::com_ptr<ID3DBlob> error;
			wis::check_hresult(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, signature.put(), error.put()));
			wis::check_hresult(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), __uuidof(*rsig), rsig.put_void()));
			return DX12RootSignature{ std::move(rsig) };
		}

		[[nodiscard]]
		DX12PipelineState CreateGraphicsPipeline(DX12GraphicsPipelineDesc desc, std::span<const InputLayoutDesc> input_layout)const //movable
		{
			winrt::com_ptr<ID3D12PipelineState> state;
			D3D12_PIPELINE_STATE_STREAM_DESC xdesc{};

			D3D12_INPUT_LAYOUT_DESC iadesc{};
			iadesc.NumElements = input_layout.size();

			wis::internals::uniform_allocator<D3D12_INPUT_ELEMENT_DESC> ia;
			for (auto& i : input_layout)
			{
				ia.allocate(D3D12_INPUT_ELEMENT_DESC
				{
					.SemanticName = i.semantic_name,
					.SemanticIndex = i.semantic_index,
					.Format = DXGI_FORMAT(i.format),
					.InputSlot = i.input_slot,
					.AlignedByteOffset = i.aligned_byte_offset,
					.InputSlotClass = D3D12_INPUT_CLASSIFICATION(i.input_slot_class),
					.InstanceDataStepRate = i.instance_data_step_rate
				});
			}
			iadesc.pInputElementDescs = ia.data();


			internals::memory_pool psta;
			psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE>() = desc.sig.GetInternal().GetRootSignature();
			psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT>() = iadesc;

			if (desc.vs)
			{
				auto d = desc.vs.GetInternal().GetShaderBytecode();
				psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_VS>() = { d.data(), d.size() };
			}
			if (desc.ps)
			{
				auto d = desc.ps.GetInternal().GetShaderBytecode();
				psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_PS>() = { d.data(), d.size() };
			}
			if (desc.gs)
			{
				auto d = desc.gs.GetInternal().GetShaderBytecode();
				psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_GS>() = { d.data(), d.size() };
			}

			if (desc.hs)
			{
				auto d = desc.hs.GetInternal().GetShaderBytecode();
				psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_HS>() = { d.data(), d.size() };
			}
			if (desc.ds)
			{
				auto d = desc.ds.GetInternal().GetShaderBytecode();
				psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_DS>() = { d.data(), d.size() };
			}
			if (desc.target_formats.size())
			{
				D3D12_RT_FORMAT_ARRAY rta{
					.NumRenderTargets = uint32_t(desc.target_formats.size())
				};
				std::memcpy(rta.RTFormats, desc.target_formats.data(), desc.target_formats.size() * sizeof(DataFormat));
				psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS>() = rta;
			}
			xdesc.pPipelineStateSubobjectStream = psta.data<void>();
			xdesc.SizeInBytes = psta.size_bytes();


			wis::check_hresult(device->CreatePipelineState(&xdesc, __uuidof(*state), state.put_void()));
			return DX12PipelineState{ std::move(state) };
		}

		[[nodiscard]]
		DX12Shader CreateShader(shared_blob<std::byte> blob, ShaderType type)const noexcept
		{
			return DX12Shader{ std::move(blob), type };
		}

		[[nodiscard]]
		DX12RenderPass CreateRenderPass(wis::Size2D, std::span<ColorAttachment> rtv_descs,
			DepthStencilAttachment dsv_desc = DepthStencilAttachment{},
			SampleCount samples = SampleCount::s1,
			DataFormat vrs_format = DataFormat::unknown)const
		{
			std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> om_rtv;
			om_rtv.reserve(rtv_descs.size());

			for (auto& x : rtv_descs)
			{
				D3D12_RENDER_PASS_BEGINNING_ACCESS begin
				{
					.Type = convert_dx(x.load)
				};
				D3D12_RENDER_PASS_ENDING_ACCESS end
				{
					.Type = convert_dx(x.store)
				};
				om_rtv.push_back({ 0, begin, end });
			}

			wis::internals::uniform_allocator<DataFormat, max_render_targets> a;
			for (auto& i : rtv_descs)
				a.allocate() = i.format;

			if (dsv_desc.format == DataFormat::unknown)
				return DX12RenderPass{ a, std::move(om_rtv) };


			D3D12_RENDER_PASS_BEGINNING_ACCESS depth_begin{ convert_dx(dsv_desc.depth_load), {} };
			D3D12_RENDER_PASS_ENDING_ACCESS depth_end{ convert_dx(dsv_desc.depth_store), {} };
			D3D12_RENDER_PASS_BEGINNING_ACCESS stencil_begin{ convert_dx(dsv_desc.stencil_load), {} };
			D3D12_RENDER_PASS_ENDING_ACCESS stencil_end{ convert_dx(dsv_desc.stencil_store), {} };

			return DX12RenderPass{ a, std::move(om_rtv), D3D12_RENDER_PASS_DEPTH_STENCIL_DESC{ 0, depth_begin, stencil_begin, depth_end, stencil_end } };
		}

		[[nodiescard]] //TODO: other formats, better allocator
		DX12RenderTargetView CreateRenderTargetView(DX12TextureView texture, RenderSelector range = {})
		{
			D3D12_RENDER_TARGET_VIEW_DESC desc
			{
				.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN,
				.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2DARRAY,
				.Texture2DArray{
				.MipSlice = range.mip,
					.FirstArraySlice = range.base_layer,
					.ArraySize = range.extent_layers,
					.PlaneSlice = 0
				}
			};
			device->CreateRenderTargetView(texture, &desc, rtv_start);
			DX12RenderTargetView rtvm{ rtv_start };
			rtv_start.Offset(1, rtv_increment);
			return rtvm;
		}
	};
}