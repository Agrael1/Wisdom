#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/generated/api/api.h>
#include <wisdom/xdx12/dx12_views.h>
#include <wisdom/util/com_ptr.h>

namespace wis {
class DX12CommandList;
struct DX12BufferBarrier2;
struct DX12TextureBarrier2;

template<>
struct Internal<DX12CommandList> {
    wis::com_ptr<ID3D12CommandAllocator> allocator;
    wis::com_ptr<ID3D12GraphicsCommandList9> list;
};

class DX12CommandList : public QueryInternal<DX12CommandList>
{
public:
    DX12CommandList() noexcept = default;
    explicit DX12CommandList(wis::com_ptr<ID3D12CommandAllocator> allocator,
                             wis::com_ptr<ID3D12GraphicsCommandList9> list) noexcept
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
    bool Closed() const noexcept { return closed; }
    WIS_INLINE bool Close() noexcept;
    [[nodiscard]] WIS_INLINE wis::Result Reset(DX12PipelineHandle pipeline = nullptr) noexcept;
    WIS_INLINE void CopyBuffer(DX12BufferView source, DX12BufferView destination, wis::BufferRegion region) const noexcept;

    WIS_INLINE void BufferBarrier(wis::BufferBarrier barrier, DX12BufferView buffer) noexcept;
    // 8 buffers at once max for efficiency
    WIS_INLINE void BufferBarriers(wis::DX12BufferBarrier2* barriers, uint32_t barrier_count) noexcept;

    WIS_INLINE void TextureBarrier(wis::TextureBarrier barrier, DX12TextureView texture) noexcept;
    // 8 textures at once max for efficiency
    WIS_INLINE void TextureBarriers(wis::DX12TextureBarrier2* barrier, uint32_t barrier_count) noexcept;

protected:
    bool closed = false;
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/dx12_command_list.cpp"
#endif // !WISDOM_HEADER_ONLY