#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_barrier.h>
#include <wisdom/global/definitions.h>
#include <wisdom/vulkan/vk_resource.h>
#include <wisdom/vulkan/vk_root_signature.h>
#include <wisdom/vulkan/vk_format.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/vulkan/vk_pipeline_state.h>
#include <wisdom/vulkan/vk_render_pass.h>
#include <wisdom/vulkan/vk_rtv.h>
#include <span>
#include <wisdom/util/small_allocator.h>


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
	protected:
		wis::shared_handle<vk::CommandPool> allocator;
		vk::CommandBuffer command_list;
		VKPipelineState pipeline;
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
		/// @brief Bind a pipeline to the command list
		/// @param xpipeline Pipeline to bind
		void SetPipeline(VKPipelineState xpipeline)noexcept
		{
			pipeline = xpipeline;
		}

		/// @brief Reset the command list
		/// @return True if the command list is ready to be used
		bool Reset()noexcept
		{
			Close();

			vk::CommandBufferBeginInfo desc{};
			closed = false;
			command_list.begin(desc);
			if(pipeline)
				command_list.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.GetInternal().GetPipeline());
			return !closed;
		}

		/// @brief Check if the command list is closed
		/// @return True if the command list is closed
		[[nodiscard]] bool IsClosed()const noexcept
		{
			return closed;
		}

		/// @brief Close the command list
		/// If the command list is already closed, this function does nothing
		/// @return True if the command list was closed
		bool Close()noexcept
		{
			if (closed)return closed;
			command_list.end();
			return closed = true;
		}

		/// @brief Set a barrirer on a buffer to perform a transition.
		/// @param barrier Barrier description.
		/// @param buffer Buffer to set the barrier on.
		void BufferBarrier(wis::BufferBarrier barrier, VKBufferView buffer)noexcept
		{
			auto acc_before = convert_vk(barrier.access_before);
			auto acc_after = convert_vk(barrier.access_after);

			if (!buffer || acc_before == acc_after)
				return;

			vk::BufferMemoryBarrier desc
			{
				acc_before, acc_after, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, buffer, 0, VK_WHOLE_SIZE
			};
			command_list.pipelineBarrier(
				vk::PipelineStageFlagBits::eAllCommands, vk::PipelineStageFlagBits::eAllCommands,
				vk::DependencyFlagBits::eByRegion,
				0, nullptr,
				1, &desc, 0, nullptr);
		}

		/// @brief Set a barrier on a texture to perform a transition.
		/// @param barrier Barrier description.
		/// @param texture Texture to set the barrier on.
		void TextureBarrier(wis::TextureBarrier barrier, VKTextureView texture)noexcept
		{
			vk::ImageLayout vk_state_before = convert_vk(barrier.state_before);
			vk::ImageLayout vk_state_after = convert_vk(barrier.state_after);

			auto acc_before = convert_vk(barrier.access_before);
			auto acc_after = convert_vk(barrier.access_after);

			if (!texture.image || (vk_state_before == vk_state_after && acc_before == acc_after))
				return;

			vk::ImageMemoryBarrier image_memory_barrier
			{
				acc_before,
					acc_after,
					vk_state_before,
					vk_state_after,
					VK_QUEUE_FAMILY_IGNORED,
					VK_QUEUE_FAMILY_IGNORED,
					texture.image,
					convert_vk(barrier.range, texture.format)
			};
			command_list.pipelineBarrier(
				vk::PipelineStageFlagBits::eAllCommands, vk::PipelineStageFlagBits::eAllCommands,
				vk::DependencyFlagBits::eByRegion,
				0, nullptr,
				0, nullptr,
				1, &image_memory_barrier);
		}

		/// @brief Copy data from one buffer to another on GPU.
		/// Function does not check if the buffers are compatible, neither does it check if the data sizes are within the buffer sizes.
		/// This is done to allow for more flexibility and performance.
		/// @param source Source buffer.
		/// @param destination Destination buffer.
		/// @param data_size Size of the data to copy.
		void CopyBuffer(VKBufferView source, VKBufferView destination, size_t data_size)noexcept
		{
			vk::BufferCopy bufCopy{
				0, // srcOffset
				0, // dstOffset,
				data_size
			}; // size
			command_list.copyBuffer(source, destination, bufCopy);
		}
		
		/// @brief Sets the root signature for the command list.
		/// @param root Root signature to set.
		void SetGraphicsRootSignature(VKRootSignatureView root)noexcept
		{
			//command_list.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
			//	root, 0, {}, {});
		}
		
		/// @brief Set viewport for the command list.
		/// @param vp Viewport to set.
		void RSSetViewport(Viewport vp)noexcept
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
		void RSSetScissorRect(ScissorRect srect)noexcept
		{
			vk::Rect2D rect;
			rect.offset.x = srect.left;
			rect.offset.y = srect.top;
			rect.extent.width = srect.right;
			rect.extent.height = srect.bottom;
			command_list.setScissor(0, 1, &rect);
		}

		/// @brief Sets the primitive topology.
		/// @param vp Primitive topology to set.
		void IASetPrimitiveTopology(PrimitiveTopology topology)noexcept
		{
			command_list.setPrimitiveTopology(convert(topology)); //TODO: PatchList
		}

		/// @brief Binds vertex buffers to the command list.
		/// max 16 buffers can be bound.
		/// @param resources Vertex buffers to bind.
		/// @param start_slot Offset to start binding from.
		void IASetVertexBuffers(std::span<const VKVertexBufferView> resources, uint32_t start_slot = 0)noexcept
		{
			assert(resources.size() <= 16);
			std::array<vk::Buffer, 16> buffers{};
			std::array<vk::DeviceSize, 16> strides{};
			std::array<vk::DeviceSize, 16> sizes{};
			constexpr static std::array<vk::DeviceSize, 16> offsets{};

			for (size_t i = 0; i < resources.size(); i++)
			{
				auto& ii = resources[i].GetInternal();

				buffers[i] = ii.GetBufferWeak();
				sizes[i] = ii.SizeBytes();
				strides[i] = ii.StrideBytes();
			}

			command_list.bindVertexBuffers2(start_slot, uint32_t(resources.size()), buffers.data(), offsets.data(), sizes.data(), strides.data());
		}
		

		/// @brief Start a render pass.
		/// @param pass Pass description.
		/// @param render_targets Render targets to bind with colors to clear them with.
		void BeginRenderPass(wis::VKRenderPassView rp, 
			std::span<const std::pair<VKRenderTargetView, ColorClear>> render_targets)noexcept
		{
			wis::internals::uniform_allocator<vk::ImageView, max_render_targets> image_views;
			wis::internals::uniform_allocator<vk::ClearValue, max_render_targets> image_clear;
			for (auto& i : render_targets)
			{
				image_views.allocate(i.first.GetInternal().GetImageView());
				image_clear.allocate().setColor(i.second);
			}

			vk::RenderPassAttachmentBeginInfo attachment_begin_info
			{
				uint32_t(render_targets.size()),
				image_views.data()
			};
			vk::RenderPassBeginInfo render_pass_info{
				rp.pass, rp.frame, vk::Rect2D{{0,0},{rp.frame_size.width, rp.frame_size.height }}, uint32_t(image_clear.size()), image_clear.data(),&attachment_begin_info
			};
			command_list.beginRenderPass(render_pass_info, vk::SubpassContents::eInline);
		}

		/// @brief Ends the render pass.
		void EndRenderPass()noexcept
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
			uint32_t StartInstanceLocation = 0)noexcept
		{
			command_list.draw(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
		}
	private:
		bool closed = true;
	};
}