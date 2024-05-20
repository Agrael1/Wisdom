include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

if(WISDOM_WINDOWS)
  find_program(
    NUGET_EXE
    NAMES nuget
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/NuGet)
  if(NOT NUGET_EXE)
    message("NUGET.EXE not found.")
    message(FATAL_ERROR "Please install this executable, and run CMake again.")
  endif()

  # Windows Implementation Library (WIL)
  message("Loading WIL...")
  load_nuget_dependency(${NUGET_EXE} "Microsoft.Windows.ImplementationLibrary"
                        WIL ${CMAKE_CURRENT_BINARY_DIR})

  set(wil_SOURCES
      ${WIL_DIR}
      CACHE INTERNAL "")
  add_library(Wil INTERFACE)
  target_include_directories(
    Wil SYSTEM BEFORE INTERFACE $<BUILD_INTERFACE:${wil_SOURCES}/include>
                                $<INSTALL_INTERFACE:include>)
endif()

# glm
find_package(glm QUIET)
if(NOT glm_FOUND)
  FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG master)
  FetchContent_GetProperties(glm)
  if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    # Disable tinyxml2 installation
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
else()

endif()

# SDL2
find_package(SDL2 CONFIG)

if(NOT SDL2_FOUND)
  FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.26.3
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE)
  FetchContent_MakeAvailable(SDL2)
endif()

# fpng for PNG loading
FetchContent_Declare(
  fpng
  GIT_REPOSITORY https://github.com/richgel999/fpng
  GIT_TAG main)
set(SSE ON)
FetchContent_GetProperties(fpng)
if(NOT fpng_POPULATED)
  FetchContent_Populate(fpng)
  set(fpng_SOURCE_DIR
      ${fpng_SOURCE_DIR}
      PARENT_SCOPE)
endif()
