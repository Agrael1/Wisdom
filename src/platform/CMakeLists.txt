if (WISDOM_DX12)
  wis_make_target_bundle(
          TARGET
          platform-dx12
          HEADERS
          "wisdom_platform/dx12/dx12_types.hpp"
          SOURCES
          "wisdom_platform/dx12/dx12_platform_win32.cpp"
          "wisdom_platform/dx12/dx12_platform_uwp.cpp"
          LINK_TARGETS
          wisdom-dx12-headers
          EXPORT_SET
          wisdom-platform-targets
          INCLUDE_DIRECTORIES
          "${CMAKE_CURRENT_SOURCE_DIR}"
          COMPILE_DEFINITIONS
          $<$<BOOL:${WISDOM_PLATFORM_WIN32_AVAILABLE}>:$<BUILD_INTERFACE:WIS_PLATFORM_WIN32_PRESENT=1>>
          $<$<BOOL:${WISDOM_PLATFORM_WIN32_AVAILABLE}>:$<BUILD_INTERFACE:WISDOM_PLATFORM_UWP_PRESENT=1>>
          DEFINITION_PREFIX WISDOM_PLATFORM)
endif()

if (WISDOM_VULKAN)
  wis_make_target_bundle(
          TARGET
          platform-vk
          HEADERS
          "wisdom_platform/vulkan/vk_types.hpp"
          SOURCES
          "wisdom_platform/vulkan/vk_platform_win32.cpp"
          "wisdom_platform/vulkan/vk_platform_wayland.cpp"
          "wisdom_platform/vulkan/vk_platform_xcb.cpp"
          "wisdom_platform/vulkan/vk_platform_xlib.cpp"
          LINK_TARGETS
          wisdom-vk-headers
          EXPORT_SET
          wisdom-platform-targets
          INCLUDE_DIRECTORIES
          "${CMAKE_CURRENT_SOURCE_DIR}"
          COMPILE_DEFINITIONS
          $<$<BOOL:${WISDOM_PLATFORM_WIN32_AVAILABLE}>:$<BUILD_INTERFACE:WIS_PLATFORM_WIN32_PRESENT=1>>
          $<$<BOOL:${WISDOM_PLATFORM_XLIB_AVAILABLE}>:$<BUILD_INTERFACE:WIS_PLATFORM_XLIB_PRESENT=1>>
          $<$<BOOL:${WISDOM_PLATFORM_XCB_AVAILABLE}>:$<BUILD_INTERFACE:WIS_PLATFORM_XCB_PRESENT=1>>
          $<$<BOOL:${WISDOM_PLATFORM_WAYLAND_AVAILABLE}>:$<BUILD_INTERFACE:WIS_PLATFORM_WAYLAND_PRESENT=1>>
          DEFINITION_PREFIX WISDOM_PLATFORM)
endif()

set(WISDOM_PLATFORM_BACKENDS)
if (WISDOM_DX12)
    list(APPEND WISDOM_PLATFORM_BACKENDS platform-dx12)
endif ()
if (WISDOM_VULKAN)
    list(APPEND WISDOM_PLATFORM_BACKENDS platform-vk)
endif ()

wis_make_unified_targets(
        TARGET wisdom-platform
        EXPORT_SET wisdom-platform-targets
        BACKENDS ${WISDOM_PLATFORM_BACKENDS}
        HEADER_COMPILE_DEFINITIONS ${WISDOM_INCLUDE_HEADER_DEFINITIONS} WISDOM_PLATFORM_STATIC=1
        STATIC_COMPILE_DEFINITIONS WISDOM_PLATFORM_BUILD_BINARIES=1 WISDOM_PLATFORM_STATIC=1
        SHARED_COMPILE_DEFINITIONS WISDOM_PLATFORM_BUILD_BINARIES=1 WISDOM_PLATFORM_SHARED_LIBRARY=1
        EXPORT_BASE_NAME WISDOM_PLATFORM
        EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/wisdom_platform/generated/wisdom_exports.h)

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