#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_command_list.h>
#include <wisdom/api/api_barrier.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_rtv.h>
#include <wisdom/dx12/dx12_resource.h>
#include <d3d12.h>
#include <span>


namespace wis
{
	class DX12CommandList;

	template<>
	class Internal<DX12CommandList>
	{
	public:
		template<class Self>
		auto GetCommandList(this Self&& s)noexcept
		{
			return s.command_list;
		}
	protected:
		winrt::com_ptr<ID3D12CommandAllocator> allocator;
		winrt::com_ptr<ID3D12GraphicsCommandList9> command_list;
	};

	class DX12CommandList : public QueryInternal<DX12CommandList>
	{
	public:
		DX12CommandList() = default;
		explicit DX12CommandList(winrt::com_ptr<ID3D12CommandAllocator> xallocator,
			winrt::com_ptr<ID3D12GraphicsCommandList9> xcommand_list)
		{
			allocator = std::move(xallocator);
			command_list = std::move(xcommand_list);
		}
	public:
		bool Reset()noexcept
		{
			return closed = !(wis::succeded_weak(allocator->Reset())
				&& wis::succeded_weak(command_list->Reset(allocator.get(), nullptr)));
		}
		[[nodiscard]] bool IsClosed()const noexcept
		{
			return closed;
		}
		bool Close()noexcept
		{
			if (closed)return closed;
			return closed = wis::succeded_weak(command_list->Close());
		}

		// TODO: span<barriers>, barrier multitype
		template<BarrierType ty>
		void ResourceBarrier(ResourceBarrier<ty, DX12Resource>&& barrier)noexcept
		{
			if constexpr (ty == BarrierType::transition)
				return TransitionBarrier(std::forward<wis::ResourceBarrier<ty, DX12Resource>>(barrier));
		}

		void ClearRenderTarget(DX12RenderTargetView rtv, std::span<const float, 4> color)noexcept
		{
			command_list->ClearRenderTargetView(rtv.GetInternal().GetHandle(), color.data(), 0, nullptr);
		}
	private:
		void TransitionBarrier(BasicTransitionBarrier<DX12Resource>&& barrier)noexcept
		{
			auto b = CD3DX12_RESOURCE_BARRIER::Transition(
				barrier.resource.GetInternal().GetResource().get(), 
				D3D12_RESOURCE_STATES(barrier.before), 
				D3D12_RESOURCE_STATES(barrier.after),
				barrier.subresource, D3D12_RESOURCE_BARRIER_FLAGS(barrier.flags)
				);
			command_list->ResourceBarrier(1, &b);
		}
	private:
		bool closed = true;
	};
}