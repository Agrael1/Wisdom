#ifndef WIS_VK_COMMAND_LIST_CPP
#define WIS_VK_COMMAND_LIST_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_ext1.hpp>
#include <wisdom/util/allocation.hpp>

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

#endif // WIS_VK_COMMAND_LIST_CPP