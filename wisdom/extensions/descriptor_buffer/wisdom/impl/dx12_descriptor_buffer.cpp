#ifndef WIS_DX12_DESCRIPTOR_BUFFER_CPP
#define WIS_DX12_DESCRIPTOR_BUFFER_CPP

#if defined(WISDOM_DX12)
#ifndef WISDOM_MODULE_DECL
#include <wisdom/dx12_descriptor_buffer.hpp>
#endif // !WISDOM_MODULE_DECL

void wis::ImplDX12DescriptorBuffer::WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::DX12SamplerView sampler) noexcept
{
    auto handle = heap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += aligned_table_offset + index * heap_increment;

    const auto& sampler_handle = std::get<0>(sampler);
    device->CopyDescriptorsSimple(1, handle, sampler_handle, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
}

void wis::ImplDX12DescriptorBuffer::WriteShaderResource(uint64_t aligned_table_offset, uint32_t index, wis::DX12ShaderResourceView resource) noexcept
{
    auto handle = heap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += index * heap_increment;

    auto sampler_handle = std::get<0>(resource);
    device->CopyDescriptorsSimple(1, handle, sampler_handle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void wis::ImplDX12DescriptorBuffer::WriteConstantBuffer(uint64_t aligned_table_offset, uint32_t index, wis::DX12BufferView buffer, uint32_t size, uint32_t offset) noexcept
{
    auto handle = heap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += index * heap_increment;

    D3D12_CONSTANT_BUFFER_VIEW_DESC desc{
        .BufferLocation = std::get<0>(buffer)->GetGPUVirtualAddress() + offset,
        .SizeInBytes = wis::detail::aligned_size(size, uint32_t(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT))
    };

    device->CreateConstantBufferView(&desc, handle);
}

#endif // WISDOM_DX12

#endif // !DX12_DESCRIPTOR_BUFFER_CPP
