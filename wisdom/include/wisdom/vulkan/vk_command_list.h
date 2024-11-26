#ifndef WIS_VK_COMMAND_LIST_H
#define WIS_VK_COMMAND_LIST_H
#include <wisdom/generated/api/api.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/global/internal.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/bridge/format.h>

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

    VkPipelineLayout pipeline_layout = nullptr;

public:
    Internal() noexcept = default;
    Internal(wis::SharedDevice device, VkCommandPool allocator, VkCommandBuffer command_list) noexcept
        : device(std::move(device)), allocator(allocator), command_list(command_list) { }

    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&& o) noexcept
    {
        if (this == &o) {
            return *this;
        }

        Destroy();
        device = std::move(o.device);
        allocator = std::move(o.allocator);
        command_list = std::move(o.command_list);
        pipeline_layout = std::move(o.pipeline_layout);
        return *this;
    }
    ~Internal() noexcept
    {
        Destroy();
    }

    void Destroy() noexcept
    {
        if (command_list) {
            // wis::lib_trace(wis::format("VKCommandList {} destroyed", (void*)command_list.handle));
            device.table().vkDestroyCommandPool(device.get(), allocator, nullptr);
        }
    }
};

class ImplVKCommandList : public QueryInternal<VKCommandList>
{
public:
    ImplVKCommandList() noexcept = default;
    explicit ImplVKCommandList(wis::SharedDevice device, VkCommandPool allocator, VkCommandBuffer command_list) noexcept
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
    bool Closed() const noexcept
    {
        return closed;
    }
    WIS_INLINE bool Close() noexcept;
    [[nodiscard]] WIS_INLINE wis::Result Reset(wis::VKPipelineView initial_state = {}) noexcept;
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

    WIS_INLINE void SetPipelineState(wis::VKPipelineView pipeline_state) noexcept;

    WIS_INLINE void IASetPrimitiveTopology(wis::PrimitiveTopology topology) noexcept;

    WIS_INLINE void IASetVertexBuffers(const wis::VKVertexBufferBinding* resources, uint32_t count, uint32_t start_slot = 0) noexcept;

    WIS_INLINE void IASetIndexBuffer(wis::VKBufferView buffer, wis::IndexType type, uint64_t offset) noexcept;
    WIS_INLINE void IASetIndexBuffer2(wis::VKBufferView buffer, wis::IndexType type, uint32_t size, uint64_t offset) noexcept;

    WIS_INLINE void RSSetViewport(wis::Viewport vp) noexcept;

    WIS_INLINE void RSSetViewports(const wis::Viewport* vp, uint32_t count) noexcept;

    WIS_INLINE void RSSetScissor(wis::Scissor scissor) noexcept;

    WIS_INLINE void RSSetScissors(const wis::Scissor* vp, uint32_t count) noexcept;

    WIS_INLINE void DrawIndexedInstanced(uint32_t vertex_count_per_instance,
                                         uint32_t instance_count = 1,
                                         uint32_t start_index = 0,
                                         uint32_t base_vertex = 0,
                                         uint32_t start_instance = 0) noexcept;

    WIS_INLINE void DrawInstanced(uint32_t vertex_count_per_instance,
                                  uint32_t instance_count = 1,
                                  uint32_t start_vertex = 0,
                                  uint32_t start_instance = 0) noexcept;

    WIS_INLINE void SetPushConstants(const void* data, uint32_t size_4bytes, uint32_t offset_4bytes, wis::ShaderStages stage) noexcept;

    WIS_INLINE void PushDescriptor(wis::DescriptorType type, uint32_t binding, wis::VKBufferView view, uint32_t offset = 0) noexcept;

    WIS_INLINE void SetDescriptorStorage(VKDescriptorStorageView desc_storage) noexcept;

protected:
    bool closed = false;
};

#pragma region VKCommandList
/**
 * @brief Represents command list for recording commands.
 * */
