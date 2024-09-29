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

class ImplDX12Memory : public QueryInternal<DX12Memory>
{
public:
    ImplDX12Memory() noexcept = default;
    explicit ImplDX12Memory(wis::com_ptr<D3D12MA::Allocator> allocator, wis::com_ptr<D3D12MA::Allocation> allocation) noexcept
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
    operator==(const ImplDX12Memory& other) const noexcept
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

#pragma region DX12Memory
/**
 * @brief Represents memory object for binding resources.
 * */
struct DX12Memory : public wis::ImplDX12Memory {
public:
    using wis::ImplDX12Memory::ImplDX12Memory;

public:
    /**
     * @brief Returns the offset of the block in the global memory.
     * @return The offset of the block in the global memory.
     * */
    inline uint64_t GetBlockOffset() const noexcept
    {
        return wis::ImplDX12Memory::GetBlockOffset();
    }
};
#pragma endregion DX12Memory

} // namespace wis

#endif // !VK_MEMORY_H
