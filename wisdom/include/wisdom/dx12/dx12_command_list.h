#ifndef WIS_DX12_COMMAND_LIST_H
#define WIS_DX12_COMMAND_LIST_H
#include <wisdom/global/internal.h>
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
    uint32_t push_constant_count = 0;
    uint32_t push_descriptor_count = 0;
};

class ImplDX12CommandList : public QueryInternal<DX12CommandList>
{
public:
    ImplDX12CommandList() noexcept = default;
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
    [[nodiscard]] WIS_INLINE wis::Result Reset(wis::DX12PipelineView pipeline = {}) noexcept;
    WIS_INLINE void CopyBuffer(DX12BufferView source, DX12BufferView destination, wis::BufferRegion region) const noexcept;

    WIS_INLINE void CopyBufferToTexture(DX12BufferView src_buffer, DX12TextureView dest_texture, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) const noexcept;

    WIS_INLINE void CopyTextureToBuffer(DX12TextureView src_texture, DX12BufferView dest_buffer, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) const noexcept;

    WIS_INLINE void CopyTexture(DX12TextureView source, DX12TextureView destination, const wis::TextureCopyRegion* regions, uint32_t region_count) const noexcept;

    WIS_INLINE void BufferBarrier(wis::BufferBarrier barrier, DX12BufferView buffer) noexcept;
    // 8 buffers at once max for efficiency
    WIS_INLINE void BufferBarriers(const wis::DX12BufferBarrier2* barriers, uint32_t barrier_count) noexcept;

    WIS_INLINE void TextureBarrier(wis::TextureBarrier barrier, DX12TextureView texture) noexcept;
    // 8 textures at once max for efficiency
    WIS_INLINE void TextureBarriers(const wis::DX12TextureBarrier2* barrier, uint32_t barrier_count) noexcept;

    WIS_INLINE void BeginRenderPass(const wis::DX12RenderPassDesc* pass_desc) noexcept;

    WIS_INLINE void EndRenderPass() noexcept;

    WIS_INLINE void SetRootSignature(wis::DX12RootSignatureView root_signature) noexcept;

    WIS_INLINE void SetComputeRootSignature(wis::DX12RootSignatureView root_signature) noexcept;

    WIS_INLINE void SetPipelineState(wis::DX12PipelineView pipeline_state) noexcept;

    WIS_INLINE void IASetPrimitiveTopology(wis::PrimitiveTopology vp) noexcept;

    WIS_INLINE void IASetVertexBuffers(const wis::DX12VertexBufferBinding* resources, uint32_t count, uint32_t start_slot = 0) noexcept;

    WIS_INLINE void IASetIndexBuffer(wis::DX12BufferView buffer, wis::IndexType type, uint64_t offset) noexcept;
    WIS_INLINE void IASetIndexBuffer2(wis::DX12BufferView buffer, wis::IndexType type, uint32_t size, uint64_t offset) noexcept;

    WIS_INLINE void RSSetViewport(wis::Viewport vp) noexcept;

    WIS_INLINE void RSSetViewports(const wis::Viewport* vp, uint32_t count) noexcept;

    WIS_INLINE void RSSetScissor(wis::Scissor sc) noexcept;

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

    WIS_INLINE void Dispatch(uint32_t group_count_x, uint32_t group_count_y, uint32_t group_count_z) noexcept;

    WIS_INLINE void SetPushConstants(const void* data, uint32_t size_4bytes, uint32_t offset_4bytes, wis::ShaderStages stage) noexcept;

    WIS_INLINE void PushDescriptor(wis::DescriptorType type, uint32_t binding, wis::DX12BufferView view, uint32_t offset = 0) noexcept;

    WIS_INLINE void SetDescriptorStorage(wis::DX12DescriptorStorageView desc_storage) noexcept;

    WIS_INLINE void SetComputeDescriptorStorage(wis::DX12DescriptorStorageView desc_storage) noexcept;

protected:
    bool closed = false;
};

#pragma region DX12CommandList
/**
 * @brief Represents command list for recording commands.
 * */
