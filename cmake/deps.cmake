include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(CPM_DONT_UPDATE_MODULE_PATH ON)
set(GET_CPM_FILE "${CMAKE_CURRENT_LIST_DIR}/deps/get_cpm.cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Set CPM source cache
if (NOT CPM_SOURCE_CACHE)
    set(CPM_SOURCE_CACHE "${CMAKE_CURRENT_BINARY_DIR}/_deps_cache")
endif ()


if (NOT EXISTS ${GET_CPM_FILE})
    file(DOWNLOAD
            https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
            "${GET_CPM_FILE}"
    )
endif ()
include(${GET_CPM_FILE})

if (WISDOM_WINDOWS)
    include(${CMAKE_CURRENT_LIST_DIR}/deps/deps_win.cmake)
endif ()

# Use fmtlib
if (WISDOM_USE_FMT)
    find_package(fmt CONFIG QUIET)
    if (fmt_FOUND)
        message("fmtlib found, skipping download.")
    else ()
        message("Loading latest fmtlib...")
        CPMAddPackage(
                NAME fmt
                GIT_REPOSITORY fmtlib/fmt
                GIT_TAG origin/master)
    endif ()
endif ()

# DXCompiler for HLSL compilation
include(${CMAKE_CURRENT_LIST_DIR}/deps/dxc.cmake)

# Vulkan dependencies
if (WISDOM_VULKAN)
    include(${CMAKE_CURRENT_LIST_DIR}/deps/deps_vulkan.cmake)
endif ()

# if tests enabled, add Catch2
if (WISDOM_BUILD_TESTS)
    CPMAddPackage(
            NAME Catch2
            GITHUB_REPOSITORY catchorg/Catch2
            GIT_TAG v3.13.0)
    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
endif()
            