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
  target_install_interface(Wil)
  target_include_directories(
    Wil SYSTEM BEFORE INTERFACE $<BUILD_INTERFACE:${wil_SOURCES}/include>
                                $<INSTALL_INTERFACE:include>)
endif()

# glm
find_package(glm CONFIG)
if(NOT glm_FOUND)
  FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG master
    OVERRIDE_FIND_PACKAGE)
  FetchContent_MakeAvailable(glm)
endif()

message("GLM: ${glm_DIR}")

# KDAB KDUtils
if(NOT WISDOM_WINDOWS_STORE AND NOT WISDOM_EXCLUDE_KDGUI)
  set(BUILD_SHARED_LIBS OFF)
  set(KDUTILS_BUILD_TESTS OFF)

  find_package(KDUtils CONFIG)
  if(NOT KDUtils_FOUND)
    FetchContent_Declare(
      KDUtils
      GIT_REPOSITORY https://github.com/KDAB/KDUtils.git
      GIT_TAG main
      OVERRIDE_FIND_PACKAGE)

    set(SPDLOG_FMT_EXTERNAL_HO
        ON
        CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(KDUtils)
  endif()
endif()