class DX12CommandList : public wis::ImplDX12CommandList
{
public:
    using wis::ImplDX12CommandList::ImplDX12CommandList;

public:
    /**
     * @brief Closes the command list for recording.
     * @return true if command list is closed. false otherwise.
     * */
    inline bool Closed() const noexcept
    {
        return wis::ImplDX12CommandList::Closed();
    }
    /**
     * @brief Closes the command list for recording.
     * @return true if command list is closed. false otherwise.
     * */
    inline bool Close() noexcept
    {
        return wis::ImplDX12CommandList::Close();
    }
    /**
     * @brief Resets the command list for recording. Can't be reset while executed!
     * @param initial_state The pipeline to use as a starting state. Default is empty pipeline.
     * */
    [[nodiscard]] inline wis::Result Reset(wis::DX12PipelineView initial_state = {}) noexcept
    {
        return wis::ImplDX12CommandList::Reset(std::move(initial_state));
    }
    /**
     * @brief Switches command list to use new pipeline. All the operations will be recorded with regards to the new bound pipeline.
     * @param pipeline The pipeline to use with the command list with.
     * */
    inline void SetPipelineState(wis::DX12PipelineView pipeline) noexcept
    {
        wis::ImplDX12CommandList::SetPipelineState(std::move(pipeline));
    }
    /**
     * @brief Copies data from one buffer to another.
     * @param source The source buffer to copy from.
     * @param destination The destination buffer to copy to.
     * @param region The region to copy.
     * */
    inline void CopyBuffer(wis::DX12BufferView source, wis::DX12BufferView destination, const wis::BufferRegion& region) noexcept
    {
        wis::ImplDX12CommandList::CopyBuffer(std::move(source), std::move(destination), region);
    }
    /**
     * @brief Copies data from buffer to texture.
     * @param source The source buffer to copy from.
     * @param destination The destination texture to copy to.
     * @param regions The regions to copy.
     * @param region_count The number of regions to copy.
     * */
    inline void CopyBufferToTexture(wis::DX12BufferView source, wis::DX12TextureView destination, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) noexcept
    {
        wis::ImplDX12CommandList::CopyBufferToTexture(std::move(source), std::move(destination), regions, region_count);
    }
    /**
     * @brief Copies data from one texture to another.
     * @param source The source texture to copy from.
     * @param destination The destination buffer to copy to.
     * @param regions The regions to copy.
     * @param region_count The number of regions to copy.
     * */
    inline void CopyTextureToBuffer(wis::DX12TextureView source, wis::DX12BufferView destination, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) noexcept
    {
        wis::ImplDX12CommandList::CopyTextureToBuffer(std::move(source), std::move(destination), regions, region_count);
    }
    /**
     * @brief Copies data from one texture to another.
     * @param source The source texture to copy from.
     * @param destination The destination texture to copy to.
     * @param regions The regions to copy.
     * @param region_count The number of regions to copy.
     * */
    inline void CopyTexture(wis::DX12TextureView source, wis::DX12TextureView destination, const wis::TextureCopyRegion* regions, uint32_t region_count) noexcept
    {
        wis::ImplDX12CommandList::CopyTexture(std::move(source), std::move(destination), regions, region_count);
    }
    /**
     * @brief Sets the barrier on the buffer.
     * @param barrier The barrier to set.
     * @param buffer The buffer to set the barrier on.
     * */
    inline void BufferBarrier(const wis::BufferBarrier& barrier, wis::DX12BufferView buffer) noexcept
    {
        wis::ImplDX12CommandList::BufferBarrier(barrier, std::move(buffer));
    }
    /**
     * @brief Sets the barriers on the buffers. You may set up to 8 buffer barriers for max efficiency.
     * @param barriers The barriers to set.
     * @param barrier_count The number of barriers to set.
     * */
    inline void BufferBarriers(const wis::DX12BufferBarrier2* barriers, uint32_t barrier_count) noexcept
    {
        wis::ImplDX12CommandList::BufferBarriers(barriers, barrier_count);
    }
    /**
     * @brief Sets the barrier on the texture.
     * @param barrier The barrier to set.
     * @param texture The texture to set the barrier on.
     * */
    inline void TextureBarrier(const wis::TextureBarrier& barrier, wis::DX12TextureView texture) noexcept
    {
        wis::ImplDX12CommandList::TextureBarrier(barrier, std::move(texture));
    }
    /**
     * @brief Sets the barriers on the textures. You may set up to 8 texture barriers for max efficiency.
     * @param barriers The barriers to set.
     * @param barrier_count The number of barriers to set.
     * */
    inline void TextureBarriers(const wis::DX12TextureBarrier2* barriers, uint32_t barrier_count) noexcept
    {
        wis::ImplDX12CommandList::TextureBarriers(barriers, barrier_count);
    }
    /**
     * @brief Begins the render pass.
     * @param pass_desc The description of the render pass to begin.
     * */
    inline void BeginRenderPass(const wis::DX12RenderPassDesc* pass_desc) noexcept
    {
        wis::ImplDX12CommandList::BeginRenderPass(pass_desc);
    }
    /**
     * @brief Ends the render pass.
     * */
    inline void EndRenderPass() noexcept
    {
        wis::ImplDX12CommandList::EndRenderPass();
    }
    /**
     * @brief Sets the pipeline signature object. Used to determine how to pick descriptors from descriptor buffer.
     * @param root_signature The root signature to set.
     * */
    inline void SetRootSignature(wis::DX12RootSignatureView root_signature) noexcept
    {
        wis::ImplDX12CommandList::SetRootSignature(std::move(root_signature));
    }
    /**
     * @brief Sets the pipeline signature object to compute pipeline. Used to determine how to pick descriptors from descriptor buffer.
     * May only work with compute pipelines.
     * @param root_signature The root signature to set.
     * */
    inline void SetComputeRootSignature(wis::DX12RootSignatureView root_signature) noexcept
    {
        wis::ImplDX12CommandList::SetComputeRootSignature(std::move(root_signature));
    }
    /**
     * @brief Sets the primitive topology. Detemines how vertices shall be processed.
     * @param topology The primitive topology to set.
     * */
    inline void IASetPrimitiveTopology(wis::PrimitiveTopology topology) noexcept
    {
        wis::ImplDX12CommandList::IASetPrimitiveTopology(topology);
    }
    /**
     * @brief Sets the vertex buffers.
     * @param resources The vertex buffers to set.
     * @param count The number of vertex buffers to set.
     * @param start_slot The start slot to set the vertex buffers to. Default is 0.
     * */
    inline void IASetVertexBuffers(const wis::DX12VertexBufferBinding* resources, uint32_t count, uint32_t start_slot = 0) noexcept
    {
        wis::ImplDX12CommandList::IASetVertexBuffers(resources, count, start_slot);
    }
    /**
     * @brief Sets the index buffer.
     * @param buffer The index buffer to set.
     * @param type The type of the index buffer.
     * @param offset The offset in the index buffer in bytes.
     * */
    inline void IASetIndexBuffer(wis::DX12BufferView buffer, wis::IndexType type, uint64_t offset) noexcept
    {
        wis::ImplDX12CommandList::IASetIndexBuffer(std::move(buffer), type, offset);
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
    inline void IASetIndexBuffer2(wis::DX12BufferView buffer, wis::IndexType type, uint32_t size, uint64_t offset) noexcept
    {
        wis::ImplDX12CommandList::IASetIndexBuffer2(std::move(buffer), type, size, offset);
    }
    /**
     * @brief Sets the viewport.
     * @param viewport The viewport to set.
     * */
    inline void RSSetViewport(const wis::Viewport& viewport) noexcept
    {
        wis::ImplDX12CommandList::RSSetViewport(viewport);
    }
    /**
     * @brief Sets multiple viewports.
     * @param viewports The viewports to set.
     * @param count The number of viewports to set.
     * */
    inline void RSSetViewports(const wis::Viewport* viewports, uint32_t count) noexcept
    {
        wis::ImplDX12CommandList::RSSetViewports(viewports, count);
    }
    /**
     * @brief Sets the scissor rect.
     * @param scissor The scissor to set.
     * */
    inline void RSSetScissor(const wis::Scissor& scissor) noexcept
    {
        wis::ImplDX12CommandList::RSSetScissor(scissor);
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
        wis::ImplDX12CommandList::RSSetScissors(scissors, count);
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
        wis::ImplDX12CommandList::DrawIndexedInstanced(vertex_count_per_instance, instance_count, start_index, base_vertex, start_instance);
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
        wis::ImplDX12CommandList::DrawInstanced(vertex_count_per_instance, instance_count, start_vertex, start_instance);
    }
    /**
     * @brief Dispatches compute shader.
     * @param group_count_x The number of groups to dispatch in X dimension.
     * @param group_count_y The number of groups to dispatch in Y dimension. Default is 1.
     * @param group_count_z The number of groups to dispatch in Z dimension. Default is 1.
     * */
    inline void Dispatch(uint32_t group_count_x, uint32_t group_count_y = 1, uint32_t group_count_z = 1) noexcept
    {
        wis::ImplDX12CommandList::Dispatch(group_count_x, group_count_y, group_count_z);
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
        wis::ImplDX12CommandList::SetPushConstants(data, size_4bytes, offset_4bytes, stage);
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
    inline void PushDescriptor(wis::DescriptorType type, uint32_t root_index, wis::DX12BufferView buffer, uint32_t offset) noexcept
    {
        wis::ImplDX12CommandList::PushDescriptor(type, root_index, std::move(buffer), offset);
    }
    /**
     * @brief Sets the descriptor storage object for graphics pipeline.
     * @param storage The descriptor storage to set.
     * */
    inline void SetDescriptorStorage(wis::DX12DescriptorStorageView storage) noexcept
    {
        wis::ImplDX12CommandList::SetDescriptorStorage(std::move(storage));
    }
    /**
     * @brief Sets the descriptor storage object for compute pipeline.
     * @param storage The descriptor storage to set.
     * */
    inline void SetComputeDescriptorStorage(wis::DX12DescriptorStorageView storage) noexcept
    {
        wis::ImplDX12CommandList::SetComputeDescriptorStorage(std::move(storage));
    }
};
#pragma endregion DX12CommandList

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/dx12_command_list.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !WIS_DX12_COMMAND_LIST_H
