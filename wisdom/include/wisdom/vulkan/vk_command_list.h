#pragma once
#include <wisdom/generated/api/api.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/global/internal.h>

namespace wis {
class VKCommandList;
struct VKBufferBarrier2;
struct VKTextureBarrier2;
struct VKRenderPassDesc;
struct VKVertexBufferBinding;

template<>
struct Internal<VKCommandList> {
    wis::SharedDevice device;
    h::VkCommandPool allocator = nullptr;
    h::VkCommandBuffer command_list = nullptr;

    wis::SharedPipeline pipeline;
    VkPipelineLayout pipeline_layout = nullptr;

    Internal() noexcept = default;
    Internal(wis::SharedDevice device, VkCommandPool allocator, VkCommandBuffer command_list) noexcept
        : device(std::move(device)), allocator(allocator), command_list(command_list) { }
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&&) noexcept = default;

    ~Internal() noexcept
    {
        if (command_list) {
            device.table().vkDestroyCommandPool(device.get(), allocator, nullptr);
        }
    }
};
class VKCommandList : public QueryInternal<VKCommandList>
{
public:
    VKCommandList() noexcept = default;
    explicit VKCommandList(wis::SharedDevice device, VkCommandPool allocator, VkCommandBuffer command_list) noexcept
        : QueryInternal(std::move(device), allocator, command_list) { }
    operator bool() const noexcept
    {
        return bool(command_list);
    }
    operator VKCommandListView() const noexcept
    {
        return command_list;
    }

public:
    bool Closed() const noexcept {
        return closed;
    }
    WIS_INLINE bool Close() noexcept;
    [[nodiscard]] WIS_INLINE wis::Result Reset(VKPipelineHandle pipeline = {}) noexcept;
    WIS_INLINE void CopyBuffer(VKBufferView source, VKBufferView destination, wis::BufferRegion region) const noexcept;
    WIS_INLINE void CopyBufferToTexture(VKBufferView src_buffer, VKTextureView dest_texture, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) const noexcept;

    WIS_INLINE void CopyTextureToBuffer(VKTextureView src_texture, VKBufferView dest_buffer, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) const noexcept;

    WIS_INLINE void BufferBarrier(wis::BufferBarrier barrier, VKBufferView buffer) noexcept;
    // 8 buffers at once max for efficiency
    WIS_INLINE void BufferBarriers(const wis::VKBufferBarrier2* barriers, uint32_t barrier_count) noexcept;

    WIS_INLINE void TextureBarrier(wis::TextureBarrier barrier, VKTextureView texture) noexcept;
    // 8 textures at once max for efficiency
    WIS_INLINE void TextureBarriers(const wis::VKTextureBarrier2* barrier, uint32_t barrier_count) noexcept;

    WIS_INLINE void BeginRenderPass(const wis::VKRenderPassDesc* render_targets) noexcept;

    WIS_INLINE void EndRenderPass() noexcept;

    WIS_INLINE void SetRootSignature(wis::VKRootSignatureView root_signature) noexcept;

    WIS_INLINE void IASetPrimitiveTopology(wis::PrimitiveTopology topology) noexcept;

    WIS_INLINE void IASetVertexBuffers(wis::VKVertexBufferBinding* resources, uint32_t count, uint32_t start_slot = 0) noexcept;

    WIS_INLINE void RSSetViewport(wis::Viewport vp) noexcept;

    WIS_INLINE void RSSetViewports(const wis::Viewport* vp, uint32_t count) noexcept;

    WIS_INLINE void RSSetScissor(wis::Scissor scissor) noexcept;

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

    WIS_INLINE void SetDescriptorBuffers(const wis::VKDescriptorBufferView* buffers, uint32_t buffer_count) noexcept;

    WIS_INLINE void SetDescriptorTableOffset(uint32_t root_table_index, wis::VKDescriptorBufferView buffer, uint32_t offset_descriptors) noexcept;

protected:
    bool closed = false;
};

} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/vk_command_list.cpp"
#endif // !WISDOM_HEADER_ONLY