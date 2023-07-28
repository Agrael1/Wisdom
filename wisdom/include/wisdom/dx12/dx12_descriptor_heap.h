#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#endif

WIS_EXPORT namespace wis
{
	class DX12DescriptorHeap;

	template<>
	class Internal<DX12DescriptorHeap>
	{
	public:
		Internal() = default;
		Internal(winrt::com_ptr<ID3D12DescriptorHeap> heap) :heap(std::move(heap)) {};
	public:
		[[nodiscard]] ID3D12DescriptorHeap* GetDescriptorHeap()const noexcept
		{
			return heap.get();
		}
	protected:
		winrt::com_ptr<ID3D12DescriptorHeap> heap;
	};


	/// @brief Shader object
	class DX12DescriptorHeap : public QueryInternal<DX12DescriptorHeap>
	{
	public:
		DX12DescriptorHeap() = default;
		explicit DX12DescriptorHeap(winrt::com_ptr<ID3D12DescriptorHeap> heap)
			:QueryInternal(std::move(heap))
		{}
		operator bool()const noexcept {
			return bool(heap);
		}
	};
}