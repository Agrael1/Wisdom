#ifndef WIS_DX12_DEVICE_CPP
#define WIS_DX12_DEVICE_CPP
#include <wisdom/dx12/dx12_device.h>

#include <d3dx12/d3dx12_check_feature_support.h>
#include <d3dx12/d3dx12_pipeline_state_stream.h>
#include <d3dx12/d3dx12_root_signature.h>
#include <wisdom/util/small_allocator.h>
#include <wisdom/util/misc.h>
#include <numeric>
#include <bit>

wis::DX12Device
wis::ImplDX12CreateDevice(wis::Result& result, wis::DX12Adapter adapter, wis::DX12DeviceExtension** extensions, uint32_t ext_count, bool force) noexcept
{
    DX12Device out_device;
    auto& internal = out_device.GetMutableInternal();
    internal.adapter = adapter.GetInternal().adapter; // Keep adapter alive
    internal.adapter->GetParent(internal.factory.iid(), internal.factory.put_void());
    HRESULT hr = D3D12CreateDevice(internal.adapter.get(), D3D_FEATURE_LEVEL_11_0,
                                   __uuidof(ID3D12Device9), internal.device.put_void());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "D3D12CreateDevice failed to create device">(hr);
        return out_device;
    }
    if (!out_device.QueryFeatureSupport(wis::DeviceFeature::EnchancedBarriers) && !force) {
        result = wis::make_result<FUNC, "Device does not support enhanced barriers">(E_FAIL);
        return out_device;
    }

    for (uint32_t i = 0; i < ext_count; i++) {
        extensions[i]->Init(out_device);
    }
    return out_device;
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

wis::DX12Fence
wis::ImplDX12Device::CreateFence(wis::Result& result, uint64_t initial_value, wis::FenceFlags flags) const noexcept
{
    wis::DX12Fence out_fence;
    auto& internal = out_fence.GetMutableInternal();

    HRESULT hr = device->CreateFence(initial_value, convert_dx(flags),
                                     internal.fence.iid(), internal.fence.put_void());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "ID3D12Device10::CreateFence failed to create fence">(hr);
    }
    return out_fence;
}

wis::DX12CommandQueue
wis::ImplDX12Device::CreateCommandQueue(wis::Result& result, wis::QueueType type) const noexcept
{
    DX12CommandQueue out_queue;
    auto& internal = out_queue.GetMutableInternal();

    D3D12_COMMAND_QUEUE_DESC desc{
        .Type = D3D12_COMMAND_LIST_TYPE(type),
        .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
    };

    HRESULT hr = device->CreateCommandQueue(&desc, internal.queue.iid(), internal.queue.put_void());
    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "ID3D12Device10::CreateCommandQueue failed to create command queue">(hr);
    }

    return out_queue;
}

wis::DX12CommandList
wis::ImplDX12Device::CreateCommandList(wis::Result& result, wis::QueueType type) const noexcept
{
    DX12CommandList out_list;
    auto& internal = out_list.GetMutableInternal();

    D3D12_COMMAND_LIST_TYPE clty = D3D12_COMMAND_LIST_TYPE(type);
    HRESULT hr = device->CreateCommandAllocator(clty, internal.allocator.iid(), internal.allocator.put_void());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to create command allocator">(hr);
        return out_list;
    }

    hr = device->CreateCommandList(0, clty, internal.allocator.get(), nullptr,
                                   internal.list.iid(), internal.list.put_void());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to create command list">(hr);
    }
    return out_list;
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

