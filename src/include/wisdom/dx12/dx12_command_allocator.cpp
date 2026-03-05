#ifndef WIS_DX12_COMMAND_ALLOCATOR_CPP
#define WIS_DX12_COMMAND_ALLOCATOR_CPP

#include <wisdom/generated/dx12_cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>




//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyCommandAllocator(WisDX12CommandAllocator* self)
{
    auto& [al, x1, x2] = *reinterpret_cast<wis::impl::DX12CommandAllocatorImpl*>(self);
    if (!al) {
        return;
    }

    al->Release();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12CommandAllocatorReset(const WisDX12CommandAllocator* self)
{
    auto& [allocator, device, type] = *reinterpret_cast<const wis::impl::DX12CommandAllocatorImpl*>(self);
    auto hr                         = allocator->Reset();
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to reset command allocator">(hr);
    }
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12CommandAllocatorCreateCommandList(const WisDX12CommandAllocator* self,
                                                                           WisDX12CommandList*            list)
{
    auto& [allocator, device, type] = *reinterpret_cast<const wis::impl::DX12CommandAllocatorImpl*>(self);

    wis::com_ptr<ID3D12GraphicsCommandList7> command_list;

    auto hr = device->CreateCommandList1(0,
                                         type,
                                         D3D12_COMMAND_LIST_FLAG_NONE,
                                         IID_ID3D12GraphicsCommandList7,
                                         command_list.put_void_unchecked());

    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create command list">(hr);
    }

    auto& internal     = *new (list) wis::impl::DX12CommandListImpl();
    internal.list      = command_list.detach();
    internal.allocator = allocator;
    internal.allocator->AddRef();
    internal.descriptor_handle.ptr = 0;
    internal.sampler_handle.ptr    = 0;
    internal.descriptor_size       = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    internal.sampler_size          = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

    return wis::detail::dx_success;
}

#endif // WIS_DX12_COMMAND_ALLOCATOR_CPP