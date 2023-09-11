#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/dx12/dx12_rtv.h>
#include <wisdom/dx12/dx12_format.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_render_pass.h>
#include <wisdom/dx12/dx12_pipeline_state.h>
#endif

namespace wis {
class DX12CommandList;

template<>
class Internal<DX12CommandList>
{
public:
    winrt::com_ptr<ID3D12CommandAllocator> allocator;
    winrt::com_ptr<ID3D12GraphicsCommandList9> command_list;
};

/// @brief A command list that can be executed by the GPU.
WIS_EXPORT class DX12CommandList : public QueryInternal<DX12CommandList>
{
public:
    DX12CommandList() = default;
    explicit DX12CommandList(winrt::com_ptr<ID3D12CommandAllocator> xallocator,
                             winrt::com_ptr<ID3D12GraphicsCommandList9> xcommand_list) noexcept
        : QueryInternal(std::move(xallocator), std::move(xcommand_list))
    {
    }
    operator bool() const noexcept
    {
        return command_list && allocator;
    }
    operator DX12CommandListView() const noexcept
    {
        return command_list.get();
    }

public:
    /// @brief Bind a pipeline state to the command list.
    /// @param xpipeline Pipeline state to bind.
    void SetPipeline(DX12PipelineState xpipeline) noexcept
    {
        pipeline_state = std::move(xpipeline);
    }

    /// @brief Reset the command list.
    /// @return True if the command list is ready to be used, false otherwise.
    bool Reset() noexcept
    {
        return closed = !(wis::succeeded(allocator->Reset()) && wis::succeeded(command_list->Reset(allocator.get(), pipeline_state.GetInternal().pipeline.get())));
    }

    /// @brief Check if the command list is closed.
    /// @return true if the command list is closed, false otherwise.
    [[nodiscard]] bool IsClosed() const noexcept
    {
        return closed;
    }

    /// @brief Close the command list.
    /// If the command list is already closed, this function does nothing.
    /// @return True if the command list was closed successfully, false otherwise.
    bool Close() noexcept
    {
        if (closed)
            return closed;
        return closed = wis::succeeded(command_list->Close());
    }

    /// @brief Set a barrirer on a buffer to perform a transition.
    /// @param barrier Barrier description.
    /// @param buffer Buffer to set the barrier on.
    void BufferBarrier(wis::BufferBarrier barrier, DX12BufferView buffer) noexcept
    {
        CD3DX12_BUFFER_BARRIER bb{
            D3D12_BARRIER_SYNC(barrier.sync_before),
            D3D12_BARRIER_SYNC(barrier.sync_after),
            convert_dx(barrier.access_before),
            convert_dx(barrier.access_after),
            std::get<0>(buffer)
        };
        CD3DX12_BARRIER_GROUP bg{ 1, &bb };
        command_list->Barrier(1, &bg);
    }

    /// @brief Set a barrier on a texture to perform a transition.
    /// @param barrier Barrier description.
    /// @param texture Texture to set the barrier on.
    void TextureBarrier(wis::TextureBarrier barrier, DX12TextureView texture) noexcept
    {
        auto r = barrier.range;
        CD3DX12_TEXTURE_BARRIER tb{
            D3D12_BARRIER_SYNC(barrier.sync_before),
            D3D12_BARRIER_SYNC(barrier.sync_after),
            convert_dx(barrier.access_before),
            convert_dx(barrier.access_after),
            convert_dx(barrier.state_before),
            convert_dx(barrier.state_after),
            std::get<0>(texture),
            r.extent_mips == r.whole ? CD3DX12_BARRIER_SUBRESOURCE_RANGE(r.whole) : CD3DX12_BARRIER_SUBRESOURCE_RANGE(r.base_mip, r.extent_mips, r.base_layer, r.extent_layers)
        };
        CD3DX12_BARRIER_GROUP bg{ 1, &tb };
        command_list->Barrier(1, &bg);
    }

    /// @brief Copy data from one buffer to another on GPU.
    /// Function does not check if the buffers are compatible, neither does it check if the data sizes are within the buffer sizes.
    /// This is done to allow for more flexibility and performance.
    /// @param source Source buffer.
    /// @param destination Destination buffer.
    /// @param data_size Size of the data to copy.
    void CopyBuffer(DX12BufferView source, DX12BufferView destination, size_t data_size) noexcept
    {
        command_list->CopyBufferRegion(std::get<0>(destination), 0, std::get<0>(source), 0, data_size);
    }

    /// @brief Sets the root signature for the command list. Only for DX12.
    /// @param root Root signature to set.
    void DXSetGraphicsRootSignature(DX12RootSignatureView root) noexcept
    {
        command_list->SetGraphicsRootSignature(std::get<0>(root));
    }

    /// @brief Set viewport for the command list.
    /// @param vp Viewport to set.
    void RSSetViewport(Viewport vp) noexcept
    {
        command_list->RSSetViewports(1, (D3D12_VIEWPORT*)&vp);
    }

    /// @brief Sets the scissor rectangle for the command list.
    /// @param rect Scissor rectangle to set.
    void RSSetScissorRect(ScissorRect rect) noexcept
    {
        command_list->RSSetScissorRects(1, (D3D12_RECT*)&rect);
    }

    /// @brief Sets the primitive topology.
    /// @param vp Primitive topology to set.
    void IASetPrimitiveTopology(PrimitiveTopology vp) noexcept
    {
        command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY(vp));
    }