wis::DX12PipelineState
wis::ImplDX12Device::CreateGraphicsPipeline(wis::Result& result, const wis::DX12GraphicsPipelineDesc& desc) const noexcept
{
    DX12PipelineState out_state;
    auto& internal = out_state.GetMutableInternal();

    //--Shader stages
    wis::detail::memory_pool pipeline_stream;
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_VS>(pipeline_stream,
                                                                       desc.shaders.vertex);
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_PS>(pipeline_stream,
                                                                       desc.shaders.pixel);
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_GS>(pipeline_stream,
                                                                       desc.shaders.geometry);
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_HS>(pipeline_stream,
                                                                       desc.shaders.hull);
    wis::detail::DX12FillShaderStage<CD3DX12_PIPELINE_STATE_STREAM_DS>(pipeline_stream,
                                                                       desc.shaders.domain);
    //--Topology
    pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY>() =
            convert_dx(desc.topology_type);

    //--Root signature

    pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE>() =
            std::get<0>(desc.root_signature);

    //--Input layout
    wis::detail::limited_allocator<D3D12_INPUT_ELEMENT_DESC, wis::max_vertex_bindings> ia_stage{
        desc.input_layout.attribute_count
    };

    auto slots =
            std::span{ desc.input_layout.slots, desc.input_layout.slots + desc.input_layout.slot_count };
    auto attrs = std::span{ desc.input_layout.attributes,
                            desc.input_layout.attributes + desc.input_layout.attribute_count };

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
    if (desc.rasterizer) {
        bool bias = desc.rasterizer->depth_bias_enable;
        pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER2>() =
                CD3DX12_RASTERIZER_DESC2{ D3D12_RASTERIZER_DESC2{
                        .FillMode = convert_dx(desc.rasterizer->fill_mode),
                        .CullMode = convert_dx(desc.rasterizer->cull_mode),
                        .FrontCounterClockwise = convert_dx(desc.rasterizer->front_face),
                        .DepthBias = bias ? desc.rasterizer->depth_bias : 0.0f,
                        .DepthBiasClamp = bias ? desc.rasterizer->depth_bias_clamp : 0.0f,
                        .SlopeScaledDepthBias = bias ? desc.rasterizer->depth_bias_slope_factor : 0.0f,
                        .DepthClipEnable = desc.rasterizer->depth_clip_enable,
                } };
    }

    //--Multisample
    if (desc.sample) {
        pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC>() = DXGI_SAMPLE_DESC{
            .Count = convert_dx(desc.sample->rate),
            .Quality = 0,
        };
        pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_MASK>() =
                desc.sample->sample_mask;
    }

    //--Depth stencil
    if (desc.depth_stencil) {
        auto& ds = *desc.depth_stencil;
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
            convert_dx(desc.attachments.depth_attachment));

    D3D12_RT_FORMAT_ARRAY rta{ .NumRenderTargets = uint32_t(std::min(
                                       desc.attachments.attachments_count, wis::max_render_targets)) };
    for (size_t i = 0; i < rta.NumRenderTargets; i++) {
        rta.RTFormats[i] = convert_dx(desc.attachments.attachment_formats[i]);
    }
    pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS>() = rta;

    //--Multiview
    D3D12_VIEW_INSTANCE_LOCATION view_locs[8]{};
    if (desc.view_mask) {
        for (uint32_t i = 0u; i < 8u; i++) {
            if (!(desc.view_mask & (1u << i)))
                continue;

            view_locs[i] = D3D12_VIEW_INSTANCE_LOCATION{
                .ViewportArrayIndex = 0,
                .RenderTargetArrayIndex = i,
            };
        }
        pipeline_stream.allocate<CD3DX12_PIPELINE_STATE_STREAM_VIEW_INSTANCING>() = CD3DX12_VIEW_INSTANCING_DESC{
            uint32_t(std::popcount(desc.view_mask)),
            view_locs,
            D3D12_VIEW_INSTANCING_FLAGS::D3D12_VIEW_INSTANCING_FLAG_ENABLE_VIEW_INSTANCE_MASKING
        };
    }

    //--Blend
    if (desc.blend) {
        auto& blend = *desc.blend;
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

    HRESULT hr = device->CreatePipelineState(&psstream_desc, internal.pipeline.iid(), internal.pipeline.put_void());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to create pipeline state">(hr);
    }
    return out_state;
}

