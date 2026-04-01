#ifndef WIS_VK_COMMAND_LIST_CPP
#define WIS_VK_COMMAND_LIST_CPP
#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/util/allocation.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>

#include <bit>

namespace wis::detail {
constexpr static uint32_t vk_max_barrier_size = std::max(
    {sizeof(VkBufferMemoryBarrier), sizeof(VkImageMemoryBarrier2), sizeof(VkMemoryBarrier2)}
);
constexpr static uint32_t vk_static_size = wis::TransientMaxBarrierCount * vk_max_barrier_size;

inline uint8_t* VKAllocateScratchSpace(const wis::impl::VKCommandListImpl& impl, uint32_t new_size)
{
    if (new_size > impl.scratch_memory_size) {
        delete[] impl.scratch_memory;
        impl.scratch_memory = new (std::nothrow) uint8_t[new_size];
    }
    return impl.scratch_memory;
}
inline constexpr VkImageAspectFlags VKExtractAspectFlags(
    WisSubresourceRange subresource,
    WisBarrierFlags flags
) noexcept
{
    VkImageAspectFlags aspect_flags = 0;
    if (flags & WisBarrierFlagsDepthResource) {
        aspect_flags |= VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    if (flags & WisBarrierFlagsStencilResource) {
        aspect_flags |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    if (aspect_flags != 0) {
        // If depth or stencil specified, ignore plane slice and return early
        // since depth/stencil views of multi-planar formats are not allowed to have a plane slice.
        return aspect_flags;
    }

    if ((flags & WisBarrierFlagsPlanarImage) == 0) {
        return VK_IMAGE_ASPECT_COLOR_BIT; // If not a planar image, return color aspect for simplicity.
    }

    for (uint16_t plane = subresource.plane_slice; plane < subresource.plane_slice_count; ++plane) {
        aspect_flags |= VK_IMAGE_ASPECT_PLANE_0_BIT << plane;
    }
    return aspect_flags;
}

inline std::array<wis::span<uint8_t>, 3> VKAllocateBarriers(
    const wis::impl::VKCommandListImpl& impl,
    uint8_t* local_scratch,
    const WisVKBarrierGroup& barriers
)
{
    std::array<wis::span<uint8_t>, 3> spans;
    std::size_t needed_size = barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)
                            + barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)
                            + barriers.global_barrier_count * sizeof(VkMemoryBarrier2);

    if (needed_size <= vk_static_size) {
        spans[0] = {local_scratch, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {spans[0].end(), barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {spans[1].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    }

    std::size_t sizes[] = {
        barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2),
        barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2),
        barriers.global_barrier_count * sizeof(VkMemoryBarrier2),
        0,
        0,
        0
    };

    sizes[3] = sizes[0] + sizes[1];
    sizes[4] = sizes[1] + sizes[2];
    sizes[5] = sizes[0] + sizes[2];

    // find closest value from below
    uint32_t closest_size = 0;
    int index = -1;
    for (int i = std::size(sizes) - 1; i >= 0; --i) {
        if (sizes[i] > vk_static_size) {
            continue;
        }

        // less than or equal to static size, check if it's the closest one
        if (vk_static_size - sizes[i] < vk_static_size - closest_size) {
            closest_size = sizes[i];
            index = i;
        }
    }

    uint32_t allocated_size = needed_size - closest_size;

    // allocate from the command list's scratch memory if the needed size exceeds the local scratch buffer size. This is
    // to avoid large stack allocations.
    auto* allocated_data = wis::detail::VKAllocateScratchSpace(impl, allocated_size);

    // set pointers to the right offsets in the allocated scratch memory
    switch (index) {
    default:
    case -1:
        // no single group can fit into the local scratch, allocate all from the command list's scratch memory
        spans[0] = {allocated_data, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {spans[0].end(), barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {spans[1].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    case 0:
        // buffer barriers fit into local scratch, texture and global barriers allocated from command list's scratch
        spans[0] = {local_scratch, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {allocated_data, barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {spans[1].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    case 1:
        // texture barriers fit into local scratch, buffer and global barriers allocated from command list's scratch
        spans[0] = {allocated_data, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {local_scratch, barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {spans[0].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    case 2:
        // global barriers fit into local scratch, buffer and texture barriers allocated from command list's scratch
        spans[0] = {allocated_data, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {spans[0].end(), barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {local_scratch, barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    case 3:
        spans[0] = {local_scratch, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {spans[0].end(), barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {allocated_data, barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    case 4:
        spans[0] = {allocated_data, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {local_scratch, barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {spans[1].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    case 5:
        spans[0] = {local_scratch, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2)};
        spans[1] = {allocated_data, barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2)};
        spans[2] = {spans[0].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2)};
        return spans;
    }
}
} // namespace wis::detail

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyCommandList(WisVKCommandList* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    if (impl.command_buffer != VK_NULL_HANDLE) {
        // free command buffer
        auto& header = impl.command_pool_header->header;
        impl.command_list_table->vkFreeCommandBuffers(header.device, header.command_pool, 1, &impl.command_buffer);
        impl.command_buffer = VK_NULL_HANDLE;

        wis::detail::VKReleaseCommandPool(impl.command_pool_header);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandListBegin(const WisVKCommandList* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);

    VkCommandBufferBeginInfo begin_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0, // Optional flags can be set here, e.g., VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        .pInheritanceInfo = nullptr, // Optional, only relevant for secondary command buffers
    };
    auto vr = impl.command_list_table->vkBeginCommandBuffer(impl.command_buffer, &begin_info);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to begin Vulkan command buffer recording">(vr);
    }
    return wis::detail::vk_success;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandListEnd(const WisVKCommandList* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto vr = impl.command_list_table->vkEndCommandBuffer(impl.command_buffer);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to end Vulkan command buffer recording">(vr);
    }
    return wis::detail::vk_success;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetDescriptorHeaps(
    const WisVKCommandList* self,
    const WisVKDescriptorHeap* resource_heap,
    const WisVKDescriptorHeap* sampler_heap
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);

    if (resource_heap) {
        auto& res_heap = wis::from_handle_ref<const wis::impl::VKDescriptorHeapImpl>(resource_heap);
        VkDeviceSize reserved_resource_descriptor_size = static_cast<std::size_t>(res_heap.reserved_size)
                                                       * res_heap.descriptor_size;
        VkDeviceSize total_resource_heap_size = static_cast<std::size_t>(res_heap.heap_size) * res_heap.descriptor_size
                                              + reserved_resource_descriptor_size;
        VkBindHeapInfoEXT bind_resource_info{
            .sType = VK_STRUCTURE_TYPE_BIND_HEAP_INFO_EXT,
            .pNext = nullptr,
            .heapRange = {res_heap.gpu_address, total_resource_heap_size},
            .reservedRangeOffset = 0,
            .reservedRangeSize = reserved_resource_descriptor_size,
        };
        impl.command_list_table->vkCmdBindResourceHeapEXT(impl.command_buffer, &bind_resource_info);
    }

    if (sampler_heap) {
        auto& samp_heap = wis::from_handle_ref<const wis::impl::VKDescriptorHeapImpl>(sampler_heap);
        VkDeviceSize reserved_sampler_descriptor_size = static_cast<std::size_t>(samp_heap.reserved_size)
                                                      * samp_heap.descriptor_size;
        VkDeviceSize total_sampler_heap_size = static_cast<std::size_t>(samp_heap.heap_size) * samp_heap.descriptor_size
                                             + reserved_sampler_descriptor_size;
        VkBindHeapInfoEXT bind_sampler_info{
            .sType = VK_STRUCTURE_TYPE_BIND_HEAP_INFO_EXT,
            .pNext = nullptr,
            .heapRange = {samp_heap.gpu_address, total_sampler_heap_size}, // Not used for bindless heaps
            .reservedRangeOffset = 0,
            .reservedRangeSize = reserved_sampler_descriptor_size,
        };
        impl.command_list_table->vkCmdBindSamplerHeapEXT(impl.command_buffer, &bind_sampler_info);
    }
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetRootSignature(
    const WisVKCommandList* self,
    WisVKRootSignatureView signature,
    WisPipelineType pipeline
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto* sig = std::bit_cast<wis::detail::VKRootSignatureControlBlock*>(signature);
    impl.root_signature_header = sig;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetPushConstants(
    const WisVKCommandList* self,
    const WisPushConstantDataDesc* data
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto* sig = impl.root_signature_header;

    uint32_t push_constant_offset = sig->GetRootBindingOffsets()[data->root_index] + data->push_offset / 4;
    VkPushDataInfoEXT push_data_info{
        .sType = VK_STRUCTURE_TYPE_PUSH_DATA_INFO_EXT,
        .pNext = nullptr,
        .offset = push_constant_offset,
        .data = {.address = data->data, .size = data->data_size}
    };
    impl.command_list_table->vkCmdPushDataEXT(impl.command_buffer, &push_data_info);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetPushDescriptor(
    const WisVKCommandList* self,
    const WisPushDescriptorDataDesc* data
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto* sig = impl.root_signature_header;

    uint32_t push_desc_offset = sig->GetRootBindingOffsets()[data->root_index];
    VkPushDataInfoEXT push_data_info{
        .sType = VK_STRUCTURE_TYPE_PUSH_DATA_INFO_EXT,
        .pNext = nullptr,
        .offset = push_desc_offset,
        .data = {.address = &data->buffer_address, .size = sizeof(uint64_t)}
    };
    impl.command_list_table->vkCmdPushDataEXT(impl.command_buffer, &push_data_info);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetDescriptorTable(
    const WisVKCommandList* self,
    const WisDescriptorTableDataDesc* data
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto* sig = impl.root_signature_header;

    uint32_t push_desc_offset = sig->GetRootBindingOffsets()[data->root_index];
    VkPushDataInfoEXT push_data_info{
        .sType = VK_STRUCTURE_TYPE_PUSH_DATA_INFO_EXT,
        .pNext = nullptr,
        .offset = push_desc_offset,
        .data = {.address = &data->heap_offset, .size = sizeof(uint32_t)}
    };
    impl.command_list_table->vkCmdPushDataEXT(impl.command_buffer, &push_data_info);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListInsertBarriers(
    const WisVKCommandList* self,
    const WisVKBarrierGroup* barriers
)
{
    // clang-format off
    if (barriers->buffer_barrier_count +
        barriers->texture_barrier_count +
        barriers->global_barrier_count == 0) {
        return;
    }
    // clang-format on

    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto& device_header = impl.command_pool_header->header;
    uint8_t local_scratch[wis::detail::vk_static_size]{};

    auto [buffer_span, texture_span, global_span] = wis::detail::VKAllocateBarriers(impl, local_scratch, *barriers);

    wis::span<VkBufferMemoryBarrier2> buffer_barriers_span{
        reinterpret_cast<VkBufferMemoryBarrier2*>(buffer_span.data()),
        barriers->buffer_barrier_count
    };
    uint32_t real_buffer_barrier_count = barriers->buffer_barrier_count;

    for (size_t i = 0; i < barriers->buffer_barrier_count; i++) {
        const auto& src = barriers->buffer_barriers[i];

        auto q1 = VK_QUEUE_FAMILY_IGNORED;
        auto q2 = VK_QUEUE_FAMILY_IGNORED;
        if (src.queue_type_before != src.queue_type_after) {
            // skip barriers that only perform queue ownership transfer
            if (impl.maintenance9) {
                real_buffer_barrier_count--;
                continue;
            }
            q1 = impl.queue_indices[src.queue_type_before].family_index;
            q2 = impl.queue_indices[src.queue_type_after].family_index;
        }

        buffer_barriers_span[i] = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2,
            .pNext = nullptr,
            .srcStageMask = wis::detail::VKConvert(src.sync_before),
            .srcAccessMask = wis::detail::VKConvert(src.access_before),
            .dstStageMask = wis::detail::VKConvert(src.sync_after),
            .dstAccessMask = wis::detail::VKConvert(src.access_after),
            .srcQueueFamilyIndex = q1,
            .dstQueueFamilyIndex = q2,
            .buffer = std::bit_cast<VkBuffer>(src.buffer),
            .offset = src.offset,
            .size = src.size,
        };
    }

    // convert texture barriers
    wis::span<VkImageMemoryBarrier2> texture_barriers_span{
        reinterpret_cast<VkImageMemoryBarrier2*>(texture_span.data()),
        barriers->texture_barrier_count
    };
    uint32_t real_texture_barrier_count = barriers->texture_barrier_count;

    for (size_t i = 0; i < barriers->texture_barrier_count; i++) {
        const auto& src = barriers->texture_barriers[i];
        auto q1 = VK_QUEUE_FAMILY_IGNORED;
        auto q2 = VK_QUEUE_FAMILY_IGNORED;

        if (src.queue_type_before != src.queue_type_after) {
            // skip barriers that only perform queue ownership transfer or relaxed transitions if maintenance9 is
            // supported
            if (impl.maintenance9
                && (impl.queue_indices[src.queue_type_before].compatible_to_families
                    & (1 << impl.queue_indices[src.queue_type_after].family_index))) {
                // Skip only release barriers
                // Acquire barriers will just perform relaxed transitions.
                if (src.queue_type_before == impl.queue_type) {
                    real_texture_barrier_count--;
                    continue;
                }
            } else {
                q1 = impl.queue_indices[src.queue_type_before].family_index;
                q2 = impl.queue_indices[src.queue_type_after].family_index;
            }
        }

        texture_barriers_span[i] = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
            .pNext = nullptr,
            .srcStageMask = wis::detail::VKConvert(src.sync_before),
            .srcAccessMask = wis::detail::VKConvert(src.access_before),
            .dstStageMask = wis::detail::VKConvert(src.sync_after),
            .dstAccessMask = wis::detail::VKConvert(src.access_after),
            .oldLayout = src.flags & WisBarrierFlagsDiscardContent ? VK_IMAGE_LAYOUT_UNDEFINED
                                                                   : wis::detail::VKConvert(src.state_before),
            .newLayout = wis::detail::VKConvert(src.state_after),
            .srcQueueFamilyIndex = q1,
            .dstQueueFamilyIndex = q2,
            .image = std::bit_cast<VkImage>(src.texture)
        };

        auto aspect_flags = wis::detail::VKExtractAspectFlags(src.subresource_range, src.flags);
        if (src.flags & WisBarrierFlagsWholeRange) {
            texture_barriers_span[i].subresourceRange = {
                .aspectMask = aspect_flags,
                .baseMipLevel = 0,
                .levelCount = VK_REMAINING_MIP_LEVELS,
                .baseArrayLayer = 0,
                .layerCount = VK_REMAINING_ARRAY_LAYERS,
            };
        } else {
            texture_barriers_span[i].subresourceRange = {
                .aspectMask = aspect_flags,
                .baseMipLevel = src.subresource_range.base_mip_level,
                .levelCount = src.subresource_range.mip_level_count,
                .baseArrayLayer = src.subresource_range.base_array_layer,
                .layerCount = src.subresource_range.array_layer_count,
            };
        }
    }

    // convert global barriers
    wis::span<VkMemoryBarrier2> global_barriers_span{
        reinterpret_cast<VkMemoryBarrier2*>(global_span.data()),
        barriers->global_barrier_count
    };

    for (size_t i = 0; i < barriers->global_barrier_count; i++) {
        const auto& src = barriers->global_barriers[i];
        global_barriers_span[i] = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2,
            .pNext = nullptr,
            .srcStageMask = wis::detail::VKConvert(src.sync_before),
            .srcAccessMask = wis::detail::VKConvert(src.access_before),
            .dstStageMask = wis::detail::VKConvert(src.sync_after),
            .dstAccessMask = wis::detail::VKConvert(src.access_after),
        };
    }

    // future work: support image barriers
    VkDependencyInfo dependency_info{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .memoryBarrierCount = static_cast<uint32_t>(barriers->global_barrier_count),
        .pMemoryBarriers = global_barriers_span.data(),
        .bufferMemoryBarrierCount = real_buffer_barrier_count,
        .pBufferMemoryBarriers = buffer_barriers_span.data(),
        .imageMemoryBarrierCount = real_texture_barrier_count,
        .pImageMemoryBarriers = texture_barriers_span.data(),
    };
    impl.command_list_table->vkCmdPipelineBarrier2(impl.command_buffer, &dependency_info);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetPipeline(
    const WisVKCommandList* self,
    WisVKPipelineView pipeline,
    WisPipelineType type
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto vk_pipeline = std::bit_cast<VkPipeline>(pipeline);
    impl.command_list_table->vkCmdBindPipeline(impl.command_buffer, wis::detail::VKConvert(type), vk_pipeline);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetViewports(
    WisVKCommandList* self,
    const WisViewport* viewports,
    size_t count
)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    auto max_count = std::min(count, static_cast<size_t>(wis::MaxViewports));
    VkViewport vk_viewports[wis::MaxViewports];

    for (size_t i = 0; i < max_count; i++) {
        const auto& vp = viewports[i];
        vk_viewports[i] = {
            .x = vp.x,
            .y = vp.y + vp.height,
            .width = vp.width,
            .height = -vp.height, // Invert height to convert from top-left origin to bottom-left origin
            .minDepth = vp.min_depth,
            .maxDepth = vp.max_depth
        };
    }
    impl.command_list_table
        ->vkCmdSetViewportWithCount(impl.command_buffer, static_cast<uint32_t>(max_count), vk_viewports);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetScissors(WisVKCommandList* self, const WisRect* scissors, size_t count)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    auto max_count = std::min(count, static_cast<size_t>(wis::MaxViewports));
    VkRect2D vk_rects[wis::MaxViewports];
    for (size_t i = 0; i < max_count; i++) {
        auto& vp = scissors[i];
        vk_rects[i] = {
            .offset = {vp.x, vp.y},
            .extent = {uint32_t(vp.width), uint32_t(vp.height)},
        };
    }
    impl.command_list_table->vkCmdSetScissorWithCount(impl.command_buffer, max_count, vk_rects);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetPrimitiveTopology(WisVKCommandList* self, WisPrimitiveTopology topology)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    impl.command_list_table->vkCmdSetPrimitiveTopology(impl.command_buffer, wis::detail::VKConvert(topology));
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetDepthBias(
    WisVKCommandList* self,
    float depth_bias,
    float depth_bias_clamp,
    float slope_scaled_depth_bias
)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    impl.command_list_table
        ->vkCmdSetDepthBias(impl.command_buffer, depth_bias, depth_bias_clamp, slope_scaled_depth_bias);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetPrimitiveRestartValue(
    WisVKCommandList* self,
    WisPrimitiveRestartValue restart_value
)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    impl.command_list_table->vkCmdSetPrimitiveRestartEnable(impl.command_buffer, restart_value != 0);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListDispatch(
    const WisVKCommandList* self,
    uint32_t group_count_x,
    uint32_t group_count_y,
    uint32_t group_count_z
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    impl.command_list_table->vkCmdDispatch(impl.command_buffer, group_count_x, group_count_y, group_count_z);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListBeginRenderPass(
    const WisVKCommandList* self,
    const WisRenderPassDesc* desc
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);

    uint32_t render_target_count = std::min(desc->render_target_count, static_cast<uint32_t>(wis::MaxRenderTargets));
    bool has_depth_stencil = desc->depth_stencil.target != 0;

    VkRenderingAttachmentInfo data[wis::MaxRenderTargets];
    VkRenderingAttachmentInfo depth_data;
    VkRenderingAttachmentInfo stencil_data;

    VkRenderingInfo rendering_info{
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .pNext = nullptr,
        .viewMask = desc->view_mask
    };

    if (render_target_count > 0) {
        auto& target = *reinterpret_cast<wis::detail::VKRenderTargetView*>(desc->render_targets[0].target);
        rendering_info.renderArea = {
            .offset = {0, 0},
            .extent = {target.width, target.height},
        };
        rendering_info.layerCount = desc->view_mask ? 1 : target.array_layer_count;

        for (uint32_t i = 0; i < render_target_count; i++) {
            const auto& rt = desc->render_targets[i];

            data[i] = {
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .pNext = nullptr,
                .imageView = reinterpret_cast<wis::detail::VKRenderTargetView*>(rt.target)->view,
                .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .loadOp = wis::detail::VKConvert(rt.load_op),
                .storeOp = wis::detail::VKConvert(rt.store_op),
                .clearValue = {.color = {rt.clear_value[0], rt.clear_value[1], rt.clear_value[2], rt.clear_value[3]}}
            };
        }
        rendering_info.pColorAttachments = data;
        rendering_info.colorAttachmentCount = render_target_count;
    }

    if (has_depth_stencil) {
        auto& target = *reinterpret_cast<wis::detail::VKRenderTargetView*>(desc->depth_stencil.target);
        if (!rendering_info.renderArea.extent.width) {
            rendering_info.renderArea = {
                .offset = {0, 0},
                .extent = {target.width, target.height},
            };
            rendering_info.layerCount = desc->view_mask ? 1 : target.array_layer_count;
        }

        bool depth_read = desc->depth_stencil.flags & WisDepthStencilFlagsReadOnlyDepth;
        bool stencil_read = desc->depth_stencil.flags & WisDepthStencilFlagsReadOnlyStencil;
        bool ignore_depth = desc->depth_stencil.flags & WisDepthStencilFlagsIgnoreDepth;
        bool ignore_stencil = desc->depth_stencil.flags & WisDepthStencilFlagsIgnoreStencil;

        VkImageLayout ds_layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        switch (desc->depth_stencil.flags & (WisDepthStencilFlagsReadOnlyStencil | WisDepthStencilFlagsReadOnlyDepth)) {
        case WisDepthStencilFlagsReadOnlyStencil:
            ds_layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
            break;
        case WisDepthStencilFlagsReadOnlyDepth:
            ds_layout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
            break;
        case (WisDepthStencilFlagsReadOnlyStencil | WisDepthStencilFlagsReadOnlyDepth):
            ds_layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            break;
        default:
            break;
        }

        if (!ignore_depth) {
            depth_data = {
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .pNext = nullptr,
                .imageView = reinterpret_cast<wis::detail::VKRenderTargetView*>(desc->depth_stencil.target)->view,
                .imageLayout = ds_layout,
                .loadOp = wis::detail::VKConvert(desc->depth_stencil.load_op_depth),
                .storeOp = wis::detail::VKConvert(desc->depth_stencil.store_op_depth),
                .clearValue = {.depthStencil = {desc->depth_stencil.clear_depth, desc->depth_stencil.clear_stencil}}
            };
            rendering_info.pDepthAttachment = &depth_data;
        }
        if (!ignore_stencil) {
            stencil_data = {
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .pNext = nullptr,
                .imageView = reinterpret_cast<wis::detail::VKRenderTargetView*>(desc->depth_stencil.target)->view,
                .imageLayout = ds_layout,
                .loadOp = wis::detail::VKConvert(desc->depth_stencil.load_op_stencil),
                .storeOp = wis::detail::VKConvert(desc->depth_stencil.store_op_stencil),
                .clearValue = {.depthStencil = {desc->depth_stencil.clear_depth, desc->depth_stencil.clear_stencil}}
            };
            rendering_info.pStencilAttachment = &stencil_data;
        }
    }

    impl.command_list_table->vkCmdBeginRendering(impl.command_buffer, &rendering_info);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListEndRenderPass(const WisVKCommandList* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    impl.command_list_table->vkCmdEndRendering(impl.command_buffer);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListDraw(
    const WisVKCommandList* self,
    uint32_t vertex_count,
    uint32_t instance_count,
    uint32_t start_vertex,
    uint32_t start_instance
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    impl.command_list_table->vkCmdDraw(impl.command_buffer, vertex_count, instance_count, start_vertex, start_instance);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListDrawIndexed(
    const WisVKCommandList* self,
    uint32_t index_count,
    uint32_t instance_count,
    uint32_t start_index,
    int32_t base_vertex,
    uint32_t start_instance
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    impl.command_list_table
        ->vkCmdDrawIndexed(impl.command_buffer, index_count, instance_count, start_index, base_vertex, start_instance);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListCopyBuffer(
    const WisVKCommandList* self,
    WisVKBufferView dst_buffer,
    WisVKBufferView src_buffer,
    const WisBufferCopyRegion* regions,
    size_t region_count
)
{
    static_assert(
        sizeof(WisBufferCopyRegion) == sizeof(VkBufferCopy),
        "WisBufferCopyRegion must be binary compatible with VkBufferCopy"
    );
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    impl.command_list_table->vkCmdCopyBuffer(
        impl.command_buffer,
        std::bit_cast<VkBuffer>(src_buffer),
        std::bit_cast<VkBuffer>(dst_buffer),
        static_cast<uint32_t>(region_count),
        reinterpret_cast<const VkBufferCopy*>(regions)
    );
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListCopyBufferToTexture(
    const WisVKCommandList* self,
    WisVKTextureView dst_texture,
    WisVKBufferView src_buffer,
    const WisBufferTextureCopyRegion* regions,
    size_t region_count
)
{
    // if not enough regions provided - schedule several times with max regions until all regions are scheduled
    VkBufferImageCopy convert_regions[wis::MaxCopyRegions];
    size_t region_offset = 0;

    while (region_offset < region_count) {
        uint32_t current_region_count = static_cast<uint32_t>(
            std::min(region_count - region_offset, static_cast<size_t>(wis::MaxCopyRegions))
        );

        for (size_t i = 0; i < current_region_count; ++i) {
            const auto& region = regions[region_offset + i];
            const auto& texture_region = region.texture_region;
            const auto& box = texture_region.box;
            const auto& subresource = texture_region.target_subresource;

            VkImageAspectFlags aspect_mask = 0;
            if (region.texture_region.flags & WisBarrierFlagsDepthResource) {
                aspect_mask |= VK_IMAGE_ASPECT_DEPTH_BIT;
            }
            if (region.texture_region.flags & WisBarrierFlagsStencilResource) {
                aspect_mask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
            if (aspect_mask == 0) {
                aspect_mask = (region.texture_region.flags & WisBarrierFlagsPlanarImage)
                                ? (VK_IMAGE_ASPECT_PLANE_0_BIT << subresource.plane_slice)
                                : VK_IMAGE_ASPECT_COLOR_BIT;
            }

            convert_regions[i] = {
                .bufferOffset = region.buffer_offset,
                .bufferRowLength = region.buffer_row_length,
                .bufferImageHeight = region.buffer_image_height,
                .imageSubresource =
                    {
                        .aspectMask = aspect_mask,
                        .mipLevel = subresource.mip_level,
                        .baseArrayLayer = subresource.array_layer,
                        .layerCount = 1,
                    },
                .imageOffset =
                    {
                        .x = static_cast<int32_t>(box.x),
                        .y = static_cast<int32_t>(box.y),
                        .z = static_cast<int32_t>(box.z),
                    },
                .imageExtent = {.width = box.width, .height = box.height, .depth = box.depth},
            };
        }

        auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
        impl.command_list_table->vkCmdCopyBufferToImage(
            impl.command_buffer,
            std::bit_cast<VkBuffer>(src_buffer),
            std::bit_cast<VkImage>(dst_texture),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            current_region_count,
            convert_regions
        );

        region_offset += current_region_count;
    }
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListCopyTextureToBuffer(
    const WisVKCommandList* self,
    WisVKBufferView dst_buffer,
    WisVKTextureView src_texture,
    const WisBufferTextureCopyRegion* regions,
    size_t region_count
)
{
    VkBufferImageCopy convert_regions[wis::MaxCopyRegions];
    size_t region_offset = 0;

    while (region_offset < region_count) {
        uint32_t current_region_count = static_cast<uint32_t>(
            std::min(region_count - region_offset, static_cast<size_t>(wis::MaxCopyRegions))
        );

        for (size_t i = 0; i < current_region_count; ++i) {
            const auto& region = regions[region_offset + i];
            const auto& texture_region = region.texture_region;
            const auto& box = texture_region.box;
            const auto& subresource = texture_region.target_subresource;

            VkImageAspectFlags aspect_mask = 0;
            if (region.texture_region.flags & WisBarrierFlagsDepthResource) {
                aspect_mask |= VK_IMAGE_ASPECT_DEPTH_BIT;
            }
            if (region.texture_region.flags & WisBarrierFlagsStencilResource) {
                aspect_mask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
            if (aspect_mask == 0) {
                aspect_mask = (region.texture_region.flags & WisBarrierFlagsPlanarImage)
                                ? (VK_IMAGE_ASPECT_PLANE_0_BIT << subresource.plane_slice)
                                : VK_IMAGE_ASPECT_COLOR_BIT;
            }

            convert_regions[i] = {
                .bufferOffset = region.buffer_offset,
                .bufferRowLength = region.buffer_row_length,
                .bufferImageHeight = region.buffer_image_height,
                .imageSubresource =
                    {
                        .aspectMask = aspect_mask,
                        .mipLevel = subresource.mip_level,
                        .baseArrayLayer = subresource.array_layer,
                        .layerCount = 1,
                    },
                .imageOffset =
                    {
                        .x = static_cast<int32_t>(box.x),
                        .y = static_cast<int32_t>(box.y),
                        .z = static_cast<int32_t>(box.z),
                    },
                .imageExtent = {.width = box.width, .height = box.height, .depth = box.depth},
            };
        }

        auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
        impl.command_list_table->vkCmdCopyImageToBuffer(
            impl.command_buffer,
            std::bit_cast<VkImage>(src_texture),
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            std::bit_cast<VkBuffer>(dst_buffer),
            current_region_count,
            convert_regions
        );

        region_offset += current_region_count;
    }
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListCopyTexture(
    const WisVKCommandList* self,
    WisVKTextureView dst_texture,
    WisVKTextureView src_texture,
    const WisTextureCopyRegion* regions,
    size_t region_count
)
{
    VkImageCopy convert_regions[wis::MaxCopyRegions];
    size_t region_offset = 0;

    while (region_offset < region_count) {
        uint32_t current_region_count = static_cast<uint32_t>(
            std::min(region_count - region_offset, static_cast<size_t>(wis::MaxCopyRegions))
        );

        for (size_t i = 0; i < current_region_count; ++i) {
            const auto& region = regions[region_offset + i];
            const auto& src_box = region.src_region.box;
            const auto& dst_box = region.dst_region.box;
            const auto& src_subresource = region.src_region.target_subresource;
            const auto& dst_subresource = region.dst_region.target_subresource;

            VkImageAspectFlags src_aspect_mask = 0;
            if (region.src_region.flags & WisBarrierFlagsDepthResource) {
                src_aspect_mask |= VK_IMAGE_ASPECT_DEPTH_BIT;
            }
            if (region.src_region.flags & WisBarrierFlagsStencilResource) {
                src_aspect_mask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
            if (src_aspect_mask == 0) {
                src_aspect_mask = (region.src_region.flags & WisBarrierFlagsPlanarImage)
                                    ? (VK_IMAGE_ASPECT_PLANE_0_BIT << src_subresource.plane_slice)
                                    : VK_IMAGE_ASPECT_COLOR_BIT;
            }

            VkImageAspectFlags dst_aspect_mask = 0;
            if (region.dst_region.flags & WisBarrierFlagsDepthResource) {
                dst_aspect_mask |= VK_IMAGE_ASPECT_DEPTH_BIT;
            }
            if (region.dst_region.flags & WisBarrierFlagsStencilResource) {
                dst_aspect_mask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
            if (dst_aspect_mask == 0) {
                dst_aspect_mask = (region.dst_region.flags & WisBarrierFlagsPlanarImage)
                                    ? (VK_IMAGE_ASPECT_PLANE_0_BIT << dst_subresource.plane_slice)
                                    : VK_IMAGE_ASPECT_COLOR_BIT;
            }

            convert_regions[i] = {
                .srcSubresource =
                    {
                        .aspectMask = src_aspect_mask,
                        .mipLevel = src_subresource.mip_level,
                        .baseArrayLayer = src_subresource.array_layer,
                        .layerCount = 1,
                    },
                .srcOffset =
                    {
                        .x = static_cast<int32_t>(src_box.x),
                        .y = static_cast<int32_t>(src_box.y),
                        .z = static_cast<int32_t>(src_box.z),
                    },
                .dstSubresource =
                    {
                        .aspectMask = dst_aspect_mask,
                        .mipLevel = dst_subresource.mip_level,
                        .baseArrayLayer = dst_subresource.array_layer,
                        .layerCount = 1,
                    },
                .dstOffset =
                    {
                        .x = static_cast<int32_t>(dst_box.x),
                        .y = static_cast<int32_t>(dst_box.y),
                        .z = static_cast<int32_t>(dst_box.z),
                    },
                .extent = {.width = src_box.width, .height = src_box.height, .depth = src_box.depth},
            };
        }

        auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
        impl.command_list_table->vkCmdCopyImage(
            impl.command_buffer,
            std::bit_cast<VkImage>(src_texture),
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            std::bit_cast<VkImage>(dst_texture),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            current_region_count,
            convert_regions
        );

        region_offset += current_region_count;
    }
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetVertexBuffers(
    WisVKCommandList* self,
    const WisVKVertexBufferDesc* buffers,
    size_t buffer_count,
    uint32_t start_slot
)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    std::size_t count = std::min(buffer_count, static_cast<size_t>(wis::AbsoluteMaxInputBindings));

    VkBuffer buffers_vk[wis::AbsoluteMaxInputBindings];
    VkDeviceSize offsets[wis::AbsoluteMaxInputBindings];
    VkDeviceSize sizes[wis::AbsoluteMaxInputBindings];
    VkDeviceSize strides[wis::AbsoluteMaxInputBindings];

    for (size_t i = 0; i < count; i++) {
        buffers_vk[i] = std::bit_cast<VkBuffer>(buffers[i].buffer);
        offsets[i] = buffers[i].offset;
        sizes[i] = buffers[i].size;
        strides[i] = buffers[i].stride;
    }

    impl.command_list_table
        ->vkCmdBindVertexBuffers2(impl.command_buffer, start_slot, count, buffers_vk, offsets, sizes, strides);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetVertexBuffers2(
    WisVKCommandList* self,
    const WisVertexBufferAddressDesc* buffers,
    size_t buffer_count,
    uint32_t start_slot
)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    std::size_t count = std::min(buffer_count, static_cast<size_t>(wis::AbsoluteMaxInputBindings));

    VkBindVertexBuffer3InfoKHR bind_vertex_buffer_infos[wis::AbsoluteMaxInputBindings];
    for (size_t i = 0; i < count; i++) {
        bind_vertex_buffer_infos[i] = {
            .sType = VK_STRUCTURE_TYPE_BIND_VERTEX_BUFFER_3_INFO_KHR,
            .pNext = nullptr,
            .setStride = VK_TRUE,
            .addressRange =
                {
                    .address = buffers[i].buffer,
                    .size = buffers[i].size,
                    .stride = buffers[i].stride,
                },
            .addressFlags = 0, // reserved for future use
        };
    }

    impl.command_list_table
        ->vkCmdBindVertexBuffers3KHR(impl.command_buffer, start_slot, count, bind_vertex_buffer_infos);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetIndexBuffer(
    WisVKCommandList* self,
    const WisVKIndexBufferDesc* buffer,
    WisIndexType index_type
)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    impl.command_list_table->vkCmdBindIndexBuffer2(
        impl.command_buffer,
        std::bit_cast<VkBuffer>(buffer->buffer),
        buffer->offset,
        buffer->size,
        wis::detail::VKConvert(index_type)
    );
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetIndexBuffer2(
    WisVKCommandList* self,
    const WisIndexBufferAddressDesc* buffer,
    WisIndexType index_type
)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);

    VkBindIndexBuffer3InfoKHR bind_index_buffer_info{
        .sType = VK_STRUCTURE_TYPE_BIND_INDEX_BUFFER_3_INFO_KHR,
        .pNext = nullptr,
        .addressRange =
            {
                .address = buffer->buffer,
                .size = buffer->size,
            },
        .addressFlags = 0, // reserved for future use
        .indexType = wis::detail::VKConvert(index_type),
    };
    impl.command_list_table->vkCmdBindIndexBuffer3KHR(impl.command_buffer, &bind_index_buffer_info);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetBlendFactors(
    const WisVKCommandList* self,
    float blend_factor_r,
    float blend_factor_g,
    float blend_factor_b,
    float blend_factor_a
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    float blend_factors[4] = {blend_factor_r, blend_factor_g, blend_factor_b, blend_factor_a};
    impl.command_list_table->vkCmdSetBlendConstants(impl.command_buffer, blend_factors);
}

#endif // WIS_VK_COMMAND_LIST_CPP
