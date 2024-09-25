#pragma once
#ifdef WISDOM_BUILD_BINARIES
#include <wisdom/dx12/dx12_device.h>
#endif // !WISDOM_HEADER_ONLY

#include <d3dx12/d3dx12_check_feature_support.h>
#include <d3dx12/d3dx12_pipeline_state_stream.h>
#include <d3dx12/d3dx12_root_signature.h>
#include <wisdom/util/small_allocator.h>
#include <wisdom/util/misc.h>
#include <numeric>

wis::ResultValue<wis::DX12Device>
wis::ImplDX12CreateDevice(wis::DX12Adapter adapter, wis::DX12DeviceExtension** extensions, uint32_t ext_count, bool force) noexcept
{
    auto in_adapter = adapter.GetInternal().adapter.get();

    wis::com_ptr<IDXGIFactory4> in_factory;
    in_adapter->GetParent(__uuidof(*in_factory), in_factory.put_void());

    wis::com_ptr<ID3D12Device10> device;

    HRESULT hr = D3D12CreateDevice(in_adapter, D3D_FEATURE_LEVEL_11_0,
                                   __uuidof(ID3D12Device9), device.put_void());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "D3D12CreateDevice failed to create device">(hr);

    auto xdevice = wis::DX12Device(std::move(device), wis::com_ptr(in_adapter), std::move(in_factory));

    if (!xdevice.QueryFeatureSupport(wis::DeviceFeature::EnchancedBarriers) && !force)
        return wis::make_result<FUNC, "Device does not support enhanced barriers">(E_FAIL);

    for (uint32_t i = 0; i < ext_count; i++) {
        extensions[i]->Init(xdevice);
    }
    return std::move(xdevice);
}

wis::Result wis::ImplDX12Device::WaitForMultipleFences(const DX12FenceView* fences,
                                                   const uint64_t* values, uint32_t count,
                                                   MutiWaitFlags wait_all,
                                                   uint64_t timeout) const noexcept
{
    unique_event e;
    HRESULT hr = S_OK;

    if (!succeeded(hr = device->SetEventOnMultipleFenceCompletion(
                           reinterpret_cast<ID3D12Fence* const*>(fences), values, count,
                           static_cast<D3D12_MULTIPLE_FENCE_WAIT_FLAGS>(wait_all), e.get())))
        return wis::make_result<FUNC, "ID3D12Device10::SetEventOnMultipleFenceCompletion failed to set "
                                      "event on multiple fence completion">(hr);

    auto st = e.wait(uint32_t(timeout));
    return st == wis::Status::Timeout  ? wis::Result{ st, "Wait timed out" }
            : st != wis::Status::Error ? wis::success
                                       : wis::make_result<FUNC, "Failed to wait for event">(E_FAIL);
}

wis::ResultValue<wis::DX12Fence>
wis::ImplDX12Device::CreateFence(uint64_t initial_value, wis::FenceFlags flags) const noexcept
{
    wis::com_ptr<ID3D12Fence1> fence;
    HRESULT hr = device->CreateFence(initial_value, convert_dx(flags),
                                     __uuidof(*fence), fence.put_void());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "ID3D12Device10::CreateFence failed to create fence">(hr);

    return DX12Fence{ std::move(fence) };
}

wis::ResultValue<wis::DX12CommandQueue>
wis::ImplDX12Device::CreateCommandQueue(wis::QueueType type) const noexcept
{
    wis::com_ptr<ID3D12CommandQueue> queue;
    D3D12_COMMAND_QUEUE_DESC desc{
        .Type = D3D12_COMMAND_LIST_TYPE(type),
        .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
    };

    HRESULT hr = device->CreateCommandQueue(&desc, __uuidof(*queue), queue.put_void());
    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "ID3D12Device10::CreateCommandQueue failed to create command queue">(hr);

    return DX12CommandQueue{ std::move(queue) };
}

