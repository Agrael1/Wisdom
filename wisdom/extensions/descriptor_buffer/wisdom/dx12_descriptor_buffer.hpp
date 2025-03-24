#ifndef WIS_DX12_DESCRIPTOR_BUFFER_H
#define WIS_DX12_DESCRIPTOR_BUFFER_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/global/internal.h>
#include <wisdom/util/com_ptr.h>
#include <d3dx12/d3dx12_root_signature.h>
#include <wisdom/util/misc.h>
#endif // !WISDOM_MODULE_DECL

WISDOM_EXPORT
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

class ImplDX12DescriptorBuffer : public QueryInternal<DX12DescriptorBuffer>
{
public:
    ImplDX12DescriptorBuffer() = default;
    explicit ImplDX12DescriptorBuffer(wis::com_ptr<ID3D12DescriptorHeap> heap, uint32_t heap_increment) noexcept
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
    WIS_INLINE void WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::DX12SamplerView sampler) noexcept;
    WIS_INLINE void WriteShaderResource(uint64_t aligned_table_offset, uint32_t index, wis::DX12ShaderResourceView resource) noexcept;
    WIS_INLINE void WriteConstantBuffer(uint64_t aligned_table_offset, uint32_t index, wis::DX12BufferView buffer, uint32_t size, uint32_t offset) noexcept;
};

#pragma region DX12DescriptorBuffer
/**
 * @brief Buffer storage for descriptors.
 * It is the most flexible implementation of storage for descriptors, as it supports both bindful and bindless
 * Descriptor approaches. Works like an aligned table of elements of the same size.
 * Alignment and size of descriptors are varying between implementation and heap types.
 * Both metrixs are available to retrieval from
 * */
class DX12DescriptorBuffer : public wis::ImplDX12DescriptorBuffer
{
public:
    using wis::ImplDX12DescriptorBuffer::ImplDX12DescriptorBuffer;

public:
    /**
     * @brief Writes the sampler to the sampler descriptor buffer.
     * Must be called with Sampler descriptor buffer, which was created with wis::DescriptorHeapType::Sampler.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with .
     * @param index Binding index in descriptor table.
     * @param sampler The sampler to write.
     * */
    inline void WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::DX12SamplerView sampler) noexcept
    {
        wis::ImplDX12DescriptorBuffer::WriteSampler(aligned_table_offset, index, std::move(sampler));
    }
    /**
     * @brief Writes the shader resource to the shader resource descriptor buffer.
     * Must be called with Shader Resource descriptor buffer, which was created with wis::DescriptorHeapType::Descriptor.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with .
     * @param index Binding index in descriptor table.
     * @param resource The shader resource to write.
     * */
    inline void WriteShaderResource(uint64_t aligned_table_offset, uint32_t index, wis::DX12ShaderResourceView resource) noexcept
    {
        wis::ImplDX12DescriptorBuffer::WriteShaderResource(aligned_table_offset, index, std::move(resource));
    }
    /**
     * @brief Writes the constant buffer to the constant buffer descriptor buffer.
     * Must be called with Constant Buffer descriptor buffer, which was created with wis::DescriptorHeapType::Descriptor.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with wis::DX12Device.
     * @param index Binding index in descriptor table.
     * @param buffer The buffer to write.
     * @param buffer_size The size of the part of the buffer in bytes.
     * @param offset Offset from buffer beginning. offset + buffer_size must be less or equal buffer overall size.
     * */
    inline void WriteConstantBuffer(uint64_t aligned_table_offset, uint32_t index, wis::DX12BufferView buffer, uint32_t buffer_size, uint32_t offset = 0) noexcept
    {
        wis::ImplDX12DescriptorBuffer::WriteConstantBuffer(aligned_table_offset, index, std::move(buffer), buffer_size, offset);
    }
};
#pragma endregion DX12DescriptorBuffer

} // namespace wis
#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_descriptor_buffer.cpp"
#endif // !WISDOM_BUILD_BINARIES

#endif // !DX12_DESCRIPTOR_BUFFER_H
