include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(GET_CPM_FILE "${CMAKE_CURRENT_LIST_DIR}/get_cpm.cmake")

if (NOT EXISTS ${GET_CPM_FILE})
  file(DOWNLOAD
      https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
      "${GET_CPM_FILE}"
  )
endif()
include(${GET_CPM_FILE})

# SDL2
CPMAddPackage(
  NAME SDL3
  GITHUB_REPOSITORY libsdl-org/SDL
  GIT_TAG preview-3.1.3
  OPTIONS
  "SDL_WERROR OFF"
)

# glm
CPMAddPackage(
  NAME glm
  GITHUB_REPOSITORY g-truc/glm
  GIT_TAG origin/master
)

# fpng
CPMAddPackage(
  NAME fpng
  GITHUB_REPOSITORY richgel999/fpng
  GIT_TAG main
  DOWNLOAD_ONLY ON
)

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
