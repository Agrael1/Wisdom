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
    closed = false;
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

void wis::DX12CommandList::BufferBarriers(const wis::DX12BufferBarrier2* barriers, uint32_t barrier_count) noexcept
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
void wis::DX12CommandList::TextureBarriers(const wis::DX12TextureBarrier2* barrier, uint32_t barrier_count) noexcept
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

void wis::DX12CommandList::BeginRenderPass(const wis::DX12RenderPassDesc* pass_desc) noexcept
{
    wis::detail::limited_allocator<D3D12_RENDER_PASS_RENDER_TARGET_DESC, 8> allocator(8, true);
    auto* data = allocator.data();

    for (size_t i = 0; i < pass_desc->target_count; i++) {
        auto& target = pass_desc->targets[i];
        data[i] = {
            .cpuDescriptor = std::get<0>(target.target),
            .BeginningAccess = {
                    .Type = convert_dx(target.load_op),
            },
            .EndingAccess = {
                    .Type = convert_dx(target.store_op),
            }
        };
        if (data[i].BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR) {
            std::copy(std::begin(target.clear_value), std::end(target.clear_value), std::begin(data[i].BeginningAccess.Clear.ClearValue.Color));
        }
    }

    list->BeginRenderPass(pass_desc->target_count, data, nullptr, convert_dx(pass_desc->flags));
}

void wis::DX12CommandList::EndRenderPass() noexcept
{
    list->EndRenderPass();
}

void wis::DX12CommandList::RSSetViewport(wis::Viewport vp) noexcept
{
    D3D12_VIEWPORT viewport{
        .TopLeftX = vp.top_leftx,
        .TopLeftY = vp.top_lefty,
        .Width = vp.width,
        .Height = vp.height,
        .MinDepth = vp.min_depth,
        .MaxDepth = vp.max_depth
    };
    list->RSSetViewports(1, &viewport);
}
void wis::DX12CommandList::RSSetViewports(const wis::Viewport* vp, uint32_t count) noexcept
{
    static_assert(sizeof(D3D12_VIEWPORT) == sizeof(wis::Viewport));
    list->RSSetViewports(count, reinterpret_cast <const D3D12_VIEWPORT*>(vp));
}

void wis::DX12CommandList::RSSetScissor(wis::Scissor sc) noexcept
{
    D3D12_RECT rect{
        .left = static_cast<LONG>(sc.left),
        .top = static_cast<LONG>(sc.top),
        .right = static_cast<LONG>(sc.right),
        .bottom = static_cast<LONG>(sc.bottom)
    };
    list->RSSetScissorRects(1, &rect);
}

void wis::DX12CommandList::IASetPrimitiveTopology(wis::PrimitiveTopology vp) noexcept
{
    list->IASetPrimitiveTopology(convert_dx(vp));
}