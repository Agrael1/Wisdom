#ifndef WIS_VK_COMMAND_LIST_CPP
#define WIS_VK_COMMAND_LIST_CPP
#include <wisdom/generated/backend_api.hpp>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>
#include <wisdom/util/allocation.hpp>
#include <bit>

namespace wis::detail {
constexpr static uint32_t vk_max_barrier_size = std::max({ sizeof(VkBufferMemoryBarrier), sizeof(VkImageMemoryBarrier2), sizeof(VkMemoryBarrier2) });
constexpr static uint32_t vk_static_size      = wis::TransientMaxBarrierCount * vk_max_barrier_size;

inline uint8_t* VKAllocateScratchSpace(const wis::impl::VKCommandListImpl& impl, uint32_t new_size)
{
    if (new_size > impl.scratch_memory_size) {
        delete[] impl.scratch_memory;
        impl.scratch_memory = new (std::nothrow) uint8_t[new_size];
    }
    return impl.scratch_memory;
}
inline constexpr VkImageAspectFlags
VKExtractAspectFlags(WisSubresourceRange subresource, WisBarrierFlags flags) noexcept
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

inline std::array<wis::span<uint8_t>, 3>
VKAllocateBarriers(const wis::impl::VKCommandListImpl& impl,
                   uint8_t*                            local_scratch,
                   const WisVKBarrierGroup&            barriers)
{
    std::array<wis::span<uint8_t>, 3> spans;
    std::size_t                       needed_size = barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2) +
            barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2) +
            barriers.global_barrier_count * sizeof(VkMemoryBarrier2);

    if (needed_size <= vk_static_size) {
        spans[0] = { local_scratch, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2) };
        spans[1] = { spans[0].end(), barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2) };
        spans[2] = { spans[1].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2) };
        return spans;
    }

    std::size_t sizes[] = { barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2),
                            barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2),
                            barriers.global_barrier_count * sizeof(VkMemoryBarrier2),
                            0,
                            0,
                            0 };

    sizes[3] = sizes[0] + sizes[1];
    sizes[4] = sizes[1] + sizes[2];
    sizes[5] = sizes[0] + sizes[2];

    // find closest value from below
    uint32_t closest_size = 0;
    int      index        = -1;
    for (int i = std::size(sizes) - 1; i >= 0; --i) {
        if (sizes[i] > vk_static_size) {
            continue;
        }

        // less than or equal to static size, check if it's the closest one
        if (vk_static_size - sizes[i] < vk_static_size - closest_size) {
            closest_size = sizes[i];
            index        = i;
        }
    }

    uint32_t allocated_size = needed_size - closest_size;

    // allocate from the command list's scratch memory if the needed size exceeds the local scratch buffer size. This is to avoid large stack allocations.
    auto* allocated_data = wis::detail::VKAllocateScratchSpace(impl, allocated_size);

    // set pointers to the right offsets in the allocated scratch memory
    switch (index) {
    default:
    case -1:
        // no single group can fit into the local scratch, allocate all from the command list's scratch memory
        spans[0] = { allocated_data, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2) };
        spans[1] = { spans[0].end(), barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2) };
        spans[2] = { spans[1].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2) };
        return spans;
    case 0:
        // buffer barriers fit into local scratch, texture and global barriers allocated from command list's scratch
        spans[0] = { local_scratch, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2) };
        spans[1] = { allocated_data, barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2) };
        spans[2] = { spans[1].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2) };
        return spans;
    case 1:
        // texture barriers fit into local scratch, buffer and global barriers allocated from command list's scratch
        spans[0] = { allocated_data, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2) };
        spans[1] = { local_scratch, barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2) };
        spans[2] = { spans[0].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2) };
        return spans;
    case 2:
        // global barriers fit into local scratch, buffer and texture barriers allocated from command list's scratch
        spans[0] = { allocated_data, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2) };
        spans[1] = { spans[0].end(), barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2) };
        spans[2] = { local_scratch, barriers.global_barrier_count * sizeof(VkMemoryBarrier2) };
        return spans;
    case 3:
        spans[0] = { local_scratch, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2) };
        spans[1] = { spans[0].end(), barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2) };
        spans[2] = { allocated_data, barriers.global_barrier_count * sizeof(VkMemoryBarrier2) };
        return spans;
    case 4:
        spans[0] = { allocated_data, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2) };
        spans[1] = { local_scratch, barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2) };
        spans[2] = { spans[1].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2) };
        return spans;
    case 5:
        spans[0] = { local_scratch, barriers.buffer_barrier_count * sizeof(VkBufferMemoryBarrier2) };
        spans[1] = { allocated_data, barriers.texture_barrier_count * sizeof(VkImageMemoryBarrier2) };
        spans[2] = { spans[0].end(), barriers.global_barrier_count * sizeof(VkMemoryBarrier2) };
        return spans;
    }
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandListBegin(const WisVKCommandList* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);

    VkCommandBufferBeginInfo begin_info{
        .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext            = nullptr,
        .flags            = 0, // Optional flags can be set here, e.g., VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        .pInheritanceInfo = nullptr, // Optional, only relevant for secondary command buffers
    };
    auto vr = impl.command_list_table->vkBeginCommandBuffer(impl.command_buffer, &begin_info);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to begin Vulkan command buffer recording">(vr);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandListEnd(const WisVKCommandList* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto  vr   = impl.command_list_table->vkEndCommandBuffer(impl.command_buffer);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to end Vulkan command buffer recording">(vr);
    }
    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetDescriptorHeaps(const WisVKCommandList*    self,
                                                                const WisVKDescriptorHeap* resource_heap,
                                                                const WisVKDescriptorHeap* sampler_heap)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);

    if (resource_heap) {
    auto&             res_heap                          = wis::from_handle_ref<const wis::impl::VKDescriptorHeapImpl>(resource_heap);
        VkDeviceSize      reserved_resource_descriptor_size = static_cast<std::size_t>(res_heap.reserved_size) * res_heap.descriptor_size;
        VkDeviceSize      total_resource_heap_size          = static_cast<std::size_t>(res_heap.heap_size) * res_heap.descriptor_size + reserved_resource_descriptor_size;
        VkBindHeapInfoEXT bind_resource_info{
            .sType               = VK_STRUCTURE_TYPE_BIND_HEAP_INFO_EXT,
            .pNext               = nullptr,
            .heapRange           = { res_heap.gpu_address, total_resource_heap_size },
            .reservedRangeOffset = 0,
            .reservedRangeSize   = reserved_resource_descriptor_size,
        };
        impl.command_list_table->vkCmdBindResourceHeapEXT(impl.command_buffer, &bind_resource_info);
    }

    if (sampler_heap) {
    auto&             samp_heap                        = wis::from_handle_ref<const wis::impl::VKDescriptorHeapImpl>(sampler_heap);
        VkDeviceSize      reserved_sampler_descriptor_size = static_cast<std::size_t>(samp_heap.reserved_size) * samp_heap.descriptor_size;
        VkDeviceSize      total_sampler_heap_size          = static_cast<std::size_t>(samp_heap.heap_size) * samp_heap.descriptor_size + reserved_sampler_descriptor_size;
        VkBindHeapInfoEXT bind_sampler_info{
            .sType               = VK_STRUCTURE_TYPE_BIND_HEAP_INFO_EXT,
            .pNext               = nullptr,
            .heapRange           = { samp_heap.gpu_address, total_sampler_heap_size }, // Not used for bindless heaps
            .reservedRangeOffset = 0,
            .reservedRangeSize   = reserved_sampler_descriptor_size,
        };
        impl.command_list_table->vkCmdBindSamplerHeapEXT(impl.command_buffer, &bind_sampler_info);
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetRootSignature(const WisVKCommandList* self,
                                                              WisVKRootSignatureView  signature,
                                                              WisPipelineType         pipeline)
{
    auto& impl                 = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto* sig                  = std::bit_cast<wis::detail::VKRootSignatureControlBlock*>(signature);
    impl.root_signature_header = sig;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetPushConstants(const WisVKCommandList*        self,
                                                              const WisPushConstantDataDesc* data)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto* sig  = impl.root_signature_header;

    uint32_t          push_constant_offset = sig->GetRootBindingOffsets()[data->root_index] + data->push_offset / 4;
    VkPushDataInfoEXT push_data_info{
        .sType  = VK_STRUCTURE_TYPE_PUSH_DATA_INFO_EXT,
        .pNext  = nullptr,
        .offset = push_constant_offset,
        .data   = { .address = data->data, .size = data->data_size }
    };
    impl.command_list_table->vkCmdPushDataEXT(impl.command_buffer, &push_data_info);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetPushDescriptor(const WisVKCommandList*          self,
                                                               const WisPushDescriptorDataDesc* data)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto* sig  = impl.root_signature_header;

    uint32_t          push_desc_offset = sig->GetRootBindingOffsets()[data->root_index];
    VkPushDataInfoEXT push_data_info{
        .sType  = VK_STRUCTURE_TYPE_PUSH_DATA_INFO_EXT,
        .pNext  = nullptr,
        .offset = push_desc_offset,
        .data   = { .address = &data->buffer_address, .size = sizeof(uint64_t) }
    };
    impl.command_list_table->vkCmdPushDataEXT(impl.command_buffer, &push_data_info);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetDescriptorTable(const WisVKCommandList*           self,
                                                                const WisDescriptorTableDataDesc* data)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto* sig  = impl.root_signature_header;

    uint32_t          push_desc_offset = sig->GetRootBindingOffsets()[data->root_index];
    VkPushDataInfoEXT push_data_info{
        .sType  = VK_STRUCTURE_TYPE_PUSH_DATA_INFO_EXT,
        .pNext  = nullptr,
        .offset = push_desc_offset,
        .data   = { .address = &data->heap_offset, .size = sizeof(uint32_t) }
    };
    impl.command_list_table->vkCmdPushDataEXT(impl.command_buffer, &push_data_info);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListInsertBarriers(const WisVKCommandList*  self,
                                                            const WisVKBarrierGroup* barriers)
{
    // clang-format off
    if (barriers->buffer_barrier_count +
        barriers->texture_barrier_count +
        barriers->global_barrier_count == 0) {
        return;
    }
    // clang-format on

    auto&   impl          = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto&   device_header = impl.command_pool_header->header;
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
            .sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2,
            .pNext               = nullptr,
            .srcStageMask        = wis::detail::VKConvert(src.sync_before),
            .srcAccessMask       = wis::detail::VKConvert(src.access_before),
            .dstStageMask        = wis::detail::VKConvert(src.sync_after),
            .dstAccessMask       = wis::detail::VKConvert(src.access_after),
            .srcQueueFamilyIndex = q1,
            .dstQueueFamilyIndex = q2,
            .buffer              = std::bit_cast<VkBuffer>(src.buffer),
            .offset              = src.offset,
            .size                = src.size,
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
        auto        q1  = VK_QUEUE_FAMILY_IGNORED;
        auto        q2  = VK_QUEUE_FAMILY_IGNORED;

        if (src.queue_type_before != src.queue_type_after) {
            // skip barriers that only perform queue ownership transfer or relaxed transitions if maintenance9 is supported
            if (impl.maintenance9 && (impl.queue_indices[src.queue_type_before].compatible_to_families & (1 << impl.queue_indices[src.queue_type_after].family_index))) {
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
            .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
            .pNext               = nullptr,
            .srcStageMask        = wis::detail::VKConvert(src.sync_before),
            .srcAccessMask       = wis::detail::VKConvert(src.access_before),
            .dstStageMask        = wis::detail::VKConvert(src.sync_after),
            .dstAccessMask       = wis::detail::VKConvert(src.access_after),
            .oldLayout           = src.flags & WisBarrierFlagsDiscardContent
                              ? VK_IMAGE_LAYOUT_UNDEFINED
                              : wis::detail::VKConvert(src.state_before),
            .newLayout           = wis::detail::VKConvert(src.state_after),
            .srcQueueFamilyIndex = q1,
            .dstQueueFamilyIndex = q2,
            .image               = std::bit_cast<VkImage>(src.texture)
        };

        auto aspect_flags = wis::detail::VKExtractAspectFlags(src.subresource_range, src.flags);
        if (src.flags & WisBarrierFlagsWholeRange) {
            texture_barriers_span[i].subresourceRange = {
                .aspectMask     = aspect_flags,
                .baseMipLevel   = 0,
                .levelCount     = VK_REMAINING_MIP_LEVELS,
                .baseArrayLayer = 0,
                .layerCount     = VK_REMAINING_ARRAY_LAYERS,
            };
        } else {
            texture_barriers_span[i].subresourceRange = {
                .aspectMask     = aspect_flags,
                .baseMipLevel   = src.subresource_range.base_mip_level,
                .levelCount     = src.subresource_range.mip_level_count,
                .baseArrayLayer = src.subresource_range.base_array_layer,
                .layerCount     = src.subresource_range.array_layer_count,
            };
        }
    }

    // convert global barriers
    wis::span<VkMemoryBarrier2> global_barriers_span{
        reinterpret_cast<VkMemoryBarrier2*>(global_span.data()),
        barriers->global_barrier_count
    };

    for (size_t i = 0; i < barriers->global_barrier_count; i++) {
        const auto& src         = barriers->global_barriers[i];
        global_barriers_span[i] = {
            .sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2,
            .pNext         = nullptr,
            .srcStageMask  = wis::detail::VKConvert(src.sync_before),
            .srcAccessMask = wis::detail::VKConvert(src.access_before),
            .dstStageMask  = wis::detail::VKConvert(src.sync_after),
            .dstAccessMask = wis::detail::VKConvert(src.access_after),
        };
    }

    // future work: support image barriers
    VkDependencyInfo dependency_info{
        .sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext                    = nullptr,
        .memoryBarrierCount       = static_cast<uint32_t>(barriers->global_barrier_count),
        .pMemoryBarriers          = global_barriers_span.data(),
        .bufferMemoryBarrierCount = real_buffer_barrier_count,
        .pBufferMemoryBarriers    = buffer_barriers_span.data(),
        .imageMemoryBarrierCount  = real_texture_barrier_count,
        .pImageMemoryBarriers     = texture_barriers_span.data(),
    };
    impl.command_list_table->vkCmdPipelineBarrier2(impl.command_buffer, &dependency_info);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetPipeline(const WisVKCommandList* self,
                                                         WisVKPipelineView       pipeline,
                                                         WisPipelineType         type)
{
    auto& impl        = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    auto  vk_pipeline = std::bit_cast<VkPipeline>(pipeline);
    impl.command_list_table->vkCmdBindPipeline(impl.command_buffer, wis::detail::VKConvert(type), vk_pipeline);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetViewports(WisVKCommandList*  self,
                                                          const WisViewport* viewports,
                                                          size_t             count)
{
    auto&      impl      = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    auto       max_count = std::min(count, static_cast<size_t>(wis::MaxViewports));
    VkViewport vk_viewports[wis::MaxViewports];

    for (size_t i = 0; i < max_count; i++) {
        const auto& vp  = viewports[i];
        vk_viewports[i] = {
            .x        = vp.top_leftx,
            .y        = vp.top_lefty,
            .width    = vp.width,
            .height   = -vp.height, // Invert height to convert from top-left origin to bottom-left origin
            .minDepth = vp.min_depth,
            .maxDepth = vp.max_depth
        };
    }
    impl.command_list_table->vkCmdSetViewport(impl.command_buffer, 0, static_cast<uint32_t>(max_count), vk_viewports);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetScissors(WisVKCommandList* self,
                                                         const WisScissor* scissors,
                                                         size_t            count)
{
    auto&    impl      = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    auto     max_count = std::min(count, static_cast<size_t>(wis::MaxViewports));
    VkRect2D vk_rects[wis::MaxViewports];
    for (size_t i = 0; i < max_count; i++) {
        auto& vp    = scissors[i];
        vk_rects[i] = {
            .offset = {                      vp.left,                       vp.top },
            .extent = { uint32_t(vp.right - vp.left), uint32_t(vp.bottom - vp.top) },
        };
    }
    impl.command_list_table->vkCmdSetScissor(impl.command_buffer, 0, max_count, vk_rects);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetPrimitiveTopology(WisVKCommandList*    self,
                                                                  WisPrimitiveTopology topology)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    impl.command_list_table->vkCmdSetPrimitiveTopology(impl.command_buffer, wis::detail::VKConvert(topology));
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetDepthBias(WisVKCommandList* self,
                                                          float             depth_bias,
                                                          float             depth_bias_clamp,
                                                          float             slope_scaled_depth_bias)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    impl.command_list_table->vkCmdSetDepthBias(impl.command_buffer, depth_bias, depth_bias_clamp, slope_scaled_depth_bias);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetPrimitiveRestartValue(WisVKCommandList*        self,
                                                                      WisPrimitiveRestartValue restart_value)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandListImpl>(self);
    impl.command_list_table->vkCmdSetPrimitiveRestartEnable(impl.command_buffer, restart_value != 0);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListDispatch(const WisVKCommandList* self,
                                                      uint32_t                group_count_x,
                                                      uint32_t                group_count_y,
                                                      uint32_t                group_count_z)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKCommandListImpl>(self);
    impl.command_list_table->vkCmdDispatch(impl.command_buffer, group_count_x, group_count_y, group_count_z);
}

#endif // WIS_VK_COMMAND_LIST_CPP