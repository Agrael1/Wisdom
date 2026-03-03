#ifndef WIS_DX12_COMMAND_LIST_CPP
#define WIS_DX12_COMMAND_LIST_CPP

#include <wisdom/generated/dx12_cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <bit>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyCommandList(WisDX12CommandList* self)
{
    auto& impl = *reinterpret_cast<DX12CommandListImpl*>(self);
    if (!impl.list) {
        return;
    }

    impl.list->Release();
    impl.allocator->Release();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12CommandListBegin(const WisDX12CommandList* self)
{
    auto& impl = *reinterpret_cast<const DX12CommandListImpl*>(self);
    auto  hr   = impl.list->Reset(impl.allocator, nullptr);
    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to reset command list for recording">(hr);
    }

    return dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12CommandListEnd(const WisDX12CommandList* self)
{
    auto& impl = *reinterpret_cast<const DX12CommandListImpl*>(self);
    auto  hr   = impl.list->Close();
    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to reset command list for recording">(hr);
    }

    return dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetDescriptorHeaps(const WisDX12CommandList*    self,
                                                                  const WisDX12DescriptorHeap* resource_heap,
                                                                  const WisDX12DescriptorHeap* sampler_heap)
{
    auto& impl = *reinterpret_cast<const DX12CommandListImpl*>(self);

    uint32_t              heap_offset = (resource_heap == 0);
    uint32_t              heap_count  = (resource_heap != 0) + (sampler_heap != 0);
    ID3D12DescriptorHeap* heaps[]     = {
        resource_heap ? reinterpret_cast<const DX12DescriptorHeapImpl*>(resource_heap)->descriptor_heap : nullptr,
        sampler_heap ? reinterpret_cast<const DX12DescriptorHeapImpl*>(sampler_heap)->descriptor_heap : nullptr,
    };

    impl.descriptor_handle = resource_heap ? reinterpret_cast<const DX12DescriptorHeapImpl*>(resource_heap)->gpu_handle : D3D12_GPU_DESCRIPTOR_HANDLE{ 0 };
    impl.sampler_handle    = sampler_heap ? reinterpret_cast<const DX12DescriptorHeapImpl*>(sampler_heap)->gpu_handle : D3D12_GPU_DESCRIPTOR_HANDLE{ 0 };

    if (heap_count > 0) {
        impl.list->SetDescriptorHeaps(heap_count, heaps + heap_offset);
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListSetRootSignature(const WisDX12CommandList* self,
                                                                WisDX12RootSignatureView  signature,
                                                                WisPipelineType           pipeline)
{
    auto& impl = *reinterpret_cast<const DX12CommandListImpl*>(self);
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
    auto& impl = *reinterpret_cast<const DX12CommandListImpl*>(self);
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
    auto& impl = *reinterpret_cast<const DX12CommandListImpl*>(self);

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
    auto& impl   = *reinterpret_cast<const DX12CommandListImpl*>(self);
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

#endif // WIS_DX12_COMMAND_LIST_CPP