#ifndef VK_COMMAND_LIST_CPP
#define VK_COMMAND_LIST_CPP
#include <wisdom/vulkan/vk_command_list.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/generated/vulkan/vk_structs.hpp>
#include <wisdom/util/small_allocator.h>

void wis::VKCommandList::CopyBuffer(VKBufferView source, VKBufferView destination, wis::BufferRegion region) const noexcept
{
    VkBufferCopy copy{
        .srcOffset = region.src_offset,
        .dstOffset = region.dst_offset,
        .size = region.size_bytes,
    };
    device.table().vkCmdCopyBuffer(command_list, std::get<0>(source), std::get<0>(destination), 1, &copy);
}

void wis::VKCommandList::CopyBufferToTexture(VKBufferView src_buffer, VKTextureView dest_texture, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) const noexcept
{
    wis::detail::limited_allocator<VkBufferImageCopy2, 8> allocator(region_count, true);
    auto* copies = allocator.data();

    for (size_t i = 0; i < region_count; i++) {
        auto& region = regions[i];
        copies[i] = VkBufferImageCopy2{
            .sType = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2,
            .bufferOffset = region.buffer_offset,
            .bufferRowLength = {},
            .bufferImageHeight = {},
            .imageSubresource = {
                    .aspectMask = aspect_flags(std::get<1>(dest_texture)),
                    .mipLevel = region.texture.mip,
                    .baseArrayLayer = region.texture.array_layer,
                    .layerCount = 1u,
            },
            .imageOffset = { int(region.texture.offset.width), int(region.texture.offset.height), int(region.texture.offset.depth_or_layers) },
            .imageExtent = { region.texture.size.width, region.texture.size.height, region.texture.size.depth_or_layers },
        };
    }

    VkCopyBufferToImageInfo2 copy{
        .sType = VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2,
        .pNext = nullptr,
        .srcBuffer = std::get<0>(src_buffer),
        .dstImage = std::get<0>(dest_texture),
        .dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .regionCount = region_count,
        .pRegions = copies,
    };
    device.table().vkCmdCopyBufferToImage2(command_list, &copy);
}

void wis::VKCommandList::CopyTextureToBuffer(VKTextureView src_texture, VKBufferView dst_buffer, const wis::BufferTextureCopyRegion* regions, uint32_t region_count) const noexcept
{
    wis::detail::limited_allocator<VkBufferImageCopy2, 8> allocator(region_count, true);
    auto* copies = allocator.data();

    for (size_t i = 0; i < region_count; i++) {
        auto& region = regions[i];
        copies[i] = VkBufferImageCopy2{
            .sType = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2,
            .bufferOffset = region.buffer_offset,
            .bufferRowLength = {},
            .bufferImageHeight = {},
            .imageSubresource = {
                    .aspectMask = aspect_flags(std::get<1>(src_texture)),
                    .mipLevel = region.texture.mip,
                    .baseArrayLayer = region.texture.array_layer,
                    .layerCount = 1u,
            },
            .imageOffset = { int(region.texture.offset.width), int(region.texture.offset.height), int(region.texture.offset.depth_or_layers) },
            .imageExtent = { region.texture.size.width, region.texture.size.height, region.texture.size.depth_or_layers },
        };
    }

    VkCopyImageToBufferInfo2 copy{
        .sType = VK_STRUCTURE_TYPE_COPY_IMAGE_TO_BUFFER_INFO_2,
        .pNext = nullptr,
        .srcImage = std::get<0>(src_texture),
        .srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        .dstBuffer = std::get<0>(dst_buffer),
        .regionCount = region_count,
        .pRegions = copies,
    };

    device.table().vkCmdCopyImageToBuffer2(command_list, &copy);
}

wis::Result wis::VKCommandList::Reset(VKPipelineHandle new_pipeline) noexcept
{
    Close();

    auto& dtable = device.table();

    auto result = dtable.vkResetCommandBuffer(command_list, {});
    if (!succeeded(result)) {
        return make_result<FUNC, "vkResetCommandBuffer failed">(result);
    }
    pipeline = std::move(std::get<0>(new_pipeline));

    VkCommandBufferBeginInfo desc{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = {},
        .pInheritanceInfo = nullptr,
    };
    result = dtable.vkBeginCommandBuffer(command_list, &desc);
    if (!succeeded(result)) {
        return make_result<FUNC, "vkBeginCommandBuffer failed">(result);
    }
    closed = false;
    if (pipeline)
        dtable.vkCmdBindPipeline(command_list, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());
    return wis::success;
}

