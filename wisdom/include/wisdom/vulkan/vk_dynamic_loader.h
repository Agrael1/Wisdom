#pragma once
#ifndef WISDOM_MODULES
#include <vulkan/vulkan.hpp>
#endif

WIS_EXPORT namespace wis
{
    struct DynamicLoader {
        using type = vk::DispatchLoaderDynamic;
        static inline vk::DispatchLoaderDynamic loader{};
        static inline bool init = false;
    };
    struct StaticLoader {
        using type = vk::DispatchLoaderStatic;
        static inline vk::DispatchLoaderStatic loader{};
        static inline constexpr bool init = true;
    };
}