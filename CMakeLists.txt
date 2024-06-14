# CMakeList.txt : Top-level CMake project file Parameters: WISDOM_GENERATE_API:
# Generate the API files (default: ON if Top Project) WISDOM_BUILD_EXAMPLES:
# Build the examples (default: ON if Top Project) WISDOM_BUILD_TESTS: Build the
# tests (default: ON if Top Project)

# WISDOM_LOG_LEVEL: Log level (values:
# debug;trace;info;warn;error;critical)(default: if DEBUG then "debug" else
# "warn")

# WISDOM_BUILD_BINARIES: Build the binaries (default: ON)

# WISDOM_DEBUG_LAYER: Enable the debug layer (default: OFF) WISDOM_FORCE_VULKAN:
# Force the use of Vulkan (default: OFF) WISDOM_USE_SYSTEM_DXC: Use the system
# DirectX Shader Compiler (default: OFF) WISDOM_USE_FMT: Use fmtlib (default:
# OFF for any system that has std::format) WISDOM_USE_WAYLAND: Use Wayland
# window system (default: ON for Linux) WISDOM_USE_X11: Use X11 window system
# (default: ON for Linux)

# Functions: wis_install_deps: install dependencies for executables

cmake_minimum_required(VERSION 3.22)

project("Wisdom" VERSION 0.1.0)

# Additional CMake functions.
include(cmake/platform.cmake)
include(cmake/settings.cmake)
include(cmake/functions.cmake)
include(cmake/misc.cmake)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Include sub-projects.
add_subdirectory("plugins")
add_subdirectory("generator")

add_subdirectory("wisdom")

if(WISDOM_BUILD_BINDINGS)
  add_subdirectory("bindings")
endif()
if(WISDOM_BUILD_EXAMPLES)
  option(WISDOM_EXCLUDE_KDGUI "Exclude KDGui Example from build" OFF)
  add_subdirectory("examples")
endif()

if(WISDOM_BUILD_TESTS AND NOT WISDOM_WINDOWS_STORE)
  enable_testing()
  add_subdirectory("tests")
endif()
