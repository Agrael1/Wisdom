#ifndef DX12_DESCRIPTOR_BUFFER_H
#define DX12_DESCRIPTOR_BUFFER_H
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

public:
    WIS_INLINE uint64_t WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::DX12SamplerView sampler) noexcept;
    WIS_INLINE uint64_t WriteShaderResource(uint64_t buffer_offset_before_table,
                                            [[maybe_unused]] uint32_t root_table_index,
                                            uint32_t binding,
                                            uint32_t array_member,
                                            [[maybe_unused]] wis::DX12RootSignatureView root_signature,
                                            wis::DX12ShaderResourceView resource) noexcept;
    WIS_INLINE uint64_t WriteConstantBuffer(uint64_t buffer_offset_before_table,
                                            [[maybe_unused]] uint32_t root_table_index,
                                            uint32_t binding,
                                            uint32_t array_member,
                                            [[maybe_unused]] wis::DX12RootSignatureView root_signature,
                                            wis::DX12BufferView buffer,
                                            uint32_t size) noexcept;

    WIS_INLINE uint64_t WriteShaderResource2(uint64_t aligned_table_offset, uint32_t index, wis::DX12ShaderResourceView resource) noexcept;
    WIS_INLINE uint64_t WriteConstantBuffer2(uint64_t aligned_table_offset, uint32_t index, wis::DX12BufferView buffer, uint32_t size) noexcept;

protected:
    WIS_INLINE uint64_t WriteDescriptor(uint64_t buffer_offset_before_table,
                                        uint32_t binding,
                                        uint32_t array_member,
                                        D3D12_CPU_DESCRIPTOR_HANDLE resource) noexcept;
};
} // namespace wis
#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_descriptor_buffer.cpp"
#endif // !WISDOM_BUILD_BINARIES

#endif // !DX12_DESCRIPTOR_BUFFER_H
