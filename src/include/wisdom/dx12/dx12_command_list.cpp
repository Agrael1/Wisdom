#ifndef WIS_DX12_COMMAND_LIST_CPP
#define WIS_DX12_COMMAND_LIST_CPP

#include <wisdom/generated/dx12_cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <bit>

namespace wis::detail {
constexpr static uint32_t dx12_max_barrier_size = std::max({ sizeof(D3D12_BUFFER_BARRIER), sizeof(D3D12_TEXTURE_BARRIER), sizeof(D3D12_GLOBAL_BARRIER) });
constexpr static uint32_t dx12_static_size      = wis::TransientMaxBarrierCount * dx12_max_barrier_size;

inline uint8_t* DX12AllocateScratchSpace(const wis::impl::DX12CommandListImpl& impl, uint32_t new_size)
{
    if (new_size > impl.scratch_memory_size) {
        delete[] impl.scratch_memory;
        impl.scratch_memory = new (std::nothrow) uint8_t[new_size];
    }
    return impl.scratch_memory;
}
inline constexpr D3D12_BARRIER_LAYOUT
DX12GetOptimalBarrierLayout(WisCommandQueueType type, WisTextureState state) noexcept
{
    switch (type) {
    case WisCommandQueueTypeGraphics:
        switch (state) {
        case WisTextureStateCommon:
            return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COMMON;
        case WisTextureStateRead:
            return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_GENERIC_READ;
        case WisTextureStateUnorderedAccess:
            return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_UNORDERED_ACCESS;
        case WisTextureStateShaderResource:
            return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_SHADER_RESOURCE;
        case WisTextureStateCopySrc:
            return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COPY_SOURCE;
        case WisTextureStateCopyDst:
            return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COPY_DEST;
        default:
            return wis::detail::convert_dx(state);
        }
    case WisCommandQueueTypeCompute:
        switch (state) {
        case WisTextureStateCommon:
            return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_COMMON;
        case WisTextureStateRead:
            return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_GENERIC_READ;
        case WisTextureStateUnorderedAccess:
            return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_UNORDERED_ACCESS;
        case WisTextureStateShaderResource:
            return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_SHADER_RESOURCE;
        case WisTextureStateCopySrc:
            return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_COPY_SOURCE;
        case WisTextureStateCopyDst:
            return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_COPY_DEST;
        default:
            return wis::detail::convert_dx(state);
        }
    default:
        return wis::detail::convert_dx(state);
    }
}

inline std::array<wis::span<uint8_t>, 3>
DX12AllocateBarriers(const wis::impl::DX12CommandListImpl& impl,
                     uint8_t*                              local_scratch,
                     const WisDX12BarrierGroup&            barriers)
{
    std::array<wis::span<uint8_t>, 3> spans;
    std::size_t                       needed_size = barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER) +
            barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER) +
            barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER);

    if (needed_size <= dx12_static_size) {
        spans[0] = { local_scratch, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER) };
        spans[1] = { spans[0].end(), barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER) };
        spans[2] = { spans[1].end(), barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER) };
        return spans;
    }

    std::size_t sizes[] = { barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER),
                            barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER),
                            barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER),
                            0,
                            0,
                            0 };

    sizes[3] = sizes[0] + sizes[1];
    sizes[4] = sizes[1] + sizes[2];
    sizes[5] = sizes[0] + sizes[2];

    // find closest value from below
    uint32_t closest_size = 0;
    int      index        = -1;
    for (int i = std::size(sizes) - 1; i >= 0; --i) {
        if (sizes[i] > dx12_static_size) {
            continue;
        }

        // less than or equal to static size, check if it's the closest one
        if (dx12_static_size - sizes[i] < dx12_static_size - closest_size) {
            closest_size = sizes[i];
            index        = i;
        }
    }

    uint32_t allocated_size = needed_size - closest_size;

    // allocate from the command list's scratch memory if the needed size exceeds the local scratch buffer size. This is to avoid large stack allocations.
    auto* allocated_data = wis::detail::DX12AllocateScratchSpace(impl, allocated_size);

    // set pointers to the right offsets in the allocated scratch memory
    switch (index) {
    default:
    case -1:
        // no single group can fit into the local scratch, allocate all from the command list's scratch memory
        spans[0] = { allocated_data, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER) };
        spans[1] = { spans[0].end(), barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER) };
        spans[2] = { spans[1].end(), barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER) };
        return spans;
    case 0:
        // buffer barriers fit into local scratch, texture and global barriers allocated from command list's scratch
        spans[0] = { local_scratch, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER) };
        spans[1] = { allocated_data, barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER) };
        spans[2] = { spans[1].end(), barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER) };
        return spans;
    case 1:
        // texture barriers fit into local scratch, buffer and global barriers allocated from command list's scratch
        spans[0] = { allocated_data, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER) };
        spans[1] = { local_scratch, barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER) };
        spans[2] = { spans[0].end(), barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER) };
        return spans;
    case 2:
        // global barriers fit into local scratch, buffer and texture barriers allocated from command list's scratch
        spans[0] = { allocated_data, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER) };
        spans[1] = { spans[0].end(), barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER) };
        spans[2] = { local_scratch, barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER) };
        return spans;
    case 3:
        spans[0] = { local_scratch, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER) };
        spans[1] = { spans[0].end(), barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER) };
        spans[2] = { allocated_data, barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER) };
        return spans;
    case 4:
        spans[0] = { allocated_data, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER) };
        spans[1] = { local_scratch, barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER) };
        spans[2] = { spans[1].end(), barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER) };
        return spans;
    case 5:
        spans[0] = { local_scratch, barriers.buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER) };
        spans[1] = { allocated_data, barriers.texture_barrier_count * sizeof(D3D12_TEXTURE_BARRIER) };
        spans[2] = { spans[0].end(), barriers.global_barrier_count * sizeof(D3D12_GLOBAL_BARRIER) };
        return spans;
    }
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyCommandList(WisDX12CommandList* self)
{
    auto& impl = *reinterpret_cast<wis::impl::DX12CommandListImpl*>(self);
    if (!impl.list) {
        return;
    }

    impl.list->Release();
    impl.allocator->Release();
    delete[] impl.scratch_memory;
    impl.list = nullptr;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12CommandListBegin(const WisDX12CommandList* self)
{
    auto& impl = *reinterpret_cast<const wis::impl::DX12CommandListImpl*>(self);
    auto  hr   = impl.list->Reset(impl.allocator, nullptr);
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to reset command list for recording">(hr);
    }

    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12CommandListEnd(const WisDX12CommandList* self)
{
    auto& impl = *reinterpret_cast<const wis::impl::DX12CommandListImpl*>(self);
    auto  hr   = impl.list->Close();
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to reset command list for recording">(hr);
    }

    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetDescriptorHeaps(const WisDX12CommandList*    self,
                                                                  const WisDX12DescriptorHeap* resource_heap,
                                                                  const WisDX12DescriptorHeap* sampler_heap)
{
    auto& impl = *reinterpret_cast<const wis::impl::DX12CommandListImpl*>(self);

    uint32_t              heap_offset = (resource_heap == 0);
    uint32_t              heap_count  = (resource_heap != 0) + (sampler_heap != 0);
    ID3D12DescriptorHeap* heaps[]     = {
        resource_heap ? reinterpret_cast<const wis::impl::DX12DescriptorHeapImpl*>(resource_heap)->descriptor_heap : nullptr,
        sampler_heap ? reinterpret_cast<const wis::impl::DX12DescriptorHeapImpl*>(sampler_heap)->descriptor_heap : nullptr,
    };

    impl.descriptor_handle = resource_heap ? reinterpret_cast<const wis::impl::DX12DescriptorHeapImpl*>(resource_heap)->gpu_handle : D3D12_GPU_DESCRIPTOR_HANDLE{ 0 };
    impl.sampler_handle    = sampler_heap ? reinterpret_cast<const wis::impl::DX12DescriptorHeapImpl*>(sampler_heap)->gpu_handle : D3D12_GPU_DESCRIPTOR_HANDLE{ 0 };

    if (heap_count > 0) {
        impl.list->SetDescriptorHeaps(heap_count, heaps + heap_offset);
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetRootSignature(const WisDX12CommandList* self,
                                                                WisDX12RootSignatureView  signature,
                                                                WisPipelineType           pipeline)
{
    auto& impl = *reinterpret_cast<const wis::impl::DX12CommandListImpl*>(self);
    auto* sig  = std::bit_cast<ID3D12RootSignature*>(signature);

    switch (pipeline) {
    default:
    case WisPipelineTypeGraphics:
        impl.list->SetGraphicsRootSignature(sig);
        break;
    case WisPipelineTypeRayTracing:
    case WisPipelineTypeCompute:
        impl.list->SetComputeRootSignature(sig);
        break;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetPushConstants(const WisDX12CommandList*      self,
                                                                const WisPushConstantDataDesc* data)
{
    auto& impl = *reinterpret_cast<const wis::impl::DX12CommandListImpl*>(self);
    switch (data->pipeline) {
    default:
    case WisPipelineTypeGraphics:
        impl.list->SetGraphicsRoot32BitConstants(data->root_index, data->data_size / 4, data->data, data->push_offset / 4);
        break;
    case WisPipelineTypeRayTracing:
    case WisPipelineTypeCompute:
        impl.list->SetComputeRoot32BitConstants(data->root_index, data->data_size / 4, data->data, data->push_offset / 4);
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetPushDescriptor(const WisDX12CommandList*        self,
                                                                 const WisPushDescriptorDataDesc* data)
{
    auto& impl = *reinterpret_cast<const wis::impl::DX12CommandListImpl*>(self);

    switch (data->pipeline) {
    default:
    case WisPipelineTypeGraphics:
        switch (data->descriptor_type) {
        case WisDescriptorTypeConstantBuffer:
            impl.list->SetGraphicsRootConstantBufferView(data->root_index, data->buffer_address);
            break;
        case WisDescriptorTypeBuffer:
            impl.list->SetGraphicsRootShaderResourceView(data->root_index, data->buffer_address);
            break;
        case WisDescriptorTypeAccelerationStructure:
            impl.list->SetGraphicsRootShaderResourceView(data->root_index, data->buffer_address);
            break;
        case WisDescriptorTypeRWBuffer:
            impl.list->SetGraphicsRootUnorderedAccessView(data->root_index, data->buffer_address);
            break;
        default:
            break;
        }
        break;
    case WisPipelineTypeRayTracing:
    case WisPipelineTypeCompute:
        switch (data->descriptor_type) {
        case WisDescriptorTypeConstantBuffer:
            impl.list->SetComputeRootConstantBufferView(data->root_index, data->buffer_address);
            break;
        case WisDescriptorTypeBuffer:
            impl.list->SetComputeRootShaderResourceView(data->root_index, data->buffer_address);
            break;
        case WisDescriptorTypeAccelerationStructure:
            impl.list->SetComputeRootShaderResourceView(data->root_index, data->buffer_address);
            break;
        case WisDescriptorTypeRWBuffer:
            impl.list->SetComputeRootUnorderedAccessView(data->root_index, data->buffer_address);
            break;
        default:
            break;
        }
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetDescriptorTable(const WisDX12CommandList*         self,
                                                                  const WisDescriptorTableDataDesc* data)
{
    auto& impl   = *reinterpret_cast<const wis::impl::DX12CommandListImpl*>(self);
    auto  handle = (data->heap_type == WisDescriptorHeapTypeSampler) ? impl.sampler_handle : impl.descriptor_handle;
    auto  stride = (data->heap_type == WisDescriptorHeapTypeSampler) ? impl.sampler_size : impl.descriptor_size;
    switch (data->pipeline) {
    default:
    case WisPipelineTypeGraphics:
        impl.list->SetGraphicsRootDescriptorTable(data->root_index, { handle.ptr + data->heap_offset * stride });
        break;
    case WisPipelineTypeRayTracing:
    case WisPipelineTypeCompute:
        impl.list->SetComputeRootDescriptorTable(data->root_index, { handle.ptr + data->heap_offset * stride });
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListInsertBarriers(const WisDX12CommandList*  self,
                                                              const WisDX12BarrierGroup* barriers)
{
    // clang-format off
    if (barriers->buffer_barrier_count +
        barriers->texture_barrier_count +
        barriers->global_barrier_count == 0) {
        return;
    }
    // clang-format on

    auto& impl = *reinterpret_cast<const wis::impl::DX12CommandListImpl*>(self);

    constexpr static uint32_t max_barrier_size = std::max(sizeof(D3D12_BUFFER_BARRIER), sizeof(D3D12_TEXTURE_BARRIER));
    constexpr static uint32_t static_size      = static_cast<uint32_t>(wis::TransientMaxBarrierCount * max_barrier_size);
    uint8_t                   local_scratch[static_size]{};

    auto [buffer_span, texture_span, global_span] = wis::detail::DX12AllocateBarriers(impl, local_scratch, *barriers);

    wis::span<D3D12_BUFFER_BARRIER> buffer_barriers_span{ reinterpret_cast<D3D12_BUFFER_BARRIER*>(buffer_span.data()), barriers->buffer_barrier_count };
    uint32_t                        real_buffer_barrier_count = barriers->buffer_barrier_count;
    // convert buffer barriers
    for (size_t i = 0; i < barriers->buffer_barrier_count; ++i) {
        auto& src = barriers->buffer_barriers[i];

        // skip barriers that only perform queue ownership transfer without any actual synchronization or access changes,
        // as they don't require an explicit barrier in D3D12 and can be handled implicitly by the driver.
        if (src.queue_type_after != src.queue_type_before) {
            real_buffer_barrier_count--;
            continue;
        }

        buffer_barriers_span[i] = D3D12_BUFFER_BARRIER{
            .SyncBefore   = wis::detail::convert_dx(src.sync_before),
            .SyncAfter    = wis::detail::convert_dx(src.sync_after),
            .AccessBefore = wis::detail::convert_dx(src.access_before),
            .AccessAfter  = wis::detail::convert_dx(src.access_after),
            .pResource    = std::bit_cast<ID3D12Resource*>(src.buffer),
            .Offset       = src.offset,
            .Size         = src.size,
        };
    }

    // convert texture barriers
    wis::span<D3D12_TEXTURE_BARRIER> texture_barriers_span{
        reinterpret_cast<D3D12_TEXTURE_BARRIER*>(texture_span.data()),
        barriers->texture_barrier_count
    };
    for (size_t i = 0; i < barriers->texture_barrier_count; ++i) {
        auto& src = barriers->texture_barriers[i];

        bool qfot_barrier    = src.queue_type_after != src.queue_type_before;
        bool acquire_barrier = qfot_barrier && src.queue_type_after == impl.queue_type;
        bool release_barrier = qfot_barrier && src.queue_type_before == impl.queue_type;

        auto layout_before = src.flags & WisBarrierFlagsDiscardContent
                ? D3D12_BARRIER_LAYOUT_UNDEFINED
                : wis::detail::DX12GetOptimalBarrierLayout(impl.queue_type, acquire_barrier ? src.state_before : WisTextureStateCommon);
        auto layout_after  = wis::detail::DX12GetOptimalBarrierLayout(
                impl.queue_type,
                release_barrier ? src.state_after : WisTextureStateCommon);

        texture_barriers_span[i] = D3D12_TEXTURE_BARRIER{
            .SyncBefore   = wis::detail::convert_dx(src.sync_before),
            .SyncAfter    = wis::detail::convert_dx(src.sync_after),
            .AccessBefore = wis::detail::convert_dx(src.access_before),
            .AccessAfter  = wis::detail::convert_dx(src.access_after),
            .LayoutBefore = layout_before,
            .LayoutAfter  = layout_after,
            .pResource    = std::bit_cast<ID3D12Resource*>(src.texture),
            .Subresources = {
                             .IndexOrFirstMipLevel = src.subresource_range.base_mip_level,
                             .NumMipLevels         = src.subresource_range.mip_level_count,
                             .FirstArraySlice      = src.subresource_range.base_array_layer,
                             .NumArraySlices       = src.subresource_range.array_layer_count,
                             .FirstPlane           = src.flags & WisBarrierFlagsPlanarImage ? src.subresource_range.plane_slice : 0u,
                             .NumPlanes            = src.flags & WisBarrierFlagsPlanarImage ? src.subresource_range.plane_slice_count : 1u,
                             },
            .Flags = src.flags & WisBarrierFlagsDiscardContent ? D3D12_TEXTURE_BARRIER_FLAG_DISCARD : D3D12_TEXTURE_BARRIER_FLAG_NONE,
        };
    }

    // convert global barriers
    wis::span<D3D12_GLOBAL_BARRIER> global_barriers_span{
        reinterpret_cast<D3D12_GLOBAL_BARRIER*>(global_span.data()),
        barriers->global_barrier_count
    };
    for (size_t i = 0; i < barriers->global_barrier_count; ++i) {
        auto& src               = barriers->global_barriers[i];
        global_barriers_span[i] = D3D12_GLOBAL_BARRIER{
            .SyncBefore   = wis::detail::convert_dx(src.sync_before),
            .SyncAfter    = wis::detail::convert_dx(src.sync_after),
            .AccessBefore = wis::detail::convert_dx(src.access_before),
            .AccessAfter  = wis::detail::convert_dx(src.access_after),
        };
    }

    D3D12_BARRIER_GROUP groups[]{
        {   .Type            = D3D12_BARRIER_TYPE_BUFFER,
         .NumBarriers     = real_buffer_barrier_count,
         .pBufferBarriers = buffer_barriers_span.data()  },
        { .Type             = D3D12_BARRIER_TYPE_TEXTURE,
         .NumBarriers      = static_cast<uint32_t>(barriers->texture_barrier_count),
         .pTextureBarriers = texture_barriers_span.data() },
        {   .Type            = D3D12_BARRIER_TYPE_GLOBAL,
         .NumBarriers     = static_cast<uint32_t>(barriers->global_barrier_count),
         .pGlobalBarriers = global_barriers_span.data()  }
    };
    impl.list->Barrier(std::size(groups), groups);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetPipeline(const WisDX12CommandList* self,
                                                           WisDX12PipelineView       pipeline,
                                                           WisPipelineType           type)
{
    auto& impl = *reinterpret_cast<const wis::impl::DX12CommandListImpl*>(self);
    auto* pipe = std::bit_cast<ID3D12PipelineState*>(pipeline);
    impl.list->SetPipelineState(pipe);
}

WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetViewports(WisDX12CommandList* self,
                                                            const WisViewport*  viewports,
                                                            size_t              count)
{
    auto&          impl      = *reinterpret_cast<wis::impl::DX12CommandListImpl*>(self);
    auto           max_count = std::min(count, static_cast<size_t>(D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE));
    D3D12_VIEWPORT dx_viewports[D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

    for (size_t i = 0; i < max_count; ++i) {
        dx_viewports[i] = {
            .TopLeftX = viewports[i].top_leftx,
            .TopLeftY = viewports[i].top_lefty,
            .Width    = viewports[i].width,
            .Height   = viewports[i].height,
            .MinDepth = viewports[i].min_depth,
            .MaxDepth = viewports[i].max_depth,
        };
    }
    impl.list->RSSetViewports(static_cast<UINT>(count), dx_viewports);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetScissors(WisDX12CommandList* self,
                                                           const WisScissor*   scissors,
                                                           size_t              count)
{
    auto&      impl = *reinterpret_cast<wis::impl::DX12CommandListImpl*>(self);
    D3D12_RECT dx_scissors[D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    auto       max_count = std::min(count, static_cast<size_t>(D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE));
    for (size_t i = 0; i < max_count; ++i) {
        dx_scissors[i] = {
            .left   = scissors[i].left,
            .top    = scissors[i].top,
            .right  = scissors[i].right,
            .bottom = scissors[i].bottom,
        };
    }

    impl.list->RSSetScissorRects(static_cast<UINT>(count), dx_scissors);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetPrimitiveTopology(WisDX12CommandList*  self,
                                                                    WisPrimitiveTopology topology)
{
    auto& impl = *reinterpret_cast<wis::impl::DX12CommandListImpl*>(self);
    impl.list->IASetPrimitiveTopology(wis::detail::convert_dx(topology));
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetDepthBias(WisDX12CommandList* self,
                                                            float               depth_bias,
                                                            float               depth_bias_clamp,
                                                            float               slope_scaled_depth_bias)
{
    auto& impl = *reinterpret_cast<wis::impl::DX12CommandListImpl*>(self);
    impl.list->RSSetDepthBias(depth_bias, depth_bias_clamp, slope_scaled_depth_bias);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetPrimitiveRestartValue(WisDX12CommandList* self,
                                                                        WisPrimitiveRestartValue value)
{
    auto& impl = *reinterpret_cast<wis::impl::DX12CommandListImpl*>(self);
    impl.list->IASetIndexBufferStripCutValue(wis::detail::convert_dx(value));
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListDispatch(const WisDX12CommandList* self,
                                                        uint32_t                  group_count_x,
                                                        uint32_t                  group_count_y,
                                                        uint32_t                  group_count_z)
{
    auto& impl = *reinterpret_cast<const wis::impl::DX12CommandListImpl*>(self);
    impl.list->Dispatch(group_count_x, group_count_y, group_count_z);
}

#endif // WIS_DX12_COMMAND_LIST_CPP