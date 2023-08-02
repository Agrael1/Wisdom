#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/vulkan/vk_format.h>
#include <wisdom/vulkan/vk_pipeline_state.h>
#include <wisdom/api/api_barrier.h>
#include <wisdom/vulkan/vk_buffer_views.h>
#include <wisdom/vulkan/vk_rtv.h>
#include <span>
#include <utility>
#endif

WIS_EXPORT namespace wis
{
    class VKCommandList;

    template<>
    class Internal<VKCommandList>
    {
    public:
        Internal() = default;
        Internal(wis::shared_handle<vk::CommandPool> allocator, vk::CommandBuffer command_list)
            : allocator(std::move(allocator)), command_list(command_list) { }

    protected:
        wis::shared_handle<vk::CommandPool> allocator;
        vk::CommandBuffer command_list;
        VKPipelineState pipeline;
    };

    class VKCommandList : public QueryInternal<VKCommandList>
    {
    public:
        VKCommandList() = default;
        explicit VKCommandList(wis::shared_handle<vk::CommandPool> allocator, vk::CommandBuffer command_list)
            : QueryInternal(std::move(allocator), command_list) { }
        operator VKCommandListView() const noexcept
        {
            return command_list;
        }

        /// @brief Bind a pipeline to the command list
        /// @param xpipeline Pipeline to bind
        void SetPipeline(VKPipelineState xpipeline) noexcept
        {
            pipeline = std::move(xpipeline);
        }

        /// @brief Reset the command list
        /// @return True if the command list is ready to be used
        bool Reset() noexcept
        {
            Close();

            vk::CommandBufferBeginInfo desc{};
            closed = false;
            command_list.begin(desc);
            if (pipeline)
                command_list.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.GetInternal().GetPipeline());
            return !closed;
        }

        /// @brief Check if the command list is closed
        /// @return True if the command list is closed
        [[nodiscard]] bool IsClosed() const noexcept
        {
            return closed;
        }

        /// @brief Close the command list
        /// If the command list is already closed, this function does nothing
        /// @return True if the command list was closed
        bool Close() noexcept
        {
            if (closed)
                return closed;
            command_list.end();
            return closed = true;
        }

        /// @brief Set a barrirer on a buffer to perform a transition.
        /// @param barrier Barrier description.
        /// @param buffer Buffer to set the barrier on.
        WIS_INLINE void BufferBarrier(wis::BufferBarrier barrier, VKBufferView buffer) noexcept;

        /// @brief Set a barrier on a texture to perform a transition.
        /// @param barrier Barrier description.
        /// @param texture Texture to set the barrier on.
        WIS_INLINE void TextureBarrier(wis::TextureBarrier barrier, VKTextureView texture) noexcept;

        /// @brief Copy data from one buffer to another on GPU.
        /// Function does not check if the buffers are compatible, neither does it check if the data sizes are within the buffer sizes.
        /// This is done to allow for more flexibility and performance.
        /// @param source Source buffer.
        /// @param destination Destination buffer.
        /// @param data_size Size of the data to copy.
        void CopyBuffer(VKBufferView source, VKBufferView destination, size_t data_size) noexcept
        {
            vk::BufferCopy bufCopy{
                0, // srcOffset
                0, // dstOffset,
                data_size
            }; // size
            command_list.copyBuffer(source, destination, bufCopy);
        }

        /// @brief Set viewport for the command list.
        /// @param vp Viewport to set.
        void RSSetViewport(Viewport vp) noexcept
        {
            vk::Viewport viewport;
            viewport.x = vp.top_leftx;
            viewport.y = vp.top_lefty + vp.height;
            viewport.width = vp.width;
            viewport.height = -vp.height;
            viewport.minDepth = vp.min_depth;
            viewport.maxDepth = vp.max_depth;
            command_list.setViewport(0, 1, &viewport);
        }

        /// @brief Sets the scissor rectangle for the command list.
        /// @param rect Scissor rectangle to set.
        void RSSetScissorRect(ScissorRect srect) noexcept
        {
            vk::Rect2D rect;
            rect.offset.x = static_cast<int>(srect.left);
            rect.offset.y = static_cast<int>(srect.top);
            rect.extent.width = srect.right;
            rect.extent.height = srect.bottom;
            command_list.setScissor(0, 1, &rect);
        }

        /// @brief Sets the primitive topology.
        /// @param vp Primitive topology to set.
        void IASetPrimitiveTopology(PrimitiveTopology topology) noexcept
        {
            command_list.setPrimitiveTopology(convert(topology)); // TODO: PatchList
        }

        /// @brief Binds vertex buffers to the command list.
        /// max 16 buffers can be bound.
        /// @param resources Vertex buffers to bind.
        /// @param start_slot Offset to start binding from.
        WIS_INLINE void IASetVertexBuffers(std::span<const VKVertexBufferView> resources, uint32_t start_slot = 0) noexcept;

        void IASetIndexBuffer(VKBufferView buffer, uint32_t, IndexType type = IndexType::uint16) noexcept
        {
            command_list.bindIndexBuffer(buffer, 0, type == IndexType::uint16 ? vk::IndexType::eUint16 : vk::IndexType::eUint32);
        }

        /// @brief Start a render pass.
        /// @param pass Pass description.
        /// @param render_targets Render targets to bind with colors to clear them with.
        WIS_INLINE void BeginRenderPass(wis::VKRenderPassView rp,
                                        std::span<const std::pair<VKRenderTargetView, ColorClear>> render_targets,
                                        std::pair<VKDepthStencilView, DepthClear> depth = {}) noexcept;

        /// @brief Ends the render pass.
        void EndRenderPass() noexcept
        {
            command_list.endRenderPass();
        }

        /// @brief Draws instanced primitives, without indices.
        /// @param VertexCountPerInstance Vertex count per single instance.
        /// @param InstanceCount Count of instances to draw.
        /// @param StartVertexLocation Start vertex location.
        /// @param StartInstanceLocation Start instance location.
        void DrawInstanced(uint32_t VertexCountPerInstance,
                           uint32_t InstanceCount = 1,
                           uint32_t StartVertexLocation = 0,
                           uint32_t StartInstanceLocation = 0) noexcept
        {
            command_list.draw(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
        }

        void DrawIndexedInstanced(uint32_t IndexCountPerInstance,
                                  uint32_t InstanceCount = 1,
                                  uint32_t StartIndexLocation = 0,
                                  uint32_t StartVertexLocation = 0,
                                  uint32_t StartInstanceLocation = 0) noexcept
        {
            command_list.drawIndexed(IndexCountPerInstance, InstanceCount, StartIndexLocation, StartVertexLocation, StartInstanceLocation);
        }

        void SetGraphicsDescriptorSet(VKRootSignatureView root) noexcept
        {
            // nothing tbd
        }
        void SetGraphicsDescriptorSet(VKRootSignatureView root, uint32_t RootParameterIndex, VKDescriptorSetBindView heap) noexcept
        {
            command_list.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, root, RootParameterIndex, 1, &heap, 0, nullptr);
        }

    private:
        bool closed = true;
    };
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/vk_command_list.inl"
#endif
