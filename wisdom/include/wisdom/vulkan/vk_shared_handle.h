#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/vulkan/vk_xshared_handle.h>
#endif

WIS_EXPORT namespace wis
{
    using vk::SharedHandle;

    template<typename HandleType>
    class shared_handle : public vk::SharedHandle<HandleType>
    {
    public:
        using vk::SharedHandle<HandleType>::SharedHandle;
    };


    //template<>
    //class shared_handle<vk::Device> : public vk::SharedHandleBase<vk::Device, shared_handle<vk::Instance>, shared_handle<vk::Device>>
    //{
    //    using base = vk::SharedHandleBase<vk::Device, shared_handle<vk::Instance>, shared_handle<vk::Device>>;
    //    using base::m_control;
    //    friend base;
    //
    //public:
    //    shared_handle() = default;
    //    explicit shared_handle(vk::Device handle, shared_handle<vk::Instance> parent)
    //        : base(handle, std::move(parent))
    //    {
    //    }
    //
    //    [[nodiscard]] const auto& getInstance() const noexcept
    //    {
    //        return getHeader();
    //    }
    //
    //protected:
    //    static void internalDestroy(const shared_handle<vk::Instance>& /*control*/, vk::Device handle) noexcept
    //    {
    //        handle.destroy();
    //    }
    //};
}
