#ifndef WIS_DX12_DESCRIPTOR_BUFFER_CPP
#define WIS_DX12_DESCRIPTOR_BUFFER_CPP
#include <wisdom/dx12/dx12_descriptor_buffer.h>

uint64_t wis::ImplDX12DescriptorBuffer::WriteSampler(uint64_t aligned_table_offset, uint32_t index, wis::DX12SamplerView sampler) noexcept
{
    auto handle = heap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += aligned_table_offset + index * heap_increment;

    auto sampler_handle = std::get<0>(sampler);
    device->CopyDescriptorsSimple(1, handle, sampler_handle, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

    return aligned_table_offset + index * heap_increment + heap_increment;
}

uint64_t wis::ImplDX12DescriptorBuffer::WriteShaderResource(uint64_t buffer_offset_before_table, uint32_t root_table_index, uint32_t binding, uint32_t array_member, wis::DX12RootSignatureView2 root_signature, wis::DX12ShaderResourceView resource) noexcept
{
    auto srv_handle = std::get<0>(resource);
    return WriteDescriptor(buffer_offset_before_table, binding, array_member, srv_handle);
}

uint64_t wis::ImplDX12DescriptorBuffer::WriteConstantBuffer(uint64_t buffer_offset_before_table, uint32_t root_table_index, uint32_t binding, uint32_t array_member, wis::DX12RootSignatureView2 root_signature, wis::DX12BufferView buffer, uint32_t size) noexcept
{
    auto* cbv = std::get<0>(buffer);
    D3D12_CONSTANT_BUFFER_VIEW_DESC desc{
        .BufferLocation = std::get<0>(buffer)->GetGPUVirtualAddress(),
        .SizeInBytes = wis::detail::aligned_size(size, uint32_t(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT))
    };

    auto handle = heap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += buffer_offset_before_table + (binding + array_member) * heap_increment;

    device->CreateConstantBufferView(&desc, handle);
    return buffer_offset_before_table + (binding + array_member) * heap_increment + heap_increment;
}

uint64_t wis::ImplDX12DescriptorBuffer::WriteShaderResource2(uint64_t aligned_table_offset, uint32_t index, wis::DX12ShaderResourceView resource) noexcept
{
    auto handle = heap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += index * heap_increment;

    auto sampler_handle = std::get<0>(resource);
    device->CopyDescriptorsSimple(1, handle, sampler_handle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    return aligned_table_offset + index * heap_increment + heap_increment;
}

uint64_t wis::ImplDX12DescriptorBuffer::WriteConstantBuffer2(uint64_t aligned_table_offset, uint32_t index, wis::DX12BufferView buffer, uint32_t size) noexcept
{
    auto handle = heap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += index * heap_increment;

    D3D12_CONSTANT_BUFFER_VIEW_DESC desc{
        .BufferLocation = std::get<0>(buffer)->GetGPUVirtualAddress(),
        .SizeInBytes = wis::detail::aligned_size(size, uint32_t(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT))
    };

    device->CreateConstantBufferView(&desc, handle);

    return aligned_table_offset + index * heap_increment + heap_increment;
}

uint64_t wis::ImplDX12DescriptorBuffer::WriteDescriptor(uint64_t buffer_offset_before_table, uint32_t binding, uint32_t array_member, D3D12_CPU_DESCRIPTOR_HANDLE resource) noexcept
{
    auto handle = heap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += buffer_offset_before_table + (binding + array_member) * heap_increment;

    device->CopyDescriptorsSimple(1, handle, resource, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    return buffer_offset_before_table + (binding + array_member) * heap_increment + heap_increment;
}

#endif // !DX12_DESCRIPTOR_BUFFER_CPP
