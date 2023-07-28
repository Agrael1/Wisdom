#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <winrt/base.h>
#include <d3dx12/d3dx12_root_signature.h>
#include <span>
#endif

WIS_EXPORT namespace wis
{
	class DX12DescriptorHeap;

	template<>
	class Internal<DX12DescriptorHeap>
	{
	public:
		Internal() = default;
		Internal(winrt::com_ptr<ID3D12DescriptorHeap> heap, uint32_t heap_increment) :heap(std::move(heap))
			, heap_start(heap->GetCPUDescriptorHandleForHeapStart())
			, heap_increment(heap_increment)
		{};
	public:
		[[nodiscard]] ID3D12DescriptorHeap* GetDescriptorHeap()const noexcept
		{
			return heap.get();
		}
	protected:
		winrt::com_ptr<ID3D12DescriptorHeap> heap;
		CD3DX12_CPU_DESCRIPTOR_HANDLE heap_start;
		uint32_t heap_increment = 0;
	};

	using DX12DescriptorSet = D3D12_CPU_DESCRIPTOR_HANDLE;
	using DX12DescriptorLayout = CD3DX12_DESCRIPTOR_RANGE1;
	using DX12DescriptorHeapView = ID3D12DescriptorHeap*;

	/// @brief Shader object
	class DX12DescriptorHeap : public QueryInternal<DX12DescriptorHeap>
	{
	public:
		DX12DescriptorHeap() = default;
		explicit DX12DescriptorHeap(winrt::com_ptr<ID3D12DescriptorHeap> heap, uint32_t heap_increment)
			:QueryInternal(std::move(heap), heap_increment)
		{}
		operator bool()const noexcept {
			return bool(heap);
		}
		operator DX12DescriptorHeapView()const noexcept {
			return GetDescriptorHeap();
		}
	public:
		std::vector<DX12DescriptorSet> AllocateDescriptorSets(std::span<DX12DescriptorLayout> layouts)
		{
			std::vector<DX12DescriptorSet> descs;
			descs.resize(layouts.size());
			for (auto& i : descs)
			{
				i = heap_start;
				heap_start.ptr += heap_increment;
			}
			return descs;
		}
		DX12DescriptorSet AllocateDescriptorSet(DX12DescriptorLayout)
		{
			auto set = heap_start;
			heap_start.ptr += heap_increment;
			return heap_start;
		}
	};
}