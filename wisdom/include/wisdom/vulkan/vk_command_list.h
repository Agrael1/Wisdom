#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_barrier.h>
#include <wisdom/vulkan/vk_resource.h>
#include <wisdom/vulkan/vk_format.h>
#include <wisdom/vulkan/vk_checks.h>
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
			:allocator(std::move(allocator)), command_list(std::move(command_list)) {}
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

	using VKCommandListView = vk::CommandBuffer;

	class VKCommandList : public QueryInternal<VKCommandList>
	{
	public:
		VKCommandList() = default;
		explicit VKCommandList(wis::shared_handle<vk::CommandPool> allocator, vk::CommandBuffer command_list)
			:QueryInternal(std::move(allocator), std::move(command_list))
		{

		}
		operator VKCommandListView()const noexcept
		{
			return command_list;
		}
	public:
		//void SetPipeline(DX12PipelineStateView xpipeline)noexcept
		//{
		//	pipeline.copy_from(xpipeline);
		//}
		bool Reset()noexcept
		{
			Close();

			vk::CommandBufferBeginInfo desc{};
			closed = false;
			command_list.begin(desc);
			return !closed;
		}
		[[nodiscard]] bool IsClosed()const noexcept
		{
			return closed;
		}
		bool Close()noexcept
		{
			if (closed)return closed;
			command_list.end();
			return true;
		}


		void ResourceBarrier(ResourceBarrier barrier, VKTextureView texture)noexcept
		{
			std::optional<vk::ImageMemoryBarrier> imb;
			if (barrier.type == BarrierType::transition)
				imb = TransitionBarrier(barrier, texture);

			if (!imb)return;
			command_list.pipelineBarrier(
				vk::PipelineStageFlagBits::eAllCommands, vk::PipelineStageFlagBits::eAllCommands,
				vk::DependencyFlagBits::eByRegion,
				0, nullptr,
				0, nullptr,
				1, &imb.value());

		}

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
		std::optional<vk::ImageMemoryBarrier> TransitionBarrier(wis::ResourceBarrier barrier, VKTextureView texture)noexcept
		{
			vk::ImageLayout vk_state_before = convert_state(barrier.before);
			vk::ImageLayout vk_state_after = convert_state(barrier.after);

			if (!texture.image || barrier.before == barrier.after)
				return{};


			vk::ImageMemoryBarrier image_memory_barrier;
			image_memory_barrier.oldLayout = vk_state_before;
			image_memory_barrier.newLayout = vk_state_after;
			image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			image_memory_barrier.image = texture.image;

			vk::ImageSubresourceRange& range = image_memory_barrier.subresourceRange;
			range.aspectMask = aspect_flags(texture.format);
			range.baseMipLevel = barrier.base_mip_level;
			range.levelCount = barrier.level_count;
			range.baseArrayLayer = barrier.base_array_layer;
			range.layerCount = barrier.layer_count;

			// Source layouts (old)
			// Source access mask controls actions that have to be finished on the old layout
			// before it will be transitioned to the new layout
			switch (image_memory_barrier.oldLayout)
			{
			case vk::ImageLayout::eUndefined:
				// Image layout is undefined (or does not matter)
				// Only valid as initial layout
				// No flags required, listed only for completeness
				image_memory_barrier.srcAccessMask = {};
				break;
			case vk::ImageLayout::ePreinitialized:
				// Image is preinitialized
				// Only valid as initial layout for linear images, preserves memory contents
				// Make sure host writes have been finished
				image_memory_barrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
				break;
			case vk::ImageLayout::eColorAttachmentOptimal:
				// Image is a color attachment
				// Make sure any writes to the color buffer have been finished
				image_memory_barrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
				break;
			case vk::ImageLayout::eDepthAttachmentOptimal:
				// Image is a depth/stencil attachment
				// Make sure any writes to the depth/stencil buffer have been finished
				image_memory_barrier.srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
				break;
			case vk::ImageLayout::eTransferSrcOptimal:
				// Image is a transfer source 
				// Make sure any reads from the image have been finished
				image_memory_barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
				break;
			case vk::ImageLayout::eTransferDstOptimal:
				// Image is a transfer destination
				// Make sure any writes to the image have been finished
				image_memory_barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
				break;

			case vk::ImageLayout::eShaderReadOnlyOptimal:
				// Image is read by a shader
				// Make sure any shader reads from the image have been finished
				image_memory_barrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
				break;
			case vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR:
				image_memory_barrier.srcAccessMask = vk::AccessFlagBits::eFragmentShadingRateAttachmentReadKHR;
			default:
				// Other source layouts aren't handled (yet)
				break;
			}

			// Target layouts (new)
			// Destination access mask controls the dependency for the new image layout
			switch (image_memory_barrier.newLayout)
			{
			case vk::ImageLayout::eTransferDstOptimal:
				// Image will be used as a transfer destination
				// Make sure any writes to the image have been finished
				image_memory_barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
				break;

			case vk::ImageLayout::eTransferSrcOptimal:
				// Image will be used as a transfer source
				// Make sure any reads from the image have been finished
				image_memory_barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
				break;

			case vk::ImageLayout::eColorAttachmentOptimal:
				// Image will be used as a color attachment
				// Make sure any writes to the color buffer have been finished
				image_memory_barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
				break;

			case vk::ImageLayout::eDepthAttachmentOptimal:
				// Image layout will be used as a depth/stencil attachment
				// Make sure any writes to depth/stencil buffer have been finished
				image_memory_barrier.dstAccessMask = image_memory_barrier.dstAccessMask | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
				break;

			case vk::ImageLayout::eShaderReadOnlyOptimal:
				// Image will be read in a shader (sampler, input attachment)
				// Make sure any writes to the image have been finished
				if (!image_memory_barrier.srcAccessMask)
				{
					image_memory_barrier.srcAccessMask = vk::AccessFlagBits::eHostWrite | vk::AccessFlagBits::eTransferWrite;
				}
				image_memory_barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
				break;
			case vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR:
				image_memory_barrier.dstAccessMask = vk::AccessFlagBits::eFragmentShadingRateAttachmentReadKHR;
				break;
			default:
				// Other source layouts aren't handled (yet)
				break;
			}

			return image_memory_barrier;
		}
	private:
		bool closed = true;
	};
}