class VKCommandList : public wis::ImplVKCommandList
{
public:
    using wis::ImplVKCommandList::ImplVKCommandList;

public:
    /**
     * @brief Closes the command list for recording.
     * @return true if command list is closed. false otherwise.
     * */
    inline bool Closed() const noexcept
    {
        return wis::ImplVKCommandList::Closed();
    }
    /**
     * @brief Closes the command list for recording.
     * @return true if command list is closed. false otherwise.
     * */
    inline bool Close() noexcept
    {
        return wis::ImplVKCommandList::Close();
    }
    /**
     * @brief Resets the command list for recording. Can't be reset while executed!
     * @param initial_state The pipeline to use as a starting state. Default is empty pipeline.
     * */
    [[nodiscard]] inline wis::Result Reset(wis::VKPipelineView initial_state = {}) noexcept
    {
        return wis::ImplVKCommandList::Reset(std::move(initial_state));
    }
    /**
     * @brief Switches command list to use new pipeline. All the operations will be recorded with regards to the new bound pipeline.
     * @param pipeline The pipeline to use with the command list with.
     * */
    inline void SetPipelineState(wis::VKPipelineView pipeline) noexcept
    {
        wis::ImplVKCommandList::SetPipelineState(std::move(pipeline));
    }
    /**
     * @brief Copies data from one buffer to another.
     * @param source The source buffer to copy from.
     * @param destination The destination buffer to copy to.
     * @param region The region to copy.
     * */
    inline void CopyBuffer(wis::VKBufferView source, wis::VKBufferView destination, wis::BufferRegion region) noexcept
    {
        wis::ImplVKCommandList::CopyBuffer(std::move(source), std::move(destination), region);
    }
    /**
     * @brief Copies data from buffer to texture.
     * @param source The source buffer to copy from.
     * @param destination The destination texture to copy to.
     * @param regions The regions to copy.
     * @param region_count The number of regions to copy.
     * */
    inline void CopyBufferToTexture(wis::VKBufferView source, wis::VKTextureView destination, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) noexcept
    {
        wis::ImplVKCommandList::CopyBufferToTexture(std::move(source), std::move(destination), regions, region_count);
    }
    /**
     * @brief Copies data from one texture to another.
     * @param source The source texture to copy from.
     * @param destination The destination buffer to copy to.
     * @param regions The regions to copy.
     * @param region_count The number of regions to copy.
     * */
    inline void CopyTextureToBuffer(wis::VKTextureView source, wis::VKBufferView destination, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) noexcept
    {
        wis::ImplVKCommandList::CopyTextureToBuffer(std::move(source), std::move(destination), regions, region_count);
    }
    /**
     * @brief Sets the barrier on the buffer.
     * @param barrier The barrier to set.
     * @param buffer The buffer to set the barrier on.
     * */
    inline void BufferBarrier(wis::BufferBarrier barrier, wis::VKBufferView buffer) noexcept
    {
        wis::ImplVKCommandList::BufferBarrier(barrier, std::move(buffer));
    }
    /**
     * @brief Sets the barriers on the buffers. You may set up to 8 buffer barriers for max efficiency.
     * @param barriers The barriers to set.
     * @param barrier_count The number of barriers to set.
     * */
    inline void BufferBarriers(const wis::VKBufferBarrier2* barriers, uint32_t barrier_count) noexcept
    {
        wis::ImplVKCommandList::BufferBarriers(barriers, barrier_count);
    }
    /**
     * @brief Sets the barrier on the texture.
     * @param barrier The barrier to set.
     * @param texture The texture to set the barrier on.
     * */
    inline void TextureBarrier(wis::TextureBarrier barrier, wis::VKTextureView texture) noexcept
    {
        wis::ImplVKCommandList::TextureBarrier(barrier, std::move(texture));
    }
    /**
     * @brief Sets the barriers on the textures. You may set up to 8 texture barriers for max efficiency.
     * @param barriers The barriers to set.
     * @param barrier_count The number of barriers to set.
     * */
    inline void TextureBarriers(const wis::VKTextureBarrier2* barriers, uint32_t barrier_count) noexcept
    {
        wis::ImplVKCommandList::TextureBarriers(barriers, barrier_count);
    }
    /**
     * @brief Begins the render pass.
     * @param pass_desc The description of the render pass to begin.
     * */
    inline void BeginRenderPass(const wis::VKRenderPassDesc* pass_desc) noexcept
    {
        wis::ImplVKCommandList::BeginRenderPass(pass_desc);
    }
    /**
     * @brief Ends the render pass.
     * */
    inline void EndRenderPass() noexcept
    {
        wis::ImplVKCommandList::EndRenderPass();
    }
    /**
     * @brief Sets the pipeline signature object. Used to determine how to pick descriptors from descriptor buffer.
     * @param root_signature The root signature to set.
     * */
    inline void SetRootSignature(wis::VKRootSignatureView root_signature) noexcept
    {
        wis::ImplVKCommandList::SetRootSignature(std::move(root_signature));
    }
    /**
     * @brief Sets the primitive topology. Detemines how vertices shall be processed.
     * @param topology The primitive topology to set.
     * */
    inline void IASetPrimitiveTopology(wis::PrimitiveTopology topology) noexcept
    {
        wis::ImplVKCommandList::IASetPrimitiveTopology(topology);
    }
    /**
     * @brief Sets the vertex buffers.
     * @param resources The vertex buffers to set.
     * @param count The number of vertex buffers to set.
     * @param start_slot The start slot to set the vertex buffers to. Default is 0.
     * */
    inline void IASetVertexBuffers(const wis::VKVertexBufferBinding* resources, uint32_t count, uint32_t start_slot = 0) noexcept
    {
        wis::ImplVKCommandList::IASetVertexBuffers(resources, count, start_slot);
    }
    /**
     * @brief Sets the index buffer.
     * @param buffer The index buffer to set.
     * @param type The type of the index buffer.
     * @param offset The offset in the index buffer in bytes.
     * */
    inline void IASetIndexBuffer(wis::VKBufferView buffer, wis::IndexType type, uint64_t offset) noexcept
    {
        wis::ImplVKCommandList::IASetIndexBuffer(std::move(buffer), type, offset);
    }
    /**
     * @brief Sets the index buffer.
     * You may provide the offset in the buffer to take only a range of the buffer.
     * Requires wis::DeviceFeature::AdvancedIndexBuffer to be supported.
     * @param buffer The index buffer to set.
     * @param type The type of the index buffer.
     * @param size The size of the index buffer in bytes.
     * @param offset The offset in the index buffer in bytes.
     * */
    inline void IASetIndexBuffer2(wis::VKBufferView buffer, wis::IndexType type, uint32_t size, uint64_t offset) noexcept
    {
        wis::ImplVKCommandList::IASetIndexBuffer2(std::move(buffer), type, size, offset);
    }
    /**
     * @brief Sets the viewport.
     * @param viewport The viewport to set.
     * */
    inline void RSSetViewport(wis::Viewport viewport) noexcept
    {
        wis::ImplVKCommandList::RSSetViewport(viewport);
    }
    /**
     * @brief Sets multiple viewports.
     * @param viewports The viewports to set.
     * @param count The number of viewports to set.
     * */
    inline void RSSetViewports(const wis::Viewport* viewports, uint32_t count) noexcept
    {
        wis::ImplVKCommandList::RSSetViewports(viewports, count);
    }
    /**
     * @brief Sets the scissor rect.
     * @param scissor The scissor to set.
     * */
    inline void RSSetScissor(wis::Scissor scissor) noexcept
    {
        wis::ImplVKCommandList::RSSetScissor(scissor);
    }
    /**
     * @brief Sets multiple scissor rects.
     * Each n-th rect corresponds to n-th Viewport set in RSSetViewports if SV_ViewportArrayIndex is used in geometry shader.
     * Otherwise the first is chosen.
     * @param scissors The scissors to set.
     * @param count The number of scissors to set.
     * */
    inline void RSSetScissors(const wis::Scissor* scissors, uint32_t count) noexcept
    {
        wis::ImplVKCommandList::RSSetScissors(scissors, count);
    }
    /**
     * @brief Draws indexed instanced geometry.
     * @param vertex_count_per_instance The number of vertices to draw per instance.
     * @param instance_count The number of instances to draw. Default is 1.
     * @param start_index The index of the first vertex to draw. Default is 0.
     * @param base_vertex The index of the first vertex to start drawing from. Default is 0.
     * @param start_instance The index of the first instance to draw. Default is 0.
     * */
    inline void DrawIndexedInstanced(uint32_t vertex_count_per_instance, uint32_t instance_count = 1, uint32_t start_index = 0, uint32_t base_vertex = 0, uint32_t start_instance = 0) noexcept
    {
        wis::ImplVKCommandList::DrawIndexedInstanced(vertex_count_per_instance, instance_count, start_index, base_vertex, start_instance);
    }
    /**
     * @brief Draws instanced geometry. (Without indexing)
     * @param vertex_count_per_instance The number of vertices to draw per instance.
     * @param instance_count The number of instances to draw. Default is 1.
     * @param start_vertex The index of the first vertex to draw. Default is 0.
     * @param start_instance The index of the first instance to draw. Default is 0.
     * */
    inline void DrawInstanced(uint32_t vertex_count_per_instance, uint32_t instance_count = 1, uint32_t start_vertex = 0, uint32_t start_instance = 0) noexcept
    {
        wis::ImplVKCommandList::DrawInstanced(vertex_count_per_instance, instance_count, start_vertex, start_instance);
    }
    /**
     * @brief Sets the root constants for the shader.
     * @param data The data to set the root constants with.
     * @param size_4bytes The size of the data in 4-byte units.
     * @param offset_4bytes The offset in the data in 4-byte units.
     * @param stage The shader stages to set the root constants for.
     * */
    inline void SetPushConstants(void* data, uint32_t size_4bytes, uint32_t offset_4bytes, wis::ShaderStages stage) noexcept
    {
        wis::ImplVKCommandList::SetPushConstants(data, size_4bytes, offset_4bytes, stage);
    }
    /**
     * @brief Pushes descriptor directly to the command list, without putting it to the table.
     * Works only with buffer bindings.
     * Buffer is always bound with full size.
     * @param type The type of the descriptor to set.
     * @param root_index The index of the root descriptor to set.
     * @param buffer The buffer to set.
     * @param offset The offset in the descriptor table to set the descriptor to.
     * */
    inline void PushDescriptor(wis::DescriptorType type, uint32_t root_index, wis::VKBufferView buffer, uint32_t offset) noexcept
    {
        wis::ImplVKCommandList::PushDescriptor(type, root_index, std::move(buffer), offset);
    }
};
#pragma endregion VKCommandList

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_command_list.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_COMMAND_LIST_H
