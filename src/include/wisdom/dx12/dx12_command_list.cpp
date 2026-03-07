#ifndef WIS_DX12_COMMAND_LIST_CPP
#define WIS_DX12_COMMAND_LIST_CPP

#include <wisdom/generated/dx12_cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <bit>

namespace wis::detail {
inline uint8_t* DX12AllocateScratchSpace(const wis::impl::DX12CommandListImpl& impl, uint32_t new_size)
{
    if (new_size > impl.scratch_memory_size) {
        delete[] impl.scratch_memory;
        impl.scratch_memory = new (std::nothrow) uint8_t[new_size];
    }
    return impl.scratch_memory;
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
        case WisDescriptorTypeRWBuffer:
            impl.list->SetComputeRootShaderResourceView(data->root_index, data->buffer_address);
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
    if (barriers->buffer_barrier_count == 0) {
        return;
    }

    auto& impl = *reinterpret_cast<const wis::impl::DX12CommandListImpl*>(self);

    constexpr static uint32_t max_barrier_size = std::max(sizeof(D3D12_BUFFER_BARRIER), sizeof(D3D12_TEXTURE_BARRIER));
    constexpr static uint32_t static_size      = static_cast<uint32_t>(wis::TransientMaxBarrierCount * max_barrier_size);
    uint8_t                   local_scratch[static_size]{};
    uint8_t*                  real_data = local_scratch;

    uint32_t needed_size = static_cast<uint32_t>(barriers->buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER));
    if (needed_size > static_size) {
        // allocate from the command list's scratch memory if the needed size exceeds the local scratch buffer size. This is to avoid large stack allocations.
        real_data = wis::detail::DX12AllocateScratchSpace(impl, needed_size);
    }

    auto* buffer_barriers = local_scratch;

    wis::span<D3D12_BUFFER_BARRIER> buffer_barriers_span{ reinterpret_cast<D3D12_BUFFER_BARRIER*>(buffer_barriers), barriers->buffer_barrier_count };
    uint32_t                        real_buffer_barrier_count = barriers->buffer_barrier_count;
    // convert buffer barriers
    for (size_t i = 0; i < barriers->buffer_barrier_count; ++i) {
        auto& src = barriers->buffer_barriers[i];

        // skip barriers that only perform queue ownership transfer without any actual synchronization or access changes,
        // as they don't require an explicit barrier in D3D12 and can be handled implicitly by the driver.
        // But the acquisition barrier must be submitted
        if (src.queue_type_after != src.queue_type_before && src.queue_type_before == impl.queue_type) {
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

    uint32_t memory_offset = barriers->buffer_barrier_count * sizeof(D3D12_BUFFER_BARRIER);

    D3D12_BARRIER_GROUP groups[]{
        {
         .Type            = D3D12_BARRIER_TYPE_BUFFER,
         .NumBarriers     = real_buffer_barrier_count,
         .pBufferBarriers = buffer_barriers_span.data(),
         },
        {
         .Type             = D3D12_BARRIER_TYPE_TEXTURE,
         .NumBarriers      = 0,
         .pTextureBarriers = nullptr,
         }
    };
    uint32_t group_count  = barriers->buffer_barrier_count != 0;
    uint32_t group_offset = barriers->buffer_barrier_count == 0;

    impl.list->Barrier(group_count, groups + group_offset);
}

#endif // WIS_DX12_COMMAND_LIST_CPP