wis::DX12Shader
wis::ImplDX12Device::CreateShader(wis::Result& result, void* data,
                                  size_t size) const noexcept
{
    DX12Shader out_shader;
    auto& internal = out_shader.GetMutableInternal();

    internal.bytecode = wis::detail::make_unique_for_overwrite<std::byte[]>(size);

    if (!internal.bytecode) {
        result = wis::make_result<FUNC, "Failed to allocate memory for shader bytecode">(E_OUTOFMEMORY);
        return out_shader;
    }
    internal.size = size;
    std::copy_n(reinterpret_cast<std::byte*>(data), size, internal.bytecode.get());
    return out_shader;
}

wis::DX12ResourceAllocator
wis::ImplDX12Device::CreateAllocator(wis::Result& result) const noexcept
{
    wis::DX12ResourceAllocator out_allocator;
    auto& internal = out_allocator.GetMutableInternal();

    D3D12MA::ALLOCATOR_DESC desc{ .Flags = D3D12MA::ALLOCATOR_FLAGS::ALLOCATOR_FLAG_NONE,
                                  .pDevice = device.get(),
                                  .PreferredBlockSize = 0,
                                  .pAllocationCallbacks = nullptr,
                                  .pAdapter = adapter.get() };

    HRESULT hr = D3D12MA::CreateAllocator(&desc, internal.allocator.put());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to create allocator">(hr);
    }
    return out_allocator;
}

wis::DX12RenderTarget
wis::ImplDX12Device::CreateRenderTarget(wis::Result& result, DX12TextureView texture, wis::RenderTargetDesc desc) const noexcept
{
    DX12RenderTarget out_target;
    auto& internal = out_target.GetMutableInternal();

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

    auto hr = device->CreateDescriptorHeap(&heap_desc, internal.heap.iid(), internal.heap.put_void());
    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to create descriptor heap for render target view">(hr);
        return out_target;
    }

    internal.handle = internal.heap->GetCPUDescriptorHandleForHeapStart();
    device->CreateRenderTargetView(std::get<0>(texture), &rtv_desc, internal.handle);
    return out_target;
}

namespace wis::detail {
inline constexpr D3D12_DSV_DIMENSION to_dsv(wis::TextureLayout layout) noexcept
{
    switch (layout) {
    case wis::TextureLayout::Texture1D:
        return D3D12_DSV_DIMENSION_TEXTURE1D;
    case wis::TextureLayout::Texture1DArray:
        return D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
    case wis::TextureLayout::Texture2D:
        return D3D12_DSV_DIMENSION_TEXTURE2D;
    case wis::TextureLayout::Texture2DArray:
        return D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
    case wis::TextureLayout::Texture2DMS:
        return D3D12_DSV_DIMENSION_TEXTURE2DMS;
    case wis::TextureLayout::Texture2DMSArray:
        return D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
    default:
        return D3D12_DSV_DIMENSION_UNKNOWN;
    }
}
} // namespace wis::detail

wis::DX12RenderTarget
wis::ImplDX12Device::CreateDepthStencilTarget(wis::Result& result, DX12TextureView texture, wis::RenderTargetDesc desc) const noexcept
{
    DX12RenderTarget out_target;
    auto& internal = out_target.GetMutableInternal();

    D3D12_DEPTH_STENCIL_VIEW_DESC rtv_desc{
        .Format = convert_dx(desc.format),
        .ViewDimension = detail::to_dsv(desc.layout),
        .Texture2DArray{
                .MipSlice = desc.mip,
                .FirstArraySlice = desc.base_array_layer,
                .ArraySize = desc.layer_count }
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
    default:
        break;
    }

    D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
        .NumDescriptors = 1,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
        .NodeMask = 0u
    };

    auto hr = device->CreateDescriptorHeap(&heap_desc, internal.heap.iid(), internal.heap.put_void());
    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to create descriptor heap for render target view">(hr);
        return out_target;
    }

    internal.handle = internal.heap->GetCPUDescriptorHandleForHeapStart();
    device->CreateDepthStencilView(std::get<0>(texture), &rtv_desc, internal.handle);
    return out_target;
}

