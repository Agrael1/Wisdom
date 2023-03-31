#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_swapchain.h>
#include <wisdom/dx12/dx12_checks.h>

#include <dxgi1_6.h>
#include <d3dx12/d3dx12.h>
#include <span>

namespace wis
{
	class DX12SwapChain;

	template<>
	class Internal<DX12SwapChain>
	{
		friend class DX12SwapChain;
		static constexpr inline bool valid = true;
	public:
		Internal() = default;
		Internal(winrt::com_ptr<IDXGISwapChain4> chain)
			:chain(std::move(chain)) {}
	public:
		template<class Self>
		[[nodiscard]] auto GetSwapChain(this Self&& s) {
			return s.chain;
		}
		[[nodiscard]] 
		std::span<const winrt::com_ptr<ID3D12Resource>> GetRenderTargets()const{
			return render_targets;
		}
	protected:
		winrt::com_ptr<IDXGISwapChain4> chain{};
		winrt::com_ptr<ID3D12DescriptorHeap> rtv_heap{};
		std::vector<winrt::com_ptr<ID3D12Resource>> render_targets{};
		uint32_t rtv_increment = 0;
	};

	class DX12SwapChain : Internal<DX12SwapChain>
	{
		using intern = Internal<DX12SwapChain>;
		friend class DX12Factory;
	public:
		DX12SwapChain() = default;
		explicit DX12SwapChain(winrt::com_ptr<IDXGISwapChain4> xchain, uint32_t frame_count)
			:intern(std::move(xchain))
		{
			winrt::com_ptr<ID3D12Device> device;
			chain->GetDevice(__uuidof(ID3D12Device), device.put_void());

			// Describe and create a render target view (RTV) descriptor heap.
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = frame_count;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			wis::check_hresult(device->CreateDescriptorHeap(&rtvHeapDesc, __uuidof(*rtv_heap), rtv_heap.put_void()));

			rtv_increment = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


			// Create frame resources.
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtv_heap->GetCPUDescriptorHandleForHeapStart());

			// Create a RTV for each frame.
			render_targets.resize(frame_count);
			for (uint32_t n = 0; n < frame_count; n++)
			{
				wis::check_hresult(chain->GetBuffer(n, __uuidof(ID3D12Resource), render_targets[n].put_void()));
				device->CreateRenderTargetView(render_targets[n].get(), nullptr, rtvHandle);
				rtvHandle.Offset(1, rtv_increment);
			}
		}
	public:
		uint32_t GetBackBufferIndex()const noexcept
		{
			return chain->GetCurrentBackBufferIndex();
		}
	public:
		[[nodiscard]]
		auto& GetInternal()const noexcept
		{
			return static_cast<const intern&>(*this);;
		}
	};
}