    /// @brief Binds vertex buffers to the command list.
    /// max 16 buffers can be bound.
    /// @param resources Vertex buffers to bind.
    /// @param start_slot Offset to start binding from.
    void IASetVertexBuffers(std::span<const DX12VertexBufferView> resources, uint32_t start_slot = 0) noexcept
    {
        command_list->IASetVertexBuffers(start_slot, resources.size(), (const D3D12_VERTEX_BUFFER_VIEW*)resources.data());
    }
    void IASetIndexBuffer(DX12BufferView buffer, uint32_t size, IndexType type = IndexType::uint16) noexcept
    {
        D3D12_INDEX_BUFFER_VIEW ibv{
            std::get<0>(buffer)->GetGPUVirtualAddress(),
            size,
            type == IndexType::uint16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT,
        };
        command_list->IASetIndexBuffer(&ibv);
    }

    /// @brief Start a render pass.
    /// @param pass Pass description.
    /// @param render_targets Render targets to bind with colors to clear them with.
    void BeginRenderPass(DX12RenderPassView pass,
                         std::span<const std::pair<DX12RenderTarget, ColorClear>> render_targets,
                         std::pair<DX12DepthStencil, DepthClear> depth = {}) noexcept
    {
        auto& rpi = *std::get<0>(pass);
        auto& rts = rpi.rt_descs;
        for (size_t i = 0; i < rpi.rt_formats.size(); i++) {
            rts[i].cpuDescriptor = render_targets[i].first.GetInternal().handle;
            rts[i].BeginningAccess.Clear.ClearValue.Color[0] = render_targets[i].second[0];
            rts[i].BeginningAccess.Clear.ClearValue.Color[1] = render_targets[i].second[1];
            rts[i].BeginningAccess.Clear.ClearValue.Color[2] = render_targets[i].second[2];
            rts[i].BeginningAccess.Clear.ClearValue.Color[3] = render_targets[i].second[3];
        }

        auto& dsdesc = rpi.ds_desc;
        if (auto ds = depth.first.GetInternal().handle; ds.ptr && rpi.ds_format != DXGI_FORMAT_UNKNOWN) {
            dsdesc.cpuDescriptor = ds;
            dsdesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Depth = depth.second;
            command_list->BeginRenderPass(render_targets.size(), rts.data(), &dsdesc, D3D12_RENDER_PASS_FLAG_NONE);
        } else
            command_list->BeginRenderPass(render_targets.size(), rts.data(), nullptr, D3D12_RENDER_PASS_FLAG_NONE);
    }

    /// @brief Ends the render pass.
    void EndRenderPass() noexcept
    {
        command_list->EndRenderPass();
    }

    /// @brief Sets render targets for the command list. Only valid for DX12 command lists.
    /// @param rtvs Render targets to set.
    /// @param dsv Depth stencil target to set. (optional, unused)
    void OMSetRenderTargetsDX(std::span<const DX12RenderTarget> rtvs, void* dsv = nullptr) noexcept
    {
        command_list->OMSetRenderTargets(uint32_t(rtvs.size()), (const D3D12_CPU_DESCRIPTOR_HANDLE*)(rtvs.data()), false, (D3D12_CPU_DESCRIPTOR_HANDLE*)dsv);
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
        command_list->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
    }
    void DrawIndexedInstanced(uint32_t IndexCountPerInstance,
                              uint32_t InstanceCount = 1,
                              uint32_t StartIndexLocation = 0,
                              uint32_t StartVertexLocation = 0,
                              uint32_t StartInstanceLocation = 0) noexcept
    {
        command_list->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, StartVertexLocation, StartInstanceLocation);
    }

    void SetGraphicsDescriptorSet(DX12RootSignatureView root) noexcept
    {
        command_list->SetGraphicsRootSignature(std::get<0>(root));
    }
    void SetGraphicsDescriptorSet(DX12RootSignatureView root, uint32_t RootParameterIndex, DX12DescriptorSetBindView heap) noexcept
    {
        command_list->SetGraphicsRootSignature(std::get<0>(root));
        command_list->SetDescriptorHeaps(1, (ID3D12DescriptorHeap* const*)&std::get<0>(heap));
        command_list->SetGraphicsRootDescriptorTable(RootParameterIndex, std::get<1>(heap));
    }

private:
    DX12PipelineState pipeline_state{};
    bool closed = true;
};
} // namespace wis