wis::DX12Sampler
wis::ImplDX12Device::CreateSampler(wis::Result& result, const wis::SamplerDesc& desc) const noexcept
{
    DX12Sampler out_sampler;
    auto& internal = out_sampler.GetMutableInternal();

    auto min_filter = !desc.anisotropic ? convert_dx(desc.min_filter) : D3D12_FILTER_TYPE_LINEAR;
    auto mag_filter = !desc.anisotropic ? convert_dx(desc.mag_filter) : D3D12_FILTER_TYPE_LINEAR;
    auto basic_filter = D3D12_ENCODE_BASIC_FILTER(min_filter, mag_filter, convert_dx(desc.mip_filter), D3D12_FILTER_REDUCTION_TYPE::D3D12_FILTER_REDUCTION_TYPE_STANDARD);
    auto filter = D3D12_FILTER(desc.anisotropic * D3D12_ANISOTROPIC_FILTERING_BIT | basic_filter);

    D3D12_SAMPLER_DESC sampler_desc{
        .Filter = filter,
        .AddressU = convert_dx(desc.address_u),
        .AddressV = convert_dx(desc.address_v),
        .AddressW = convert_dx(desc.address_w),
        .MipLODBias = desc.mip_lod_bias,
        .MaxAnisotropy = desc.anisotropic ? std::min(uint32_t(D3D12_MAX_MAXANISOTROPY), desc.max_anisotropy) : 0,
        .ComparisonFunc = convert_dx(desc.comparison_op),
        .BorderColor = { desc.border_color[0], desc.border_color[1], desc.border_color[2], desc.border_color[3] },
        .MinLOD = desc.min_lod,
        .MaxLOD = desc.max_lod,
    };

    D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
        .NumDescriptors = 1,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE
    };

    auto x = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
    device->CreateDescriptorHeap(&heap_desc, internal.heap.iid(), internal.heap.put_void());
    device->CreateSampler(&sampler_desc, internal.heap->GetCPUDescriptorHandleForHeapStart());
    return out_sampler;
}

wis::DX12ShaderResource
wis::ImplDX12Device::CreateShaderResource(wis::Result& result, DX12TextureView texture, wis::ShaderResourceDesc desc) const noexcept
{
    DX12ShaderResource out_resource;
    auto& internal = out_resource.GetMutableInternal();

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

    D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        .NumDescriptors = 1,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE
    };

    auto x = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    device->CreateDescriptorHeap(&heap_desc, internal.heap.iid(), internal.heap.put_void());
    device->CreateShaderResourceView(std::get<0>(texture), &srv_desc, internal.heap->GetCPUDescriptorHandleForHeapStart());
    return out_resource;
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
    case wis::DeviceFeature::WaitForPresent:
    case wis::DeviceFeature::DynamicVSync:
    case wis::DeviceFeature::AdvancedIndexBuffer:
    case wis::DeviceFeature::UnusedRenderTargets:
    case wis::DeviceFeature::PushDescriptors:
        return true;
    default:
        return false;
    }
}

