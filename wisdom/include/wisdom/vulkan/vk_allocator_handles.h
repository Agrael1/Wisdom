#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/vulkan/vk_shared_handle.h>
#include <vk_mem_alloc.hpp>
#endif // !WISDOM_MODULES

WIS_EXPORT namespace wis
{
    struct AllocatorHeader {
        vk::SharedDevice device;
    };

    template<>
    class shared_handle<vma::Allocator> : public vk::SharedHandleBase<vma::Allocator, AllocatorHeader, shared_handle<vma::Allocator>>
    {
        using base = vk::SharedHandleBase<vma::Allocator, AllocatorHeader, shared_handle<vma::Allocator>>;
        using base::m_control;
        friend base;

    public:
        shared_handle() = default;
        explicit shared_handle(vma::Allocator handle, vk::SharedDevice parent)
            : base(handle, AllocatorHeader{ std::move(parent) })
        {
        }

        [[nodiscard]] const auto &getParent() const noexcept
        {
            return getHeader().device;
        }

    protected:
        static void internalDestroy(const AllocatorHeader & /*control*/, vma::Allocator handle) noexcept
        {
            handle.destroy();
        }
    };

    struct AllocationHeader {
        shared_handle<vma::Allocator> allocator;
    };

    template<>
    class shared_handle<vma::Allocation> : public vk::SharedHandleBase<vma::Allocation, AllocationHeader, shared_handle<vma::Allocation>>
    {
        using base = vk::SharedHandleBase<vma::Allocation, AllocationHeader, shared_handle<vma::Allocation>>;
        using base::m_control;
        using base::m_handle;
        friend base;

    public:
        shared_handle() = default;
        explicit shared_handle(vma::Allocation handle, shared_handle<vma::Allocator> parent)
            : base(handle, AllocationHeader{ std::move(parent) })
        {
        }

        [[nodiscard]] const auto &getAllocator() const noexcept
        {
            return m_control->m_header.allocator;
        }

    protected:
        static void internalDestroy(const AllocationHeader &control, vma::Allocation handle) noexcept
        {
            control.allocator->freeMemory(handle);
        }
    };
}
