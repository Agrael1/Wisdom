#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_barrier.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_rtv.h>
#include <wisdom/dx12/dx12_resource.h>
#include <wisdom/dx12/dx12_pipeline_state.h>
#include <wisdom/dx12/dx12_root_signature.h>
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

		[[deprecated]]
		void ResourceBarrier(ResourceBarrier barrier, DX12BufferView resource)noexcept
		{
			std::vector<CD3DX12_RESOURCE_BARRIER> rb;
			if (barrier.type == BarrierType::transition)
				rb = TransitionBarrier(barrier, resource);
		
			if (rb.empty())return;
			command_list->ResourceBarrier(rb.size(), rb.data());
		}


		void TextureBarrier(std::initializer_list<std::pair<wis::ResourceBarrier, DX12BufferView>> barriers)noexcept //strengthened
		{
			return TextureBarrier(std::span{barriers.begin(), barriers.size()});
		}
		void TextureBarrier(std::span<const std::pair<wis::ResourceBarrier, DX12BufferView>> barriers)noexcept
		{

		}

		void ClearRenderTarget(DX12RenderTargetView rtv, std::span<const float, 4> color)noexcept
		{
			command_list->ClearRenderTargetView(rtv.GetInternal().GetHandle(), color.data(), 0, nullptr);
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
		std::vector<CD3DX12_RESOURCE_BARRIER> TransitionBarrier(wis::ResourceBarrier barrier, DX12BufferView resource)noexcept
		{
			if (barrier.before == barrier.after)return{};

			std::vector<CD3DX12_RESOURCE_BARRIER> barriers;
			auto desc = resource->GetDesc();
			auto before = D3D12_RESOURCE_STATES(barrier.before);
			auto after = D3D12_RESOURCE_STATES(barrier.after);
			auto flags = D3D12_RESOURCE_BARRIER_FLAGS(barrier.flags);

			if (barrier.base_mip_level == 0 && barrier.level_count == desc.MipLevels &&
				barrier.base_array_layer == 0 && barrier.layer_count == desc.DepthOrArraySize)
			{
				barriers.emplace_back(CD3DX12_RESOURCE_BARRIER::Transition(resource, before, after, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, flags));
				return barriers;
			}

			for (uint32_t i = barrier.base_mip_level; i < barrier.base_mip_level + barrier.level_count; ++i)
				for (uint32_t j = barrier.base_array_layer; j < barrier.base_array_layer + barrier.layer_count; ++j)
				{
					uint32_t subresource = i + j * desc.MipLevels;
					barriers.emplace_back(CD3DX12_RESOURCE_BARRIER::Transition(resource, before, after, subresource, flags));
				}

			return barriers;
		}
	private:
		bool closed = true;
	};
}