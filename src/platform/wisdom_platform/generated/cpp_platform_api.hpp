// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_PLATFORM_API_HPP
#define WISDOM_CPP_PLATFORM_API_HPP
#ifdef __cplusplus
#include <wisdom_platform/generated/c_platform_api.h>
#include <wisdom/global/internal.hpp>
#include <wisdom/bridge/span.hpp>

#ifdef WISDOM_DX12
#include <wisdom_platform/dx12/dx12_platform_types.hpp>
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
#include <wisdom_platform/vulkan/vk_platform_types.hpp>
#endif // WISDOM_VULKAN

namespace wis {
/**
 * @brief Provided by Wisdom 0.7.0. X11 platform using Xlib.
 *
 * */
/**
 * @brief Provided by Wisdom 0.7.0. X11 surface creation info. Uses opaque types to avoid Xlib.h inclusion in public headers.
 *
 * */
struct XlibWindowDesc {
    void*         display; ///< Pointer to the X11 Display. Cast to Display* internally.
    std::uint64_t window; ///< The X11 Window ID. Fits standard 32/64-bit window handles.
};

#if defined(WISDOM_VULKAN)

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

#endif // defined(WISDOM_VULKAN)

/**
 * @brief Provided by Wisdom 0.7.0. X11 platform using XCB.
 *
 * */
/**
 * @brief Provided by Wisdom 0.7.0. X11 surface creation info for XCB. Uses opaque types to avoid XCB headers in public headers.
 *
 * */
struct XCBWindowDesc {
    void*         connection; ///< Pointer to the XCB connection. Cast to xcb_connection_t* internally.
    std::uint32_t window; ///< The X11 Window ID. Fits standard 32/64-bit window handles.
};

#if defined(WISDOM_VULKAN)

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

#endif // defined(WISDOM_VULKAN)

/**
 * @brief Provided by Wisdom 0.7.0. Wayland platform.
 *
 * */
/**
 * @brief Provided by Wisdom 0.7.0. Wayland surface creation info. Uses opaque types to avoid Wayland headers in public headers.
 *
 * */
struct WaylandWindowDesc {
    void* display; ///< Pointer to the Wayland display. Cast to wl_display* internally.
    void* surface; ///< Pointer to the Wayland surface. Cast to wl_surface* internally.
};

#if defined(WISDOM_VULKAN)

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

#endif // defined(WISDOM_VULKAN)

/**
 * @brief Provided by Wisdom 0.7.0. Standard Windows platform.
 *
 * */
/**
 * @brief Provided by Wisdom 0.7.0. Win32 surface creation info.
 *
 * */
struct Win32WindowDesc {
    void* hinstance; ///< HINSTANCE of the window. Cast to HINSTANCE internally.
    void* hwnd; ///< HWND of the window. Cast to HWND internally.
};

#if defined(WISDOM_DX12)

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

#endif // defined(WISDOM_DX12)

#if defined(WISDOM_VULKAN)

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

#endif // defined(WISDOM_VULKAN)

/**
 * @brief Provided by Wisdom 0.7.0. Universal Windows Platform.
 *
 * */
/**
 * @brief Provided by Wisdom 0.7.0. UWP surface creation info. Uses opaque types to avoid Windows Runtime headers in public headers.
 *
 * */
struct UWPWindowDesc {
    void* core_window; ///< Pointer to the UWP CoreWindow. Cast to ICoreWindow* internally.
};

#if defined(WISDOM_DX12)

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

#endif // defined(WISDOM_DX12)

} // namespace wis
#endif // __cplusplus
#endif // WISDOM_CPP_PLATFORM_API_HPP
