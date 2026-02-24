#ifndef WIS_VK_COMMAND_ALLOCATOR_CPP
#define WIS_VK_COMMAND_ALLOCATOR_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>
#include <wisdom/util/allocation.hpp>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyCommandAllocator(WisVKCommandAllocator* self)
{
    auto& impl = *reinterpret_cast<VKCommandAllocatorImpl*>(self);
    if (impl.command_pool != VK_NULL_HANDLE) {
        detail::release_vk_command_pool(impl.command_pool, impl.command_pool_header);
        impl.command_pool = VK_NULL_HANDLE;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandAllocatorReset(const WisVKCommandAllocator* self)
{
    auto& impl          = *reinterpret_cast<const VKCommandAllocatorImpl*>(self);
    auto& header        = impl.command_pool_header->header;
    auto& device_header = header.device_header->header;
    auto  result        = device_header.device_table.vkResetCommandPool(header.device, impl.command_pool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);

    if (!succeeded(result)) {
        return make_result<Func(), "vkResetCommandPool failed">(result);
    }

    return vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandAllocatorCreateCommandList(const WisVKCommandAllocator* self,
                                                              WisVKCommandList*            list)
{
    auto& impl          = *reinterpret_cast<const VKCommandAllocatorImpl*>(self);
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
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to allocate Vulkan command buffer">(vr);
    }

    // Fill command list impl
    auto& list_impl               = *new (list) VKCommandListImpl();
    list_impl.command_buffer      = command_buffer;
    list_impl.command_pool        = impl.command_pool;
    list_impl.command_list_table  = &device_header.command_list_table; // point to main command list table for faster access
    list_impl.command_pool_header = impl.command_pool_header;
    list_impl.command_pool_header->AddRef(); // hold reference to command pool header for command list impl
    return vk_success;
}
#endif // WIS_VK_COMMAND_ALLOCATOR_CPP