cmake_minimum_required(VERSION 3.22)

include(cmake/version.cmake)
project(wisdom VERSION ${WISDOM_VERSION} LANGUAGES C CXX)

include(cmake/platform.cmake)
include(cmake/misc.cmake)
wisdom_detect_platform()

# Determine which options to use by default
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
   AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "13"
   OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
   AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16")
  set (LOCAL_USE_FMTLIB TRUE)
else()
  set (LOCAL_USE_FMTLIB FALSE)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(WTOP ON)
else()
  set(WTOP OFF)
endif()

# Options
option(WISDOM_USE_FMT "Build Wisdom with fmtlib" ${LOCAL_USE_FMTLIB})
option(WISDOM_FORCE_VULKAN "Force Vulkan support" OFF)
option(WISDOM_BUILD_EXAMPLES "Build the example project." ${WTOP})
option(WISDOM_BUILD_TESTS "Build the tests." ${WTOP})
option(WISDOM_BUILD_BINARIES "Build the static lib." ON)
option(WISDOM_BUILD_DOCS "Build the documentation." ${WTOP})

# DXC deployment options
set(WISDOM_DXC_PATH "" CACHE PATH "Path to custom DXC installation (optional)")

# Load all dependencies
include(cmake/deps.cmake)
include(cmake/doc.cmake)

message(STATUS "Wisdom Configuration:
    WISDOM_VULKAN: ${WISDOM_VULKAN}
    WISDOM_DX12: ${WISDOM_DX12}
    WISDOM_USE_FMT: ${WISDOM_USE_FMT}
    WISDOM_VERSION: ${WISDOM_VERSION}
    WISDOM_PLATFORM: ${WISDOM_PLATFORM}

    WISDOM_BUILD_DOCS: ${WISDOM_BUILD_DOCS}

    DXC Configuration:
      Custom Path: ${WISDOM_DXC_PATH}
      Executable: ${DXC_EXECUTABLE}
")

add_subdirectory(generator)
add_subdirectory(src) # add main libraries