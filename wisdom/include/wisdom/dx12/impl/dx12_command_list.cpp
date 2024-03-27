#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/dx12/dx12_command_list.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/generated/dx12/dx12_structs.hpp>
#include <wisdom/util/small_allocator.h>
#include <d3dx12/d3dx12_resource_helpers.h>

void wis::DX12CommandList::CopyBuffer(DX12BufferView source, DX12BufferView destination, wis::BufferRegion region) const noexcept
{
    list->CopyBufferRegion(std::get<0>(destination), region.dst_offset, std::get<0>(source), region.src_offset, region.size_bytes);
}
void wis::DX12CommandList::CopyBufferToTexture(DX12BufferView src_buffer, DX12TextureView dest_texture, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) noexcept
{
    auto texture = std::get<0>(dest_texture);
    auto texture_desc = texture->GetDesc();

    wis::com_ptr<ID3D12Device> device;
    auto hr = texture->GetDevice(__uuidof(*device), device.put_void());

    for (uint32_t i = 0; i < region_count; i++) {
        auto& region = regions[i];
        D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout{};
        UINT num_rows = 0;
        UINT64 row_size = 0;
        UINT64 required_size = 0;

        

        UINT dest_subresource = D3D12CalcSubresource(region.dst.mip, region.dst.array_layer, 0u, texture_desc.MipLevels, texture_desc.DepthOrArraySize);
        D3D12_TEXTURE_COPY_LOCATION dst{
            .pResource = texture,
            .Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
            .SubresourceIndex = dest_subresource
        };

        UINT row_pitch = 0;
        D3D12_PROPERTY_LAYOUT_FORMAT_TABLE::CalculateMinimumRowMajorRowPitch(texture_desc.Format, region.src.size.width, row_pitch);

        D3D12_TEXTURE_COPY_LOCATION src{
            .pResource = std::get<0>(src_buffer),
            .Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,
            .PlacedFootprint = {
                    .Offset = region.src.offset,
                    .Footprint = {
                            .Format = texture_desc.Format,
                            .Width = region.src.size.width,
                            .Height = region.src.size.height,
                            .Depth = region.src.size.depth_or_layers,
                            .RowPitch = row_pitch
                    } }
        };

        list->CopyTextureRegion(&dst, region.dst.offset.width, region.dst.offset.height, region.dst.offset.depth_or_layers, &src, nullptr);
    }
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

void wis::DX12CommandList::IASetPrimitiveTopology(wis::PrimitiveTopology vp) noexcept
{
    list->IASetPrimitiveTopology(convert_dx(vp));
}

void wis::DX12CommandList::IASetVertexBuffers(wis::DX12VertexBufferBinding* resources, uint32_t count, uint32_t start_slot) noexcept
{
    wis::detail::limited_allocator<D3D12_VERTEX_BUFFER_VIEW, 8> allocator(count, true);
    auto* data = allocator.data();
    for (size_t i = 0; i < count; i++) {
        data[i] = {
            .BufferLocation = std::get<0>(resources[i].buffer)->GetGPUVirtualAddress(),
            .SizeInBytes = resources[i].size,
            .StrideInBytes = resources[i].stride
        };
    }

    list->IASetVertexBuffers(start_slot, count, data);
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
    list->RSSetViewports(count, reinterpret_cast<const D3D12_VIEWPORT*>(vp));
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

void wis::DX12CommandList::SetRootSignature(wis::DX12RootSignatureView root_signature) noexcept
{
    list->SetGraphicsRootSignature(std::get<0>(root_signature));
    root_stage_map = std::get<1>(root_signature);
}

void wis::DX12CommandList::DrawIndexedInstanced(uint32_t vertex_count_per_instance,
                                                uint32_t instance_count,
                                                uint32_t start_index,
                                                uint32_t base_vertex,
                                                uint32_t start_instance) noexcept
{
    list->DrawIndexedInstanced(vertex_count_per_instance, instance_count, start_index, base_vertex, start_instance);
}

void wis::DX12CommandList::DrawInstanced(uint32_t vertex_count_per_instance,
                                         uint32_t instance_count,
                                         uint32_t base_vertex,
                                         uint32_t start_instance) noexcept
{
    list->DrawInstanced(vertex_count_per_instance, instance_count, base_vertex, start_instance);
}

void wis::DX12CommandList::SetRootConstants(const void* data, uint32_t size_4bytes, uint32_t offset_4bytes, wis::ShaderStages stage) noexcept
{
    list->SetGraphicsRoot32BitConstants(UINT(root_stage_map[uint32_t(stage)]), size_4bytes, data, offset_4bytes);
}