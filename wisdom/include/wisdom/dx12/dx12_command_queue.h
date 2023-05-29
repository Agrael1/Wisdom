#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/dx12/dx12_checks.h>
#include <d3d12.h>

namespace wis
{
	class DX12CommandQueue;

	template<>
	class Internal<DX12CommandQueue>
	{
	public:
		Internal() = default;
		Internal(winrt::com_ptr<ID3D12CommandQueue> queue)noexcept :queue(std::move(queue)){}
	public:
		[[nodiscard]] ID3D12CommandQueue* GetQueue()const noexcept {
			return queue.get();
		}
	protected:
		winrt::com_ptr<ID3D12CommandQueue> queue{};
	};


	/// @brief A command queue is used to submit command lists to the GPU.
	class DX12CommandQueue : public QueryInternal<DX12CommandQueue>
	{
		using intern = QueryInternal<DX12CommandQueue>;
	public:
		DX12CommandQueue() = default;
		explicit DX12CommandQueue(winrt::com_ptr<ID3D12CommandQueue> queue)noexcept
			:intern(std::move(queue)){}
		operator DX12CommandQueueView()const noexcept{
			return GetQueue();
		}
	public:

		/// @brief Execute a command list on the GPU.
		/// @param list List to execute.
		void ExecuteCommandList(DX12CommandListView list)noexcept
		{
			queue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&list));
		}

		/// @brief Signal a fence with some value.
		/// @param fence Fence to signal.
		/// @param value Value to signal with.
		/// @return true if call succeeded.
		bool Signal(DX12FenceView fence, uint64_t value)noexcept
		{
			return wis::succeded_weak(queue->Signal(fence, value));
		}
	};
}