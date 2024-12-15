#ifndef WISDOM_DESCRIPTOR_BUFFER_DX_CPP
#define WISDOM_DESCRIPTOR_BUFFER_DX_CPP

#if defined(WISDOM_DX12)
#include <wisdom/wisdom_descriptor_buffer.h>
#include <wisdom/util/small_allocator.h>

namespace wis::detail {
constexpr inline D3D12_ROOT_PARAMETER_TYPE to_dx_ext(wis::DescriptorType type) noexcept
{
    switch (type) {
    case wis::DescriptorType::Buffer:
    case wis::DescriptorType::Texture:
        return D3D12_ROOT_PARAMETER_TYPE_SRV;
    case wis::DescriptorType::ConstantBuffer:
        return D3D12_ROOT_PARAMETER_TYPE_CBV;
    case wis::DescriptorType::RWBuffer:
    case wis::DescriptorType::RWTexture:
        return D3D12_ROOT_PARAMETER_TYPE_UAV;
    default:
        return D3D12_ROOT_PARAMETER_TYPE_CBV;
    }
}
} // namespace wis::detail

wis::DX12RootSignature
wis::ImplDX12DescriptorBufferExtension::CreateRootSignature(wis::Result& result, const PushConstant* root_constants,
                                                            uint32_t constants_size,
                                                            const PushDescriptor* push_descriptors,
                                                            uint32_t push_descriptors_size,
                                                            const wis::DescriptorTable* tables,
                                                            uint32_t tables_count) const noexcept
{
    DX12RootSignature out_signature;
    auto& internal = out_signature.GetMutableInternal();

    if (constants_size > wis::max_push_constants) {
        result = wis::make_result<FUNC, "constants_size exceeds max_push_constants">(E_INVALIDARG);
        return out_signature;
    }
    if (push_descriptors_size > wis::max_push_descriptors) {
        result = wis::make_result<FUNC, "push_descriptors_size exceeds max_push_descriptors">(E_INVALIDARG);
        return out_signature;
    }
    if (tables_count + constants_size + push_descriptors_size > 64) {
        result = wis::make_result<FUNC, "sum of all parameters exceeds max amount of root parameters">(E_INVALIDARG);
        return out_signature;
    }

    D3D12_ROOT_PARAMETER1 root_params[64]{}; // max overall size of root parameters

    // push constants
    std::array<int8_t, size_t(wis::ShaderStages::Count)> stage_map{};
    std::fill(stage_map.begin(), stage_map.end(), -1);

    for (uint32_t i = 0; i < constants_size; ++i) {
        auto& constant = root_constants[i];
        root_params[i] = {
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

    // push descriptors
    for (uint32_t i = 0; i < push_descriptors_size; ++i) {
        auto& descriptor = push_descriptors[i];
        root_params[i + constants_size] = {
            .ParameterType = detail::to_dx_ext(descriptor.type),
            .Descriptor = {
                    .ShaderRegister = i,
                    .RegisterSpace = 0, // always 0 for push descriptors
            },
            .ShaderVisibility = convert_dx(descriptor.stage),
        };
    }

    size_t memory_size = 0;
    for (uint32_t i = 0; i < tables_count; ++i) {
        auto& table = tables[i];
        memory_size += table.entry_count;
    }

    auto memory = wis::detail::make_unique_for_overwrite<D3D12_DESCRIPTOR_RANGE1[]>(memory_size);
    if (!memory) {
        result = wis::make_result<FUNC, "Failed to allocate memory for descriptor ranges">(E_OUTOFMEMORY);
        return out_signature;
    }

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

        root_params[i] = {
            .ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
            .DescriptorTable = {
                    .NumDescriptorRanges = table.entry_count,
                    .pDescriptorRanges = memory.get() + offset,
            },
            .ShaderVisibility = D3D12_SHADER_VISIBILITY(table.stage),
        };
        offset += table.entry_count;
    }

    D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;
    desc.Init_1_1(constants_size + push_descriptors_size + tables_count, root_params, 0, nullptr, flags);

    wis::com_ptr<ID3DBlob> signature;
    wis::com_ptr<ID3DBlob> error;
    HRESULT hr = D3D12SerializeVersionedRootSignature(&desc, signature.put(), error.put());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to serialize root signature">(hr);
        return out_signature;
    }

    hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
                                     __uuidof(*internal.root), internal.root.put_void());
    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to create root signature">(hr);
        return out_signature;
    }
    internal.push_constant_count = constants_size;
    internal.push_descriptor_count = push_descriptors_size;
    internal.stage_map = stage_map;
    return out_signature;
}

wis::DX12DescriptorBuffer
wis::ImplDX12DescriptorBufferExtension::CreateDescriptorBuffer(wis::Result& result, wis::DescriptorHeapType heap_type,
                                                               wis::DescriptorMemory memory_type,
                                                               uint64_t memory_bytes) const noexcept
{
    DX12DescriptorBuffer out_buffer;
    auto& internal = out_buffer.GetMutableInternal();

    auto xheap_type = convert_dx(heap_type);
    auto inc_size = device->GetDescriptorHandleIncrementSize(xheap_type);
    auto aligned_size = wis::detail::aligned_size(memory_bytes, uint64_t(inc_size));
    D3D12_DESCRIPTOR_HEAP_DESC desc{
        .Type = convert_dx(heap_type),
        .NumDescriptors = uint32_t(aligned_size / inc_size),
        .Flags = convert_dx(memory_type),
        .NodeMask = 0,
    };

    auto hr = device->CreateDescriptorHeap(&desc, internal.heap.iid(), internal.heap.put_void());
    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to create descriptor heap">(hr);
        return out_buffer;
    }
    internal.heap_increment = device->GetDescriptorHandleIncrementSize(desc.Type);
    internal.heap_start = CD3DX12_CPU_DESCRIPTOR_HANDLE(internal.heap->GetCPUDescriptorHandleForHeapStart());
    internal.heap_gpu_start = CD3DX12_GPU_DESCRIPTOR_HANDLE(internal.heap->GetGPUDescriptorHandleForHeapStart());
    internal.heap->GetDevice(internal.device.iid(), internal.device.put_void());
    return out_buffer;
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
