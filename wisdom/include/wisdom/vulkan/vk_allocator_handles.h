#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/vulkan/vk_shared_handle.h>
#include <vk_mem_alloc.h>
#endif // !WISDOM_MODULES

namespace wis {
template<>
WIS_EXPORT class shared_handle<VmaAllocator> : public vk::SharedHandleBase<VmaAllocator, vk::SharedDevice, shared_handle<VmaAllocator>>
{
    using base = vk::SharedHandleBase<VmaAllocator, vk::SharedDevice, shared_handle<VmaAllocator>>;
    friend base;

public:
    shared_handle() = default;
    explicit shared_handle(VmaAllocator handle, vk::SharedDevice parent) noexcept
        : base(handle, std::move(parent))
    {
    }

    [[nodiscard]] const auto& getParent() const noexcept
    {
        return getHeader();
    }

protected:
    static void internalDestroy(const vk::SharedDevice& /*control*/, VmaAllocator handle) noexcept
    {
        vmaDestroyAllocator(handle);
    }
};

template<>
WIS_EXPORT class shared_handle<VmaAllocation> : public vk::SharedHandleBase<VmaAllocation, shared_handle<VmaAllocator>, shared_handle<VmaAllocation>>
{
    using base = vk::SharedHandleBase<VmaAllocation, shared_handle<VmaAllocator>, shared_handle<VmaAllocation>>;
    friend base;

public:
    shared_handle() = default;
    explicit shared_handle(VmaAllocation handle, shared_handle<VmaAllocator> parent) noexcept
        : base(handle, std::move(parent))
    {
    }

    [[nodiscard]] const auto& getAllocator() const noexcept
    {
        return getHeader();
    }

protected:
    static void internalDestroy(const shared_handle<VmaAllocator>& control, VmaAllocation handle) noexcept
    {
        vmaFreeMemory(control.get(), handle);
    }
};
} // namespace wis
