#pragma once
#include <wisdom/api/api_internal.h>
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
	};
}