wis::DX12DescriptorStorage
wis::ImplDX12Device::CreateDescriptorStorage(wis::Result& result, const wis::DescriptorStorageDesc& desc) const noexcept
{
    DX12DescriptorStorage out_storage;
    auto& internal = out_storage.GetMutableInternal();

    uint32_t size_resources = desc.cbuffer_count + desc.sbuffer_count + desc.stexture_count + desc.texture_count + desc.rbuffer_count;
    uint32_t size_samplers = desc.sampler_count;

    std::array<uint32_t, +wis::BindingIndex::Count> sizes{
        desc.sampler_count, desc.cbuffer_count, desc.texture_count, desc.stexture_count, desc.sbuffer_count, desc.rbuffer_count
    };

    D3D12_DESCRIPTOR_HEAP_DESC resource_heap_desc{
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        .NumDescriptors = size_resources,
        .Flags = convert_dx(desc.memory),
        .NodeMask = 0u
    };

    D3D12_DESCRIPTOR_HEAP_DESC sampler_heap_desc{
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
        .NumDescriptors = size_samplers,
        .Flags = convert_dx(desc.memory),
        .NodeMask = 0u
    };

    if (size_resources) {
        // create resource heap
        auto hr = device->CreateDescriptorHeap(&resource_heap_desc, __uuidof(*internal.heap_resource), internal.heap_resource.put_void());
        if (!wis::succeeded(hr)) {
            result = wis::make_result<FUNC, "Failed to create descriptor heap for resources">(hr);
            return out_storage;
        }

        internal.heap_resource_increment = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        internal.heap_starts[1] = internal.heap_resource->GetCPUDescriptorHandleForHeapStart();
        internal.heap_gpu_starts[0] = internal.heap_resource->GetGPUDescriptorHandleForHeapStart();

        for (uint32_t i = 2; i < sizes.size(); i++) { // first one is sampler
            internal.heap_starts[i] =
                    sizes[i]
                    ? CD3DX12_CPU_DESCRIPTOR_HANDLE(internal.heap_starts[i - 1], sizes[i - 1], internal.heap_resource_increment)
                    : CD3DX12_CPU_DESCRIPTOR_HANDLE();
        }
    }

    if (size_samplers) {
        // create sampler heap
        auto hr = device->CreateDescriptorHeap(&sampler_heap_desc, __uuidof(*internal.heap_sampler), internal.heap_sampler.put_void());
        if (!wis::succeeded(hr)) {
            result = wis::make_result<FUNC, "Failed to create descriptor heap for samplers">(hr);
            return out_storage;
        }
        internal.heap_sampler_increment = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
        internal.heap_starts[0] = internal.heap_sampler->GetCPUDescriptorHandleForHeapStart();
        internal.heap_gpu_starts[1] = internal.heap_sampler->GetGPUDescriptorHandleForHeapStart();
    }
    internal.device = device;

    return out_storage;
}

namespace wis::detail {
constexpr inline D3D12_ROOT_PARAMETER_TYPE to_dx(wis::DescriptorType type) noexcept
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
wis::ImplDX12Device::CreateRootSignature(wis::Result& result,
                                         const wis::PushConstant* constants,
                                         uint32_t constants_size,
                                         const PushDescriptor* push_descriptors,
                                         uint32_t push_descriptors_size,
                                         uint32_t space_overlap_count) const noexcept
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
    if (space_overlap_count > wis::max_descriptor_space_overlap) {
        result = wis::make_result<FUNC, "space_overlap_count exceeds max_descriptor_space_overlap">(E_INVALIDARG);
        return out_signature;
    }

    uint32_t push_constants_count = constants_size;
    uint32_t push_descriptors_count = push_descriptors_size;
    uint32_t space_overlap = space_overlap_count;
    constexpr static uint32_t tables_size = +wis::BindingIndex::Count;

    // max push constants + max push descriptors + max tables
    D3D12_ROOT_PARAMETER1 root_params[wis::max_push_constants + wis::max_push_descriptors + tables_size]{};

    // push constants
    std::array<int8_t, size_t(wis::ShaderStages::Count)> stage_map{};
    std::fill(stage_map.begin(), stage_map.end(), -1);

