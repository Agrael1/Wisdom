#ifndef WIS_VK_COMMAND_LIST_CPP
#define WIS_VK_COMMAND_LIST_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>
#include <wisdom/util/allocation.hpp>
#include <bit>

namespace wis::detail {
inline uint8_t* VKAllocateScratchSpace(const wis::impl::VKCommandListImpl& impl, uint32_t new_size)
{
    if (new_size > impl.scratch_memory_size) {
        delete[] impl.scratch_memory;
        impl.scratch_memory = new (std::nothrow) uint8_t[new_size];
    }
    return impl.scratch_memory;
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyCommandList(WisVKCommandList* self)
{
    auto& impl = *reinterpret_cast<wis::impl::VKCommandListImpl*>(self);
    if (impl.command_buffer != VK_NULL_HANDLE) {
        // free command buffer
        auto& header = impl.command_pool_header->header;
        impl.command_list_table->vkFreeCommandBuffers(header.device, impl.command_pool, 1, &impl.command_buffer);
        impl.command_buffer = VK_NULL_HANDLE;

        wis::detail::release_vk_command_pool(impl.command_pool, impl.command_pool_header);
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandListBegin(const WisVKCommandList* self)
{
    auto& impl = *reinterpret_cast<const wis::impl::VKCommandListImpl*>(self);

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
    auto& impl = *reinterpret_cast<const wis::impl::VKCommandListImpl*>(self);
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
    auto& impl = *reinterpret_cast<const wis::impl::VKCommandListImpl*>(self);

    if (resource_heap) {
        auto&             res_heap                          = *reinterpret_cast<const wis::impl::VKDescriptorHeapImpl*>(resource_heap);
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
        auto&             samp_heap                        = *reinterpret_cast<const wis::impl::VKDescriptorHeapImpl*>(sampler_heap);
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
    auto& impl                 = *reinterpret_cast<const wis::impl::VKCommandListImpl*>(self);
    auto* sig                  = std::bit_cast<wis::detail::VKRootSignatureControlBlock*>(signature);
    impl.root_signature_header = sig;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetPushConstants(const WisVKCommandList*        self,
                                                              const WisPushConstantDataDesc* data)
{
    auto& impl = *reinterpret_cast<const wis::impl::VKCommandListImpl*>(self);
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
    auto& impl = *reinterpret_cast<const wis::impl::VKCommandListImpl*>(self);
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
    auto& impl = *reinterpret_cast<const wis::impl::VKCommandListImpl*>(self);
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
    if (barriers->buffer_barrier_count == 0) {
        return;
    }

    auto& impl = *reinterpret_cast<const wis::impl::VKCommandListImpl*>(self);

    static constexpr uint32_t max_barrier_size = std::max(sizeof(VkBufferMemoryBarrier2), sizeof(VkImageMemoryBarrier2));
    static constexpr uint32_t static_size      = static_cast<uint32_t>(wis::TransientMaxBarrierCount * max_barrier_size);
    uint8_t                   local_scratch[static_size]{};
    uint8_t*                  real_data = local_scratch;

    uint32_t needed_size = static_cast<uint32_t>(barriers->buffer_barrier_count * sizeof(VkBufferMemoryBarrier2));
    if (needed_size > static_size) {
        // allocate from the command list's scratch memory if the needed size exceeds the local scratch buffer size. This is to avoid large stack allocations.
        real_data = wis::detail::VKAllocateScratchSpace(impl, needed_size);
    }

    auto* buffer_barriers = local_scratch;

    wis::span<VkBufferMemoryBarrier2> buffer_barriers_span{ reinterpret_cast<VkBufferMemoryBarrier2*>(buffer_barriers), barriers->buffer_barrier_count };
    uint32_t                          real_buffer_barrier_count = barriers->buffer_barrier_count;

    for (size_t i = 0; i < barriers->buffer_barrier_count; i++) {
        const auto& src = barriers->buffer_barriers[i];

        auto q1 = VK_QUEUE_FAMILY_IGNORED;
        auto q2 = VK_QUEUE_FAMILY_IGNORED;
        if (src.queue_type_before != src.queue_type_after) {
            // skip barriers that only perform queue ownership transfer
            if (impl.maintenance9 && impl.queue_type == src.queue_type_before) {
                real_buffer_barrier_count--;
                continue;
            }

            if (!impl.maintenance9) {
                q1 = src.queue_type_before >= WisCommandQueueTypeCount ? VK_QUEUE_FAMILY_IGNORED : impl.queue_residency[src.queue_type_before];
                q2 = src.queue_type_after >= WisCommandQueueTypeCount ? VK_QUEUE_FAMILY_IGNORED : impl.queue_residency[src.queue_type_after];
            }
        }

        buffer_barriers_span[i] = {
            .sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2,
            .pNext               = nullptr,
            .srcStageMask        = wis::detail::convert_vk(src.sync_before),
            .srcAccessMask       = wis::detail::convert_vk(src.access_before),
            .dstStageMask        = wis::detail::convert_vk(src.sync_after),
            .dstAccessMask       = wis::detail::convert_vk(src.access_after),
            .srcQueueFamilyIndex = q1,
            .dstQueueFamilyIndex = q2,
            .buffer              = std::bit_cast<VkBuffer>(src.buffer),
            .offset              = src.offset,
            .size                = src.size,
        };
    }

    // future work: support image barriers
    VkDependencyInfo dependency_info{
        .sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext                    = nullptr,
        .bufferMemoryBarrierCount = real_buffer_barrier_count,
        .pBufferMemoryBarriers    = buffer_barriers_span.data(),
        .imageMemoryBarrierCount  = 0,
        .pImageMemoryBarriers     = nullptr,
    };
    impl.command_list_table->vkCmdPipelineBarrier2(impl.command_buffer, &dependency_info);
}

#endif // WIS_VK_COMMAND_LIST_CPP