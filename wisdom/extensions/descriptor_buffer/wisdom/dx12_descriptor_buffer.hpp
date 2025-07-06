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
    void WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::DX12SamplerView sampler) noexcept
    {
        auto handle = heap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += aligned_table_offset + index * heap_increment;
        const auto& sampler_handle = std::get<0>(sampler);
        device->CopyDescriptorsSimple(1, handle, sampler_handle, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
    }
    void WriteTexture(uint64_t aligned_table_offset, uint32_t index, wis::DX12ShaderResourceView resource) noexcept
    {
        auto handle = heap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += index * heap_increment;
        auto& sampler_handle = std::get<0>(resource);
        device->CopyDescriptorsSimple(1, handle, sampler_handle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }
    void WriteConstantBuffer(uint64_t aligned_table_offset, uint32_t index, wis::DX12BufferView buffer, uint32_t size, uint32_t offset) noexcept
    {
        auto handle = heap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += aligned_table_offset + index * heap_increment;
        D3D12_CONSTANT_BUFFER_VIEW_DESC desc{
            .BufferLocation = std::get<0>(buffer)->GetGPUVirtualAddress() + offset,
            .SizeInBytes = wis::detail::aligned_size(size, uint32_t(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT))
        };
        device->CreateConstantBufferView(&desc, handle);
    }
    void WriteRWTexture(uint64_t aligned_table_offset, uint32_t index, wis::DX12UnorderedAccessTextureView uav) noexcept
    {
        auto handle = heap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += index * heap_increment;

        auto& uav_handle = std::get<0>(uav);
        device->CopyDescriptorsSimple(1, handle, uav_handle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }
    void WriteRWStructuredBuffer(uint64_t aligned_table_offset, uint32_t index, wis::DX12BufferView buffer, uint32_t stride, uint32_t element_count, uint32_t offset_elements = 0) noexcept
    {
        D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc{
            .Format = DXGI_FORMAT_UNKNOWN,
            .ViewDimension = D3D12_UAV_DIMENSION_BUFFER,
            .Buffer{
                    .FirstElement = offset_elements,
                    .NumElements = element_count,
                    .StructureByteStride = stride,
                    .CounterOffsetInBytes = 0,
                    .Flags = D3D12_BUFFER_UAV_FLAG_NONE },
        };
        auto handle = heap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += aligned_table_offset + index * heap_increment;
        device->CreateUnorderedAccessView(std::get<0>(buffer), nullptr, &uav_desc, handle);
    }
    void WriteStructuredBuffer(uint64_t aligned_table_offset, uint32_t index, wis::DX12BufferView buffer, uint32_t stride, uint32_t element_count, uint32_t offset_elements = 0) noexcept
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{
            .Format = DXGI_FORMAT_UNKNOWN,
            .ViewDimension = D3D12_SRV_DIMENSION_BUFFER,
            .Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING,
            .Buffer{
                    .FirstElement = offset_elements,
                    .NumElements = element_count,
                    .StructureByteStride = stride,
                    .Flags = D3D12_BUFFER_SRV_FLAG_NONE },
        };
        auto handle = heap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += aligned_table_offset + index * heap_increment;
        device->CreateShaderResourceView(std::get<0>(buffer), &srv_desc, handle);
    }
    void WriteAccelerationStructure(uint64_t aligned_table_offset, uint32_t index, uint64_t acceleration_structure_device_address) const noexcept
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC desc{
            .Format = DXGI_FORMAT_UNKNOWN,
            .ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE,
            .Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING,
            .RaytracingAccelerationStructure = { acceleration_structure_device_address }
        };
        auto handle = heap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += aligned_table_offset + index * heap_increment;
        device->CreateShaderResourceView(nullptr, &desc, handle);
    }
};

#pragma region DX12DescriptorBuffer
/**
 * @brief Buffer storage for descriptors.
 * It is the most flexible implementation of storage for descriptors, as it supports both bindful and bindless
 * Descriptor approaches. Works like an aligned table of elements of the same size.
 * Alignment and size of descriptors are varying between implementation and heap types.
 * Both metrixs are available to retrieval from
 * */
WISDOM_EXPORT
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
    inline void WriteTexture(uint64_t aligned_table_offset, uint32_t index, wis::DX12ShaderResourceView resource) noexcept
    {
        wis::ImplDX12DescriptorBuffer::WriteTexture(aligned_table_offset, index, std::move(resource));
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
    /**
     * @brief Writes the storage texture to the storage texture descriptor buffer.
     * Must be called with Storage Texture descriptor buffer, which was created with wis::DescriptorHeapType::Descriptor.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with .
     * @param index Binding index in descriptor table.
     * @param uav The storage texture to write.
     * */
    inline void WriteRWTexture(uint64_t aligned_table_offset, uint32_t index, wis::DX12UnorderedAccessTextureView uav) noexcept
    {
        wis::ImplDX12DescriptorBuffer::WriteRWTexture(aligned_table_offset, index, std::move(uav));
    }
    /**
     * @brief Writes the storage structured buffer to the storage buffer descriptor buffer.
     * Must be called with Storage Buffer descriptor buffer, which was created with wis::DescriptorHeapType::Descriptor.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with .
     * @param index Binding index in descriptor table.
     * @param buffer The buffer to write.
     * @param stride The stride of each element in the structured buffer in bytes.
     * @param element_count The number of elements in the structured buffer.
     * @param offset_elements The offset in elements from the beginning of the buffer. Default is 0.
     * */
    inline void WriteRWStructuredBuffer(uint64_t aligned_table_offset, uint32_t index, wis::DX12BufferView buffer, uint32_t stride, uint32_t element_count, uint32_t offset_elements = 0) noexcept
    {
        wis::ImplDX12DescriptorBuffer::WriteRWStructuredBuffer(aligned_table_offset, index, std::move(buffer), stride, element_count, offset_elements);
    }
    /**
     * @brief Writes the structured buffer to the shader resource descriptor buffer.
     * Must be called with Shader Resource descriptor buffer, which was created with wis::DescriptorHeapType::Descriptor.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with .
     * @param index Binding index in descriptor table.
     * @param buffer The buffer to write.
     * @param stride The stride of each element in the structured buffer in bytes.
     * @param element_count The number of elements in the structured buffer.
     * @param offset_elements The offset in elements from the beginning of the buffer. Default is 0.
     * */
    inline void WriteStructuredBuffer(uint64_t aligned_table_offset, uint32_t index, wis::DX12BufferView buffer, uint32_t stride, uint32_t element_count, uint32_t offset_elements = 0) noexcept
    {
        wis::ImplDX12DescriptorBuffer::WriteStructuredBuffer(aligned_table_offset, index, std::move(buffer), stride, element_count, offset_elements);
    }
    /**
     * @brief Writes the acceleration structure to the acceleration structure descriptor buffer.
     * Must be called with Acceleration Structure descriptor buffer, which was created with wis::DescriptorHeapType::Descriptor.
     * @param aligned_table_offset Byte offset from the buffer beginning in table alignment sizes.
     * Alignment may be queried with .
     * @param index Binding index in descriptor table.
     * @param acceleration_structure_device_address The device address of the acceleration structure to write. Can be queried with .
     * */
    inline void WriteAccelerationStructure(uint64_t aligned_table_offset, uint32_t index, uint64_t acceleration_structure_device_address) noexcept
    {
        wis::ImplDX12DescriptorBuffer::WriteAccelerationStructure(aligned_table_offset, index, acceleration_structure_device_address);
    }
};
#pragma endregion DX12DescriptorBuffer

} // namespace wis

#endif // !DX12_DESCRIPTOR_BUFFER_H
