#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_queue.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_command_queue.h>
#include <wisdom/dx12/dx12_command_list.h>
#include <wisdom/dx12/dx12_fence.h>
#include <wisdom/dx12/dx12_root_signature.h>
#include <wisdom/dx12/dx12_graphics_pipeline.h>
#include <d3d12.h>
#include <d3dx12/d3dx12.h>

namespace wis
{
	class DX12Device;

	template<>
	class Internal<DX12Device>
	{
		static constexpr inline bool valid = true;
	public:
		template<class Self>
		[[nodiscard]] auto GetDevice(this Self&& s)noexcept {
			return s.device;
		}
	protected:
		winrt::com_ptr<ID3D12Device10> device{};
	};


	class DX12Device final: public QueryInternal<DX12Device>
	{
	public:
		DX12Device() = default;
		DX12Device(DX12Adapter adapter) {
			wis::check_hresult(D3D12CreateDevice(adapter.GetInternal().GetAdapter().get(),
				D3D_FEATURE_LEVEL_11_0, __uuidof(*device), device.put_void()));
		};
	public:
		bool Initialize(DX12Adapter adapter)noexcept {
			return wis::succeded(D3D12CreateDevice(adapter.GetInternal().GetAdapter().get(),
				D3D_FEATURE_LEVEL_11_0, __uuidof(*device), device.put_void()));
		}
		explicit operator bool()const noexcept
		{
			return bool(device);
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
		DX12GraphicsPipeline CreatePipeline()const
		{
			winrt::com_ptr<ID3D12PipelineState> rsig;


			return DX12GraphicsPipeline{ std::move(rsig) };
		}
	};
}