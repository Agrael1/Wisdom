#pragma once
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/global/internal.h>
#include <wisdom/util/com_ptr.h>
#include <d3dx12/d3dx12_root_signature.h>
#include <wisdom/util/misc.h>

namespace wis {

class DX12DescriptorBuffer;

template<>
struct Internal<DX12DescriptorBuffer> {
    wis::com_ptr<ID3D12Device> device;
    wis::com_ptr<ID3D12DescriptorHeap> heap;
    CD3DX12_CPU_DESCRIPTOR_HANDLE heap_start;
    CD3DX12_GPU_DESCRIPTOR_HANDLE heap_gpu_start;
    uint32_t heap_increment = 0;

    Internal() = default;
    Internal(wis::com_ptr<ID3D12DescriptorHeap> heap, uint32_t heap_increment) noexcept
        : heap(std::move(heap)), heap_increment(heap_increment)
    {
        heap_start = CD3DX12_CPU_DESCRIPTOR_HANDLE(this->heap->GetCPUDescriptorHandleForHeapStart());
        heap_gpu_start = CD3DX12_GPU_DESCRIPTOR_HANDLE(this->heap->GetGPUDescriptorHandleForHeapStart());
        this->heap->GetDevice(device.iid(), device.put_void());
    }
};

class DX12DescriptorBuffer : public QueryInternal<DX12DescriptorBuffer>
{
public:
    DX12DescriptorBuffer() = default;
    explicit DX12DescriptorBuffer(wis::com_ptr<ID3D12DescriptorHeap> heap, uint32_t heap_increment) noexcept
        : QueryInternal(std::move(heap), heap_increment)
    {
    }

    operator bool() const noexcept
    {
        return bool(heap);
    }

    operator DX12DescriptorBufferView() const noexcept
    {
        return { heap.get() };
    }
    operator DX12DescriptorBufferGPUView() const noexcept
    {
        return { heap_gpu_start, heap_increment };
    }

    void WriteSampler(uint32_t index, wis::DX12SamplerView sampler) noexcept
    {
        auto handle = heap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += index * heap_increment;

        auto sampler_handle = std::get<0>(sampler);
        device->CopyDescriptorsSimple(1, handle, sampler_handle, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
    }

    void WriteShaderResource(uint32_t index, wis::DX12ShaderResourceView resource) noexcept
    {
        auto handle = heap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += index * heap_increment;

        auto sampler_handle = std::get<0>(resource);
        device->CopyDescriptorsSimple(1, handle, sampler_handle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    void WriteConstantBuffer(uint32_t index, wis::DX12BufferView buffer, uint32_t size) noexcept
    {
        auto handle = heap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += index * heap_increment;

        D3D12_CONSTANT_BUFFER_VIEW_DESC desc
        {
            .BufferLocation = std::get<0>(buffer)->GetGPUVirtualAddress(),
            .SizeInBytes = wis::detail::aligned_size(size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT)
        };

        device->CreateConstantBufferView(&desc, handle);
    }
};
} // namespace wis