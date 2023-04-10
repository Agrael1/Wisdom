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
		template<class Self>
		[[nodiscard]] auto GetQueue(this Self&& s)noexcept {
			return s.queue;
		}
	protected:
		winrt::com_ptr<ID3D12CommandQueue> queue{};
	};


	class DX12CommandQueue : public QueryInternal<DX12CommandQueue>
	{
		using intern = QueryInternal<DX12CommandQueue>;
	public:
		DX12CommandQueue() = default;
		DX12CommandQueue(winrt::com_ptr<ID3D12CommandQueue> queue)noexcept
			:intern(std::move(queue)){}
	public:
		void ExecuteCommandList(const DX12CommandList& list)
		{
			auto* cl = list.GetInternal().GetCommandList();
			queue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&cl));
		}
		bool Signal(DX12Fence& fence, uint64_t value)
		{
			return wis::succeded_weak(queue->Signal(fence.GetInternal().GetFence().get(), value));
		}
	};
}