wis::ResultValue<wis::DX12CommandList>
wis::ImplDX12Device::CreateCommandList(wis::QueueType type) const noexcept
{
    D3D12_COMMAND_LIST_TYPE clty = D3D12_COMMAND_LIST_TYPE(type);
    wis::com_ptr<ID3D12CommandAllocator> allocator;
    wis::com_ptr<ID3D12GraphicsCommandList9> command_list;

    HRESULT hr = device->CreateCommandAllocator(clty, __uuidof(*allocator), allocator.put_void());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to create command allocator">(hr);

    hr = device->CreateCommandList(0, clty, allocator.get(), nullptr,
                                   __uuidof(*command_list), command_list.put_void());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to create command list">(hr);

    return DX12CommandList{ std::move(allocator), std::move(command_list) };
}

wis::ResultValue<wis::DX12RootSignature>
wis::ImplDX12Device::CreateRootSignature(const RootConstant* root_constants,
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
                    .ShaderRegister = DX12RootSignature::root_const_register,
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

namespace wis::detail {
template<typename Stage>
inline void DX12FillShaderStage(wis::detail::memory_pool<1024>& pipeline_stream,
                                wis::DX12ShaderView shader) noexcept
{
    auto d = std::get<0>(shader);
    if (d) {
        auto s = std::get<1>(shader);
        pipeline_stream.allocate<Stage>() = { d, s };
    }
}
} // namespace wis::detail

wis::ResultValue<wis::DX12PipelineState>
wis::ImplDX12Device::CreateGraphicsPipeline(const wis::DX12GraphicsPipelineDesc* desc) const noexcept
{
    wis::com_ptr<ID3D12PipelineState> state;
    //--Shader stages
    wis::detail::memory_pool pipeline_stream;
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_VS>(pipeline_stream,
                                                                       desc->shaders.vertex);
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_PS>(pipeline_stream,
                                                                       desc->shaders.pixel);
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_GS>(pipeline_stream,
                                                                       desc->shaders.geometry);
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_HS>(pipeline_stream,
                                                                       desc->shaders.hull);
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_DS>(pipeline_stream,
                                                                       desc->shaders.domain);
    //--Topology
    pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY>() =
            convert_dx(desc->topology_type);

    //--Root signature

    pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE>() =
            std::get<0>(desc->root_signature);

    //--Input layout
    wis::detail::limited_allocator<D3D12_INPUT_ELEMENT_DESC, wis::max_vertex_bindings> ia_stage{
        desc->input_layout.attribute_count
    };

    auto slots =
            std::span{ desc->input_layout.slots, desc->input_layout.slots + desc->input_layout.slot_count };
    auto attrs = std::span{ desc->input_layout.attributes,
                            desc->input_layout.attributes + desc->input_layout.attribute_count };

    for (auto& i : attrs) {
        auto slot =
                std::find_if(slots.begin(), slots.end(), [&](auto& s) {
                    return s.slot == i.input_slot;
                });
        if (slot == slots.end())
            continue;

        *ia_stage.allocate() = { .SemanticName = i.semantic_name,
                                 .SemanticIndex = i.semantic_index,
                                 .Format = convert_dx(i.format),
                                 .InputSlot = i.input_slot,
                                 .AlignedByteOffset = i.offset_bytes,
                                 .InputSlotClass = D3D12_INPUT_CLASSIFICATION(slot->input_class),
                                 .InstanceDataStepRate = slot->input_class == wis::InputClass::PerInstance ? slot->stride_bytes : 0 };
    }
    pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT>() = {
        .pInputElementDescs = ia_stage.data(),
        .NumElements = ia_stage.size(),
    };

    //--Rasterizer
    if (desc->rasterizer) {
        bool bias = desc->rasterizer->depth_bias_enable;
        pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER2>() =
                CD3DX12_RASTERIZER_DESC2{ D3D12_RASTERIZER_DESC2{
                        .FillMode = convert_dx(desc->rasterizer->fill_mode),
                        .CullMode = convert_dx(desc->rasterizer->cull_mode),
                        .FrontCounterClockwise = convert_dx(desc->rasterizer->front_face),
                        .DepthBias = bias ? desc->rasterizer->depth_bias : 0.0f,
                        .DepthBiasClamp = bias ? desc->rasterizer->depth_bias_clamp : 0.0f,
                        .SlopeScaledDepthBias = bias ? desc->rasterizer->depth_bias_slope_factor : 0.0f,
                        .DepthClipEnable = desc->rasterizer->depth_clip_enable,
                } };
    }

    //--Multisample
    if (desc->sample) {
        pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC>() = DXGI_SAMPLE_DESC{
            .Count = convert_dx(desc->sample->rate),
            .Quality = 0,
        };
        pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_MASK>() =
                desc->sample->sample_mask;
    }

    //--Depth stencil
    if (desc->depth_stencil) {
        auto& ds = *desc->depth_stencil;
        pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL2>() =
                CD3DX12_DEPTH_STENCIL_DESC2{ D3D12_DEPTH_STENCIL_DESC2{
                        .DepthEnable = ds.depth_enable,
                        .DepthWriteMask = D3D12_DEPTH_WRITE_MASK(ds.depth_write_enable),
                        .DepthFunc = convert_dx(ds.depth_comp),
                        .StencilEnable = ds.stencil_enable,
                        .FrontFace =
                                D3D12_DEPTH_STENCILOP_DESC1{
                                        .StencilFailOp = convert_dx(ds.stencil_front.fail_op),
                                        .StencilDepthFailOp = convert_dx(ds.stencil_front.depth_fail_op),
                                        .StencilPassOp = convert_dx(ds.stencil_front.pass_op),
                                        .StencilFunc = convert_dx(ds.stencil_front.comparison),
                                        .StencilReadMask = ds.stencil_front.read_mask,
                                        .StencilWriteMask = ds.stencil_front.write_mask,
                                },
                        .BackFace =
                                D3D12_DEPTH_STENCILOP_DESC1{
                                        .StencilFailOp = convert_dx(ds.stencil_back.fail_op),
                                        .StencilDepthFailOp = convert_dx(ds.stencil_back.depth_fail_op),
                                        .StencilPassOp = convert_dx(ds.stencil_back.pass_op),
                                        .StencilFunc = convert_dx(ds.stencil_back.comparison),
                                        .StencilReadMask = ds.stencil_back.read_mask,
                                        .StencilWriteMask = ds.stencil_back.write_mask,
                                },
                        .DepthBoundsTestEnable = ds.depth_bound_test,
                } };
    }

    //--Render targets
    pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT>(
            convert_dx(desc->attachments.depth_attachment));

    D3D12_RT_FORMAT_ARRAY rta{ .NumRenderTargets = uint32_t(std::min(
                                       desc->attachments.attachments_count, wis::max_render_targets)) };
    for (size_t i = 0; i < rta.NumRenderTargets; i++) {
        rta.RTFormats[i] = convert_dx(desc->attachments.attachment_formats[i]);
    }
    pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS>() = rta;

    //--Blend
    if (desc->blend) {
        auto& blend = *desc->blend;
        D3D12_BLEND_DESC bdesc{
            .AlphaToCoverageEnable = false,
            .IndependentBlendEnable = !blend.logic_op_enable,
        };
        if (blend.logic_op_enable) {
            bdesc.RenderTarget[0].LogicOpEnable = true;
            bdesc.RenderTarget[0].LogicOp = convert_dx(blend.logic_op);
        } else {
            for (size_t i = 0; i < blend.attachment_count; i++) {
                auto& a = blend.attachments[i];
                bdesc.RenderTarget[i] = D3D12_RENDER_TARGET_BLEND_DESC{
                    .BlendEnable = a.blend_enable,
                    .LogicOpEnable = false,
                    .SrcBlend = convert_dx(a.src_color_blend),
                    .DestBlend = convert_dx(a.dst_color_blend),
                    .BlendOp = convert_dx(a.color_blend_op),
                    .SrcBlendAlpha = convert_dx(a.src_alpha_blend),
                    .DestBlendAlpha = convert_dx(a.dst_alpha_blend),
                    .BlendOpAlpha = convert_dx(a.alpha_blend_op),
                    .LogicOp = D3D12_LOGIC_OP_NOOP,
                    .RenderTargetWriteMask = UINT8(a.color_write_mask),
                };
            }
        }
        pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC>() =
                CD3DX12_BLEND_DESC{ bdesc };
    }

    D3D12_PIPELINE_STATE_STREAM_DESC psstream_desc{
        .SizeInBytes = pipeline_stream.size_bytes(),
        .pPipelineStateSubobjectStream = pipeline_stream.data<void>(),
    };

    HRESULT hr = device->CreatePipelineState(&psstream_desc, __uuidof(*state), state.put_void());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to create pipeline state">(hr);

    return DX12PipelineState{ std::move(state) };
}

wis::ResultValue<wis::DX12Shader> wis::ImplDX12Device::CreateShader(void* data,
                                                                size_t size) const noexcept
{
    auto x = wis::detail::make_unique_for_overwrite<std::byte[]>(size);

    if (!x)
        return wis::make_result<FUNC, "Failed to allocate memory for shader bytecode">(E_OUTOFMEMORY);

    std::copy_n(reinterpret_cast<std::byte*>(data), size, x.get());
    return DX12Shader{ std::move(x), size };
}

wis::ResultValue<wis::DX12ResourceAllocator>
wis::ImplDX12Device::CreateAllocator() const noexcept
{
    D3D12MA::ALLOCATOR_DESC desc{ .Flags = D3D12MA::ALLOCATOR_FLAGS::ALLOCATOR_FLAG_NONE,
                                  .pDevice = device.get(),
                                  .PreferredBlockSize = 0,
                                  .pAllocationCallbacks = nullptr,
                                  .pAdapter = adapter.get() };
    wis::com_ptr<D3D12MA::Allocator> allocator;
    HRESULT hr = D3D12MA::CreateAllocator(&desc, allocator.put());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to create allocator">(hr);

    return DX12ResourceAllocator{ std::move(allocator), device.get() };
}

wis::ResultValue<wis::DX12RenderTarget>
wis::ImplDX12Device::CreateRenderTarget(DX12TextureView texture, wis::RenderTargetDesc desc) const noexcept
{
    D3D12_RENDER_TARGET_VIEW_DESC rtv_desc{
        .Format = convert_dx(desc.format),
        .ViewDimension = D3D12_RTV_DIMENSION(desc.layout),
        .Texture2DArray{
                .MipSlice = desc.mip,
                .FirstArraySlice = desc.base_array_layer,
                .ArraySize = desc.layer_count,
                .PlaneSlice = 0 }
    };
    switch (desc.layout) {
    case wis::TextureLayout::Texture1D:
        rtv_desc.Texture1D = {
            .MipSlice = desc.mip
        };
        break;
    case wis::TextureLayout::Texture1DArray:
        rtv_desc.Texture1DArray = {
            .MipSlice = desc.mip,
            .FirstArraySlice = desc.base_array_layer,
            .ArraySize = desc.layer_count
        };
        break;
    case wis::TextureLayout::Texture2D:
        rtv_desc.Texture2D = {
            .MipSlice = desc.mip
        };
        break;
    case wis::TextureLayout::Texture2DMSArray:
        rtv_desc.Texture2DMSArray = {
            .FirstArraySlice = desc.base_array_layer,
            .ArraySize = desc.layer_count
        };
        break;
    case wis::TextureLayout::Texture3D:
        rtv_desc.Texture3D = {
            .MipSlice = desc.mip,
            .FirstWSlice = desc.base_array_layer,
            .WSize = desc.layer_count
        };
        break;
    default:
        break;
    }

    D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = 1,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
        .NodeMask = 0u
    };

    wis::com_ptr<ID3D12DescriptorHeap> heap;
    auto hr = device->CreateDescriptorHeap(&heap_desc, __uuidof(*heap), heap.put_void());
    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to create descriptor heap for render target view">(hr);

    auto handle = heap->GetCPUDescriptorHandleForHeapStart();
    device->CreateRenderTargetView(std::get<0>(texture), &rtv_desc, handle);
    return DX12RenderTarget{ std::move(heap), handle };
}

