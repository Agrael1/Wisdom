#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_barrier.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_rtv.h>
#include <wisdom/dx12/dx12_format.h>
#include <wisdom/dx12/dx12_resource.h>
#include <wisdom/dx12/dx12_pipeline_state.h>
#include <wisdom/dx12/dx12_root_signature.h>
#include <wisdom/dx12/dx12_render_pass.h>
#include <d3d12.h>
#include <span>
#include <wisdom/api/api_common.h>


namespace wis
{
	class DX12CommandList;

	template<>
	class Internal<DX12CommandList>
	{
	public:
		ID3D12GraphicsCommandList9* GetCommandList()const noexcept
		{
			return command_list.get();
		}
		ID3D12CommandAllocator* GetCommandAllocator()const noexcept
		{
			return allocator.get();
		}
		ID3D12PipelineState* GetBoundState()const noexcept
		{
			return pipeline.get();
		}
	protected:
		winrt::com_ptr<ID3D12CommandAllocator> allocator;
		winrt::com_ptr<ID3D12GraphicsCommandList9> command_list;
		winrt::com_ptr<ID3D12PipelineState> pipeline;
	};

	class DX12CommandList : public QueryInternal<DX12CommandList>
	{
	public:
		DX12CommandList() = default;
		explicit DX12CommandList(winrt::com_ptr<ID3D12CommandAllocator> xallocator,
			winrt::com_ptr<ID3D12GraphicsCommandList9> xcommand_list)
		{
			allocator = std::move(xallocator);
			command_list = std::move(xcommand_list);
		}
	public:
		void SetPipeline(DX12PipelineStateView xpipeline)noexcept
		{
			pipeline.copy_from(xpipeline);
		}
		bool Reset()noexcept
		{
			return closed = !(wis::succeded_weak(allocator->Reset())
				&& wis::succeded_weak(command_list->Reset(allocator.get(), pipeline.get())));
		}
		[[nodiscard]] bool IsClosed()const noexcept
		{
			return closed;
		}
		bool Close()noexcept
		{
			if (closed)return closed;
			return closed = wis::succeded_weak(command_list->Close());
		}


		void BufferBarrier(wis::BufferBarrier barrier, DX12BufferView buffer)noexcept
		{
			CD3DX12_BUFFER_BARRIER bb
			{
				D3D12_BARRIER_SYNC_ALL,
				D3D12_BARRIER_SYNC_ALL,
				convert_dx(barrier.access_before),
				convert_dx(barrier.access_after),
				buffer
			};
			CD3DX12_BARRIER_GROUP bg{ 1, &bb };
			command_list->Barrier(1, &bg);
		}
		void TextureBarrier(wis::TextureBarrier barrier, DX12TextureView texture)noexcept
		{
			auto r = barrier.range;
			CD3DX12_TEXTURE_BARRIER tb
			{
				D3D12_BARRIER_SYNC_ALL, 
				D3D12_BARRIER_SYNC_ALL,
				convert_dx(barrier.access_before),
				convert_dx(barrier.access_after),
				convert_dx(barrier.state_before),
				convert_dx(barrier.state_after),
				texture,
				r.extent_mips == r.whole? CD3DX12_BARRIER_SUBRESOURCE_RANGE(r.whole):
				CD3DX12_BARRIER_SUBRESOURCE_RANGE(r.base_mip, r.extent_mips, r.base_layer, r.extent_layers)
			};
			CD3DX12_BARRIER_GROUP bg{ 1, &tb };
			command_list->Barrier(1, &bg);
		}

		void CopyBuffer(DX12BufferView source, DX12BufferView destination, size_t data_size)noexcept
		{
			command_list->CopyBufferRegion(destination, 0, source, 0, data_size);
		}

		void SetGraphicsRootSignature(DX12RootSignatureView root)noexcept
		{
			command_list->SetGraphicsRootSignature(root);
		}

		void RSSetViewport(Viewport vp)noexcept
		{
			command_list->RSSetViewports(1, (D3D12_VIEWPORT*)&vp);
		}
		void RSSetScissorRect(ScissorRect rect)noexcept
		{
			command_list->RSSetScissorRects(1, (D3D12_RECT*)&rect);
		}
		void IASetPrimitiveTopology(PrimitiveTopology vp)noexcept
		{
			command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY(vp));
		}
		void IASetVertexBuffers(std::span<const DX12VertexBufferView> resources, uint32_t start_slot = 0)noexcept
		{
			command_list->IASetVertexBuffers(start_slot, resources.size(), (const D3D12_VERTEX_BUFFER_VIEW*)resources.data());
		}

		//TODO: fill framebuffer
		void BeginRenderPass(DX12RenderPassView pass)
		{
			auto& i = pass.GetInternal();
			auto rts = i.GetRTDescs();
			command_list->BeginRenderPass(rts.size(), rts.data(), i.GetDSDesc(), D3D12_RENDER_PASS_FLAG_NONE);
		}

		void OMSetRenderTargets(std::span<const DX12RenderTargetView> rtvs, void* dsv = nullptr)noexcept
		{
			command_list->OMSetRenderTargets(uint32_t(rtvs.size()), (const D3D12_CPU_DESCRIPTOR_HANDLE*)(rtvs.data()), false, (D3D12_CPU_DESCRIPTOR_HANDLE*)dsv);
		}
		void DrawInstanced(uint32_t VertexCountPerInstance,
			uint32_t InstanceCount = 1,
			uint32_t StartVertexLocation = 0,
			uint32_t StartInstanceLocation = 0)noexcept
		{
			command_list->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
		}
	private:
		bool closed = true;
	};
}