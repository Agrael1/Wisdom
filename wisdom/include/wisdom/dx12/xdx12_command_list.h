#pragma once
#include <wisdom/api/consts.h>
#include <wisdom/api/internal.h>
#include <wisdom/dx12/xdx12_views.h>
#include <wisdom/util/com_ptr.h>

namespace wis {
class DX12CommandList;

template<>
struct Internal<DX12CommandList> {
    wis::com_ptr<ID3D12CommandAllocator> allocator;
    wis::com_ptr<ID3D12CommandList> list;
};

class DX12CommandList : public QueryInternal<DX12CommandList>
{
public:
    DX12CommandList() noexcept = default;
    DX12CommandList(wis::com_ptr<ID3D12CommandAllocator> allocator,
                    wis::com_ptr<ID3D12CommandList> list) noexcept
        : QueryInternal(std::move(allocator), std::move(list))
    {
    }
    operator bool() const noexcept
    {
        return bool(list);
    }
    operator DX12CommandListView() const noexcept
    {
        return { list.get() };
    }

public:

};
} // namespace wis