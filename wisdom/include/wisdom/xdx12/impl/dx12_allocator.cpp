#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xdx12/dx12_allocator.h>
#endif // !WISDOM_HEADER_ONLY

[[nodiscard]] wis::ResultValue<wis::DX12Buffer>
wis::DX12ResourceAllocator::CreateBuffer(const D3D12MA::ALLOCATION_DESC& all_desc, const D3D12_RESOURCE_DESC& res_desc, D3D12_RESOURCE_STATES state) const noexcept
{
    wis::com_ptr<ID3D12Resource> rc;
    wis::com_ptr<D3D12MA::Allocation> al;
    HRESULT hr = allocator->CreateResource(&all_desc, &res_desc,
                                           state, nullptr,
                                           al.put(), __uuidof(*rc), rc.put_void());

    if (!wis::succeeded(hr))
        return wis::make_result<FUNC, "Buffer Allocation failed">(hr);

    return DX12Buffer{ std::move(rc), std::move(al) };
}