wis::ResultValue<wis::DX12DescriptorBuffer>
wis::ImplDX12Device::CreateDescriptorBuffer(wis::DescriptorHeapType heap_type, wis::DescriptorMemory memory_type, uint32_t descriptor_count) const noexcept
{
    D3D12_DESCRIPTOR_HEAP_DESC desc{
        .Type = convert_dx(heap_type),
        .NumDescriptors = descriptor_count,
        .Flags = convert_dx(memory_type),
        .NodeMask = 0,
    };
    wis::com_ptr<ID3D12DescriptorHeap> heap;
    auto hr = device->CreateDescriptorHeap(&desc, heap.iid(), heap.put_void());
    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Failed to create descriptor buffer">(hr);

    return DX12DescriptorBuffer{ std::move(heap), device->GetDescriptorHandleIncrementSize(desc.Type) };
}

wis::ResultValue<wis::DX12Sampler>
wis::ImplDX12Device::CreateSampler(const wis::SamplerDesc* desc) const noexcept
{
    auto min_filter = !desc->anisotropic ? convert_dx(desc->min_filter) : D3D12_FILTER_TYPE_LINEAR;
    auto mag_filter = !desc->anisotropic ? convert_dx(desc->mag_filter) : D3D12_FILTER_TYPE_LINEAR;
    auto basic_filter = D3D12_ENCODE_BASIC_FILTER(min_filter, mag_filter, convert_dx(desc->mip_filter), D3D12_FILTER_REDUCTION_TYPE::D3D12_FILTER_REDUCTION_TYPE_STANDARD);
    auto filter = D3D12_FILTER(desc->anisotropic * D3D12_ANISOTROPIC_FILTERING_BIT | basic_filter);

    D3D12_SAMPLER_DESC sampler_desc{
        .Filter = filter,
        .AddressU = convert_dx(desc->address_u),
        .AddressV = convert_dx(desc->address_v),
        .AddressW = convert_dx(desc->address_w),
        .MipLODBias = desc->mip_lod_bias,
        .MaxAnisotropy = desc->anisotropic ? std::min(uint32_t(D3D12_MAX_MAXANISOTROPY), desc->max_anisotropy) : 0,
        .ComparisonFunc = convert_dx(desc->comparison_op),
        .BorderColor = { desc->border_color[0], desc->border_color[1], desc->border_color[2], desc->border_color[3] },
        .MinLOD = desc->min_lod,
        .MaxLOD = desc->max_lod,
    };

    wis::com_ptr<ID3D12DescriptorHeap> heap;
    D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
        .NumDescriptors = 1,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE
    };

    auto x = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
    device->CreateDescriptorHeap(&heap_desc, heap.iid(), heap.put_void());
    device->CreateSampler(&sampler_desc, heap->GetCPUDescriptorHandleForHeapStart());
    return wis::DX12Sampler{ std::move(heap) };
}

