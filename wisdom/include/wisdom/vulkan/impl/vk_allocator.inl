#ifndef WISDOM_MODULES
// #include <wisdom/vulkan/vk_allocator.h>
#include <wisdom/vulkan/vk_device.h>
#endif // !WISDOM_MODULES

// NOLINTNEXTLINE
wis::VKResourceAllocator::VKResourceAllocator(const VKDevice& in_device) noexcept
{
    const auto& d_internal = in_device.GetInternal();

    vma::AllocatorCreateInfo allocatorInfo{
        vma::AllocatorCreateFlags(0),
        d_internal.adapter,
        d_internal.device.get(),
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        d_internal.instance.get(),
        GetApiVersion()
    };

    vma::Allocator al;
    auto result = vma::createAllocator(&allocatorInfo, &al);
    if (!wis::succeeded(result))
        return;

    allocator = wis::shared_handle<vma::Allocator>{ al, d_internal.device };
}
