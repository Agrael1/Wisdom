#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_queue.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_command_queue.h>
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
		[[nodiscard]] auto GetDevice(this Self&& s) {
			return s.device;
		}
	protected:
		winrt::com_ptr<ID3D12Device10> device{};
	};


	class DX12Device : private Internal<DX12Device>
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
		[[nodiscard]]
		auto& GetInternal()const noexcept
		{
			return static_cast<const Internal<DX12Device>&>(*this);
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
	};
}