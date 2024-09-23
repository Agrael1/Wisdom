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
        return Executor<wis::DX12FactoryExtension>{}(std::forward<Args>(args)...);
    }
}

//-------------------------------------------------------------------------

template<wis::DeviceExtID>
struct DX12DeviceExtensionMap {
    using Type = wis::DX12DeviceExtension;
};
//-------------------------------------------------------------------------

template<template<typename T> typename Executor, typename... Args>
constexpr static inline decltype(auto) DX12DeviceExtensionBridge(wis::DeviceExtID id, Args&&... args)
{
    switch (id) {
    default:
        return Executor<wis::DX12DeviceExtension>{}(std::forward<Args>(args)...);
    }
}
// DX12DebugExtension methods --
extern "C" WisResult DX12DebugExtensionCreateDebugMessenger(DX12DebugExtension self, DebugCallback callback, void* user_data, DX12DebugMessenger* messenger)
{
    auto* xself = reinterpret_cast<wis::DX12DebugExtension*>(self);
    auto&& [res, value] = xself->CreateDebugMessenger(reinterpret_cast<wis::DebugCallback>(callback), user_data);

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *messenger = reinterpret_cast<DX12DebugMessenger>(new (std::nothrow) wis::DX12DebugMessenger(std::move(value)));
    if (!*messenger)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::DX12DebugMessenger." };
    return reinterpret_cast<WisResult&>(res);
}
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
        return Executor<wis::VKFactoryExtension>{}(std::forward<Args>(args)...);
    }
}

//-------------------------------------------------------------------------

template<wis::DeviceExtID>
struct VKDeviceExtensionMap {
    using Type = wis::VKDeviceExtension;
};
//-------------------------------------------------------------------------

template<template<typename T> typename Executor, typename... Args>
constexpr static inline decltype(auto) VKDeviceExtensionBridge(wis::DeviceExtID id, Args&&... args)
{
    switch (id) {
    default:
        return Executor<wis::VKDeviceExtension>{}(std::forward<Args>(args)...);
    }
}
// VKDebugExtension methods --
extern "C" WisResult VKDebugExtensionCreateDebugMessenger(VKDebugExtension self, DebugCallback callback, void* user_data, VKDebugMessenger* messenger)
{
    auto* xself = reinterpret_cast<wis::VKDebugExtension*>(self);
    auto&& [res, value] = xself->CreateDebugMessenger(reinterpret_cast<wis::DebugCallback>(callback), user_data);

    if (res.status != wis::Status::Ok)
        return reinterpret_cast<WisResult&>(res);

    *messenger = reinterpret_cast<VKDebugMessenger>(new (std::nothrow) wis::VKDebugMessenger(std::move(value)));
    if (!*messenger)
        return WisResult{ StatusOutOfMemory, "Failed to allocate memory for  wis::VKDebugMessenger." };
    return reinterpret_cast<WisResult&>(res);
}
#endif
