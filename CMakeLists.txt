cmake_minimum_required(VERSION 3.22)

include(cmake/version.cmake)
project(
        wisdom
        VERSION ${WISDOM_VERSION}
        LANGUAGES C CXX)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(cmake/functions.cmake)
wisdom_detect_platform()
include(cmake/platform.cmake)


# Determine which options to use by default
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
        AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "13"
        OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
        AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16")
    set(LOCAL_USE_FMTLIB TRUE)
else ()
    set(LOCAL_USE_FMTLIB FALSE)
endif ()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(WTOP ON)
else ()
    set(WTOP OFF)
endif ()

# Options
option(WISDOM_USE_FMT "Build Wisdom with fmtlib" ${LOCAL_USE_FMTLIB})
option(WISDOM_FORCE_VULKAN "Force Vulkan support" OFF)
option(WISDOM_BUILD_EXAMPLES "Build the example project." ${WTOP})
option(WISDOM_BUILD_TESTS "Build the tests." ${WTOP})
option(WISDOM_BUILD_STATIC "Build the static lib." ON)
option(WISDOM_BUILD_SHARED "Build the dynamic lib." ON)
option(WISDOM_BUILD_PLATFORM "Build unified platform extension library." ON)
option(WISDOM_BUILD_DOCS "Build the documentation." OFF)

if (WISDOM_BUILD_PLATFORM)
    wisdom_detect_platform_extensions()
else ()
    set(WISDOM_AVAILABLE_PLATFORMS "Disabled")
endif ()

# DXC deployment options
set(WISDOM_DXC_PATH
        ""
        CACHE PATH "Path to custom DXC installation (optional)")
set(WISDOM_VULKAN_HEADER_PATH
        ""
        CACHE PATH "Path to custom Vulkan Headers (optional)")

# Load all dependencies
include(cmake/deps.cmake)
include(cmake/doc.cmake)
include(cmake/targets.cmake)

message(
        STATUS
        "Wisdom Configuration:
    WISDOM_VULKAN: ${WISDOM_VULKAN}
    WISDOM_DX12: ${WISDOM_DX12}
    WISDOM_USE_FMT: ${WISDOM_USE_FMT}
    WISDOM_VERSION: ${WISDOM_VERSION}
    WISDOM_PLATFORM: ${WISDOM_PLATFORM}

    WISDOM_BUILD_DOCS: ${WISDOM_BUILD_DOCS}
    WISDOM_BUILD_EXAMPLES: ${WISDOM_BUILD_EXAMPLES}
    WISDOM_BUILD_TESTS: ${WISDOM_BUILD_TESTS}
    WISDOM_BUILD_STATIC: ${WISDOM_BUILD_STATIC}
    WISDOM_BUILD_SHARED: ${WISDOM_BUILD_SHARED}
    WISDOM_BUILD_PLATFORM: ${WISDOM_BUILD_PLATFORM}
    WISDOM_AVAILABLE_PLATFORMS: ${WISDOM_AVAILABLE_PLATFORMS}

    WISDOM_VULKAN_HEADER_PATH: ${WISDOM_VULKAN_HEADER_PATH}

    DXC Configuration:
      Custom Path: ${WISDOM_DXC_PATH}
      Executable: ${DXC_EXECUTABLE}
        ")

if (WISDOM_BUILD_EXAMPLES AND WISDOM_BUILD_TESTS)
    add_subdirectory(generator)
endif ()
add_subdirectory(src) # add main libraries

if (WISDOM_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if (WISDOM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

install(FILES README.md DESTINATION .)
install(FILES LICENSE.txt DESTINATION .)
install(FILES version/VERSION DESTINATION version)
install(FILES cmake/functions.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/wisdom-config-version.cmake
        VERSION ${WISDOM_VERSION}
        COMPATIBILITY SameMajorVersion)
configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/install/wisdom-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/wisdom-config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/wisdom-config-version.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/wisdom-config.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)
include(cmake/install/nuget.cmake)
