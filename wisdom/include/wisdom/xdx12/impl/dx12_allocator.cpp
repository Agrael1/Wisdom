#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xdx12/dx12_allocator.h>
#endif // !WISDOM_HEADER_ONLY

#include <d3dx12/d3dx12_core.h>

wis::ResultValue<wis::DX12Buffer>
wis::DX12ResourceAllocator::CreateBuffer(const D3D12MA::ALLOCATION_DESC& all_desc, const D3D12_RESOURCE_DESC1& res_desc, D3D12_RESOURCE_STATES state) const noexcept
{
    wis::com_ptr<ID3D12Resource> rc;
    wis::com_ptr<D3D12MA::Allocation> al;


    HRESULT hr = allocator->CreateResource2(&all_desc, &res_desc,
                                           state, nullptr,
                                           al.put(), __uuidof(*rc), rc.put_void());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Buffer Allocation failed">(hr);

    return DX12Buffer{ std::move(rc), std::move(al) };
}

wis::ResultValue<wis::DX12Buffer>
wis::DX12ResourceAllocator::CreateCommitedBuffer(size_t size, BufferFlags) const noexcept
{
    return CreateBuffer({ .HeapType = D3D12_HEAP_TYPE_DEFAULT }, CD3DX12_RESOURCE_DESC1::Buffer(size), D3D12_RESOURCE_STATE_COMMON);
}

wis::ResultValue<wis::DX12Buffer>
wis::DX12ResourceAllocator::CreateUploadBuffer(size_t size) const noexcept
{
    return CreateBuffer({ .HeapType = D3D12_HEAP_TYPE_UPLOAD }, CD3DX12_RESOURCE_DESC1::Buffer(size), D3D12_RESOURCE_STATE_GENERIC_READ);
}