wis::ResultValue<wis::DX12ShaderResource>
wis::ImplDX12Device::CreateShaderResource(DX12TextureView texture, wis::ShaderResourceDesc desc) const noexcept
{
    D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{
        .Format = convert_dx(desc.format),
        .ViewDimension = convert_dx(desc.view_type),
        .Shader4ComponentMapping = UINT(D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(
                convert_dx(desc.component_mapping.r),
                convert_dx(desc.component_mapping.g),
                convert_dx(desc.component_mapping.b),
                convert_dx(desc.component_mapping.a))),
    };

    switch (desc.view_type) {
    case wis::TextureViewType::Texture1D:
        srv_desc.Texture1D = {
            .MostDetailedMip = desc.subresource_range.base_mip_level,
            .MipLevels = desc.subresource_range.level_count,
            .ResourceMinLODClamp = 0.0f,
        };
        break;
    case wis::TextureViewType::Texture1DArray:
        srv_desc.Texture1DArray = {
            .MostDetailedMip = desc.subresource_range.base_mip_level,
            .MipLevels = desc.subresource_range.level_count,
            .FirstArraySlice = desc.subresource_range.base_array_layer,
            .ArraySize = desc.subresource_range.layer_count,
            .ResourceMinLODClamp = 0.0f,
        };
        break;
    default:
    case wis::TextureViewType::Texture2D:
        srv_desc.Texture2D = {
            .MostDetailedMip = desc.subresource_range.base_mip_level,
            .MipLevels = desc.subresource_range.level_count,
            .PlaneSlice = 0,
            .ResourceMinLODClamp = 0.0f,
        };
        break;
    case wis::TextureViewType::Texture2DArray:
        srv_desc.Texture2DArray = {
            .MostDetailedMip = desc.subresource_range.base_mip_level,
            .MipLevels = desc.subresource_range.level_count,
            .FirstArraySlice = desc.subresource_range.base_array_layer,
            .ArraySize = desc.subresource_range.layer_count,
            .PlaneSlice = 0,
            .ResourceMinLODClamp = 0.0f,
        };
        break;
    case wis::TextureViewType::Texture2DMS:
        srv_desc.Texture2DMS = {};
        break;
    case wis::TextureViewType::Texture2DMSArray:
        srv_desc.Texture2DMSArray = {
            .FirstArraySlice = desc.subresource_range.base_array_layer,
            .ArraySize = desc.subresource_range.layer_count,
        };
        break;
    case wis::TextureViewType::Texture3D:
        srv_desc.Texture3D = {
            .MostDetailedMip = desc.subresource_range.base_mip_level,
            .MipLevels = desc.subresource_range.level_count,
            .ResourceMinLODClamp = 0.0f,
        };
        break;
    case wis::TextureViewType::TextureCube:
        srv_desc.TextureCube = {
            .MostDetailedMip = desc.subresource_range.base_mip_level,
            .MipLevels = desc.subresource_range.level_count,
            .ResourceMinLODClamp = 0.0f,
        };
        break;
    case wis::TextureViewType::TextureCubeArray:
        srv_desc.TextureCubeArray = {
            .MostDetailedMip = desc.subresource_range.base_mip_level,
            .MipLevels = desc.subresource_range.level_count,
            .First2DArrayFace = desc.subresource_range.base_array_layer,
            .NumCubes = desc.subresource_range.layer_count / 6,
            .ResourceMinLODClamp = 0.0f,
        };
        break;
    }

    wis::com_ptr<ID3D12DescriptorHeap> heap;
    D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        .NumDescriptors = 1,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE
    };

    auto x = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    device->CreateDescriptorHeap(&heap_desc, heap.iid(), heap.put_void());
    device->CreateShaderResourceView(std::get<0>(texture), &srv_desc, heap->GetCPUDescriptorHandleForHeapStart());
    return wis::DX12ShaderResource{ std::move(heap) };
}

bool wis::ImplDX12Device::QueryFeatureSupport(wis::DeviceFeature feature) const noexcept
{
    switch (feature) {
    case wis::DeviceFeature::EnchancedBarriers: {
        D3D12_FEATURE_DATA_D3D12_OPTIONS12 options12 = {};
        bool EnhancedBarriersSupported = false;
        if (wis::succeeded(device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS12, &options12, sizeof(options12)))) {
            EnhancedBarriersSupported = options12.EnhancedBarriersSupported;
        }
        return EnhancedBarriersSupported;
    }
    case wis::DeviceFeature::DescriptorBuffer:
    case wis::DeviceFeature::WaitForPresent:
    case wis::DeviceFeature::DescriptorEqualSize:
    case wis::DeviceFeature::DynamicVSync:
    case wis::DeviceFeature::AdvancedIndexBuffer:
        return true;
    default:
        return false;
    }
}
