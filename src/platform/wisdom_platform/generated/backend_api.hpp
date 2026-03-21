// This file is generated. Do not edit directly.
#ifndef WISDOM_PLATFORM_CPP_BACKEND_API_HPP
#define WISDOM_PLATFORM_CPP_BACKEND_API_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include "cpp_api.hpp"
#include "backend_api.h"
#include <wisdom/global/internal.hpp>

#ifdef WISDOM_DX12
#include <wisdom_platform/dx12/dx12_types.hpp>

namespace wis {
struct DX12Win32ExtensionDeleter {
    void operator()(WisDX12Win32Extension* handle) noexcept
    {
        ::wisDX12DestroyWin32Extension(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Extension for Win32 surface creation functions.
 *
 * */
class DX12Win32Extension : public wis::impl::Implements<wis::impl::DX12Win32ExtensionImpl, WisDX12Win32Extension, wis::DX12Win32ExtensionDeleter>
{
public:
    DX12Win32Extension() noexcept
        : ImplType(std::in_place)
    {
        ::wisDX12InitWin32Extension(GetStorage());
    }
    // Operator & overload
    wis::DX12InstanceExtensionHeader* operator&() noexcept
    {
        return &GetMutableInternal().header;
    }

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a surface using Win32.
     * @param info Win32 windowing data.
     * @param out_result denoting the outcome of operation.
     * @return surface points to wis::Surface, initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12Surface CreateSurface(const wis::Win32WindowDesc& info,
                                                        wis::Result&                out_result) noexcept
    {
        wis::DX12Surface surface;
        out_result = convert_result_dx(::wisDX12Win32ExtensionCreateSurface(&_impl_storage,
                                                                            reinterpret_cast<const WisWin32WindowDesc*>(&info),
                                                                            surface.GetStorage()));
        return surface;
    }
};

struct DX12UWPExtensionDeleter {
    void operator()(WisDX12UWPExtension* handle) noexcept
    {
        ::wisDX12DestroyUWPExtension(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Extension for UWP surface creation functions.
 *
 * */
class DX12UWPExtension : public wis::impl::Implements<wis::impl::DX12UWPExtensionImpl, WisDX12UWPExtension, wis::DX12UWPExtensionDeleter>
{
public:
    DX12UWPExtension() noexcept
        : ImplType(std::in_place)
    {
        ::wisDX12InitUWPExtension(GetStorage());
    }
    // Operator & overload
    wis::DX12InstanceExtensionHeader* operator&() noexcept
    {
        return &GetMutableInternal().header;
    }

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a surface using UWP.
     * @param info UWP windowing data.
     * @param out_result denoting the outcome of operation.
     * @return surface points to wis::Surface, initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12Surface CreateSurface(const wis::UWPWindowDesc& info,
                                                        wis::Result&              out_result) noexcept
    {
        wis::DX12Surface surface;
        out_result = convert_result_dx(::wisDX12UWPExtensionCreateSurface(&_impl_storage,
                                                                          reinterpret_cast<const WisUWPWindowDesc*>(&info),
                                                                          surface.GetStorage()));
        return surface;
    }
};

} // namespace wis
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
#include <wisdom_platform/vulkan/vk_types.hpp>

namespace wis {
struct VKXlibExtensionDeleter {
    void operator()(WisVKXlibExtension* handle) noexcept
    {
        ::wisVKDestroyXlibExtension(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Extension for Xlib surface creation functions.
 *
 * */
class VKXlibExtension : public wis::impl::Implements<wis::impl::VKXlibExtensionImpl, WisVKXlibExtension, wis::VKXlibExtensionDeleter>
{
public:
    VKXlibExtension() noexcept
        : ImplType(std::in_place)
    {
        ::wisVKInitXlibExtension(GetStorage());
    }
    // Operator & overload
    wis::VKInstanceExtensionHeader* operator&() noexcept
    {
        return &GetMutableInternal().header;
    }

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a Vulkan surface using Xlib.
     * @param info Xlib windowing data.
     * @param out_result denoting the outcome of operation.
     * @return surface points to wis::Surface, initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKSurface CreateSurface(const wis::XlibWindowDesc& info,
                                                      wis::Result&               out_result) noexcept
    {
        wis::VKSurface surface;
        out_result = convert_result_vk(::wisVKXlibExtensionCreateSurface(&_impl_storage,
                                                                         reinterpret_cast<const WisXlibWindowDesc*>(&info),
                                                                         surface.GetStorage()));
        return surface;
    }
};

struct VKXCBExtensionDeleter {
    void operator()(WisVKXCBExtension* handle) noexcept
    {
        ::wisVKDestroyXCBExtension(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Extension for Xlib surface creation functions.
 *
 * */
class VKXCBExtension : public wis::impl::Implements<wis::impl::VKXCBExtensionImpl, WisVKXCBExtension, wis::VKXCBExtensionDeleter>
{
public:
    VKXCBExtension() noexcept
        : ImplType(std::in_place)
    {
        ::wisVKInitXCBExtension(GetStorage());
    }
    // Operator & overload
    wis::VKInstanceExtensionHeader* operator&() noexcept
    {
        return &GetMutableInternal().header;
    }

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a surface using Win32.
     * @param info XCB windowing data.
     * @param out_result denoting the outcome of operation.
     * @return surface points to wis::Surface, initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKSurface CreateSurface(const wis::XCBWindowDesc& info,
                                                      wis::Result&              out_result) noexcept
    {
        wis::VKSurface surface;
        out_result = convert_result_vk(::wisVKXCBExtensionCreateSurface(&_impl_storage,
                                                                        reinterpret_cast<const WisXCBWindowDesc*>(&info),
                                                                        surface.GetStorage()));
        return surface;
    }
};

struct VKWaylandExtensionDeleter {
    void operator()(WisVKWaylandExtension* handle) noexcept
    {
        ::wisVKDestroyWaylandExtension(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Extension for Xlib surface creation functions.
 *
 * */
class VKWaylandExtension : public wis::impl::Implements<wis::impl::VKWaylandExtensionImpl, WisVKWaylandExtension, wis::VKWaylandExtensionDeleter>
{
public:
    VKWaylandExtension() noexcept
        : ImplType(std::in_place)
    {
        ::wisVKInitWaylandExtension(GetStorage());
    }
    // Operator & overload
    wis::VKInstanceExtensionHeader* operator&() noexcept
    {
        return &GetMutableInternal().header;
    }

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a surface using Wayland.
     * @param info Wayland windowing data.
     * @param out_result denoting the outcome of operation.
     * @return surface points to wis::Surface, initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKSurface CreateSurface(const wis::WaylandWindowDesc& info,
                                                      wis::Result&                  out_result) noexcept
    {
        wis::VKSurface surface;
        out_result = convert_result_vk(::wisVKWaylandExtensionCreateSurface(&_impl_storage,
                                                                            reinterpret_cast<const WisWaylandWindowDesc*>(&info),
                                                                            surface.GetStorage()));
        return surface;
    }
};

struct VKWin32ExtensionDeleter {
    void operator()(WisVKWin32Extension* handle) noexcept
    {
        ::wisVKDestroyWin32Extension(handle);
    }
};
/**
 * @brief Provided by Wisdom 0.7.0. Extension for Win32 surface creation functions.
 *
 * */
class VKWin32Extension : public wis::impl::Implements<wis::impl::VKWin32ExtensionImpl, WisVKWin32Extension, wis::VKWin32ExtensionDeleter>
{
public:
    VKWin32Extension() noexcept
        : ImplType(std::in_place)
    {
        ::wisVKInitWin32Extension(GetStorage());
    }
    // Operator & overload
    wis::VKInstanceExtensionHeader* operator&() noexcept
    {
        return &GetMutableInternal().header;
    }

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a surface using Win32.
     * @param info Win32 windowing data.
     * @param out_result denoting the outcome of operation.
     * @return surface points to wis::Surface, initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKSurface CreateSurface(const wis::Win32WindowDesc& info,
                                                      wis::Result&                out_result) noexcept
    {
        wis::VKSurface surface;
        out_result = convert_result_vk(::wisVKWin32ExtensionCreateSurface(&_impl_storage,
                                                                          reinterpret_cast<const WisWin32WindowDesc*>(&info),
                                                                          surface.GetStorage()));
        return surface;
    }
};

} // namespace wis
#endif // WISDOM_VULKAN

#endif // WISDOM_PLATFORM_CPP_BACKEND_API_HPP