    for (uint32_t i = 0; i < push_constants_count; ++i) {
        auto& constant = constants[i];
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
    for (uint32_t i = 0; i < push_descriptors_count; ++i) {
        auto& descriptor = push_descriptors[i];
        root_params[i + push_constants_count] = {
            .ParameterType = detail::to_dx(descriptor.type),
            .Descriptor = {
                    .ShaderRegister = i,
                    .RegisterSpace = 0, // always 0 for push descriptors
            },
            .ShaderVisibility = convert_dx(descriptor.stage),
        };
    }

    //
    D3D12_DESCRIPTOR_RANGE1 memory[+wis::BindingIndex::Count][wis::max_descriptor_space_overlap]{};
    constexpr static D3D12_DESCRIPTOR_RANGE_TYPE types[+wis::BindingIndex::Count]{
        D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, // sampler
        D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, // cbuffer
        D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, // texture
        D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV, // stexture
        D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV, // sbuffer
        D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, // read only buffer
    };

    uint32_t spaces = 1; // 0 is reserved for push constants
    for (uint32_t i = 0; i < +wis::BindingIndex::Count; ++i) {
        for (uint32_t j = 0; j < space_overlap; ++j) {
            memory[i][j] = {
                .RangeType = types[i],
                .NumDescriptors = UINT32_MAX,
                .BaseShaderRegister = 0,
                .RegisterSpace = spaces++,
                .Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE, // always volatile for unbounded arrays
                .OffsetInDescriptorsFromTableStart = j ? 0 : D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND,
            };
        }

        root_params[i + push_constants_count + push_descriptors_count] = {
            .ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
            .DescriptorTable = {
                    .NumDescriptorRanges = space_overlap,
                    .pDescriptorRanges = memory[i],
            },
            .ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL,
        };
    }

    D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;
    desc.Init_1_1(push_constants_count + push_descriptors_count + tables_size, root_params, 0, nullptr, flags);

    wis::com_ptr<ID3DBlob> signature;
    wis::com_ptr<ID3DBlob> error;
    HRESULT hr = D3D12SerializeVersionedRootSignature(&desc, signature.put(), error.put());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to serialize root signature">(hr);
        return out_signature;
    }
    hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
                                     internal.root.iid(), internal.root.put_void());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to create root signature">(hr);
        return out_signature;
    }
    internal.stage_map = stage_map;
    internal.push_constant_count = push_constants_count;
    internal.push_descriptor_count = push_descriptors_count;

    return out_signature;
}

namespace wis::detail {
inline constexpr uint32_t allocate_space(
        D3D12_ROOT_PARAMETER1* root_params,
        D3D12_DESCRIPTOR_RANGE1* memory,
        uint32_t spaces,
        uint32_t base_space)
{
    spaces = std::min(spaces, 1u);
    for (uint32_t j = 0; j < spaces; ++j) {
        memory[j] = {
            .RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
            .NumDescriptors = UINT32_MAX,
            .BaseShaderRegister = 0,
            .RegisterSpace = base_space + j,
            .Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE,
            .OffsetInDescriptorsFromTableStart = j ? 0 : D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND,
        };
    }
    *root_params = {
        .ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        .DescriptorTable = {
                .NumDescriptorRanges = spaces,
                .pDescriptorRanges = memory,
        },
        .ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL,
    };
    return spaces;
}
} // namespace wis::detail

