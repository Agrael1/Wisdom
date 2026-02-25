#ifndef WIS_DX12_COMMAND_LIST_CPP
#define WIS_DX12_COMMAND_LIST_CPP

#include <wisdom/generated/dx12_cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyCommandList(WisDX12CommandList* self)
{
    auto& [list, all] = *reinterpret_cast<DX12CommandListImpl*>(self);
    if (!list) {
        return;
    }

    list->Release();
    all->Release();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12CommandListBegin(const WisDX12CommandList* self)
{
    auto& [list, all] = *reinterpret_cast<const DX12CommandListImpl*>(self);
    auto hr           = list->Reset(all, nullptr);
    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to reset command list for recording">(hr);
    }

    return dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12CommandListEnd(const WisDX12CommandList* self)
{
    auto& [list, all] = *reinterpret_cast<const DX12CommandListImpl*>(self);
    auto hr           = list->Close();
    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to reset command list for recording">(hr);
    }

    return dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12CommandListBindDescriptorHeaps(const WisDX12CommandList*    self,
                                                                   const WisDX12DescriptorHeap* resource_heap,
                                                                   const WisDX12DescriptorHeap* sampler_heap)
{
    auto& [list, all] = *reinterpret_cast<const DX12CommandListImpl*>(self);

    uint32_t              heap_offset = (resource_heap == 0);
    uint32_t              heap_count = (resource_heap != 0) + (sampler_heap != 0);
    ID3D12DescriptorHeap* heaps[] = {
        resource_heap ? reinterpret_cast<const DX12DescriptorHeapImpl*>(resource_heap)->descriptor_heap : nullptr,
        sampler_heap ? reinterpret_cast<const DX12DescriptorHeapImpl*>(sampler_heap)->descriptor_heap : nullptr,
    };

    if (heap_count > 0) {
        list->SetDescriptorHeaps(heap_count, heaps + heap_offset);
    }
}

#endif // WIS_DX12_COMMAND_LIST_CPP