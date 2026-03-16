function(wisdom_detect_platform_extensions)
    set(WISDOM_PLATFORM_WIN32_AVAILABLE FALSE CACHE INTERNAL "Win32 platform extension available" FORCE)
    set(WISDOM_PLATFORM_UWP_AVAILABLE FALSE CACHE INTERNAL "UWP platform extension available" FORCE)
    set(WISDOM_PLATFORM_XLIB_AVAILABLE FALSE CACHE INTERNAL "Xlib platform extension available" FORCE)
    set(WISDOM_PLATFORM_XCB_AVAILABLE FALSE CACHE INTERNAL "XCB platform extension available" FORCE)
    set(WISDOM_PLATFORM_WAYLAND_AVAILABLE FALSE CACHE INTERNAL "Wayland platform extension available" FORCE)

    set(_wisdom_available_platforms)

    if (WISDOM_WINDOWS)
        set(WISDOM_PLATFORM_WIN32_AVAILABLE TRUE CACHE INTERNAL "Win32 platform extension available" FORCE)
        list(APPEND _wisdom_available_platforms "WIN32")

        if (WISDOM_WINDOWS_STORE)
            set(WISDOM_PLATFORM_UWP_AVAILABLE TRUE CACHE INTERNAL "UWP platform extension available" FORCE)
            list(APPEND _wisdom_available_platforms "UWP")
        endif ()
    endif ()

    if (WISDOM_LINUX)
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/ecm")

        find_package(X11 QUIET)
        if (X11_FOUND)
            set(WISDOM_PLATFORM_XLIB_AVAILABLE TRUE CACHE INTERNAL "Xlib platform extension available" FORCE)
            list(APPEND _wisdom_available_platforms "XLIB")
        endif ()

        find_package(XCB COMPONENTS XCB QUIET)
        if (XCB_FOUND)
            set(WISDOM_PLATFORM_XCB_AVAILABLE TRUE CACHE INTERNAL "XCB platform extension available" FORCE)
            list(APPEND _wisdom_available_platforms "XCB")
        endif ()

        find_package(Wayland QUIET)
        if (Wayland_FOUND)
            set(WISDOM_PLATFORM_WAYLAND_AVAILABLE TRUE CACHE INTERNAL "Wayland platform extension available" FORCE)
            list(APPEND _wisdom_available_platforms "WAYLAND")
        endif ()
    endif ()

    if (_wisdom_available_platforms)
        list(JOIN _wisdom_available_platforms ", " WISDOM_AVAILABLE_PLATFORMS)
    else ()
        set(WISDOM_AVAILABLE_PLATFORMS "None")
    endif ()

    set(WISDOM_AVAILABLE_PLATFORMS ${WISDOM_AVAILABLE_PLATFORMS} CACHE INTERNAL "Available platform extensions" FORCE)
endfunction()
