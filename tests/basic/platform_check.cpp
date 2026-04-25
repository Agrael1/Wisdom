#include <wisdom/wisdom.h>
#include <wisdom/wisdom_platform.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("check_platform_support")
{
    WisXCBExtension xcb_extension = {0};
    WisXlibExtension xlib_extension = {0};
    WisWaylandExtension wayland_extension = {0};
    WisWin32Extension win32_extension = {0};
    wisInitXCBExtension(&xcb_extension);
    wisInitXlibExtension(&xlib_extension);
    wisInitWaylandExtension(&wayland_extension);
    wisInitWin32Extension(&win32_extension);

    WisInstance instance = {0};
    WisInstanceExtensionHeader* extensions[] = {
        &xcb_extension.header,
        &xlib_extension.header,
        &wayland_extension.header,
        &win32_extension.header,
    };
    WisResult result = wisCreateInstance(
        NULL,
        extensions,
        sizeof(extensions) / sizeof(WisInstanceExtensionHeader*),
        &instance
    );

    // Expect partial success
    REQUIRE(result.status >= 0); 

#ifdef WISDOM_WINDOWS
    printf("XCB supported: %s\n", wisXCBExtensionSupported(&xcb_extension) ? "Yes" : "No");
    printf("Xlib supported: %s\n", wisXlibExtensionSupported(&xlib_extension) ? "Yes" : "No");
    printf("Wayland supported: %s\n", wisWaylandExtensionSupported(&wayland_extension) ? "Yes" : "No");
    printf("Win32 supported: %s\n", wisWin32ExtensionSupported(&win32_extension) ? "Yes" : "No");
    REQUIRE(wisWin32ExtensionSupported(&win32_extension));
#elif defined(WISDOM_LINUX)
    // just print ones supported, as support can vary based on environment (e.g. Wayland vs X11)
    bool xcb_supported = wisXCBExtensionSupported(&xcb_extension);
    bool xlib_supported = wisXlibExtensionSupported(&xlib_extension);
    bool wayland_supported = wisWaylandExtensionSupported(&wayland_extension);
    bool win32_supported = wisWin32ExtensionSupported(&win32_extension);


    printf("XCB supported: %s\n", xcb_supported ? "Yes" : "No");
    printf("Xlib supported: %s\n", xlib_supported ? "Yes" : "No");
    printf("Wayland supported: %s\n", wayland_supported ? "Yes" : "No");
    printf("Win32 supported: %s\n", win32_supported ? "Yes" : "No");

    // At least one of the Linux surface extensions should be supported
    REQUIRE(xcb_supported || xlib_supported || wayland_supported);
#endif

    wisDestroyInstance(&instance);
    REQUIRE(!wisHandleValid(&instance));

    wisDestroyXCBExtension(&xcb_extension);
    wisDestroyXlibExtension(&xlib_extension);
    wisDestroyWaylandExtension(&wayland_extension);
    wisDestroyWin32Extension(&win32_extension);

    REQUIRE(!wisHandleValid(&xcb_extension));
    REQUIRE(!wisHandleValid(&xlib_extension));
    REQUIRE(!wisHandleValid(&wayland_extension));
    REQUIRE(!wisHandleValid(&win32_extension));
}