bool wis::VKCommandList::Close() noexcept
{
    if (closed) {
        return true;
    }
    return closed = succeeded(device.table().vkEndCommandBuffer(command_list));
}

namespace wis::detail {
inline VkBufferMemoryBarrier2 to_vk(wis::BufferBarrier barrier, VkBuffer buffer) noexcept
{
    return VkBufferMemoryBarrier2{
        .sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2,
        .pNext = nullptr,
        .srcStageMask = convert_vk(barrier.sync_before),
        .srcAccessMask = convert_vk(barrier.access_before),
        .dstStageMask = convert_vk(barrier.sync_after),
        .dstAccessMask = convert_vk(barrier.access_after),
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .buffer = buffer,
        .offset = barrier.offset,
        .size = barrier.size,
    };
}
inline VkImageMemoryBarrier2 to_vk(wis::TextureBarrier barrier, VkImage texture, VkFormat format) noexcept
{
    auto& subresource = barrier.subresource_range;
    return VkImageMemoryBarrier2{
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .pNext = nullptr,
        .srcStageMask = convert_vk(barrier.sync_before),
        .srcAccessMask = convert_vk(barrier.access_before),
        .dstStageMask = convert_vk(barrier.sync_after),
        .dstAccessMask = convert_vk(barrier.access_after),
        .oldLayout = convert_vk(barrier.state_before),
        .newLayout = convert_vk(barrier.state_after),
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = texture,
        .subresourceRange = {
                .aspectMask = aspect_flags(format),
                .baseMipLevel = subresource.base_mip_level,
                .levelCount = subresource.level_count,
                .baseArrayLayer = subresource.base_array_layer,
                .layerCount = subresource.layer_count,
        }
    };
}
} // namespace wis::detail

void wis::VKCommandList::BufferBarrier(wis::BufferBarrier barrier, VKBufferView buffer) noexcept
{
    auto hbuffer = std::get<0>(buffer);
    if (!hbuffer)
        return;

    VkBufferMemoryBarrier2 desc = detail::to_vk(barrier, hbuffer);
    VkDependencyInfo depinfo{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
        .bufferMemoryBarrierCount = 1,
        .pBufferMemoryBarriers = &desc,
    };
    device.table().vkCmdPipelineBarrier2(command_list, &depinfo);
}

void wis::VKCommandList::BufferBarriers(const wis::VKBufferBarrier2* barriers, uint32_t barrier_count) noexcept
{
    wis::detail::limited_allocator<VkBufferMemoryBarrier2, 8> allocator(barrier_count, true);
    auto* data = allocator.data();

    for (size_t i = 0; i < barrier_count; i++) {
        data[i] = detail::to_vk(barriers[i].barrier, std::get<0>(barriers[i].buffer));
    }
    VkDependencyInfo depinfo{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
        .bufferMemoryBarrierCount = barrier_count,
        .pBufferMemoryBarriers = data,
    };
    device.table().vkCmdPipelineBarrier2(command_list, &depinfo);
}

void wis::VKCommandList::TextureBarrier(wis::TextureBarrier barrier, VKTextureView texture) noexcept
{
    auto htexture = std::get<0>(texture);
    if (!htexture)
        return;

    VkImageMemoryBarrier2 image_memory_barrier = detail::to_vk(barrier, htexture, std::get<1>(texture));
    VkDependencyInfo depinfo{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &image_memory_barrier,
    };
    device.table().vkCmdPipelineBarrier2(command_list, &depinfo);
}

void wis::VKCommandList::TextureBarriers(const wis::VKTextureBarrier2* barriers, uint32_t barrier_count) noexcept
{
    wis::detail::limited_allocator<VkImageMemoryBarrier2, 8> allocator(barrier_count, true);
    auto* data = allocator.data();

    for (size_t i = 0; i < barrier_count; i++) {
        data[i] = detail::to_vk(barriers[i].barrier, std::get<0>(barriers[i].texture), std::get<1>(barriers[i].texture));
    }
    VkDependencyInfo depinfo{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
        .imageMemoryBarrierCount = barrier_count,
        .pImageMemoryBarriers = data,
    };
    device.table().vkCmdPipelineBarrier2(command_list, &depinfo);
}

