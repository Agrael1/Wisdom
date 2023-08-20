#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <winrt/base.h>
#include <d3dx12/d3dx12_root_signature.h>
#include <wisdom/api/api_common.h>
#include <wisdom/dx12/dx12_views.h>
#include <span>
#endif

WIS_EXPORT namespace wis
{
class DX12DescriptorSetLayout;

using DX12DescriptorSetLayoutView = std::span<const CD3DX12_DESCRIPTOR_RANGE1>;

template<>
class Internal<DX12DescriptorSetLayout>
{
public:
    std::vector<CD3DX12_DESCRIPTOR_RANGE1> ranges;
};

class DX12DescriptorSetLayout : public QueryInternal<DX12DescriptorSetLayout>
{
public:
    using QueryInternal::QueryInternal;
    operator DX12DescriptorSetLayoutView() const noexcept
    {
        return ranges;
    }
};

class DX12DescriptorSet;

template<>
class Internal<DX12DescriptorSet>
{
public:
    winrt::com_ptr<ID3D12DescriptorHeap> heap;
    CD3DX12_CPU_DESCRIPTOR_HANDLE heap_start;
    CD3DX12_CPU_DESCRIPTOR_HANDLE heap_end;
    CD3DX12_GPU_DESCRIPTOR_HANDLE heap_gpu_start;
    uint32_t heap_increment = 0;
};

class DX12DescriptorSet : public QueryInternal<DX12DescriptorSet>
{
public:
    using QueryInternal::QueryInternal;
    operator DX12DescriptorSetView() const noexcept
    {
        return std::make_tuple(heap_start, heap_end, heap_increment);
    }
    operator DX12DescriptorSetBindView() const noexcept
    {
        return std::make_tuple(heap.get(), heap_gpu_start);
    }
};

class DX12DescriptorHeap;

template<>
class Internal<DX12DescriptorHeap>
{
public:
    winrt::com_ptr<ID3D12DescriptorHeap> heap;
    CD3DX12_CPU_DESCRIPTOR_HANDLE heap_start;
    CD3DX12_GPU_DESCRIPTOR_HANDLE heap_gpu_start;
    uint32_t heap_increment = 0;
};

using DX12DescriptorHeapView = ID3D12DescriptorHeap*;

/// @brief Shader object
class DX12DescriptorHeap : public QueryInternal<DX12DescriptorHeap>
{
public:
    DX12DescriptorHeap() = default;
    explicit DX12DescriptorHeap(winrt::com_ptr<ID3D12DescriptorHeap> heap, uint32_t heap_increment)
        : QueryInternal(std::move(heap),
                        CD3DX12_CPU_DESCRIPTOR_HANDLE(heap->GetCPUDescriptorHandleForHeapStart()),
                        CD3DX12_GPU_DESCRIPTOR_HANDLE(heap->GetGPUDescriptorHandleForHeapStart()),
                        heap_increment)
    {
    }
    operator bool() const noexcept
    {
        return bool(heap);
    }
    operator DX12DescriptorHeapView() const noexcept
    {
        return heap.get();
    }

public:
    DX12DescriptorSet AllocateDescriptorSet(DX12DescriptorSetLayoutView layout)
    {
        auto start = heap_start;
        auto gpu_start = heap_gpu_start;
        heap_start.Offset(heap_increment * layout.size());
        heap_gpu_start.Offset(heap_increment * layout.size());
        auto end = heap_start;
        return DX12DescriptorSet{ heap, start, end, gpu_start, heap_increment };
    }
};
}
