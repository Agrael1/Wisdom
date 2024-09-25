#ifndef WIS_DX12_MEMORY_H
#define WIS_DX12_MEMORY_H
#include <wisdom/global/internal.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/util/com_ptr.h>
#include <D3D12MemAlloc.h>

namespace wis {
class DX12Memory;

template<>
struct Internal<DX12Memory> {
    wis::com_ptr<D3D12MA::Allocator> allocator;
    wis::com_ptr<D3D12MA::Allocation> allocation;

    Internal() noexcept = default;
    Internal(wis::com_ptr<D3D12MA::Allocator> allocator, wis::com_ptr<D3D12MA::Allocation> allocation) noexcept
        : allocator(std::move(allocator)), allocation(std::move(allocation)) { }
};

class DX12Memory : public QueryInternal<DX12Memory>
{
public:
    DX12Memory() noexcept = default;
    explicit DX12Memory(wis::com_ptr<D3D12MA::Allocator> allocator, wis::com_ptr<D3D12MA::Allocation> allocation) noexcept
        : QueryInternal<DX12Memory>(std::move(allocator), std::move(allocation))
    {
    }
    operator bool() const noexcept
    {
        return bool(allocation);
    }
    operator DX12MemoryView() const noexcept
    {
        return { allocator.get(), allocation.get() };
    }
    [[nodiscard]] bool
    operator==(std::nullptr_t) const noexcept
    {
        return !allocation;
    }
    [[nodiscard]] bool
    operator==(const DX12Memory& other) const noexcept
    {
        return allocation.get() == other.allocation.get();
    }

public:
    [[nodiscard]] uint64_t GetBlockOffset() const noexcept
    {
        if (!allocation)
            return 0;

        return allocation->GetOffset();
    }
};

} // namespace wis

#endif // !VK_MEMORY_H
