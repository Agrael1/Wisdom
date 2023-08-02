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
        Internal() = default;
        Internal(std::vector<CD3DX12_DESCRIPTOR_RANGE1> ranges)
            : ranges(std::move(ranges))
        {
        }

    public:
        [[nodiscard]] std::span<const CD3DX12_DESCRIPTOR_RANGE1> GetRanges() const noexcept
        {
            return ranges;
        }

    protected:
        std::vector<CD3DX12_DESCRIPTOR_RANGE1> ranges;
    };

    class DX12DescriptorSetLayout : public QueryInternal<DX12DescriptorSetLayout>
    {
    public:
        using QueryInternal::QueryInternal;
        operator DX12DescriptorSetLayoutView() const noexcept
        {
            return GetRanges();
        }
    };

    class DX12DescriptorSet;

    template<>
    class Internal<DX12DescriptorSet>
    {
    public:
        Internal() = default;
        Internal(winrt::com_ptr<ID3D12DescriptorHeap> heap, CD3DX12_CPU_DESCRIPTOR_HANDLE heap_start, CD3DX12_CPU_DESCRIPTOR_HANDLE heap_end, CD3DX12_GPU_DESCRIPTOR_HANDLE heap_gpu_start, uint32_t heap_increment)
            : heap(std::move(heap))
            , heap_start(heap_start)
            , heap_end(heap_end)
            , heap_gpu_start(heap_gpu_start)
            , heap_increment(heap_increment){};

    public:
        [[nodiscard]] CD3DX12_CPU_DESCRIPTOR_HANDLE GetDescriptorHeapStart() const noexcept
        {
            return heap_start;
        }
        [[nodiscard]] CD3DX12_CPU_DESCRIPTOR_HANDLE GetDescriptorHeapEnd() const noexcept
        {
            return heap_end;
        }
        [[nodiscard]] uint32_t GetDescriptorHeapIncrement() const noexcept
        {
            return heap_increment;
        }
        [[nodiscard]] CD3DX12_GPU_DESCRIPTOR_HANDLE GetDescriptorHeapGpuStart() const noexcept
        {
            return heap_gpu_start;
        }
        auto get() const noexcept
        {
            return heap.get();
        }

    protected:
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
            return std::make_tuple(GetDescriptorHeapStart(), GetDescriptorHeapEnd(), GetDescriptorHeapIncrement());
        }
        operator DX12DescriptorSetBindView() const noexcept
        {
            return std::make_tuple(heap.get(), GetDescriptorHeapGpuStart());
        }
    };

    class DX12DescriptorHeap;

    template<>
    class Internal<DX12DescriptorHeap>
    {
    public:
        Internal() = default;
        Internal(winrt::com_ptr<ID3D12DescriptorHeap> heap, uint32_t heap_increment)
            : heap(std::move(heap))
            , heap_start(this->heap->GetCPUDescriptorHandleForHeapStart())
            , heap_gpu_start(this->heap->GetGPUDescriptorHandleForHeapStart())
            , heap_increment(heap_increment){};

    public:
        [[nodiscard]] ID3D12DescriptorHeap* GetDescriptorHeap() const noexcept
        {
            return heap.get();
        }

    protected:
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
            : QueryInternal(std::move(heap), heap_increment)
        {
        }
        operator bool() const noexcept
        {
            return bool(heap);
        }
        operator DX12DescriptorHeapView() const noexcept
        {
            return GetDescriptorHeap();
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