void wis::VKCommandList::BeginRenderPass(const wis::VKRenderPassDesc* pass_desc) noexcept
{
    auto ds_selector = pass_desc->depth_stencil ? pass_desc->depth_stencil->depth_stencil_select : DSSelect::None;

    auto& dtable = device.table();
    wis::detail::limited_allocator<VkRenderingAttachmentInfo, 8> allocator(pass_desc->target_count, true);
    auto* data = allocator.data();
    wis::Size2D extent = std::get<1>(pass_desc->targets[0].target);

    for (size_t i = 0; i < pass_desc->target_count; i++) {
        auto& target = pass_desc->targets[i];
        data[i] = VkRenderingAttachmentInfo{
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .pNext = nullptr,
            .imageView = std::get<0>(target.target),
            .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .loadOp = convert_vk(target.load_op),
            .storeOp = convert_vk(target.store_op),
        };
        if (data[i].loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR)
            data[i].clearValue = {
                .color = { .float32{ target.clear_value[0], target.clear_value[1], target.clear_value[2], target.clear_value[3] } }
            };
    }

    VkRenderingAttachmentInfo d_info{};
    VkRenderingAttachmentInfo s_info{};
    if (ds_selector & DSSelect::Depth)
    {
        d_info = {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .pNext = nullptr,
            .imageView = std::get<0>(pass_desc->depth_stencil->target),
            .imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
            .loadOp = convert_vk(pass_desc->depth_stencil->load_op_depth),
            .storeOp = convert_vk(pass_desc->depth_stencil->store_op_depth),
        };
        if (d_info.loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR)
            d_info.clearValue = {
                .depthStencil = { .depth = pass_desc->depth_stencil->clear_depth, .stencil = pass_desc->depth_stencil->clear_stencil }
            };
    }
    if (ds_selector & DSSelect::Stencil)
    {
        s_info = {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .pNext = nullptr,
            .imageView = std::get<0>(pass_desc->depth_stencil->target),
            .imageLayout = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL,
            .loadOp = convert_vk(pass_desc->depth_stencil->load_op_stencil),
            .storeOp = convert_vk(pass_desc->depth_stencil->store_op_stencil),
        };
        if (s_info.loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR)
            s_info.clearValue = {
                .depthStencil = { .depth = pass_desc->depth_stencil->clear_depth, .stencil = pass_desc->depth_stencil->clear_stencil }
            };
    }

    VkRenderingInfo info{
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .pNext = nullptr,
        .flags = convert_vk(pass_desc->flags),
        .renderArea = {
                .offset = { 0, 0 },
                .extent = { extent.width, extent.height },
        },
        .layerCount = 1,
        .viewMask = 0,
        .colorAttachmentCount = pass_desc->target_count,
        .pColorAttachments = data,
        .pDepthAttachment = ds_selector & DSSelect::Depth ? &d_info : nullptr,
        .pStencilAttachment = ds_selector & DSSelect::Stencil ? &s_info : nullptr,
    };

    dtable.vkCmdBeginRendering(command_list, &info);
}

void wis::VKCommandList::EndRenderPass() noexcept
{
    device.table().vkCmdEndRendering(command_list);
}

void wis::VKCommandList::RSSetViewport(wis::Viewport vp) noexcept
{
    VkViewport vkvp{
        .x = vp.top_leftx,
        .y = vp.top_lefty + vp.height,
        .width = vp.width,
        .height = -vp.height,
        .minDepth = vp.min_depth,
        .maxDepth = vp.max_depth,
    };
    device.table().vkCmdSetViewport(command_list, 0, 1, &vkvp);
}
void wis::VKCommandList::RSSetViewports(const wis::Viewport* vp, uint32_t count) noexcept
{
    wis::detail::limited_allocator<VkViewport, 8> allocator(count, true);
    auto* viewports = allocator.data();

    for (size_t i = 0; i < count; i++) {
        viewports[i] = {
            .x = vp[i].top_leftx,
            .y = vp[i].top_lefty + vp[i].height,
            .width = vp[i].width,
            .height = -vp[i].height,
            .minDepth = vp[i].min_depth,
            .maxDepth = vp[i].max_depth,
        };
    }
    device.table().vkCmdSetViewport(command_list, 0, count, viewports);
}

