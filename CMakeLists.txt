cmake_minimum_required(VERSION 3.22)

include(cmake/version.cmake)
project(Wisdom VERSION ${WISDOM_VERSION} LANGUAGES C CXX)

include(cmake/platform.cmake)
wisdom_detect_platform()


if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
   AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "13"
   OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
   AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16")
  set (LOCAL_USE_FMTLIB TRUE PARENT_SCOPE)
else()
  set (LOCAL_USE_FMTLIB FALSE PARENT_SCOPE)
endif()

option(WISDOM_USE_FMT "Build Wisdom with fmtlib" ${LOCAL_USE_FMTLIB})

# Load all dependencies
include(cmake/deps.cmake)

message(STATUS "Wisdom Configuration:
    WISDOM_VULKAN: ${WISDOM_VULKAN}
    WISDOM_DX12: ${WISDOM_DX12}
    WISDOM_USE_FMT: ${WISDOM_USE_FMT}
    WISDOM_VERSION: ${WISDOM_VERSION}
    WISDOM_PLATFORM: ${WISDOM_PLATFORM}
")

add_library(WisdomCore INTERFACE)