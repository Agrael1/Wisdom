# CMakeList.txt : Top-level CMake project file
cmake_minimum_required(VERSION 3.25)

project("Wisdom" VERSION 0.1.0)

# Enable Hot Reload for MSVC compilers if supported.
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
    "$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CURR_LEVEL "debug")
else()
  set(CURR_LEVEL "warn")
endif()

# Build type
set(DEF_BUILD
    "static"
    CACHE INTERNAL "Default build type for library")
set(BUILD_TYPES
    "headers;static;modules"
    CACHE INTERNAL "List of possible build types for library")
set(WISDOM_BUILD_TYPE
    ${DEF_BUILD}
    CACHE STRING "Wisdom library build type")
set_property(CACHE WISDOM_BUILD_TYPE PROPERTY STRINGS ${BUILD_TYPES})
list(FIND BUILD_TYPES ${WISDOM_BUILD_TYPE} BUILD_INDEX)
message("[Wisdom] Build is ${WISDOM_BUILD_TYPE} [${BUILD_INDEX}]")

# Log level
set(WISDOM_LOG_LEVEL
    ${CURR_LEVEL}
    CACHE STRING "Wisdom library logging level")
set(SEVERITY_LEVELS
    "debug;trace;info;warn;error;critical"
    CACHE INTERNAL "List of possible severities for logging")
set_property(CACHE WISDOM_LOG_LEVEL PROPERTY STRINGS ${SEVERITY_LEVELS})
list(FIND SEVERITY_LEVELS ${WISDOM_LOG_LEVEL} SEV_INDEX)
message("[Wisdom] Log Level is set to ${WISDOM_LOG_LEVEL} [${SEV_INDEX}]")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(WTOP ON)
else()
  set(WTOP OFF)
endif()

if(WTOP AND NOT WINDOWS_STORE)
  set(GEN ON)
endif()

option(WISDOM_RUNTIME_ASSERTS
  "Turn on/off runtime assertions (turned on might slightly affect performance)"
  ON)
option(WISDOM_FORCE_VULKAN
       "Forces the default device to be vulkan, instead of platform specific"
       OFF)
option(WISDOM_BUILD_EXAMPLES "Build the example project." ${WTOP})
option(WISDOM_BUILD_TESTS "Build the tests." ${WTOP})
option(WISDOM_BUILD_DOCS "Build the documentation." ${WTOP})
option(WISDOM_GENERATE_API "Generate the API" ${GEN})
option(WISDOM_USE_SYSTEM_DXC "Use dxc from PATH" OFF)

if(WISDOM_FORCE_VULKAN)
  message("[Wisdom] Forcing vulkan as default device")
else()
  message("[Wisdom] Using platform specific default device")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
   AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "13"
   OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
   AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16")
  option(WISDOM_USE_FMT "Build Wisdom with fmtlib" ON)
else()
  option(WISDOM_USE_FMT "Build Wisdom with fmtlib" OFF)
endif()

if(WISDOM_BUILD_TYPE STREQUAL "modules")
  message(
    WARNING
      "WARNING: modules are at beta support, use this build mode at your own risk."
  )
  if(CMAKE_MINOR_VERSION EQUAL 25)
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API
        "3c375311-a3c9-4396-a187-3227ef642046")
  else()
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API
        "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
  endif()
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
endif()

set(WISDOM_VERSION "v${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR}")

# TODO: find a better way to do this (only required by vulkan-hpp) and make
# wayland support a build option
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_compile_definitions(VK_USE_PLATFORM_WAYLAND_KHR)
endif()

# Additional CMake functions.
include(Misc.cmake)
include(cmake/Functions.cmake)

# Include sub-projects.
add_subdirectory("plugins")
if(WISDOM_GENERATE_API)
  add_subdirectory("generator")
endif()

add_subdirectory("wisdom")

if(WISDOM_BUILD_EXAMPLES)
  add_subdirectory("examples")
endif()

if(WISDOM_BUILD_TESTS)
  add_subdirectory("tests")
endif()
