#ifndef WIS_VK_COMMAND_LIST_CPP
#define WIS_VK_COMMAND_LIST_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>
#include <wisdom/util/allocation.hpp>
#include <bit>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyCommandList(WisVKCommandList* self)
{
    auto& impl = *reinterpret_cast<VKCommandListImpl*>(self);
    if (impl.command_buffer != VK_NULL_HANDLE) {
        // free command buffer
        auto& header = impl.command_pool_header->header;
        impl.command_list_table->vkFreeCommandBuffers(header.device, impl.command_pool, 1, &impl.command_buffer);
        impl.command_buffer = VK_NULL_HANDLE;

        detail::release_vk_command_pool(impl.command_pool, impl.command_pool_header);
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandListBegin(const WisVKCommandList* self)
{
    auto& impl = *reinterpret_cast<const VKCommandListImpl*>(self);

    VkCommandBufferBeginInfo begin_info{
        .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext            = nullptr,
        .flags            = 0, // Optional flags can be set here, e.g., VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        .pInheritanceInfo = nullptr, // Optional, only relevant for secondary command buffers
    };
    auto vr = impl.command_list_table->vkBeginCommandBuffer(impl.command_buffer, &begin_info);
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to begin Vulkan command buffer recording">(vr);
    }
    return vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandListEnd(const WisVKCommandList* self)
{
    auto& impl = *reinterpret_cast<const VKCommandListImpl*>(self);
    auto  vr   = impl.command_list_table->vkEndCommandBuffer(impl.command_buffer);
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to end Vulkan command buffer recording">(vr);
    }
    return vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetDescriptorHeaps(const WisVKCommandList*    self,
                                                                const WisVKDescriptorHeap* resource_heap,
                                                                const WisVKDescriptorHeap* sampler_heap)
{
#if !WISDOM_VULKAN_ALPHA_DESCRIPTOR_HEAP_SUPPORT
    return; // Descriptor heap binding is not supported, silently ignore
#endif

    auto& impl = *reinterpret_cast<const VKCommandListImpl*>(self);

    if (resource_heap) {
        auto&             res_heap                          = *reinterpret_cast<const VKDescriptorHeapImpl*>(resource_heap);
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
        auto&             samp_heap                        = *reinterpret_cast<const VKDescriptorHeapImpl*>(sampler_heap);
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
#if !WISDOM_VULKAN_ALPHA_DESCRIPTOR_HEAP_SUPPORT
    return; // Descriptor heap binding is not supported, silently ignore
#endif

    auto& impl                 = *reinterpret_cast<const VKCommandListImpl*>(self);
    auto* sig                  = std::bit_cast<detail::VKRootSignatureControlBlock*>(signature);
    impl.root_signature_header = sig;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKCommandListSetPushConstants(const WisVKCommandList*        self,
                                                              const WisPushConstantDataDesc* data)
{
#if !WISDOM_VULKAN_ALPHA_DESCRIPTOR_HEAP_SUPPORT
    return; // Descriptor heap binding is not supported, silently ignore
#endif

    auto& impl = *reinterpret_cast<const VKCommandListImpl*>(self);
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
#if !WISDOM_VULKAN_ALPHA_DESCRIPTOR_HEAP_SUPPORT
    return; // Descriptor heap binding is not supported, silently ignore
#endif

    auto& impl = *reinterpret_cast<const VKCommandListImpl*>(self);
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
#if !WISDOM_VULKAN_ALPHA_DESCRIPTOR_HEAP_SUPPORT
    return; // Descriptor heap binding is not supported, silently ignore
#endif

    auto& impl = *reinterpret_cast<const VKCommandListImpl*>(self);
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

#endif // WIS_VK_COMMAND_LIST_CPP