#ifndef WIS_VK_RESOURCE_ALLOCATOR_CPP
#define WIS_VK_RESOURCE_ALLOCATOR_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;


//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyResourceAllocator(WisVKResourceAllocator* self)
{
    auto& impl = *reinterpret_cast<VKResourceAllocatorImpl*>(self);
    if (impl.allocator) {

        // Get device from allocator
        VmaAllocatorInfo allocator_info{};
        vmaGetAllocatorInfo(impl.allocator, &allocator_info);

        detail::release_vk_device(allocator_info.device, impl.device_header);
        impl.allocator = nullptr;
    }
}
#endif // WIS_VK_RESOURCE_ALLOCATOR_CPP