void wis::VKCommandList::RSSetScissor(wis::Scissor scissor) noexcept
{
    VkRect2D rect{
        .offset = { scissor.left, scissor.top },
        .extent = { uint32_t(scissor.right - scissor.left), uint32_t(scissor.bottom - scissor.top) },
    };
    device.table().vkCmdSetScissor(command_list, 0, 1, &rect);
}

void wis::VKCommandList::IASetPrimitiveTopology(wis::PrimitiveTopology topology) noexcept
{
    device.table().vkCmdSetPrimitiveTopology(command_list, convert_vk(topology));
}

void wis::VKCommandList::SetRootSignature(wis::VKRootSignatureView root_signature) noexcept
{
    pipeline_layout = std::get<0>(root_signature);
}

void wis::VKCommandList::IASetVertexBuffers(wis::VKVertexBufferBinding* resources, uint32_t count, uint32_t start_slot) noexcept
{
    wis::detail::limited_allocator<VkBuffer, 8> allocator(count, true);
    auto* buffers = allocator.data();

    wis::detail::limited_allocator<VkDeviceSize, 8> offset_allocator(count, true);
    auto* offsets = offset_allocator.data();

    wis::detail::limited_allocator<VkDeviceSize, 8> size_allocator(count, true);
    auto* sizes = size_allocator.data();

    wis::detail::limited_allocator<VkDeviceSize, 8> stride_allocator(count, true);
    auto* strides = stride_allocator.data();

    for (size_t i = 0; i < count; i++) {
        buffers[i] = std::get<0>(resources[i].buffer);
        sizes[i] = resources[i].size;
        strides[i] = resources[i].stride;
    }
    device.table().vkCmdBindVertexBuffers2(command_list, start_slot, count, buffers, offsets, sizes, strides);
}

void wis::VKCommandList::DrawIndexedInstanced(uint32_t vertex_count_per_instance,
                                              uint32_t instance_count,
                                              uint32_t start_index,
                                              uint32_t base_vertex,
                                              uint32_t start_instance) noexcept
{
    device.table().vkCmdDrawIndexed(command_list, vertex_count_per_instance, instance_count, start_index, base_vertex, start_instance);
}

void wis::VKCommandList::DrawInstanced(uint32_t vertex_count_per_instance,
                                       uint32_t instance_count,
                                       uint32_t base_vertex,
                                       uint32_t start_instance) noexcept
{
    device.table().vkCmdDraw(command_list, vertex_count_per_instance, instance_count, base_vertex, start_instance);
}

void wis::VKCommandList::SetRootConstants(const void* data, uint32_t size_4bytes, uint32_t offset_4bytes, wis::ShaderStages stage) noexcept
{
    device.table().vkCmdPushConstants(command_list, pipeline_layout, convert_vk(stage), offset_4bytes * 4, size_4bytes * 4, data);
}

void wis::VKCommandList::SetDescriptorBuffers(const wis::VKDescriptorBufferView* buffers, uint32_t buffer_count) noexcept
{
    if (buffer_count == 0)
        return;

    VkDescriptorBufferBindingInfoEXT infos[2] = {
        {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_BUFFER_BINDING_INFO_EXT,
                .usage = VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT,
        },

        {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_BUFFER_BINDING_INFO_EXT,
                .usage = VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT,
        }
    };

    uint8_t id = false;

    for (size_t i = 0; i < buffer_count; i++) {
        auto& buffer = buffers[i];
        auto address = std::get<0>(buffer);
        if (std::get<1>(buffer) == wis::DescriptorHeapType::Descriptor) {
            infos[0].address = address;
            id |= 1;
        } else {
            infos[1].address = address;
            id |= 2;
        }
    }

    auto* pinfos = infos + (id == 2);
    uint32_t xbuffer_count = 1 + uint32_t(id == 3);

    device.table().vkCmdBindDescriptorBuffersEXT(command_list, xbuffer_count, pinfos);
}

void wis::VKCommandList::SetDescriptorTableOffset(uint32_t root_table_index, wis::VKDescriptorBufferView buffer, uint32_t offset_bytes) noexcept
{
    auto binding = std::get<1>(buffer);
    uint32_t index = uint32_t(binding == wis::DescriptorHeapType::Sampler);
    VkDeviceSize offset = offset_bytes;

    device.table().vkCmdSetDescriptorBufferOffsetsEXT(command_list, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, root_table_index, 1,
                                                      &index, &offset);
}

#endif // !
