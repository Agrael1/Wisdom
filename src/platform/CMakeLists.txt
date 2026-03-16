if (WISDOM_DX12)
  wis_make_target_bundle(
          TARGET
          platform-dx12
          HEADERS
          "wisdom_platform/dx12/dx12_platform_types.hpp"
          SOURCES
          "wisdom_platform/dx12/dx12_platform_win32.cpp"
          "wisdom_platform/dx12/dx12_platform_uwp.cpp"
          LINK_TARGETS
          wisdom-dx12-headers
          INCLUDE_DIRECTORIES
          "${CMAKE_CURRENT_SOURCE_DIR}"
          COMPILE_DEFINITIONS
          $<BUILD_INTERFACE:WIS_PLATFORM_WIN32_PRESENT=${WISDOM_PLATFORM_WIN32_AVAILABLE}>
          $<BUILD_INTERFACE:WIS_PLATFORM_UWP_PRESENT=${WISDOM_PLATFORM_UWP_AVAILABLE}>
          DEFINITION_PREFIX WISDOM_PLATFORM)
endif()

if (WISDOM_VULKAN)
  wis_make_target_bundle(
          TARGET
          platform-vk
          HEADERS
          "wisdom_platform/vulkan/vk_platform_types.hpp"
          SOURCES
          "wisdom_platform/vulkan/vk_platform_win32.cpp"
          LINK_TARGETS
          wisdom-vk-headers
          INCLUDE_DIRECTORIES
          "${CMAKE_CURRENT_SOURCE_DIR}"
          COMPILE_DEFINITIONS
          $<BUILD_INTERFACE:WIS_PLATFORM_WIN32_PRESENT=${WISDOM_PLATFORM_WIN32_AVAILABLE}>
          $<BUILD_INTERFACE:WIS_PLATFORM_XLIB_PRESENT=${WISDOM_PLATFORM_WIN32_AVAILABLE}>
          $<BUILD_INTERFACE:WIS_PLATFORM_XCB_PRESENT=${WISDOM_PLATFORM_WIN32_AVAILABLE}>
          $<BUILD_INTERFACE:WIS_PLATFORM_WAYLAND_PRESENT=${WISDOM_PLATFORM_WIN32_AVAILABLE}>
          DEFINITION_PREFIX WISDOM_PLATFORM)
endif()

# Unified targets Create a convenience target for all headers
add_library(wisdom-platform-headers INTERFACE)
add_library(wis::wisdom-platform-headers ALIAS wisdom-platform-headers)
if (WISDOM_DX12)
    target_link_libraries(wisdom-platform-headers INTERFACE wisdom-platform-dx12-headers)
endif ()
if (WISDOM_VULKAN)
    target_link_libraries(wisdom-platform-headers INTERFACE wisdom-platform-vk-headers)
endif ()

# Install everything, including .cpp for header only usage
install(
        TARGETS wisdom-platform-headers
        EXPORT wisdom-platform-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom_platform/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom_platform)

# Create a convenience target for all static libraries
if (WISDOM_BUILD_STATIC)
    add_library(wisdom-platform STATIC)
    add_library(wis::wisdom-platform ALIAS wisdom-platform)
    target_link_libraries(wisdom-platform PUBLIC wisdom-platform-headers)
    if (WISDOM_DX12)
        target_sources(wisdom-platform PRIVATE $<TARGET_OBJECTS:wisdom-platform-dx12-objects>)
    endif ()
    if (WISDOM_VULKAN)
        target_sources(wisdom-platform PRIVATE $<TARGET_OBJECTS:wisdom-platform-vk-objects>)
    endif ()
    target_compile_definitions(wisdom-platform PUBLIC WISDOM_BUILD_BINARIES=1)
    set_target_properties(wisdom-platform PROPERTIES CXX_STANDARD 20 DEBUG_POSTFIX d)

    install(
            TARGETS wisdom-platform
            EXPORT wisdom-platform-targets
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()

# Create a convenience target for all dynamic libraries
if (WISDOM_BUILD_SHARED)
    include(GenerateExportHeader)

    add_library(wisdom-platform-shared SHARED)
    add_library(wis::wisdom-platform-shared ALIAS wisdom-platform-shared)
    target_link_libraries(wisdom-platform-shared PUBLIC wisdom-platform-headers)
    if (WISDOM_DX12)
        target_sources(wisdom-platform-shared
                PRIVATE $<TARGET_OBJECTS:wisdom-platform-dx12-objects-shared>)
    endif ()
    if (WISDOM_VULKAN)
        target_sources(wisdom-platform-shared
                PRIVATE $<TARGET_OBJECTS:wisdom-platform-vk-objects-shared>)
    endif ()
    target_compile_definitions(wisdom-platform-shared PUBLIC WISDOM_PLATFORM_BUILD_BINARIES=1
            WISDOM_PLATFORM_SHARED_LIBRARY=1)
    set_target_properties(
            wisdom-platform-shared
            PROPERTIES CXX_STANDARD 20
            POSITION_INDEPENDENT_CODE ON
            DEBUG_POSTFIX d)

    generate_export_header(
            wisdom-platform-shared
            BASE_NAME
            WISDOM_PLATFORM
            EXPORT_MACRO_NAME
            WISDOM_PLATFORM_API
            EXPORT_FILE_NAME
            ${CMAKE_CURRENT_SOURCE_DIR}/wisdom_platform/generated/wisdom_exports.h
            STATIC_DEFINE
            WISDOM_PLATFORM_STATIC
            INCLUDE_GUARD_NAME
            WISDOM_PLATFORM_EXPORTS_H)

    install(
            TARGETS wisdom-platform-shared
            EXPORT wisdom-platform-targets
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()