include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

if(WISDOM_USE_FMT)
  find_package(fmt 10.0 CONFIG)
  if(fmt_FOUND)
    message("fmtlib found, skipping download.")
  else()
    message("Loading latest fmtlib...")
    FetchContent_Declare(
      fmt
      GIT_REPOSITORY https://github.com/fmtlib/fmt.git
      GIT_TAG origin/master)
    FetchContent_MakeAvailable(fmt)
  endif()
endif()

# Win32 plugins
if(WISDOM_WINDOWS)
  find_program(
    NUGET_EXE
    NAMES nuget
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/NuGet)
  if(NOT NUGET_EXE)
    message("NUGET.EXE not found.")
    message(FATAL_ERROR "Please install this executable, and run CMake again.")
  endif()

  add_subdirectory("DX12Agility")
  add_subdirectory("DX12Allocator")
endif()

find_package(Vulkan)

# UWP not supported
if(Vulkan_FOUND AND NOT WISDOM_WINDOWS_STORE)
  message("Loading wisvk utils...")
  FetchContent_Declare(
    wisvk
    GIT_REPOSITORY https://github.com/Agrael1/Wisdom-VkUtils.git
    GIT_TAG origin/master)
  set(WISVK_ONLY_HEADERS
      ON
      CACHE BOOL "" FORCE)
  FETCHCONTENT_MAKEAVAILABLE(wisvk)

  # Vulkan Memory Allocator
  add_subdirectory("VKAllocator")
else()
  set(WISDOM_FORCE_VULKAN
      FALSE
      CACHE BOOL "Force Vulkan" FORCE)
endif()

add_subdirectory("DXC")

set(PLUGINS_LOADED
    TRUE
    CACHE BOOL "Plugins Loaded switch")
