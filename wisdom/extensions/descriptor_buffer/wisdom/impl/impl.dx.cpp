#ifndef WISDOM_DESCRIPTOR_BUFFER_DX_CPP
#define WISDOM_DESCRIPTOR_BUFFER_DX_CPP

#if defined(WISDOM_DX12)
#include <wisdom/wisdom_descriptor_buffer.h>
#include <wisdom/util/small_allocator.h>

wis::ResultValue<wis::DX12RootSignature>
wis::ImplDX12DescriptorBufferExtension::CreateRootSignature(const RootConstant* root_constants,
                                                            uint32_t constants_size,
                                                            const wis::DescriptorTable* tables,
                                                            uint32_t tables_count) const noexcept
{
    wis::com_ptr<ID3D12RootSignature> rsig;

    D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    wis::detail::limited_allocator<D3D12_ROOT_PARAMETER1, 16> root_params{ constants_size + tables_count, true };

    std::array<int8_t, size_t(wis::ShaderStages::Count)> stage_map{};
    std::fill(stage_map.begin(), stage_map.end(), -1);

    for (uint32_t i = 0; i < constants_size; ++i) {
        auto& constant = root_constants[i];
        root_params.data()[i] = {
            .ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS,
            .Constants = {
                    .ShaderRegister = constant.bind_register,
                    .RegisterSpace = 0,
                    .Num32BitValues = constant.size_bytes / 4,
            },
            .ShaderVisibility = D3D12_SHADER_VISIBILITY(constant.stage),
        };
        stage_map[+constant.stage] = i;
    }

    size_t memory_size = 0;
    for (uint32_t i = 0; i < tables_count; ++i) {
        auto& table = tables[i];
        memory_size += table.entry_count;
    }

    auto memory = wis::detail::make_unique_for_overwrite<D3D12_DESCRIPTOR_RANGE1[]>(memory_size);
    if (!memory)
        return wis::make_result<FUNC, "Failed to allocate memory for descriptor ranges">(E_OUTOFMEMORY);

    uint32_t offset = 0;
    for (uint32_t i = constants_size; i < constants_size + tables_count; ++i) {
        auto& table = tables[i - constants_size];

        for (size_t j = offset; j < table.entry_count + offset; j++) {
            auto& entry = table.entries[j - offset];
            memory[j] = {
                .RangeType = convert_dx(entry.type),
                .NumDescriptors = entry.count,
                .BaseShaderRegister = entry.bind_register,
                .RegisterSpace = 0,
                .Flags = entry.count == UINT32_MAX ? D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE : D3D12_DESCRIPTOR_RANGE_FLAG_NONE, // always volatile for unbounded arrays
                .OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND,
            };
        }

        root_params.data()[i] = {
            .ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
            .DescriptorTable = {
                    .NumDescriptorRanges = table.entry_count,
                    .pDescriptorRanges = memory.get() + offset,
            },
            .ShaderVisibility = D3D12_SHADER_VISIBILITY(table.stage),
        };
        offset += table.entry_count;
    }

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;
    desc.Init_1_1(constants_size + tables_count, root_params.data(), 0, nullptr, flags);

    wis::com_ptr<ID3DBlob> signature;
    wis::com_ptr<ID3DBlob> error;
    HRESULT hr = D3D12SerializeVersionedRootSignature(&desc, signature.put(), error.put());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to serialize root signature">(hr);

    hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
                                     __uuidof(*rsig), rsig.put_void());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to create root signature">(hr);

    return DX12RootSignature{ std::move(rsig), stage_map, constants_size };
}

wis::ResultValue<wis::DX12DescriptorBuffer>
wis::ImplDX12DescriptorBufferExtension::CreateDescriptorBuffer(wis::DescriptorHeapType heap_type,
                                                               wis::DescriptorMemory memory_type,
                                                               uint64_t memory_bytes) const noexcept
{
    auto xheap_type = convert_dx(heap_type);
    auto inc_size = device->GetDescriptorHandleIncrementSize(xheap_type);
    auto aligned_size = wis::detail::aligned_size(memory_bytes, uint64_t(inc_size));
    D3D12_DESCRIPTOR_HEAP_DESC desc{
        .Type = convert_dx(heap_type),
        .NumDescriptors = uint32_t(aligned_size / inc_size),
        .Flags = convert_dx(memory_type),
        .NodeMask = 0,
    };
    wis::com_ptr<ID3D12DescriptorHeap> heap;
    auto hr = device->CreateDescriptorHeap(&desc, heap.iid(), heap.put_void());
    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to create descriptor buffer">(hr);

    return DX12DescriptorBuffer{ std::move(heap), device->GetDescriptorHandleIncrementSize(desc.Type) };
}

void wis::ImplDX12DescriptorBufferExtension::SetDescriptorBuffers(wis::DX12CommandListView cmd_list,
                                                                  wis::DX12DescriptorBufferView resource_desc_buffer,
                                                                  wis::DX12DescriptorBufferView sampler_desc_buffer) const noexcept
{
    auto* list = reinterpret_cast<ID3D12GraphicsCommandList*>(std::get<0>(cmd_list));

    auto* b1 = std::get<0>(resource_desc_buffer);
    auto* b2 = std::get<0>(sampler_desc_buffer);

    std::array<ID3D12DescriptorHeap*, 2> buffers{
        b1 ? b1 : b2,
        b2,
    };

    auto buffer_count = (b1 != nullptr) + (b2 != nullptr);
    list->SetDescriptorHeaps(buffer_count, buffers.data());
}

void wis::ImplDX12DescriptorBufferExtension::SetDescriptorTableOffset(wis::DX12CommandListView cmd_list,
                                                                      wis::DX12RootSignatureView root_signature,
                                                                      uint32_t root_table_index,
                                                                      wis::DX12DescriptorBufferGPUView buffer,
                                                                      uint32_t table_aligned_byte_offset) const noexcept
{
    auto handle = std::get<0>(buffer);
    auto* list = reinterpret_cast<ID3D12GraphicsCommandList*>(std::get<0>(cmd_list));
    uint32_t root_table_offset = std::get<2>(root_signature); // offset of the root table in the root signature (after push constants and push desriptors)
    list->SetGraphicsRootDescriptorTable(root_table_offset + root_table_index,
                                         CD3DX12_GPU_DESCRIPTOR_HANDLE(handle, table_aligned_byte_offset));
}

#endif // WISDOM_DX12
#endif // WISDOM_EXTENDED_ALLOCATION_CPP
