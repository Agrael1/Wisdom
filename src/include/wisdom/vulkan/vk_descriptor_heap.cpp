#ifndef WIS_VK_DESCRIPTOR_HEAP_CPP
#define WIS_VK_DESCRIPTOR_HEAP_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyDescriptorHeap(WisVKDescriptorHeap* self)
{
    auto& impl = *reinterpret_cast<VKDescriptorHeapImpl*>(self);
    if (impl.buffer) {
        // Destroy buffer
        if (impl.memory_type == WisDescriptorMemoryType::WisDescriptorMemoryTypeCpuOnly) {
            std::free(impl.buffer);
        } else {
            vmaDestroyBuffer(impl.device_header->header.allocator, impl.buffer, impl.allocation);
        }

        detail::release_vk_device(impl.device, impl.device_header);
        impl.buffer = VK_NULL_HANDLE;
    }
}

#endif // WIS_VK_DESCRIPTOR_HEAP_CPP
