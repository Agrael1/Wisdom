#ifndef WIS_VK_COMMAND_ALLOCATOR_CPP
#define WIS_VK_COMMAND_ALLOCATOR_CPP
#include <wisdom/generated/backend_api.hpp>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>
#include <wisdom/util/allocation.hpp>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyCommandAllocator(WisVKCommandAllocator* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKCommandAllocatorImpl>(self);
    if (impl.command_pool != VK_NULL_HANDLE) {
        wis::detail::release_vk_command_pool(impl.command_pool_header);
        impl.command_pool = VK_NULL_HANDLE;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandAllocatorReset(const WisVKCommandAllocator* self)
{
    auto& impl          = wis::from_handle_ref<const wis::impl::VKCommandAllocatorImpl>(self);
    auto& header        = impl.command_pool_header->header;
    auto& device_header = header.device_header->header;
    auto  result        = device_header.device_table.vkResetCommandPool(header.device, impl.command_pool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);

    if (!wis::detail::succeeded(result)) {
        return wis::detail::make_result<wis::detail::Func(), "vkResetCommandPool failed">(result);
    }

    return wis::detail::vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandAllocatorCreateCommandList(const WisVKCommandAllocator* self,
                                                                         WisVKCommandList*            list)
{
    auto& impl          = wis::from_handle_ref<const wis::impl::VKCommandAllocatorImpl>(self);
    auto& header        = impl.command_pool_header->header;
    auto& device_header = header.device_header->header;

    // Create command buffer
    VkCommandBufferAllocateInfo alloc_info{
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext              = nullptr,
        .commandPool        = impl.command_pool,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer command_buffer = VK_NULL_HANDLE;
    VkResult        vr             = device_header.device_table.vkAllocateCommandBuffers(header.device, &alloc_info, &command_buffer);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to allocate Vulkan command buffer">(vr);
    }

    // Fill command list impl
    auto& list_impl = *new (list) wis::impl::VKCommandListImpl{
        .command_buffer     = command_buffer,
        .command_list_table = &device_header.command_list_table, // point to main command list table for faster access

        .command_pool_header = impl.command_pool_header,

        .queue_indices = header.device_header->header.queue_family_extras.data(),
        .maintenance9  = device_header.features.maintenance9, // copy maintenance9 support from command allocator
    };
    list_impl.command_pool_header->AddRef(); // hold reference to command pool header for command list impl
    return wis::detail::vk_success;
}
#endif // WIS_VK_COMMAND_ALLOCATOR_CPP