#pragma once
#include <wisdom/api/internal.h>
#include <wisdom/api/consts.h>
#include <wisdom/dx12/xdx12_views.h>
#include <wisdom/util/com_ptr.h>
#include <D3D12MemAlloc.h>

namespace wis {
class DX12Buffer;

template<>
struct Internal<DX12Buffer> {
    wis::com_ptr<ID3D12Resource> resource;
    wis::com_ptr<D3D12MA::Allocation> allocation;
};

class DX12Buffer : public QueryInternal<DX12Buffer>
{
public:
    DX12Buffer()noexcept = default;
    explicit DX12Buffer(wis::com_ptr<ID3D12Resource> rc, wis::com_ptr<D3D12MA::Allocation> al) noexcept
        : QueryInternal(std::move(rc), std::move(al)) { }
    operator bool() const noexcept { return bool(resource); }

public:

};

using DX12Texture = DX12Buffer;

} // namespace wis