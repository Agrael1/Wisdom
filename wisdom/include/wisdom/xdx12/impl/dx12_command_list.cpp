#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/xdx12/dx12_command_list.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/xdx12/dx12_checks.h>
#include <wisdom/generated/dx12/dx12_structs.hpp>
#include <wisdom/util/small_allocator.h>

void wis::DX12CommandList::CopyBuffer(DX12BufferView source, DX12BufferView destination, wis::BufferRegion region) const noexcept
{
    list->CopyBufferRegion(std::get<0>(destination), region.dst_offset, std::get<0>(source), region.src_offset, region.size_bytes);
}

wis::Result wis::DX12CommandList::Reset(wis::DX12PipelineHandle pipeline) noexcept
{
    Close();
    auto hr = allocator->Reset();
    if (!wis::succeeded(hr)) {
        return wis::make_result<FUNC, "Reset failed (allocator)">(hr);
    }
    hr = list->Reset(allocator.get(), std::get<0>(pipeline));
    return wis::succeeded(hr) ? wis::success : wis::make_result<FUNC, "Reset failed (command list)">(hr);
}

bool wis::DX12CommandList::Close() noexcept
{
    if (closed) {
        return true;
    }
    return closed = succeeded(list->Close());
}

namespace wis::detail {
inline D3D12_BUFFER_BARRIER to_dx(wis::BufferBarrier barrier, ID3D12Resource* buffer) noexcept
{
    return D3D12_BUFFER_BARRIER{
        .SyncBefore = convert_dx(barrier.sync_before),
        .SyncAfter = convert_dx(barrier.sync_after),
        .AccessBefore = convert_dx(barrier.access_before),
        .AccessAfter = convert_dx(barrier.access_after),
        .pResource = buffer,
        .Offset = barrier.offset,
        .Size = barrier.size
    };
}
inline D3D12_TEXTURE_BARRIER to_dx(wis::TextureBarrier barrier, ID3D12Resource* buffer) noexcept
{
    auto& subresource = barrier.subresource_range;
    return D3D12_TEXTURE_BARRIER{
        .SyncBefore = convert_dx(barrier.sync_before),
        .SyncAfter = convert_dx(barrier.sync_after),
        .AccessBefore = convert_dx(barrier.access_before),
        .AccessAfter = convert_dx(barrier.access_after),
        .LayoutBefore = convert_dx(barrier.state_before),
        .LayoutAfter = convert_dx(barrier.state_after),
        .pResource = buffer,
        .Subresources = {
                .IndexOrFirstMipLevel = subresource.base_mip_level,
                .NumMipLevels = subresource.level_count,
                .FirstArraySlice = subresource.base_array_layer,
                .NumArraySlices = subresource.layer_count,
                .FirstPlane = 0,
                .NumPlanes = 1 }
    };
}
} // namespace wis::detail

void wis::DX12CommandList::BufferBarrier(wis::BufferBarrier barrier, DX12BufferView buffer) noexcept
{
    D3D12_BUFFER_BARRIER bb = detail::to_dx(barrier, std::get<0>(buffer));
    D3D12_BARRIER_GROUP bg{
        .Type = D3D12_BARRIER_TYPE_BUFFER,
        .NumBarriers = 1,
        .pBufferBarriers = &bb
    };
    list->Barrier(1, &bg);
}

void wis::DX12CommandList::BufferBarriers(wis::DX12BufferBarrier2* barriers, uint32_t barrier_count) noexcept
{
    wis::detail::limited_allocator<D3D12_BUFFER_BARRIER, 8> allocator(barrier_count, true);
    auto* data = allocator.data();

    for (size_t i = 0; i < barrier_count; i++) {
        data[i] = detail::to_dx(barriers[i].barrier, std::get<0>(barriers[i].buffer));
    }

    D3D12_BARRIER_GROUP bg{
        .Type = D3D12_BARRIER_TYPE_BUFFER,
        .NumBarriers = barrier_count,
        .pBufferBarriers = data
    };
    list->Barrier(1, &bg);
}

void wis::DX12CommandList::TextureBarrier(wis::TextureBarrier barrier, DX12TextureView texture) noexcept
{
    D3D12_TEXTURE_BARRIER tb = detail::to_dx(barrier, std::get<0>(texture));
    D3D12_BARRIER_GROUP bg{
        .Type = D3D12_BARRIER_TYPE_TEXTURE,
        .NumBarriers = 1,
        .pTextureBarriers = &tb
    };
    list->Barrier(1, &bg);
}
void wis::DX12CommandList::TextureBarriers(wis::DX12TextureBarrier2* barrier, uint32_t barrier_count) noexcept
{
    wis::detail::limited_allocator<D3D12_TEXTURE_BARRIER, 8> allocator(barrier_count, true);
    auto* data = allocator.data();

    for (size_t i = 0; i < barrier_count; i++) {
        data[i] = detail::to_dx(barrier[i].barrier, std::get<0>(barrier[i].texture));
    }

    D3D12_BARRIER_GROUP bg{
        .Type = D3D12_BARRIER_TYPE_TEXTURE,
        .NumBarriers = barrier_count,
        .pTextureBarriers = data
    };
    list->Barrier(1, &bg);
}