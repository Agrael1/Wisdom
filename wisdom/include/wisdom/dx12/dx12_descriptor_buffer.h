#ifndef WIS_DX12_DESCRIPTOR_BUFFER_H
#define WIS_DX12_DESCRIPTOR_BUFFER_H
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
    WIS_INLINE uint64_t WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::DX12SamplerView sampler) noexcept;
    WIS_INLINE uint64_t WriteShaderResource(uint64_t buffer_offset_before_table,
                                            [[maybe_unused]] uint32_t root_table_index,
                                            uint32_t binding,
                                            uint32_t array_member,
                                            [[maybe_unused]] wis::DX12RootSignatureView2 root_signature,
                                            wis::DX12ShaderResourceView resource) noexcept;
    WIS_INLINE uint64_t WriteConstantBuffer(uint64_t buffer_offset_before_table,
                                            [[maybe_unused]] uint32_t root_table_index,
                                            uint32_t binding,
                                            uint32_t array_member,
                                            [[maybe_unused]] wis::DX12RootSignatureView2 root_signature,
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

#pragma region DX12DescriptorBuffer
/**
 * @brief Represents descriptor buffer for binding descriptors.
 * */
struct DX12DescriptorBuffer : public wis::ImplDX12DescriptorBuffer {
public:
    using wis::ImplDX12DescriptorBuffer::ImplDX12DescriptorBuffer;

public:
    /**
     * @brief Writes the sampler to the sampler descriptor buffer.
     * Must be called with Sampler descriptor buffer, which was created with wis::DescriptorHeapType::Sampler.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with wis::DX12Device.
     * @param index Binding index in descriptor table.
     * @param sampler The sampler to write.
     * @return Byte offset from buffer beginning. May help determine next table address.
     * */
    inline uint64_t WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::DX12SamplerView sampler) noexcept
    {
        return wis::ImplDX12DescriptorBuffer::WriteSampler(aligned_table_offset, index, std::move(sampler));
    }
    /**
     * @brief Writes the shader resource to the shader resource descriptor buffer.
     * Must be called with Shader Resource descriptor buffer, which was created with wis::DescriptorHeapType::Descriptor.
     * Requires wis::DeviceFeature::DescriptorEqualSize to run, otherwise program is ill-formed.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with wis::DX12Device.
     * @param index Binding index in descriptor table.
     * @param resource The shader resource to write.
     * @return Byte offset from buffer beginning. May help determine next table address.
     * */
    inline uint64_t WriteShaderResource2(uint64_t aligned_table_offset, uint32_t index, wis::DX12ShaderResourceView resource) noexcept
    {
        return wis::ImplDX12DescriptorBuffer::WriteShaderResource2(aligned_table_offset, index, std::move(resource));
    }
    /**
     * @brief Writes the constant buffer to the constant buffer descriptor buffer.
     * Must be called with Constant Buffer descriptor buffer, which was created with wis::DescriptorHeapType::Descriptor.
     * Requires wis::DeviceFeature::DescriptorEqualSize to run, otherwise program is ill-formed.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with wis::DX12Device.
     * @param index Binding index in descriptor table.
     * @param buffer The buffer to write.
     * @param buffer_size The size of the buffer in bytes.
     * @return Byte offset from buffer beginning. May help determine next table address.
     * */
    inline uint64_t WriteConstantBuffer2(uint64_t aligned_table_offset, uint32_t index, wis::DX12BufferView buffer, uint32_t buffer_size) noexcept
    {
        return wis::ImplDX12DescriptorBuffer::WriteConstantBuffer2(aligned_table_offset, index, std::move(buffer), buffer_size);
    }
    /**
     * @brief Writes the shader resource to the shader resource descriptor buffer.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with wis::DX12Device.
     * @param root_table_index Index of the descriptor table in wis::DX12RootSignature
     * @param binding Binding index in descriptor table.
     * @param array_member Array member index in the binding.
     * @param root_signature The root signature to get the binding position from.
     * @param resource The shader resource to write.
     * @return Byte offset from buffer beginning. May help determine next table address.
     * */
    inline uint64_t WriteShaderResource(uint64_t aligned_table_offset, uint32_t root_table_index, uint32_t binding, uint32_t array_member, wis::DX12RootSignatureView2 root_signature, wis::DX12ShaderResourceView resource) noexcept
    {
        return wis::ImplDX12DescriptorBuffer::WriteShaderResource(aligned_table_offset, root_table_index, binding, array_member, std::move(root_signature), std::move(resource));
    }
    /**
     * @brief Writes the constant buffer to the constant buffer descriptor buffer.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with wis::DX12Device.
     * @param root_table_index Index of the descriptor table in wis::DX12RootSignature
     * @param binding Binding index in descriptor table.
     * @param array_member Array member index in the binding.
     * @param root_signature The root signature to get the binding position from.
     * @param buffer The buffer to write.
     * @param buffer_size The size of the buffer in bytes.
     * @return Byte offset from buffer beginning. May help determine next table address.
     * */
    inline uint64_t WriteConstantBuffer(uint64_t aligned_table_offset, uint32_t root_table_index, uint32_t binding, uint32_t array_member, wis::DX12RootSignatureView2 root_signature, wis::DX12BufferView buffer, uint32_t buffer_size) noexcept
    {
        return wis::ImplDX12DescriptorBuffer::WriteConstantBuffer(aligned_table_offset, root_table_index, binding, array_member, std::move(root_signature), std::move(buffer), buffer_size);
    }
};
#pragma endregion DX12DescriptorBuffer

} // namespace wis
#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_descriptor_buffer.cpp"
#endif // !WISDOM_BUILD_BINARIES

#endif // !DX12_DESCRIPTOR_BUFFER_H
