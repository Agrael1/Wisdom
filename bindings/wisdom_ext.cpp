#include "wisdom.h"

#include "wisdom/wisdom_debug.h"

#if defined(WISDOM_DX12)
#include <wisdom/wisdom_dx12.hpp>

template<wis::FactoryExtID>
struct DX12FactoryExtensionMap {
    using Type = wis::DX12FactoryExtension;
};
template<>
struct DX12FactoryExtensionMap<wis::FactoryExtID::DebugExtension> {
    using Type = wis::DX12DebugExtension;
};
//-------------------------------------------------------------------------

template<template<typename T> typename Executor, typename... Args>
constexpr static inline decltype(auto) DX12FactoryExtensionBridge(wis::FactoryExtID id, Args&&... args)
{
    switch (id) {
    case wis::FactoryExtID::DebugExtension:
        return Executor<typename DX12FactoryExtensionMap<wis::FactoryExtID::DebugExtension>::Type>{}(std::forward<Args>(args)...);
    default:
        return Executor<wis::DX12DebugExtension>{}(std::forward<Args>(args)...);
    }
}

//-------------------------------------------------------------------------

#endif

#if defined(WISDOM_VULKAN)
#include <wisdom/wisdom_vk.hpp>

template<wis::FactoryExtID>
struct VKFactoryExtensionMap {
    using Type = wis::VKFactoryExtension;
};
template<>
struct VKFactoryExtensionMap<wis::FactoryExtID::DebugExtension> {
    using Type = wis::VKDebugExtension;
};
//-------------------------------------------------------------------------

template<template<typename T> typename Executor, typename... Args>
constexpr static inline decltype(auto) VKFactoryExtensionBridge(wis::FactoryExtID id, Args&&... args)
{
    switch (id) {
    case wis::FactoryExtID::DebugExtension:
        return Executor<typename VKFactoryExtensionMap<wis::FactoryExtID::DebugExtension>::Type>{}(std::forward<Args>(args)...);
    default:
        return Executor<wis::VKDebugExtension>{}(std::forward<Args>(args)...);
    }
}

//-------------------------------------------------------------------------

#endif
