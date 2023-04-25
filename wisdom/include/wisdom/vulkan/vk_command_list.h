#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_barrier.h>
#include <span>


namespace wis
{
	class VKCommandList;

	template<>
	class Internal<VKCommandList>
	{
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::CommandPool> allocator, vk::CommandBuffer command_list)
			:allocator(std::move(allocator)), command_list(std::move(command_list)){}
	public:
		//ID3D12GraphicsCommandList9* GetCommandList()const noexcept
		//{
		//	return command_list.get();
		//}
		//ID3D12CommandAllocator* GetCommandAllocator()const noexcept
		//{
		//	return allocator.get();
		//}
		//ID3D12PipelineState* GetBoundState()const noexcept
		//{
		//	return pipeline.get();
		//}
	protected:
		wis::shared_handle<vk::CommandPool> allocator;
		vk::CommandBuffer command_list;
		//winrt::com_ptr<ID3D12PipelineState> pipeline;
	};

	class VKCommandList : public QueryInternal<VKCommandList>
	{
	public:
		VKCommandList() = default;
		explicit VKCommandList(wis::shared_handle<vk::CommandPool> allocator, vk::CommandBuffer command_list)
			:QueryInternal(std::move(allocator), std::move(command_list))
		{

		}
	public:
		//void SetPipeline(DX12PipelineStateView xpipeline)noexcept
		//{
		//	pipeline.copy_from(xpipeline);
		//}
		//bool Reset()noexcept
		//{
		//	return closed = !(wis::succeded_weak(allocator->Reset())
		//		&& wis::succeded_weak(command_list->Reset(allocator.get(), pipeline.get())));
		//}
		//[[nodiscard]] bool IsClosed()const noexcept
		//{
		//	return closed;
		//}
		//bool Close()noexcept
		//{
		//	if (closed)return closed;
		//	return closed = wis::succeded_weak(command_list->Close());
		//}
		//
		//// TODO: span<barriers>, barrier multitype
		//template<BarrierType ty>
		//void ResourceBarrier(ResourceBarrier<ty, DX12Resource>&& barrier)noexcept
		//{
		//	if constexpr (ty == BarrierType::transition)
		//		return TransitionBarrier(std::forward<wis::ResourceBarrier<ty, DX12Resource>>(barrier));
		//}
		//
		//void ClearRenderTarget(DX12RenderTargetView rtv, std::span<const float, 4> color)noexcept
		//{
		//	command_list->ClearRenderTargetView(rtv.GetInternal().GetHandle(), color.data(), 0, nullptr);
		//}
		//
		//void CopyBuffer(DX12ResourceView source, DX12ResourceView destination, size_t data_size)noexcept
		//{
		//	command_list->CopyBufferRegion(destination, 0, source, 0, data_size);
		//}
		//
		//void SetGraphicsRootSignature(DX12RootSignatureView root)noexcept
		//{
		//	command_list->SetGraphicsRootSignature(root);
		//}
		//
		//void RSSetViewport(Viewport vp)noexcept
		//{
		//	command_list->RSSetViewports(1, (D3D12_VIEWPORT*)&vp);
		//}
		//void RSSetScissorRect(ScissorRect rect)noexcept
		//{
		//	command_list->RSSetScissorRects(1, (D3D12_RECT*)&rect);
		//}
		//void IASetPrimitiveTopology(PrimitiveTopology vp)noexcept
		//{
		//	command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY(vp));
		//}
		//void IASetVertexBuffers(std::span<const DX12VertexBufferView> resources, uint32_t start_slot = 0)noexcept
		//{
		//	command_list->IASetVertexBuffers(start_slot, resources.size(), (const D3D12_VERTEX_BUFFER_VIEW*)resources.data());
		//}
		//
		//void OMSetRenderTargets(std::span<const DX12RenderTargetView> rtvs, void* dsv = nullptr)noexcept
		//{
		//	command_list->OMSetRenderTargets(uint32_t(rtvs.size()), (const D3D12_CPU_DESCRIPTOR_HANDLE*)(rtvs.data()), false, (D3D12_CPU_DESCRIPTOR_HANDLE*)dsv);
		//}
		//void DrawInstanced(uint32_t VertexCountPerInstance,
		//	uint32_t InstanceCount = 1,
		//	uint32_t StartVertexLocation = 0,
		//	uint32_t StartInstanceLocation = 0)noexcept
		//{
		//	command_list->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
		//}
	private:
		//void TransitionBarrier(BasicTransitionBarrier<DX12Resource>&& barrier)noexcept
		//{
		//	auto b = CD3DX12_RESOURCE_BARRIER::Transition(
		//		barrier.resource.GetInternal().GetResource(),
		//		D3D12_RESOURCE_STATES(barrier.before),
		//		D3D12_RESOURCE_STATES(barrier.after),
		//		barrier.subresource, D3D12_RESOURCE_BARRIER_FLAGS(barrier.flags)
		//	);
		//	command_list->ResourceBarrier(1, &b);
		//}
	private:
		bool closed = true;
	};
}