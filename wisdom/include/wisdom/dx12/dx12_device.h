#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_command_queue.h>
#include <wisdom/dx12/dx12_command_list.h>
#include <wisdom/dx12/dx12_fence.h>
#include <wisdom/dx12/dx12_root_signature.h>
#include <wisdom/dx12/dx12_pipeline_state.h>
#include <wisdom/dx12/dx12_state_builder.h>
#include <d3d12.h>
#include <d3dx12/d3dx12.h>
#include <wisdom/api/api_input_layout.h>

namespace wis
{
	class DX12Device;

	template<>
	class Internal<DX12Device>
	{
		static constexpr inline bool valid = true;
	public:
		[[nodiscard]] 
		ID3D12Device10* GetDevice()const noexcept {
			return device.get();
		}
	protected:
		winrt::com_ptr<ID3D12Device10> device{};
	};
	using DX12DeviceView = ID3D12Device10*;


	class DX12Device final: public QueryInternal<DX12Device>
	{
		struct PipelineStreamAllocator
		{
			static inline constexpr auto allocator_size = 1024;
			alignas(void*)std::array<std::byte, allocator_size> allocator{};
			size_t size = 0;

			template<typename T>
			T& allocate()noexcept
			{
				T& x = *reinterpret_cast<T*>(allocator.data() + size);
				size += sizeof(T);
				return x;
			};

		};
	public:
		DX12Device() = default;
		DX12Device(DX12AdapterView adapter) {
			wis::check_hresult(D3D12CreateDevice(adapter,
				D3D_FEATURE_LEVEL_11_0, __uuidof(*device), device.put_void()));
		};
	public:
		bool Initialize(DX12AdapterView adapter)noexcept {
			return wis::succeded(D3D12CreateDevice(adapter,
				D3D_FEATURE_LEVEL_11_0, __uuidof(*device), device.put_void()));
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
		DX12CommandList CreateCommandList(CommandListType list_type)const
		{
			D3D12_COMMAND_LIST_TYPE clty = D3D12_COMMAND_LIST_TYPE(list_type);
			winrt::com_ptr<ID3D12CommandAllocator> xallocator;
			winrt::com_ptr<ID3D12GraphicsCommandList9> xcommand_list;

			wis::check_hresult(device->CreateCommandAllocator(clty, __uuidof(*xallocator), xallocator.put_void()));
			wis::check_hresult(device->CreateCommandList1(0, clty, D3D12_COMMAND_LIST_FLAG_NONE, __uuidof(*xcommand_list), xcommand_list.put_void()));

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

			PipelineStreamAllocator ia;
			for (auto& i : input_layout)
			{
				ia.allocate<D3D12_INPUT_ELEMENT_DESC>() = 
				{
					.SemanticName = i.semantic_name,
					.SemanticIndex = i.semantic_index,
					.Format = DXGI_FORMAT(i.format),
					.InputSlot = i.input_slot,
					.AlignedByteOffset = i.aligned_byte_offset,
					.InputSlotClass = D3D12_INPUT_CLASSIFICATION(i.input_slot_class),
					.InstanceDataStepRate = i.instance_data_step_rate
				};
			}
			iadesc.pInputElementDescs = reinterpret_cast<D3D12_INPUT_ELEMENT_DESC*>(ia.allocator.data());


			PipelineStreamAllocator psta;
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
			if (desc.num_targets)
			{
				D3D12_RT_FORMAT_ARRAY rta{
					.NumRenderTargets = desc.num_targets
				};
				std::memcpy(rta.RTFormats, desc.target_formats.data(), desc.target_formats.size() * sizeof(DataFormat));
				psta.allocate<CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS>() = rta;
			}
			xdesc.pPipelineStateSubobjectStream = psta.allocator.data();
			xdesc.SizeInBytes = psta.size;


			wis::check_hresult(device->CreatePipelineState(&xdesc, __uuidof(*state), state.put_void()));
			return DX12PipelineState{ std::move(state) };
		}
	};
}