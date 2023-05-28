#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_command_list.h>
#include <wisdom/dx12/dx12_definitions.h>
#include <wisdom/dx12/dx12_fence.h>
#include <d3d12.h>

namespace wis
{
	class DX12CommandQueue;

	template<>
	class Internal<DX12CommandQueue>
	{
		static constexpr inline bool valid = true;
	public:
		Internal() = default;
		Internal(winrt::com_ptr<ID3D12CommandQueue> queue)noexcept :queue(std::move(queue)){}
	public:
		ID3D12CommandQueue* GetQueue()const noexcept {
			return queue.get();
		}
	protected:
		winrt::com_ptr<ID3D12CommandQueue> queue{};
	};

	using DX12CommandQueueView = ID3D12CommandQueue*;


	/// @brief A command queue is used to submit command lists to the GPU.
	class DX12CommandQueue : public QueryInternal<DX12CommandQueue>
	{
		using intern = QueryInternal<DX12CommandQueue>;
	public:
		DX12CommandQueue() = default;
		DX12CommandQueue(winrt::com_ptr<ID3D12CommandQueue> queue)noexcept
			:intern(std::move(queue)){}
		operator DX12CommandQueueView()const noexcept{
			return GetQueue();
		}
	public:

		/// @brief Execute a command list on the GPU.
		/// @param list List to execute.
		void ExecuteCommandList(const DX12CommandList& list)noexcept
		{
			auto* cl = list.GetInternal().GetCommandList();
			queue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&cl));
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