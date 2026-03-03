#ifndef WIS_DX12_DESCRIPTOR_HEAP_CPP
#define WIS_DX12_DESCRIPTOR_HEAP_CPP

#include <wisdom/generated/dx12_api.h>
#include <wisdom/dx12/dx12_types.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyDescriptorHeap(WisDX12DescriptorHeap* self)
{
    auto& heap = *reinterpret_cast<wis::impl::DX12DescriptorHeapImpl*>(self);
    if (!heap.descriptor_heap) {
        return;
    }
    heap.descriptor_heap->Release();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void* wisDX12DescriptorHeapGetCPUHandle(const WisDX12DescriptorHeap* self)
{
    auto& heap = *reinterpret_cast<const wis::impl::DX12DescriptorHeapImpl*>(self);
    return reinterpret_cast<void*>(heap.descriptor_heap->GetCPUDescriptorHandleForHeapStart().ptr);
}

#endif // WIS_DX12_DESCRIPTOR_HEAP_CPP