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
}
