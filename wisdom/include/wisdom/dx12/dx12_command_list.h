#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/generated/api/api.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/util/com_ptr.h>

namespace wis {
class DX12CommandList;
struct DX12BufferBarrier2;
struct DX12TextureBarrier2;
struct DX12RenderPassDesc;
struct DX12VertexBufferBinding;

template<>
struct Internal<DX12CommandList> {
    wis::com_ptr<ID3D12CommandAllocator> allocator;
    wis::com_ptr<ID3D12GraphicsCommandList9> list;
    std::array<int8_t, size_t(wis::ShaderStages::Count)> root_stage_map;
    uint32_t root_table_offset = 0;
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
    bool Closed() const noexcept
    {
        return closed;
    }
    WIS_INLINE bool Close() noexcept;
    [[nodiscard]] WIS_INLINE wis::Result Reset(DX12PipelineHandle pipeline = nullptr) noexcept;
    WIS_INLINE void CopyBuffer(DX12BufferView source, DX12BufferView destination, wis::BufferRegion region) const noexcept;

    WIS_INLINE void CopyBufferToTexture(DX12BufferView src_buffer, DX12TextureView dest_texture, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) const noexcept;

    WIS_INLINE void CopyTextureToBuffer(DX12TextureView src_texture, DX12BufferView dest_buffer, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) const noexcept;

    WIS_INLINE void BufferBarrier(wis::BufferBarrier barrier, DX12BufferView buffer) noexcept;
    // 8 buffers at once max for efficiency
    WIS_INLINE void BufferBarriers(const wis::DX12BufferBarrier2* barriers, uint32_t barrier_count) noexcept;

    WIS_INLINE void TextureBarrier(wis::TextureBarrier barrier, DX12TextureView texture) noexcept;
    // 8 textures at once max for efficiency
    WIS_INLINE void TextureBarriers(const wis::DX12TextureBarrier2* barrier, uint32_t barrier_count) noexcept;

    WIS_INLINE void BeginRenderPass(const wis::DX12RenderPassDesc* pass_desc) noexcept;

    WIS_INLINE void EndRenderPass() noexcept;

    WIS_INLINE void SetRootSignature(wis::DX12RootSignatureView root_signature) noexcept;

    WIS_INLINE void IASetPrimitiveTopology(wis::PrimitiveTopology vp) noexcept;

    WIS_INLINE void IASetVertexBuffers(wis::DX12VertexBufferBinding* resources, uint32_t count, uint32_t start_slot = 0) noexcept;

    WIS_INLINE void RSSetViewport(wis::Viewport vp) noexcept;

    WIS_INLINE void RSSetViewports(const wis::Viewport* vp, uint32_t count) noexcept;

    WIS_INLINE void RSSetScissor(wis::Scissor sc) noexcept;

    WIS_INLINE void DrawIndexedInstanced(uint32_t vertex_count_per_instance,
                                         uint32_t instance_count = 1,
                                         uint32_t start_index = 0,
                                         uint32_t base_vertex = 0,
                                         uint32_t start_instance = 0) noexcept;

    WIS_INLINE void DrawInstanced(uint32_t vertex_count_per_instance,
                                  uint32_t instance_count = 1,
                                  uint32_t start_vertex = 0,
                                  uint32_t start_instance = 0) noexcept;

    WIS_INLINE void SetRootConstants(const void* data, uint32_t size_4bytes, uint32_t offset_4bytes, wis::ShaderStages stage) noexcept;

    WIS_INLINE void SetDescriptorBuffers(const wis::DX12DescriptorBufferView* buffers, uint32_t buffer_count) noexcept;

    WIS_INLINE void SetDescriptorTableOffset(uint32_t root_table_index, wis::DX12DescriptorBufferGPUView buffer, uint32_t offset_bytes) noexcept;

    WIS_INLINE void SetDescriptorTableOffset2(uint32_t root_table_index, wis::DX12DescriptorBufferGPUView buffer, uint32_t offset_descriptors) noexcept;

protected:
    bool closed = false;
};
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_command_list.cpp"
#endif // !WISDOM_HEADER_ONLY
