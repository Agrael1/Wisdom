#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_views.h>
#include <wil/resource.h>
#include <d3d12.h>

namespace wis
{
	class DX12Fence;

	template<>
	class Internal<DX12Fence>
	{
	public:
		Internal() = default;
		Internal(winrt::com_ptr<ID3D12Fence1> fence) 
			: fence(std::move(fence))
		{
			wis::check_hresult(fence_event.create()); // rethrow windows error (nothrow policy)
		}
	public:
		[[nodiscard]]
		auto* GetFence()const noexcept {
			return fence.get();
		}
	protected:
		winrt::com_ptr<ID3D12Fence1> fence;
		wil::unique_event_nothrow fence_event;
	};


	/// @brief A fence is a synchronization primitive that allows the CPU to wait for the GPU to finish rendering a frame.
	class DX12Fence : public QueryInternal<DX12Fence>
	{
	public:
		DX12Fence() = default;
		explicit DX12Fence(winrt::com_ptr<ID3D12Fence1> xfence)noexcept
			: QueryInternal(std::move(xfence)){}
		operator DX12FenceView()const noexcept{
			return GetFence();
		}
	public:
		/// @brief Get the current value of the fence.
		/// @return Value of the fence.
		uint64_t GetCompletedValue()const noexcept
		{
			return fence->GetCompletedValue();
		}

		/// @brief Wait for the fence to reach a certain value.
		/// @param value Value to wait for.
		/// @return Boolean indicating whether the fence reached the value.
		bool Wait(uint64_t value)const noexcept
		{
			return GetCompletedValue() >= value ?
				true :
				wis::succeded_weak(fence->SetEventOnCompletion(value, fence_event.get()))
				&& fence_event.wait();
		}

		/// @brief Signal the fence from CPU.
		/// @param value Value to signal.
		void Signal(uint64_t value)noexcept
		{
			fence->Signal(value);
		}
	};
}