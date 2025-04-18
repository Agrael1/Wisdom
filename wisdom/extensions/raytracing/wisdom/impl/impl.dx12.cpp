#ifndef WISDOM_RAYTRACING_DX12_CPP
#define WISDOM_RAYTRACING_DX12_CPP
#ifndef WISDOM_MODULE_DECL
#include <wisdom/impl.dx12.h>
#endif // !WISDOM_MODULE_DECL

#if defined(WISDOM_DX12)
wis::DX12RaytracingPipeline
wis::ImplDX12Raytracing::CreateRaytracingPipeline(wis::Result& result, const wis::DX12RaytracingPipeineDesc& desc) const noexcept
{
    wis::DX12RaytracingPipeline out_pipeline;
    auto& pipe_i = out_pipeline.GetMutableInternal();

    uint32_t num_raygen = 0;
    uint32_t num_miss = 0;
    uint32_t num_callable = 0;

    constexpr static std::wstring_view hit_group_exa = L"H|A|C|I|00000000";
    static auto precalc_wchspace = [](const char* data) {
        return MultiByteToWideChar(CP_UTF8, 0, data, strlen(data) + 1, nullptr, 0);
    };
    static auto convert_to_wch = [](const char* data, wchar_t** wch) {
        size_t szc = strlen(data) + 1;
        auto wch_spc = MultiByteToWideChar(CP_UTF8, 0, data, szc, nullptr, 0);
        MultiByteToWideChar(CP_UTF8, 0, data, szc, *wch, wch_spc);
        auto* prev = *wch;
        *wch += wch_spc;
        return prev;
    };
    static auto rename_export = [](const wchar_t* name, wchar_t** wch, uint32_t index) {
        size_t szc = wcslen(name);
        memcpy(*wch, name, szc * sizeof(wchar_t));
        // append index
        auto* prev = *wch;
        *wch += szc;
        auto offset = swprintf_s(*wch, 10, L"|%08x", index);
        *wch += offset + 1;
        return prev;
    };
    static auto name_hit_group = [](D3D12_HIT_GROUP_DESC& hg, wchar_t** wch, uint32_t index) {
        // format: H|A|C|I|00000000
        hg.HitGroupExport = *wch;
        **wch = L'H';
        (*wch)++;
        **wch = L'|';
        (*wch)++;
        **wch = hg.AnyHitShaderImport ? L'A' : L'0';
        (*wch)++;
        **wch = L'|';
        (*wch)++;
        **wch = hg.ClosestHitShaderImport ? L'C' : L'0';
        (*wch)++;
        **wch = L'|';
        (*wch)++;
        **wch = hg.IntersectionShaderImport ? L'I' : L'0';
        (*wch)++;
        **wch = L'|';
        (*wch)++;
        auto offset = swprintf_s(*wch, 9, L"%08x", index);
        *wch += offset + 1;
    };

    // count all the string space
    size_t wchspace = 0;
    for (uint32_t i = 0; i < desc.export_count; ++i) {
        auto sz = precalc_wchspace(desc.exports[i].entry_point);
        if (sz == 0) {
            result = wis::make_result<wis::Func<wis::FuncD()>(), "Invalid shader export name">(E_INVALIDARG);
            return out_pipeline;
        }
        wchspace += sz;
    }

    for (uint32_t i = 0; i < desc.export_count; ++i) {
        num_raygen += desc.exports[i].shader_type == wis::RaytracingShaderType::Raygen;
        num_miss += desc.exports[i].shader_type == wis::RaytracingShaderType::Miss;
        num_callable += desc.exports[i].shader_type == wis::RaytracingShaderType::Callable;
    }

    uint32_t num_subobjects = desc.shader_count + desc.hit_group_count + 3; // root signature and max recursion depth
    size_t string_offset = num_subobjects * sizeof(D3D12_STATE_SUBOBJECT) +
            desc.shader_count * sizeof(D3D12_DXIL_LIBRARY_DESC) +
            desc.export_count * sizeof(D3D12_EXPORT_DESC) +
            desc.hit_group_count * sizeof(D3D12_HIT_GROUP_DESC) +
            (num_callable + num_miss + num_raygen) * sizeof(wchar_t*);

    size_t allocation_size = string_offset + // callable, miss, raygen
                                             // string names
            wchspace * sizeof(wchar_t) * 2u + // entry points
            desc.export_count * sizeof(wchar_t) * 9u + // unique names + entry points
            desc.hit_group_count * sizeof(wchar_t) * (hit_group_exa.size() + 1u) // hit group names format: H|A|C|I|00000000
            ;
    std::unique_ptr<uint8_t[]> subobjects = wis::detail::make_unique_for_overwrite<uint8_t[]>(allocation_size);

    // burn shader bytecodes
    std::span<D3D12_STATE_SUBOBJECT> subobjects_span(reinterpret_cast<D3D12_STATE_SUBOBJECT*>(subobjects.get()), num_subobjects);
    std::span<D3D12_DXIL_LIBRARY_DESC> dxil_library_span(reinterpret_cast<D3D12_DXIL_LIBRARY_DESC*>(subobjects_span.data() + num_subobjects), desc.shader_count);
    std::span<D3D12_EXPORT_DESC> export_span(reinterpret_cast<D3D12_EXPORT_DESC*>(dxil_library_span.data() + desc.shader_count), desc.export_count);
    std::span<D3D12_HIT_GROUP_DESC> hit_group_span(reinterpret_cast<D3D12_HIT_GROUP_DESC*>(export_span.data() + desc.export_count), desc.hit_group_count);
    std::span<wchar_t> wch_span(reinterpret_cast<wchar_t*>(subobjects.get() + string_offset), wchspace);

    for (uint32_t i = 0; i < desc.shader_count; ++i) {
        dxil_library_span[i] = {
            .DXILLibrary = { .pShaderBytecode = std::get<0>(desc.shaders[i]), .BytecodeLength = std::get<1>(desc.shaders[i]) }
        };
        subobjects_span[i] = {
            .Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY,
            .pDesc = &dxil_library_span[i]
        };
    }

    // burn exports with bucket sort O(n)
    for (uint32_t i = 0; i < desc.export_count; ++i) {
        dxil_library_span[desc.exports[i].shader_array_index].NumExports++;
    }

    // place exports in reverse order
    wchar_t* names = wch_span.data(); // pointer is incremented in convert_to_wch
    wchar_t* renames = names + wchspace;

    wchar_t** callable_span = reinterpret_cast<wchar_t**>(subobjects.get() + string_offset); // callable pointers in reverse order
    wchar_t** miss_span = callable_span - num_callable;
    wchar_t** raygen_span = miss_span - num_miss;

    dxil_library_span[0].pExports = export_span.data() + dxil_library_span[0].NumExports;
    for (uint32_t i = 1; i < desc.shader_count; ++i) {
        dxil_library_span[i].pExports = dxil_library_span[i - 1].pExports + dxil_library_span[i].NumExports;
    }
    for (int32_t i = desc.export_count - 1; i >= 0; --i) {
        uint32_t index = desc.exports[i].shader_array_index;
        auto x = --dxil_library_span[index].pExports;
        auto& _export = const_cast<D3D12_EXPORT_DESC&>(*x);

        auto* name = convert_to_wch(desc.exports[i].entry_point, &names);
        auto* rename = rename_export(name, &renames, i);

        switch (desc.exports[i].shader_type) {
        case wis::RaytracingShaderType::Raygen:
            --raygen_span;
            *(raygen_span) = rename;
            break;
        case wis::RaytracingShaderType::Miss:
            --miss_span;
            *(miss_span) = rename;
            break;
        case wis::RaytracingShaderType::Callable:
            --callable_span;
            *(callable_span) = rename;
            break;
        default:
            break;
        }

        _export = {
            .Name = rename,
            .ExportToRename = name,
            .Flags = D3D12_EXPORT_FLAG_NONE
        };
    }

    // hit groups
    wchar_t* hg_names = renames;
    for (uint32_t i = 0; i < desc.hit_group_count; ++i) {
        auto& hg = desc.hit_groups[i];
        hit_group_span[i] = {
            .Type = convert_dx(desc.hit_groups[i].type),
            .AnyHitShaderImport = hg.any_hit_export_index == UINT32_MAX ? nullptr : export_span[hg.any_hit_export_index].Name,
            .ClosestHitShaderImport = hg.closest_hit_export_index == UINT32_MAX ? nullptr : export_span[hg.closest_hit_export_index].Name,
            .IntersectionShaderImport = hg.intersection_export_index == UINT32_MAX ? nullptr : export_span[hg.intersection_export_index].Name
        };
        name_hit_group(hit_group_span[i], &renames, i);
        subobjects_span[desc.shader_count + i] = {
            .Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP,
            .pDesc = &hit_group_span[i]
        };
    }

    // shader config
    D3D12_RAYTRACING_SHADER_CONFIG shader_config{
        .MaxPayloadSizeInBytes = desc.max_payload_size,
        .MaxAttributeSizeInBytes = desc.max_attribute_size
    };
    subobjects_span[num_subobjects - 3] = {
        .Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG,
        .pDesc = &shader_config
    };

    // root signature
    D3D12_GLOBAL_ROOT_SIGNATURE global_root_signature{
        .pGlobalRootSignature = std::get<0>(desc.root_signature)
    };
    subobjects_span[num_subobjects - 2] = {
        .Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE,
        .pDesc = &global_root_signature // place root signature here
    };

    // max recursion depth
    D3D12_RAYTRACING_PIPELINE_CONFIG1 pipeline_config{
        .MaxTraceRecursionDepth = desc.max_recursion_depth,
        .Flags = D3D12_RAYTRACING_PIPELINE_FLAG_NONE
    };
    subobjects_span[num_subobjects - 1] = {
        .Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG1,
        .pDesc = &pipeline_config
    };

    // pipeline
    D3D12_STATE_OBJECT_DESC pipeline_desc{
        .Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE,
        .NumSubobjects = num_subobjects,
        .pSubobjects = subobjects_span.data()
    };

    // get shader identifiers
    pipe_i.shader_identifiers = wis::detail::make_unique_for_overwrite<uint8_t[]>(
            (num_raygen +
             num_miss +
             num_callable +
             desc.hit_group_count) *
            D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);

    if (!pipe_i.shader_identifiers) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to allocate shader identifiers">(E_OUTOFMEMORY);
        return out_pipeline;
    }

    auto hr = shared_device->CreateStateObject(&pipeline_desc, pipe_i.state_object.iid(), pipe_i.state_object.put_void());
    if (!wis::succeeded(hr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to create raytracing pipeline">(hr);
        return out_pipeline;
    }

    // get shader handles raygen, miss, hit, callable
    wis::com_ptr<ID3D12StateObjectProperties> state_object_props;
    std::ignore = pipe_i.state_object.as<ID3D12StateObjectProperties>(&state_object_props);

    uint8_t* raygen_ids = pipe_i.shader_identifiers.get();
    uint8_t* miss_ids = raygen_ids + num_raygen * D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
    uint8_t* hit_ids = miss_ids + num_miss * D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
    uint8_t* callable_ids = hit_ids + desc.hit_group_count * D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;

    for (uint32_t i = 0; i < num_raygen; ++i) {
        auto id = state_object_props->GetShaderIdentifier(raygen_span[i]);
        memcpy(raygen_ids, id, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
        raygen_ids += D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
    }

    for (uint32_t i = 0; i < num_miss; ++i) {
        auto id = state_object_props->GetShaderIdentifier(miss_span[i]);
        memcpy(miss_ids, id, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
        miss_ids += D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
    }

    for (uint32_t i = 0; i < desc.hit_group_count; ++i) {
        auto id = state_object_props->GetShaderIdentifier(hit_group_span[i].HitGroupExport);
        memcpy(hit_ids, id, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
        hit_ids += D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
    }

    for (uint32_t i = 0; i < num_callable; ++i) {
        auto id = state_object_props->GetShaderIdentifier(callable_span[i]);
        memcpy(callable_ids, id, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
        callable_ids += D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
    }

    return out_pipeline;
}

#endif // WISDOM_DX12
#endif // WISDOM_RAYTRACING_DX12_CPP
