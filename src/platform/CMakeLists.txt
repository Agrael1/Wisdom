set(WISDOM_PLATFORM_SOURCES)
set(WISDOM_PLATFORM_DEFINES
    $<BUILD_INTERFACE:$<$<BOOL:${WISDOM_PLATFORM_WIN32_PRESENT}>:WIS_PLATFORM_WIN32_PRESENT=1>>
    $<BUILD_INTERFACE:$<$<BOOL:${WISDOM_PLATFORM_WIN32_PRESENT}>:WISDOM_PLATFORM_UWP_PRESENT=1>>
    $<BUILD_INTERFACE:$<$<BOOL:${WISDOM_PLATFORM_XLIB_PRESENT}>:WIS_PLATFORM_XLIB_PRESENT=1>>
    $<BUILD_INTERFACE:$<$<BOOL:${WISDOM_PLATFORM_XCB_PRESENT}>:WIS_PLATFORM_XCB_PRESENT=1>>
    $<BUILD_INTERFACE:$<$<BOOL:${WISDOM_PLATFORM_WAYLAND_PRESENT}>:WIS_PLATFORM_WAYLAND_PRESENT=1>>
)

if (WISDOM_DX12)
    list(APPEND WISDOM_PLATFORM_SOURCES
            "wisdom_platform/dx12/dx12_platform_win32.cpp"
            "wisdom_platform/dx12/dx12_platform_uwp.cpp")
endif ()

if (WISDOM_VULKAN)
    list(APPEND WISDOM_PLATFORM_SOURCES
            "wisdom_platform/vulkan/vk_platform_win32.cpp"
            "wisdom_platform/vulkan/vk_platform_wayland.cpp"
            "wisdom_platform/vulkan/vk_platform_xcb.cpp"
            "wisdom_platform/vulkan/vk_platform_xlib.cpp")
endif ()

# Platform header target
add_library(wisdom-platform-headers INTERFACE)
add_library(wis::wisdom-platform-headers ALIAS wisdom-platform-headers)

target_include_directories(wisdom-platform-headers
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(wisdom-platform-headers INTERFACE wis::wisdom-headers)
target_compile_definitions(wisdom-platform-headers INTERFACE WISDOM_PLATFORM_STATIC=1 ${WISDOM_PLATFORM_DEFINES})

install(
    TARGETS wisdom-platform-headers
    EXPORT wisdom-platform-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Platform library target
if (WISDOM_BUILD_STATIC)
    add_library(wisdom-platform STATIC ${WISDOM_PLATFORM_SOURCES})
    add_library(wis::wisdom-platform ALIAS wisdom-platform)

    target_link_libraries(wisdom-platform PRIVATE wisdom)
    target_compile_definitions(wisdom-platform PUBLIC WISDOM_PLATFORM_STATIC=1 ${WISDOM_PLATFORM_DEFINES})
    target_include_directories(wisdom-platform
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

    set_target_properties(wisdom-platform PROPERTIES
            CXX_STANDARD 20
            # UNITY_BUILD ON
            DEBUG_POSTFIX d)


    install(
        TARGETS wisdom-platform
        EXPORT wisdom-platform-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()

if (WISDOM_BUILD_SHARED)
    add_library(wisdom-platform-shared SHARED ${WISDOM_PLATFORM_SOURCES})
    add_library(wis::wisdom-platform-shared ALIAS wisdom-platform-shared)

    target_link_libraries(wisdom-platform-shared PRIVATE wisdom)
    target_include_directories(wisdom-platform-shared
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
    target_compile_definitions(wisdom-platform-shared
            PUBLIC WISDOM_PLATFORM_SHARED_LIBRARY=1 ${WISDOM_PLATFORM_DEFINES}
            PRIVATE wisdom_platform_shared_EXPORTS=1)

    set_target_properties(wisdom-platform-shared PROPERTIES
            CXX_STANDARD 20
            # UNITY_BUILD ON
            POSITION_INDEPENDENT_CODE ON
            DEBUG_POSTFIX d)

    include(GenerateExportHeader)
    generate_export_header(
            wisdom-platform-shared
            BASE_NAME WISDOM_PLATFORM
            EXPORT_MACRO_NAME WISDOM_PLATFORM_API
            EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/wisdom_platform/generated/wisdom_exports.h
            STATIC_DEFINE WISDOM_PLATFORM_STATIC
            INCLUDE_GUARD_NAME WISDOM_PLATFORM_EXPORTS_H)

    install(
        TARGETS wisdom-platform-shared
        EXPORT wisdom-platform-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom_platform/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom_platform)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom)

install(
        EXPORT wisdom-platform-targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
        NAMESPACE wis::
        FILE wisdom-platform-targets.cmake
)