wis::DX12RootSignature
wis::ImplDX12Device::CreateRootSignature2(wis::Result& result, const wis::PushConstant* push_constants,
                                          uint32_t constants_count,
                                          const wis::PushDescriptor* push_descriptors,
                                          uint32_t push_descriptors_count,
                                          const wis::DescriptorSpacing* descriptor_spacing) const noexcept
{
    DX12RootSignature out_state;
    auto& internal = out_state.GetMutableInternal();

    if (constants_count > wis::max_push_constants) {
        result = wis::make_result<FUNC, "constants_count exceeds max_push_constants">(E_INVALIDARG);
        return out_state;
    }
    if (push_descriptors_count > wis::max_push_descriptors) {
        result = wis::make_result<FUNC, "push_descriptors_count exceeds max_push_descriptors">(E_INVALIDARG);
        return out_state;
    }
    wis::DescriptorSpacing default_spacing{};
    if (!descriptor_spacing) {
        descriptor_spacing = &default_spacing;
    }

    uint32_t push_constants_count = constants_count;
    constexpr static uint32_t tables_size = +wis::BindingIndex::Count;

    // max push constants + max push descriptors + max tables
    D3D12_ROOT_PARAMETER1 root_params[wis::max_push_constants + wis::max_push_descriptors + tables_size]{};

    // push constants
    std::array<int8_t, size_t(wis::ShaderStages::Count)> stage_map{};
    std::fill(stage_map.begin(), stage_map.end(), -1);

    for (uint32_t i = 0; i < push_constants_count; ++i) {
        auto& constant = push_constants[i];
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
    for (uint32_t i = 0; i < push_descriptors_count; ++i) {
        auto& descriptor = push_descriptors[i];
        root_params[i + push_constants_count] = {
            .ParameterType = detail::to_dx(descriptor.type),
            .Descriptor = {
                    .ShaderRegister = i,
                    .RegisterSpace = 0, // always 0 for push descriptors
            },
            .ShaderVisibility = convert_dx(descriptor.stage),
        };
    }

    //
    D3D12_DESCRIPTOR_RANGE1 memory[+wis::BindingIndex::Count][wis::max_descriptor_space_overlap]{};
    constexpr static D3D12_DESCRIPTOR_RANGE_TYPE types[+wis::BindingIndex::Count]{
        D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, // sampler
        D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, // cbuffer
        D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, // texture
        D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV, // stexture
        D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV, // sbuffer
        D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, // read only buffer
    };

    uint32_t base_space = 1; // 0 is reserved for push constants
    uint32_t allocated_spaces = 0; // 0 is reserved for push constants
    allocated_spaces = detail::allocate_space(&root_params[0 + push_constants_count + push_descriptors_count], memory[0], descriptor_spacing->sampler_count, base_space);
    allocated_spaces = detail::allocate_space(&root_params[1 + push_constants_count + push_descriptors_count], memory[1], descriptor_spacing->cbuffer_count, base_space += allocated_spaces);
    allocated_spaces = detail::allocate_space(&root_params[2 + push_constants_count + push_descriptors_count], memory[2], descriptor_spacing->texture_count, base_space += allocated_spaces);
    allocated_spaces = detail::allocate_space(&root_params[3 + push_constants_count + push_descriptors_count], memory[3], descriptor_spacing->stexture_count, base_space += allocated_spaces);
    allocated_spaces = detail::allocate_space(&root_params[4 + push_constants_count + push_descriptors_count], memory[4], descriptor_spacing->sbuffer_count, base_space += allocated_spaces);
    allocated_spaces = detail::allocate_space(&root_params[5 + push_constants_count + push_descriptors_count], memory[5], descriptor_spacing->rbuffer_count, base_space += allocated_spaces);

    D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;
    desc.Init_1_1(push_constants_count + push_descriptors_count + tables_size, root_params, 0, nullptr, flags);

    wis::com_ptr<ID3DBlob> signature;
    wis::com_ptr<ID3DBlob> error;
    HRESULT hr = D3D12SerializeVersionedRootSignature(&desc, signature.put(), error.put());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to serialize root signature">(hr);
        return out_state;
    }
    hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
                                     internal.root.iid(), internal.root.put_void());

    if (!wis::succeeded(hr)) {
        result = wis::make_result<FUNC, "Failed to create root signature">(hr);
        return out_state;
    }
    internal.stage_map = stage_map;
    internal.push_constant_count = push_constants_count;
    internal.push_descriptor_count = push_descriptors_count;
    return out_state;
}

#endif // !DX12_DEVICE_CPP
